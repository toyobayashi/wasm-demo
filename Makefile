ROOT         = .
OUT          = .\temp

CC           = emcc
CXX          = em++
LD           = em++
AR           = emar
ARFLAGS      = rcs
LDFLAGS      = -Wall
DEFINES      = -DAES256=1 -DCBC=1 -DCRYPTO_ENC_KEY=\"12345678901234561234567890123456\" -DCRYPTO_DEC_KEY=\"11111222223333344444555556666677\"
COMMONFLAGS  = $(LDFLAGS) -s DISABLE_EXCEPTION_CATCHING=0 --bind -I$(ROOT)/deps/aes $(DEFINES)
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

$(OUT)/base64.o : $(ROOT)/src/base64.c
	echo $<
	$(CC) $(CFLAGS) -c -o $@ $^

$(OUT)/base64.a : $(OUT)/base64.o
	echo base64.a
	$(AR) $(ARFLAGS) $@ $^

$(OUT)/crypto.o : $(ROOT)/src/crypto.cpp
	echo $<
	$(CXX) $(CXXFLAGS) -c -o $@ $^

$(OUT)/wasm.o : $(ROOT)/src/wasm.cpp
	echo $<
	$(CXX) $(CXXFLAGS) -c -o $@ $^

$(OUT)/crypto.js : $(OUT)/wasm.o $(OUT)/crypto.o $(OUT)/base64.a $(ROOT)/deps/aes/aes.a
	echo Linking...
	$(CXX) $(CXXFLAGS) -o $@ $^

lib : $(OUT)/crypto.js
	echo $(COMMONFLAGS)
	copy /Y $(OUT)\crypto.js .\public\crypto.js
	copy /Y $(OUT)\crypto.wasm .\public\crypto.wasm

clean:
	rd /s /q $(OUT)
