(function () {
  Module.onRuntimeInitialized = function () {
    var textarea = document.getElementById('body');
    textarea.value = '{"a":1,"b":"2","c":true,"d":["list","item"],"e":null}';
    document.getElementById('send').addEventListener('click', function () {
      if (textarea.value) {
        var body = Module.encryptJson(textarea.value);
        fetch('http://127.0.0.1:8096/api/secret', {
          method: 'POST',
          body: body
        }).then(function (response) {
          return response.text();
        }).then(function (text) {
          var json = Module.decryptJson(text);
          console.log(json);
        });
      }
    }, false);
  };
})();
