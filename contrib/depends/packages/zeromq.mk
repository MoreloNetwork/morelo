package=zeromq
$(package)_version=4.3.5
$(package)_download_path=https://github.com/zeromq/libzmq/releases/download/v$($(package)_version)/
$(package)_file_name=$(package)-$($(package)_version).tar.gz
$(package)_sha256_hash=6653ef5910f17954861fe72332e68b03ca6e4d9c7160eb3a8de5a5a913bfab43
$(package)_dependencies=sodium

define $(package)_set_vars
  $(package)_config_opts=--without-docs --enable-static=yes --enable-shared=no --with-libsodium=yes --with-pgm=no --with-norm=no --disable-perf --disable-Werror --disable-drafts --disable-libunwind --disable-libbsd --disable-curve-keygen
  $(package)_config_opts_linux=--with-pic
  $(package)_config_opts_android=--with-pic
  $(package)_cxxflags+=-std=c++14
  $(package)_cxxflags_darwin=-fvisibility=default
endef

define $(package)_config_cmds
  $($(package)_autoconf) AR_FLAGS=$($(package)_arflags)
endef

define $(package)_build_cmds
  $(MAKE) -j$(JOBS)
endef

define $(package)_stage_cmds
  $(MAKE) DESTDIR=$($(package)_staging_dir) install
endef

define $(package)_postprocess_cmds
  rm -rf share lib/*.la
endef
