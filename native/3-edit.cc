#include <Magick++.h>
#include <napi.h>
#define BYTES_IN_A_BUFFER_WORD 4

class ImageModifier: public Napi::AsyncWorker {
  public:
  ImageModifier(Napi::Buffer<uint32_t> buffer, Napi::Function cb) : Napi::AsyncWorker(cb),
  input(Napi::Persistent(buffer)),
  data(buffer.Data()),
  length(buffer.Length()) {
  }
  ~ImageModifier() {}

  // This is a background thread. Node buffers are the only thing we can access
  // in this background thread.
  void Execute() {
    // While Buffer deals in words, Blob deals in bytes.
    Magick::Blob blob(data, length * BYTES_IN_A_BUFFER_WORD);
    Magick::Image image(blob);
    std::vector<Magick::Drawable> drawList;
    drawList.push_back(Magick::DrawableStrokeColor("red")); // Outline color
    drawList.push_back(Magick::DrawableStrokeWidth(0));     // Stroke width
    drawList.push_back(Magick::DrawableFillColor("green")); // Fill color
    drawList.push_back(Magick::DrawableText(100, 100, std::string("Welcome to Cascadia JS")));
    image.draw(drawList);
    Magick::Blob output;
    image.write(&output);
    data = malloc(output.length());
    memcpy(data, output.data(), output.length());
    length = output.length() / BYTES_IN_A_BUFFER_WORD;
  }

  void OnOK(){
    Napi::HandleScope scope(Env());
    Callback().Call({Env().Undefined(),
      Napi::Buffer<uint32_t>::New(Env(),
        reinterpret_cast<uint32_t *>(data),
        length, [](Napi::Env env, uint32_t *finalizeData) {
          free(finalizeData);
        })});
  }

private:
  Napi::Reference<Napi::Buffer<uint32_t>> input;
  void *data;
  int length;
};

/**
 * Method to edit an image with ImageMagick in a background thread.
 **/
Napi::Value Edit(const Napi::CallbackInfo &info)
{
  auto env = info.Env();
  { // Same validation as above.

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
  auto worker = new ImageModifier(info[0].As<Napi::Buffer<uint32_t>>(), info[1].As<Napi::Function>());
  worker->Queue();
  return env.Null();
}

/**
 * Initializes a node module.
 **/
Napi::Object Init(Napi::Env env, Napi::Object exports)
{
  // Equivalent to setting `exports.GetVersion = GetVersion`
  exports.Set(Napi::String::New(env, "edit"), Napi::Function::New(env, Edit));
  return exports;
}

/**
 * Export Init as a module to Node.js
 * Equivalent to `init(module.exports);`
 **/
NODE_API_MODULE(NODE_GYP_MODULE_NAME, Init);
