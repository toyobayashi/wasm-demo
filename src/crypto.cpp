#include "aes.hpp"
#include "crypto.hpp"
#include "json.hpp"
#include <cstddef>
#include <stdexcept>
#include <random>
#include <cmath>
#include "base64.h"

namespace crypto {

static std::random_device _rd;
static std::default_random_engine _e(_rd());
static std::uniform_real_distribution<double> _u(0, 1);

static double random() {
  return _u(_e);
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

static std::string createRandomNumberString(int n) {
  std::string res = "";
  for (int i = 0; i < n; i++) {
    res += std::to_string((int32_t)::floor(10 * random()));
  }
  return res;
}

static std::string to_b64(const std::vector<uint8_t>& buffer) {
  const uint8_t* src = buffer.data();
  std::size_t slen = buffer.size();
  std::size_t dlen = ::base64_encode(src, slen, nullptr);
  if (dlen == 0) return "";
  std::string res;
  res.resize(dlen);
  ::base64_encode(src, slen, &res[0]);
  return res;
}

std::string encryptRawData(const std::vector<uint8_t>& data) {
  std::string iv = createRandomNumberString(16);
  std::string key = CRYPTO_SECRET_KEY;
  std::vector<uint8_t> raw = enc(data, std::vector<uint8_t>(key.begin(), key.end()), std::vector<uint8_t>(iv.begin(), iv.end()));
  std::string res = iv + to_b64(raw);
  return res;
}

std::string encryptJson(const std::string& json) {
  std::string b64 = to_b64(nlohmann::json::to_msgpack(nlohmann::json::parse(json)));
  return encryptRawData(std::vector<uint8_t>(b64.begin(), b64.end()));
}

#ifndef WASM_DISABLE_DECRYPT

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

static std::vector<uint8_t> from_b64(const std::string& b64) {
  const char* src = b64.c_str();
  std::size_t slen = b64.length();
  std::size_t dlen = ::base64_decode(src, slen, nullptr);
  if (dlen == 0) return {};
  std::vector<uint8_t> res(dlen);
  ::base64_decode(src, slen, &res[0]);
  return res;
}

std::vector<uint8_t> decryptRawData(const std::string& b64) {
  std::string iv = b64.substr(0, 16);
  std::string key = CRYPTO_SECRET_KEY;
  std::vector<uint8_t> raw = from_b64(b64.substr(16));
  return dec(raw, std::vector<uint8_t>(key.begin(), key.end()), std::vector<uint8_t>(iv.begin(), iv.end()));
}

std::string decryptJson(const std::string& b64) {
  std::vector<uint8_t> data = decryptRawData(b64);
  return nlohmann::json::from_msgpack(from_b64(std::string(data.begin(), data.end()))).dump();
}

#endif

}
