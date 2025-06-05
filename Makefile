# Copyright (c) 2025, The Morelo Network
# Copyright (c) 2018-2020, The Arqma Network
# Copyright (c) 2014-2020, The Monero Project
#
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without modification, are
# permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice, this list of
#    conditions and the following disclaimer.
#
# 2. Redistributions in binary form must reproduce the above copyright notice, this list
#    of conditions and the following disclaimer in the documentation and/or other
#    materials provided with the distribution.
#
# 3. Neither the name of the copyright holder nor the names of its contributors may be
#    used to endorse or promote products derived from this software without specific
#    prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
# EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
# MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
# THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
# PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
# STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
# THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

ANDROID_STANDALONE_TOOLCHAIN_PATH ?= /usr/local/toolchain

builddir := build
topdir   := ../..
deldirs  := $(builddir)/debug $(builddir)/release $(builddir)/fuzz

all: release-all

depends:
	cd contrib/depends && $(MAKE) HOST=$(target) && cd ../.. && mkdir -p $(builddir)/release
	cd $(builddir)/release && cmake -DBUILD_TESTS=OFF -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=$(CURDIR)/contrib/depends/$(target)/share/toolchain.cmake -DBUILD_TAG="$(buildtag)" ../.. && $(MAKE)

depends-compat:
	cd contrib/depends && $(MAKE) HOST=$(target) && cd ../.. && mkdir -p $(builddir)/release
	cd $(builddir)/release && cmake -DBUILD_TESTS=OFF -DCMAKE_BUILD_TYPE=Release -DBACKCOMPAT=ON -DCMAKE_TOOLCHAIN_FILE=$(CURDIR)/contrib/depends/$(target)/share/toolchain.cmake -DBUILD_TAG="$(buildtag)" ../.. && $(MAKE)

cmake-debug:
	mkdir -p $(builddir)/debug
	cd $(builddir)/debug && cmake -DBUILD_TESTS=OFF -DCMAKE_BUILD_TYPE=Debug $(topdir)

debug: cmake-debug
	cd $(builddir)/debug && $(MAKE)

# Temporarily disable some tests:
#  * libwallet_api_tests fail (Issue #895)
#debug-test:
#	mkdir -p build/debug
#	cd build/debug && cmake -DBUILD_TESTS=OFF -DCMAKE_BUILD_TYPE=Debug ../.. &&  $(MAKE) && $(MAKE) ARGS="-E libwallet_api_tests" test

debug-all:
	mkdir -p $(builddir)/debug
	cd $(builddir)/debug && cmake -DBUILD_TESTS=OFF -DBUILD_SHARED_LIBS=OFF -DCMAKE_BUILD_TYPE=Debug $(topdir) && $(MAKE)

debug-static-all:
	mkdir -p $(builddir)/debug
	cd $(builddir)/debug && cmake -DBUILD_TESTS=OFF -DSTATIC=ON -DCMAKE_BUILD_TYPE=Debug $(topdir) && $(MAKE)

debug-static-win-x86_64:
	mkdir -p $(builddir)/debug
	cd $(builddir)/debug && cmake -G "MSYS Makefiles" -DBUILD_TESTS=OFF -DARCH="x86-64" -DBUILD_64=ON -DCMAKE_BUILD_TYPE=Debug -DBUILD_TAG="win-x64" -DCMAKE_TOOLCHAIN_FILE=$(topdir)/cmake/64-bit-toolchain.cmake -DMSYS2_FOLDER=$(shell cd ${MINGW_PREFIX}/.. && pwd -W) $(topdir) && $(MAKE)

debug-win-x86_64:
	mkdir -p $(builddir)/debug
	cd $(builddir)/debug && cmake -G "MSYS Makefiles" -DBUILD_TESTS=OFF -DSTATIC=OFF -DARCH="x86-64" -DBUILD_64=ON -DCMAKE_BUILD_TYPE=Debug -DBUILD_TAG="win-x64" -DCMAKE_TOOLCHAIN_FILE=$(topdir)/cmake/64-bit-toolchain.cmake -DMSYS2_FOLDER=$(shell cd ${MINGW_PREFIX}/.. && pwd -W) $(topdir) && $(MAKE)

cmake-release:
	mkdir -p $(builddir)/release
	cd $(builddir)/release && cmake -DBUILD_TESTS=OFF -DCMAKE_BUILD_TYPE=Release $(topdir)

release: cmake-release
	cd $(builddir)/release && $(MAKE)

release-asan:
	mkdir -p $(builddir)/release-asan
	cd $(builddir)/release-asan && cmake -DSANITIZE=ON -DBUILD_TESTS=OFF -DCMAKE_BUILD_TYPE=Release $(topdir) && $(MAKE)

debug-asan:
	mkdir -p $(builddir)/debug-asan
	cd $(builddir)/debug-asan && cmake -DSANITIZE=ON -DBUILD_TESTS=OFF -DCMAKE_BUILD_TYPE=Debug $(topdir) && $(MAKE)

release-all:
	mkdir -p $(builddir)/release
	cd $(builddir)/release && cmake -DBUILD_TESTS=OFF -DCMAKE_BUILD_TYPE=Release $(topdir) && $(MAKE)

release-static:
	mkdir -p $(builddir)/release
	cd $(builddir)/release && cmake -DBUILD_TESTS=OFF -DSTATIC=ON -DARCH="x86-64" -DBUILD_64=ON -DCMAKE_BUILD_TYPE=Release $(topdir) && $(MAKE)

coverage:
	mkdir -p $(builddir)/debug
	cd $(builddir)/debug && cmake -DBUILD_TESTS=OFF -DCMAKE_BUILD_TYPE=Debug -DCOVERAGE=ON $(topdir) && $(MAKE) && $(MAKE) test

# Targets for specific prebuilt builds which will be advertised for updates by their build tag

release-static-android-armv7:
	mkdir -p $(builddir)/release/translations
	cd $(builddir)/release/translations && cmake ../../../translations && $(MAKE)
	cd $(builddir)/release && CC=arm-linux-androideabi-clang CXX=arm-linux-androideabi-clang++ cmake -DBUILD_TESTS=OFF -DARCH="armv7-a" -DSTATIC=ON -DBUILD_64=OFF -DCMAKE_BUILD_TYPE=Release -DANDROID=true -DINSTALL_VENDORED_LIBUNBOUND=ON -DBUILD_TAG="android-armv7" -DCMAKE_SYSTEM_NAME="Android" -DCMAKE_ANDROID_STANDALONE_TOOLCHAIN="${ANDROID_STANDALONE_TOOLCHAIN_PATH}" -DCMAKE_ANDROID_ARM_MODE=ON -DCMAKE_ANDROID_ARCH_ABI="armeabi-v7a" ../.. && $(MAKE)

release-static-android-armv8:
	mkdir -p $(builddir)/release/translations
	cd $(builddir)/release/translations && cmake ../../../translations && $(MAKE)
	cd $(builddir)/release && CC=aarch64-linux-android-clang CXX=aarch64-linux-android-clang++ cmake -DBUILD_TESTS=OFF -DARCH="armv8-a" -DSTATIC=ON -DBUILD_64=ON -DCMAKE_BUILD_TYPE=Release -DANDROID=true -DINSTALL_VENDORED_LIBUNBOUND=ON -DBUILD_TAG="android-armv8" -DCMAKE_SYSTEM_NAME="Android" -DCMAKE_ANDROID_STANDALONE_TOOLCHAIN="${ANDROID_STANDALONE_TOOLCHAIN_PATH}" -DCMAKE_ANDROID_ARCH_ABI="arm64-v8a" ../.. && $(MAKE)

release-static-linux-armv6:
	mkdir -p $(builddir)/release
	cd $(builddir)/release && cmake -DBUILD_TESTS=OFF -DARCH="armv6zk" -DSTATIC=ON -DBUILD_64=OFF -DCMAKE_BUILD_TYPE=Release -DBUILD_TAG="linux-armv6" $(topdir) && $(MAKE)

release-static-linux-armv7:
	mkdir -p $(builddir)/release
	cd $(builddir)/release && cmake -DBUILD_TESTS=OFF -DARCH="armv7-a" -DSTATIC=ON -DBUILD_64=OFF -DCMAKE_BUILD_TYPE=Release -DBUILD_TAG="linux-armv7" $(topdir) && $(MAKE)

release-static-linux-armv8:
	mkdir -p $(builddir)/release
	cd $(builddir)/release && cmake -DBUILD_TESTS=OFF -DARCH="armv8-a" -DSTATIC=ON -DBUILD_64=ON -DCMAKE_BUILD_TYPE=Release -DBUILD_TAG="linux-armv8" $(topdir) && $(MAKE)

release-static-linux-x86_64:
	mkdir -p $(builddir)/release
	cd $(builddir)/release && cmake  -DBUILD_TESTS=OFF -DSTATIC=ON -DARCH="x86-64" -DBUILD_64=ON -DCMAKE_BUILD_TYPE=Release -DBUILD_TAG="linux-x64" $(topdir) && $(MAKE)

release-ubuntu22:
	mkdir -p $(builddir)/release
	cd $(builddir)/release && cmake  -DBUILD_TESTS=OFF -DARCH="x86-64" -DBUILD_64=ON -DCMAKE_BUILD_TYPE=Release -DBUILD_TAG="linux-x64-ubuntu22" $(topdir) && $(MAKE)

release-ubuntu24:
	mkdir -p $(builddir)/release
	cd $(builddir)/release && cmake  -DBUILD_TESTS=OFF -DARCH="x86-64" -DBUILD_64=ON -DCMAKE_BUILD_TYPE=Release -DBUILD_TAG="linux-x64-ubuntu24" $(topdir) && $(MAKE)

release-static-freebsd-x86_64:
	mkdir -p $(builddir)/release
	cd $(builddir)/release && cmake -DBUILD_TESTS=OFF -DSTATIC=ON -DARCH="x86-64" -DBUILD_64=ON -DCMAKE_BUILD_TYPE=Release -DBUILD_TAG="freebsd-x64" $(topdir) && $(MAKE)

release-static-mac-x86_64:
	mkdir -p $(builddir)/release
	cd $(builddir)/release && cmake -DBUILD_TESTS=OFF -DSTATIC=ON -DARCH="x86-64" -DBUILD_64=ON -DCMAKE_BUILD_TYPE=Release -DBUILD_TAG="mac-x64" $(topdir) && $(MAKE)

release-static-win-x86_64:
	mkdir -p $(builddir)/release
	cd $(builddir)/release && cmake -G "MSYS Makefiles" -DBUILD_TESTS=OFF -DARCH="x86-64" -DBUILD_64=ON -D_FORTIFY_SOURCE=0 -DCMAKE_BUILD_TYPE=Release -DBUILD_TAG="win-x64" -DCMAKE_TOOLCHAIN_FILE=$(topdir)/cmake/64-bit-toolchain.cmake -DMSYS2_FOLDER=$(shell cd ${MINGW_PREFIX}/.. && pwd -W) $(topdir) && $(MAKE)

release-win-x86_64:
	mkdir -p $(builddir)/release
	cd $(builddir)/release && cmake -G "MSYS Makefiles" -DBUILD_TESTS=OFF -DSTATIC=OFF -DARCH="x86-64" -DBUILD_64=ON -D_FORTIFY_SOURCE=0 -DCMAKE_BUILD_TYPE=Release -DBUILD_TAG="win-x64" -DCMAKE_TOOLCHAIN_FILE=$(topdir)/cmake/64-bit-toolchain.cmake -DMSYS2_FOLDER=$(shell cd ${MINGW_PREFIX}/.. && pwd -W) $(topdir) && $(MAKE)

fuzz:
	mkdir -p $(builddir)/fuzz
	cd $(builddir)/fuzz && cmake -DSTATIC=ON -DSANITIZE=ON -DBUILD_TESTS=ON -DUSE_LTO=OFF -DCMAKE_C_COMPILER=afl-gcc -DCMAKE_CXX_COMPILER=afl-g++ -DARCH="x86-64" -DCMAKE_BUILD_TYPE=fuzz -DBUILD_TAG="linux-x64" $(topdir) && $(MAKE)

clean:
	@echo "WARNING: Back-up your wallet if it exists within ./"$(deldirs)"!" ; \
    read -r -p "This will destroy the build directory, continue (y/N)?: " CONTINUE; \
	[ $$CONTINUE = "y" ] || [ $$CONTINUE = "Y" ] || (echo "Exiting."; exit 1;)
	rm -rf $(deldirs)

clean-all:
	@echo "WARNING: Back-up your wallet if it exists within ./build!" ; \
	read -r -p "This will destroy all build directories, continue (y/N)?: " CONTINUE; \
	[ $$CONTINUE = "y" ] || [ $$CONTINUE = "Y" ] || (echo "Exiting."; exit 1;)
	rm -rf ./build

tags:
	ctags -R --sort=1 --c++-kinds=+p --fields=+iaS --extra=+q --language-force=C++ src contrib tests/gtest

.PHONY: all cmake-debug debug debug-test debug-all cmake-release release release-test release-all clean tags
