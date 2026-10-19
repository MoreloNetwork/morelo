package=sodium
$(package)_version=1.0.22
$(package)_download_path=https://github.com/jedisct1/libsodium/releases/download/$($(package)_version)-RELEASE/
$(package)_file_name=libsodium-$($(package)_version).tar.gz
$(package)_sha256_hash=adbdd8f16149e81ac6078a03aca6fc03b592b89ef7b5ed83841c086191be3349

define $(package)_set_vars
$(package)_config_opts=--enable-static --disable-shared
$(package)_config_opts+=--prefix=$(host_prefix)
$(package)_config_opts_linux=--with-pic
$(package)_config_opts_android=--with-pic
$(package)_config_opts_darwin=RANLIB="$(host_prefix)/native/bin/x86_64-apple-darwin19.2.0-ranlib" AR="$(host_prefix)/native/bin/x86_64-apple-darwin19.2.0-ar" CC="$(host_prefix)/native/bin/$($(package)_cc)"
endef

define $(package)_config_cmds
  $($(package)_autoconf) $($(package)_config_opts) AR_FLAGS=$($(package)_arflags)
endef

define $(package)_build_cmds
  $(MAKE) -j$(JOBS)
endef

define $(package)_stage_cmds
  $(MAKE) DESTDIR=$($(package)_staging_dir) install
endef
