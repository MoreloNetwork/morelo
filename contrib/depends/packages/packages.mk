packages:=boost openssl zeromq

native_packages :=

hardware_packages := hidapi libusb
hardware_native_packages :=

multiprocess_packages = libmultiprocess capnp
multiprocess_native_packages = native_libmultiprocess native_capnp

darwin_native_packages = native_biplist native_ds_store native_mac_alias $(hardware_native_packages)
darwin_packages = sodium ncurses readline $(hardware_packages)

linux_packages = eudev ncurses readline sodium $(hardware_packages)
linux_native_packages = $(hardware_native_packages)

android_packages = sodium

ifeq ($(build_tests),ON)
packages += gtest
endif

mingw32_packages = termcap readline sodium $(hardware_packages)
mingw32_native_packages = $(hardware_native_packages)

ifneq ($(build_os),darwin)
darwin_native_packages += native_cctools native_cdrkit native_libdmg-hfsplus
endif
