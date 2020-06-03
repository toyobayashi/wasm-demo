# wasm-demo

AJAX encryption demo based on WebAssembly.

Client: use `public/crypto.wasm` and `public/crypto.js`

Server: use `build/Release/addon.node`

## Example

request:

`{"a":1,"b":"2","c":true,"d":["list","item"],"e":null}`

==== Client `Module.encryptJson(request)` ====>

`4921999568155729cEgfKvCXPwElpWXWr9xxzFirFco4u9Nai0/VZOa8Q8x3pCekR5vWmuaW+9hvtT3y`

response:

`7291012914083737w75+D0LFkJTqi1rk26KvZwfKpRgdjgO0COGijyUlNdTr3W16ROSkQWQHLpffqG4Izm6wwJmyAlRIYveC5lc0C/uWdcl4f4ozAzSM8BWJ4wk=`

==== Client `Module.decryptJson(response)` ====>

`{"code":0,data:{"a":1,"b":"2","c":true,"d":["list","item"],"e":null},"error":null,"message":"OK"}`

## Require:

* Node.js

* System C/C++ compiler tool chain (cl.exe / gcc / clang)

## Build

``` bash
$ npm install
$ npm run build
```

Defines: `CRYPTO_ENC_KEY` & `CRYPTO_DEC_KEY`, opposite from server and client

## Run

``` bash
$ npm start
```

`http://127.0.0.1:8096`
