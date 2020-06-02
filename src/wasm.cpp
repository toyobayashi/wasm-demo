#include <string>
#include <iostream>
#include <exception>
#include "emscripten/bind.h"
#include "emscripten/val.h"
#include "crypto.hpp"

static emscripten::val enc(const std::string& arr,
                           const std::string& key,
                           const std::string& iv) {
  std::vector<uint8_t> buf;
  try {
    buf = crypto::enc(std::vector<uint8_t>(arr.begin(), arr.end()),
                      std::vector<uint8_t>(key.begin(), key.end()),
                      std::vector<uint8_t>(iv.begin(), iv.end()));
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

static emscripten::val dec(const std::string& arr,
                           const std::string& key,
                           const std::string& iv) {
  std::vector<uint8_t> buf;
  try {
    buf = crypto::dec(std::vector<uint8_t>(arr.begin(), arr.end()),
                      std::vector<uint8_t>(key.begin(), key.end()),
                      std::vector<uint8_t>(iv.begin(), iv.end()));
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

EMSCRIPTEN_BINDINGS(strtest) {
  emscripten::function("enc", enc);
  emscripten::function("dec", dec);
}
