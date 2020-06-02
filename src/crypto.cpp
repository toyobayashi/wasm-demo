#include "json.hpp"
#include "emscripten/bind.h"
#include "emscripten/val.h"
#include "aes/aes.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <cstdint>
#include <cstddef>

/* std::vector<uint8_t> enc(const std::vector<uint8_t>& data,
                         const std::vector<uint8_t>& iv,
                         const std::vector<uint8_t>& key) {
  size_t dataLength = data.size();
  const byte* strBuf = (const byte*) data.toCString();

  uint8_t* dataBuf = nullptr;

  int padding = dataLength % 16;
  uint32_t encryptLength = 0;
  if (padding != 0) {
      padding = 16 - padding;
      encryptLength = (uint32_t) (dataLength + padding);
      dataBuf = new uint8_t[encryptLength];
      for (int i = 0; i < dataLength; i++) {
          dataBuf[i] = strBuf[i];
      }
      for (int i = 0; i < padding; i++) {
          dataBuf[i + dataLength] = (uint8_t) padding;
      }
  } else {
      encryptLength = (uint32_t) (dataLength);
      dataBuf = new uint8_t[dataLength];
      for (int i = 0; i < dataLength; i++) {
          dataBuf[i] = strBuf[i];
      }
  }

  struct AES_ctx ctx;
  AES_init_ctx_iv(&ctx, key.buffer(), iv.buffer());
  AES_CBC_encrypt_buffer(&ctx, dataBuf, encryptLength);

  Buffer out = Buffer::alloc(encryptLength);
  for (int i = 0; i < (int)encryptLength; i++) {
    out[i] = dataBuf[i];
  }

  delete[] dataBuf;
  dataBuf = nullptr;

  return out;
} */

emscripten::val test(const emscripten::val& arr) {
  std::cout << arr.isArray() << "\n";
  // std::string str = arr.as<std::string>();
  // std::cout << str.size() << "\n";
  // std::cout << str.length() << "\n";
  std::vector<uint8_t> r = emscripten::vecFromJSArray<uint8_t>(arr);
  for (int i = 0; i < r.size(); i++) {
    std::cout << r[i] << "\n";
    // r.push_back((uint8_t)str[i]);
  }
  // nlohmann::json obj = nlohmann::json::parse(str);
  // obj["hello"] = "world";
  return emscripten::val::array(r);
}

EMSCRIPTEN_BINDINGS(strtest) {
  // emscripten::register_vector<uint8_t>("vector<uint8_t>");
  emscripten::function("test", test);
}
