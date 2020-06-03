#include <exception>
#include "emscripten/bind.h"
#include "emscripten/val.h"
#include "crypto.hpp"

static std::string encryptRawData(const std::string& data) {
  std::string res = "";
  try {
    res = crypto::encryptRawData(std::vector<uint8_t>(data.begin(), data.end()));
  } catch (const std::exception& err) {
    emscripten::val::global("Error").new_(std::string(err.what())).throw_();
  }
  return res;
}

static std::string encryptJson(const emscripten::val& json) {
  std::string res = "";
  try {
    if (json.isString()) {
      res = crypto::encryptJson(json.as<std::string>());
    } else {
      res = crypto::encryptJson(emscripten::val::global("JSON").call<emscripten::val>("stringify", json).as<std::string>());
    }
  } catch (const std::exception& err) {
    emscripten::val::global("Error").new_(std::string(err.what())).throw_();
  }
  return res;
}

#ifndef WASM_DISABLE_DECRYPT

static emscripten::val decryptRawData(const std::string& b64) {
  std::vector<uint8_t> buf;
  try {
    buf = crypto::decryptRawData(b64);
  } catch (const std::exception& err) {
    emscripten::val::global("Error").new_(std::string(err.what())).throw_();
    return emscripten::val::undefined();
  }
  emscripten::val res = emscripten::val::global("Uint8Array").new_(buf.size());
  for (size_t i = 0; i < buf.size(); i++) {
    res.set(i, buf[i]);
  }
  return res;
}

static emscripten::val decryptJson(const std::string& b64) {
  std::string jsonString;
  try {
    jsonString = crypto::decryptJson(b64);
    return emscripten::val::global("JSON").call<emscripten::val>("parse", jsonString);
  } catch (const std::exception& err) {
    emscripten::val::global("Error").new_(std::string(err.what())).throw_();
    return emscripten::val::undefined();
  }
}

#endif

EMSCRIPTEN_BINDINGS(strtest) {
  emscripten::function("encryptRawData", encryptRawData);
  emscripten::function("encryptJson", encryptJson);
#ifndef WASM_DISABLE_DECRYPT
  emscripten::function("decryptRawData", decryptRawData);
  emscripten::function("decryptJson", decryptJson);
#endif
}
