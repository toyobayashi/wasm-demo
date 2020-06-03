#include <napi.h>
#include <exception>
#include "crypto.hpp"

static Napi::Value encryptRawData(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (info.Length() != 1) {
    Napi::Error::New(env, "arguments.length !== 1").ThrowAsJavaScriptException();
    return env.Undefined();
  }
  Napi::Value arg = info[0];
  if (!arg.IsString() && !arg.IsBuffer() && arg.As<Napi::Object>().Get("constructor") != env.Global().Get("Uint8Array")) {
    Napi::TypeError::New(env, "typeof arguments[0] !== [Uint8Array, Buffer, string]").ThrowAsJavaScriptException();
    return env.Undefined();
  }

  std::vector<uint8_t> raw;
  if (arg.IsString()) {
    std::string str = arg.As<Napi::String>().Utf8Value();
    raw.assign(str.begin(), str.end());
  } else {
    uint32_t len = arg.As<Napi::Object>().Get("length").As<Napi::Number>().Uint32Value();
    for (uint32_t i = 0; i < len; i++) {
      raw.push_back((uint8_t)arg.As<Napi::Object>().Get(i).As<Napi::Number>().Uint32Value());
    }
  }

  std::string res = "";
  try {
    res = crypto::encryptRawData(std::vector<uint8_t>(raw.begin(), raw.end()));
  } catch (const std::exception& err) {
    Napi::Error::New(env, err.what()).ThrowAsJavaScriptException();
    return env.Undefined();
  }
  return Napi::String::New(env, res);
}

static Napi::Value encryptJson(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (info.Length() != 1) {
    Napi::Error::New(env, "arguments.length !== 1").ThrowAsJavaScriptException();
    return env.Undefined();
  }

  Napi::Value json = info[0];
  std::string res = "";
  try {
    if (json.IsString()) {
      res = crypto::encryptJson(json.As<Napi::String>().Utf8Value());
    } else {
      Napi::String jsstr = env.Global().Get("JSON").As<Napi::Object>().Get("stringify").As<Napi::Function>().Call(env.Null(), { json }).As<Napi::String>();
      res = crypto::encryptJson(jsstr.Utf8Value());
    }
  } catch (const std::exception& err) {
    Napi::Error::New(env, err.what()).ThrowAsJavaScriptException();
    return env.Undefined();
  }
  return Napi::String::New(env, res);
}

#ifndef WASM_DISABLE_DECRYPT

static Napi::Value decryptRawData(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (info.Length() != 1) {
    Napi::Error::New(env, "arguments.length !== 1").ThrowAsJavaScriptException();
    return env.Undefined();
  }

  Napi::Value b64 = info[0];
  if (!b64.IsString()) {
    Napi::TypeError::New(env, "typeof arguments[0] !== string").ThrowAsJavaScriptException();
    return env.Undefined();
  }

  std::vector<uint8_t> buf;
  try {
    buf = crypto::decryptRawData(b64.As<Napi::String>().Utf8Value());
  } catch (const std::exception& err) {
    Napi::Error::New(env, err.what()).ThrowAsJavaScriptException();
    return env.Undefined();
  }

  Napi::Object res = env.Global().Get("Uint8Array").As<Napi::Function>().New({ Napi::Number::New(env, buf.size()) });

  for (size_t i = 0; i < buf.size(); i++) {
    res.Set(i, buf[i]);
  }
  return res;
}

static Napi::Value decryptJson(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (info.Length() != 1) {
    Napi::Error::New(env, "arguments.length !== 1").ThrowAsJavaScriptException();
    return env.Undefined();
  }

  Napi::Value b64 = info[0];
  if (!b64.IsString()) {
    Napi::TypeError::New(env, "typeof arguments[0] !== string").ThrowAsJavaScriptException();
    return env.Undefined();
  }
  std::string jsonString;
  try {
    jsonString = crypto::decryptJson(b64.As<Napi::String>().Utf8Value());
    return env.Global().Get("JSON").As<Napi::Object>().Get("parse").As<Napi::Function>().Call(env.Null(), { Napi::String::New(env, jsonString) }).As<Napi::Object>();
  } catch (const std::exception& err) {
    Napi::Error::New(env, err.what()).ThrowAsJavaScriptException();
    return env.Undefined();
  }
}

#endif

static Napi::Object _init(Napi::Env env, Napi::Object exports) {
  exports["encryptRawData"] = Napi::Function::New(env, encryptRawData, "encryptRawData");
  exports["encryptJson"] = Napi::Function::New(env, encryptJson, "encryptJson");
#ifndef WASM_DISABLE_DECRYPT
  exports["decryptRawData"] = Napi::Function::New(env, decryptRawData, "decryptRawData");
  exports["decryptJson"] = Napi::Function::New(env, decryptJson, "decryptJson");
#endif
  return exports;
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, _init)
