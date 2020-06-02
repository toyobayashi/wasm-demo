(function () {
  var Module = typeof window === 'undefined' ? require('./dist/crypto.js') : window.Module;
  Module.onRuntimeInitialized = function () {
    console.log(Object.keys(Module));

    var key = '12345678901234561234567890123456';
    var iv = '1234567890123456';
    var str = 'hello world';
    var secret = Module.enc(str, key, iv);
    console.log(secret);
    var decoder = new TextDecoder();
    var plain = Module.dec(secret, key, iv);
    console.log(plain);
    var plainString = decoder.decode(plain);
    console.log(plainString);
  };
  console.log(Object.keys(Module));
})();
