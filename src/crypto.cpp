#include "aes.hpp"
#include "crypto.hpp"
#include <cstddef>
#include <stdexcept>
#include <iostream>

namespace crypto {

static size_t pkcs7cut(uint8_t *p, size_t plen) {
  uint8_t last = p[plen - 1];
  if (last > 0 && last <= 16) {
    for (size_t x = 2; x <= last; x++) {
      if (p[plen - x] != last) {
        return plen;
      }
    }
    return plen - last;
  }

  return plen;
}

std::vector<uint8_t> enc(const std::vector<uint8_t>& data,
                         const std::vector<uint8_t>& key,
                         const std::vector<uint8_t>& iv) {
  if (iv.size() != 16) {
    throw std::runtime_error("Invalid vector");
  }

  if (key.size() != 32) {
    throw std::runtime_error("Invalid key");
  }

  size_t dataLength = data.size();
  const uint8_t* strBuf = (const uint8_t*) data.data();

  uint8_t* dataBuf = nullptr;

  size_t padding = dataLength % 16;
  size_t encryptLength = 0;
  if (padding != 0) {
      padding = 16 - padding;
      encryptLength = dataLength + padding;
      dataBuf = new uint8_t[encryptLength];
      for (size_t i = 0; i < dataLength; i++) {
          dataBuf[i] = strBuf[i];
      }
      for (size_t i = 0; i < padding; i++) {
          dataBuf[i + dataLength] = (uint8_t) padding;
      }
  } else {
      encryptLength = dataLength;
      dataBuf = new uint8_t[dataLength];
      for (int i = 0; i < dataLength; i++) {
          dataBuf[i] = strBuf[i];
      }
  }

  struct AES_ctx ctx;
  AES_init_ctx_iv(&ctx, key.data(), iv.data());
  AES_CBC_encrypt_buffer(&ctx, dataBuf, encryptLength);

  std::vector<uint8_t> out(encryptLength);
  for (size_t i = 0; i < encryptLength; i++) {
    out[i] = dataBuf[i];
  }

  delete[] dataBuf;
  dataBuf = nullptr;

  return out;
}

std::vector<uint8_t> dec(const std::vector<uint8_t>& data,
                         const std::vector<uint8_t>& key,
                         const std::vector<uint8_t>& iv) {
  if (iv.size() != 16) {
    throw std::runtime_error("Invalid vector");
  }

  if (key.size() != 32) {
    throw std::runtime_error("Invalid key");
  }

  size_t l = data.size();
  std::vector<uint8_t> dataCopy = data;

  struct AES_ctx ctx;
  AES_init_ctx_iv(&ctx, key.data(), iv.data());
  AES_CBC_decrypt_buffer(&ctx, dataCopy.data(), l);

  size_t realLength = pkcs7cut(dataCopy.data(), l);
  std::vector<uint8_t> res(realLength);
  res.assign(dataCopy.begin(), dataCopy.begin() + realLength);

  return res;
}

}
