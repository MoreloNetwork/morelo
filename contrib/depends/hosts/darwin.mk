OSX_MIN_VERSION=10.14
OSX_SDK_VERSION=10.15.1
LD64_VERSION=530

OSX_SDK=$(SDK_PATH)/MacOSX$(OSX_SDK_VERSION).sdk
darwin_CC=clang -target $(host) -mmacosx-version-min=$(OSX_MIN_VERSION) --sysroot $(OSX_SDK) -mlinker-version=$(LD64_VERSION)
darwin_CXX=clang++ -target $(host) -mmacosx-version-min=$(OSX_MIN_VERSION) --sysroot $(OSX_SDK) -stdlib=libc++ -mlinker-version=$(LD64_VERSION)

darwin_CFLAGS=-pipe
darwin_CXXFLAGS=$(darwin_CFLAGS)
darwin_ARFLAGS=cr

darwin_Release_CFLAGS=-O2
darwin_Release_CXXFLAGS=$(darwin_Release_CFLAGS)

darwin_Debug_CFLAGS=-O1
darwin_Debug_CXXFLAGS=$(darwin_Debug_CFLAGS)

darwin_native_toolchain=native_cctools
