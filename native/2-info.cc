#include <Magick++.h>
#include <napi.h>

/**
 * Method to get the version of ImageMagick.
 **/
Napi::Value ProcessImage(const Napi::CallbackInfo &info)
{

  /**
   * Get the Data from the JS engine.
   **/
  auto env = info.Env();

  /**
   *  Should validate and not crash the process. This is C++.
   * There is no process.on('uncaughtException') here.
   **/
  if (info.Length() < 1)
  {
    // Throw exceptions in a synchronous method. return error as first argument if callback.
    // If returning promises, reject. Follow conventions.
    Napi::TypeError::New(env, "Need at least one argument.").ThrowAsJavaScriptException();

    // Always return even if you throw. The C++ execution needs a return statement.
    return env.Null();
  }

  // Can check for all JS types - https://nodejs.github.io/node-addon-api/class_napi_1_1_value.html
  if (!info[0].IsBuffer())
  {
    Napi::TypeError::New(env, "Need a Node.js Buffer as an argument.").ThrowAsJavaScriptException();
    return env.Null();
  }

  // All JS types are available. JS is not Ruby. Everything is not an object.
  // See https://nodejs.github.io/node-addon-api/class_napi_1_1_value.html for the inheritance chart.
  auto buffer = info[0].As<Napi::Buffer<uint8_t>>();

  /**
   *  Processing in ImageMagick.
   **/

  // Create a imagemagick image object
  Magick::Blob blob(buffer.Data(), buffer.Length());
  Magick::Image image(blob);
  double width = image.columns();
  double height = image.rows();
  auto created = image.attribute("date:create");
  auto modified = image.attribute("date:modified");

  /**
   * Returning the data to JavaScript
   **/

  /**
   * let obj = {
   *    width: width,
   *    height: height,
   *    created: created,
   *    modified: modified
   * };
   * return obj;
   **/
  Napi::Object obj = Napi::Object::New(env);
  // Simple properties
  // An object is a key value map where both keys and values can be anything.
  obj.Set(Napi::String::New(env, "width"), Napi::Number::New(env, width));
  obj.Set(Napi::String::New(env, "height"), Napi::Number::New(env, height));

  obj.Set(Napi::String::New(env, "created"), Napi::String::New(env, created));
  obj.Set(Napi::String::New(env, "modified"), Napi::String::New(env, modified));

  return obj;
}

/**
 * Initializes a node module.
 **/
Napi::Object Init(Napi::Env env, Napi::Object exports)
  {
    // Equivalent to setting `exports.processImage = ProcessImage`
    exports.Set(Napi::String::New(env, "processImage"), Napi::Function::New(env, ProcessImage));
    return exports;
}

/**
 * Export Init as a module to Node.js
 * Equivalent to `init(module.exports);`
 **/
NODE_API_MODULE(NODE_GYP_MODULE_NAME, Init);
