# Requires ANDROID_NDK pointing to an Android NDK r23 or newer (LLVM toolchain)
ifeq ($(ANDROID_NDK),)
$(error ANDROID_NDK is not set, point it to an Android NDK installation)
endif
ANDROID_API ?= 24

android_toolchain_bin=$(ANDROID_NDK)/toolchains/llvm/prebuilt/$(build_os)-x86_64/bin
android_clang_prefix_aarch64=aarch64-linux-android
android_clang_prefix_arm=armv7a-linux-androideabi

android_CC=$(android_toolchain_bin)/$(android_clang_prefix_$(host_arch))$(ANDROID_API)-clang
android_CXX=$(android_toolchain_bin)/$(android_clang_prefix_$(host_arch))$(ANDROID_API)-clang++
android_AR=$(android_toolchain_bin)/llvm-ar
android_RANLIB=$(android_toolchain_bin)/llvm-ranlib
android_STRIP=$(android_toolchain_bin)/llvm-strip
android_NM=$(android_toolchain_bin)/llvm-nm

android_CFLAGS=-pipe -fPIC -ffunction-sections -fdata-sections
android_CXXFLAGS=$(android_CFLAGS)
android_ARFLAGS=crs

android_Release_CFLAGS=-O2
android_Release_CXXFLAGS=$(android_Release_CFLAGS)

android_Debug_CFLAGS=-O1
android_Debug_CXXFLAGS=$(android_Debug_CFLAGS)
