package=boost
$(package)_version=1.92.0
$(package)_download_path=https://archives.boost.io/release/$($(package)_version)/source/
$(package)_file_name=$(package)_$(subst .,_,$($(package)_version)).tar.bz2
$(package)_sha256_hash=5c1d40cb8e19adbf740a4ec2da35b3e58f3f5804b1dce44deb53df72193cbc6c

define $(package)_set_vars
$(package)_config_opts_Release=variant=release
$(package)_config_opts_Debug=variant=debug
$(package)_config_opts=--layout=tagged --user-config=user-config.jam
$(package)_config_opts+=threading=multi link=static -sNO_BZIP2=1 -sNO_ZLIB=1 -sNO_LZMA=1 -sNO_ZSTD=1
$(package)_config_opts_linux=target-os=linux threadapi=pthread runtime-link=static
$(package)_config_opts_android=target-os=android threadapi=pthread runtime-link=static
$(package)_config_opts_darwin=target-os=darwin runtime-link=static
$(package)_config_opts_mingw32=binary-format=pe target-os=windows threadapi=win32 runtime-link=static
$(package)_config_opts_x86_64_mingw32=address-model=64
$(package)_config_opts_aarch64_android=address-model=64 architecture=arm
$(package)_config_opts_arm_android=address-model=32 architecture=arm
$(package)_toolset_$(host_os)=gcc
$(package)_toolset_darwin=clang
ifneq (,$(findstring clang,$($(package)_cxx)))
   $(package)_toolset_$(host_os)=clang
endif
$(package)_archiver_$(host_os)=$($(package)_ar)
$(package)_config_libraries=chrono,filesystem,program_options,thread,date_time,regex,serialization,atomic
$(package)_cxxflags+=-std=c++14
$(package)_cxxflags_linux=-fPIC
# statx needs glibc 2.28, keep the Linux binaries runnable on glibc 2.27 (Ubuntu 18.04)
$(package)_cppflags_linux=-DBOOST_FILESYSTEM_DISABLE_STATX
$(package)_cxxflags_android=-fPIC
$(package)_cxxflags_darwin=-fvisibility=default
endef

define $(package)_preprocess_cmds
  echo "using $($(package)_toolset_$(host_os)) : : $($(package)_cxx) : <cxxflags>\"$($(package)_cxxflags) $($(package)_cppflags)\" <linkflags>\"$($(package)_ldflags)\" <archiver>\"$(boost_archiver_$(host_os))\" <arflags>\"$($(package)_arflags)\" <striper>\"$(host_STRIP)\" <ranlib>\"$(host_RANLIB)\" <rc>\"$(host_WINDRES)\" : ;" > user-config.jam
endef

define $(package)_config_cmds
  ./bootstrap.sh --without-icu --with-libraries=$(boost_config_libraries) --with-toolset=gcc
endef

define $(package)_build_cmds
  ./b2 -d1 -j$(JOBS) --prefix=$($(package)_staging_prefix_dir) $($(package)_config_opts) toolset=$($(package)_toolset_$(host_os)) stage
endef

define $(package)_stage_cmds
  ./b2 -d0 -j$(JOBS) --prefix=$($(package)_staging_prefix_dir) $($(package)_config_opts) toolset=$($(package)_toolset_$(host_os)) install
endef
