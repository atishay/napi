#include <Magick++.h>
#include <napi.h>

/**
 * The AsyncWorker is provided by N-API to perform tasks in the node's background
 * queues. The class has a method called execute that is executed in one of the
 * the background threads in the node's thread pool. The result is supposed to be
 * passed back in the main/js thread.
 *
 * All V8 objects are not allowed to be accessed on the background thread.
 * Node's Buffer is a special object whose underlying data can be used in the
 * background thread.
 *
 * The standard practice is to use the constructor to convert from V8 to C++
 * and then in the OnOk or OnError, going back to V8.
 **/
class ImageModifier: public Napi::AsyncWorker {
  public:
  // Async worker takes a callback and automatically handles conversion to C++ as well
  // as memory management for the callback. Therefore until node-addon-api/issues/231
  // is resolved, callbacks are the preferred ways to work with N-API async objects.
  ImageModifier(Napi::Buffer<uint8_t> buffer, Napi::Function cb) : Napi::AsyncWorker(cb),
  // Manually specify Buffer objects to be persistent until the input object is destroyed
  input(Napi::Persistent(buffer)),
  // Store the raw data as that needs to be referenced in the background thread.
  data(buffer.Data()),
  length(buffer.Length()) {
  }
  ~ImageModifier() {}

  // This is a background thread. Data stored in the node buffers are
  // the only thing we can access in this background thread.
  void Execute() {
    // Some processing in ImageMagick.
    Magick::Blob blob(data, length);
    Magick::Image image(blob), image2("water.png"), image3("car.png");
    Magick::Geometry g( 2 * image.columns(), 2 * image.rows());
    image2.zoom(g);
    // Add the raid effect
    image.spread(10);
    // Add the raid drops overlay
    image.composite(image2, Magick::CenterGravity, Magick::SrcOverCompositeOp);

    auto height = image.rows();
    auto width = image.columns();
    const double INNER_WIDTH = 640.0;
    const double INNER_HEIGHT = 540.0;
    // Clip the image and fit into 640x540
    if (width * 1.0 / height > INNER_WIDTH / INNER_HEIGHT)
    {
      g.height(INNER_HEIGHT);
      g.width(INNER_HEIGHT * width / height);
    } else {
      g.width(INNER_WIDTH);
      g.height(INNER_WIDTH * height / width);
    }
    image.scale(g);

    // Pad to the frame size
    g.width(1000);
    g.height(665);
    image.extent(g, Magick::CenterGravity);
    image.composite(image3, Magick::CenterGravity, Magick::SrcOverCompositeOp);
    image.strokeColor("white");
    image.fillColor("white");
    image.fontFamily("Apple Chancery");
    image.fontPointsize(30);
    image.annotate(" Cascadia JS 2018 ", Magick::SouthEastGravity);
    // image.frame();
    Magick::Blob output;
    image.write(&output);
    // Though node buffers can take ownership of raw binary data,
    // ImageMagick Blobs are freed when the object goes out of use.
    // We could have kept the blob in RAM if we did not wish to copy
    // this data and free up the blob object in the buffer destructor.
    data = malloc(output.length());
    memcpy(data, output.data(), output.length());
    length = output.length();
  }

  void OnOK(){
    // Creating a JS scope. The variables in this scope
    // Get destroyed once the scope is complete, V8 allows
    // then to be referenced in other scopes so that they
    // are not cleaned up.
    Napi::HandleScope scope(Env());
    // Calling the Callback method.
    Callback().Call({Env().Undefined(),
      Napi::Buffer<uint8_t>::New(Env(),
        reinterpret_cast<uint8_t *>(data), length,
      // Buffer constructor provides us with a way to free memory in C++
      // when the buffer s destroyed. We pass a C++ Lambda
        [](Napi::Env env, uint8_t *finalizeData) {
          free(finalizeData);
        })});
  }

private:
  Napi::Reference<Napi::Buffer<uint8_t>> input;
  void *data;
  int length;
};

/**
 * Method to edit an image with ImageMagick in a background thread.
 **/
Napi::Value Edit(const Napi::CallbackInfo &info)
{
  auto env = info.Env();
  { // Same validation as in 2-Info.

    if (info.Length() < 2)
    {
      Napi::TypeError::New(env, "Need at least one argument.").ThrowAsJavaScriptException();
      return env.Null();
    }
    if (!info[0].IsBuffer())
    {
      Napi::TypeError::New(env, "Need a Node.js Buffer as the first argument.").ThrowAsJavaScriptException();
      return env.Null();
    }
    if (!info[1].IsFunction())
    {
      Napi::TypeError::New(env, "Need a Node.js Function as the second argument.").ThrowAsJavaScriptException();
      return env.Null();
    }
  }
  // Async worker deletes itself when the work is complete.
  // Will have more options after - https://github.com/nodejs/node-addon-api/issues/231
  auto worker = new ImageModifier(info[0].As<Napi::Buffer<uint8_t>>(), info[1].As<Napi::Function>());
  // Remember to put the worker in queue, otherwise work will not be scheduled.
  worker->Queue();
  return env.Null();
}

/**
 * Initializes a node module.
 **/
Napi::Object Init(Napi::Env env, Napi::Object exports)
{
  // Equivalent to setting `exports.edit = Edit`
  exports.Set(Napi::String::New(env, "edit"), Napi::Function::New(env, Edit));
  return exports;
}

/**
 * Export Init as a module to Node.js
 * Equivalent to `init(module.exports);`
 **/
NODE_API_MODULE(NODE_GYP_MODULE_NAME, Init);
