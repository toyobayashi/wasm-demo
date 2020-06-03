CC           = emcc
CXX          = em++
LD           = em++
AR           = emar
ARFLAGS      = rcs
LDFLAGS      = -Wall
COMMONFLAGS  = $(LDFLAGS) -s DISABLE_EXCEPTION_CATCHING=0 --bind -I../deps/aes -DAES256=1 -DCBC=1 -DCRYPTO_SECRET_KEY=\"12345678901234561234567890123456\"
CFLAGS       = $(COMMONFLAGS)
CXXFLAGS     = $(COMMONFLAGS) -std=c++11

ifdef DEBUG
COMMONFLAGS += -g
else
COMMONFLAGS += -O3
endif

default: lib

.SILENT:
.PHONY:  clean

base64.o : ../src/base64.c
	echo $<
	$(CC) $(CFLAGS) -c -o $@ $^

base64.a : base64.o
	echo base64.a
	$(AR) $(ARFLAGS) $@ $^

crypto.o : ../src/crypto.cpp
	echo $<
	$(CXX) $(CXXFLAGS) -c -o $@ $^

wasm.o : ../src/wasm.cpp
	echo $<
	$(CXX) $(CXXFLAGS) -c -o $@ $^

crypto.js : wasm.o crypto.o base64.a ../deps/aes/aes.a
	echo Linking...
	$(CXX) $(CXXFLAGS) -o $@ $^

lib : crypto.js
	echo $(COMMONFLAGS)

clean:
	del /Q *.OBJ *.LST *.o *.gch *.out *.hex *.map *.elf *.a
