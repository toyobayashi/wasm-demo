(function () {
  var Module = typeof window === 'undefined' ? require('./build/crypto.js') : window.Module;
  Module.onRuntimeInitialized = function () {
    try {
      const b64 = Module.encryptJson({ a: 1, b: '2', c: null });
      console.log(b64);
      const origin = Module.decryptJson(b64);
      console.log(origin);
    } catch (err) {
      console.error(err);
    }
  };
})();
