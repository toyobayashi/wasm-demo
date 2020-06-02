type="Release"

until [ $# -eq 0 ]
do
if [ "$1" == "Release" ]; then type="$1"; fi
if [ "$1" == "Debug" ]; then type="$1"; fi
shift
done

def="-DAES256=1"

if [ "$type" == "Release" ]; then
  echo emcc -o ./dist/crypto.js src/crypto.cpp --bind -g $def
  emcc -o ./dist/crypto.js src/crypto.cpp --bind -g $def
else
  echo emcc -o ./dist/crypto.js ./src/crypto.cpp --bind -O3 $def
  emcc -o ./dist/crypto.js ./src/crypto.cpp --bind -O3 $def
fi
