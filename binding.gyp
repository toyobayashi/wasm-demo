{
  "variables": {
    "PRODUCT_DIR": "./build/Release"
  },
  'target_defaults': {
    'defines': [
      'CBC=1',
      'AES256=1',
      'CRYPTO_ENC_KEY="11111222223333344444555556666677"',
      'CRYPTO_DEC_KEY="12345678901234561234567890123456"',
    ]
  },
  'targets': [
    {
      'target_name': 'addon',
      'sources': [
        'src/base64.c',
        'src/crypto.cpp',
        'deps/aes/aes.c',
        'src/addon.cpp'
      ],
      'include_dirs': [
        'deps/aes'
      ],
      'includes': [
        './common.gypi'
      ]
    }
  ]
}
