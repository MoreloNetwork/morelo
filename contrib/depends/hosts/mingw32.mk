mingw32_CFLAGS=-pipe
mingw32_CXXFLAGS=$(mingw32_CFLAGS)
mingw32_ARFLAGS=cr

mingw32_Release_CFLAGS=-O2
mingw32_Release_CXXFLAGS=$(mingw32_Release_CFLAGS)

mingw32_Debug_CFLAGS=-O1
mingw32_Debug_CXXFLAGS=$(mingw32_Debug_CFLAGS)

mingw32_Debug_CPPFLAGS=-D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC

