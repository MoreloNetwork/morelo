linux_CFLAGS=-pipe -ffunction-sections -fdata-sections
linux_CXXFLAGS=$(linux_CFLAGS)
linux_ARFLAGS=cr

linux_Release_CFLAGS=-O2
linux_Release_CXXFLAGS=$(linux_Release_CFLAGS)

linux_Debug_CFLAGS=-O1
linux_Debug_CXXFLAGS=$(linux_Debug_CFLAGS)

linux_Debug_CPPFLAGS=-D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC

ifeq (86,$(findstring 86,$(build_arch)))
i686_linux_CC=gcc -m32
i686_linux_CXX=g++ -m32
i686_linux_AR=ar
i686_linux_RANLIB=ranlib
i686_linux_NM=nm
i686_linux_STRIP=strip

x86_64_linux_CC=gcc -m64
x86_64_linux_CXX=g++ -m64
x86_64_linux_AR=ar
x86_64_linux_RANLIB=ranlib
x86_64_linux_NM=nm
x86_64_linux_STRIP=strip
else
i686_linux_CC=$(default_host_CC) -m32
i686_linux_CXX=$(default_host_CXX) -m32
x86_64_linux_CC=$(default_host_CC) -m64
x86_64_linux_CXX=$(default_host_CXX) -m64
endif
