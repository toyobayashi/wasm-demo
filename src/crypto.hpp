#ifndef __CRYPTO_HPP__
#define __CRYPTO_HPP__

#include <vector>
#include <cstdint>

namespace crypto {

std::vector<uint8_t> enc(const std::vector<uint8_t>& data,
                         const std::vector<uint8_t>& iv,
                         const std::vector<uint8_t>& key);

std::vector<uint8_t> dec(const std::vector<uint8_t>& data,
                         const std::vector<uint8_t>& iv,
                         const std::vector<uint8_t>& key);

};

#endif
