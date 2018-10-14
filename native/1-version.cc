#include <Magick++.h>
#include <napi.h>

using namespace Magick;

/**
 * Method to get the version of ImageMagick.
 **/
Napi::Value
GetVersion(const Napi::CallbackInfo &info)
{
  // return's string
  return Napi::String::New(info.Env(), MagickCore::GetMagickVersion(NULL));
}

/**
 * Initializes a node module.
 **/
Napi::Object Init(Napi::Env env, Napi::Object exports)
{
  // Equivalent to setting `exports.GetVersion = GetVersion`
  exports.Set(Napi::String::New(env, "version"), Napi::Function::New(env, GetVersion));
  return exports;
}

/**
 * Export Init as a module to Node.js
 * Equivalent to `init(module.exports);`
 **/
NODE_API_MODULE(NODE_GYP_MODULE_NAME, Init);
