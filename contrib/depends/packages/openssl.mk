package=openssl
$(package)_version=3.5.9
$(package)_download_path=https://github.com/openssl/openssl/releases/download/openssl-$($(package)_version)/
$(package)_file_name=$(package)-$($(package)_version).tar.gz
$(package)_sha256_hash=603f5602e2eef00d77fbd429d34dcd5822bb301757a1bc9cdb24c670f1eb859a

define $(package)_set_vars
$(package)_config_env=AR="$($(package)_ar)" ARFLAGS=$($(package)_arflags) RANLIB="$($(package)_ranlib)" CC="$($(package)_cc)"
$(package)_config_opts=--prefix=$(host_prefix) --openssldir=$(host_prefix)/etc/openssl --libdir=lib
$(package)_config_opts+=no-apps
# don't read openssl.cnf from the (build machine) OPENSSLDIR at runtime
$(package)_config_opts+=no-autoload-config
$(package)_config_opts+=no-capieng
$(package)_config_opts+=no-docs
$(package)_config_opts+=no-dso
$(package)_config_opts+=no-dtls1
$(package)_config_opts+=no-engine
$(package)_config_opts+=no-gost
$(package)_config_opts+=no-legacy
$(package)_config_opts+=no-md2
$(package)_config_opts+=no-module
$(package)_config_opts+=no-quic
$(package)_config_opts+=no-rc5
$(package)_config_opts+=no-rfc3779
$(package)_config_opts+=no-sctp
$(package)_config_opts+=no-shared
$(package)_config_opts+=no-ssl-trace
$(package)_config_opts+=no-ssl3
$(package)_config_opts+=no-tests
$(package)_config_opts+=no-weak-ssl-ciphers
$(package)_config_opts+=no-zlib
$(package)_config_opts+=$($(package)_cflags) $($(package)_cppflags)
$(package)_config_opts_linux=-fPIC -Wa,--noexecstack
$(package)_config_opts_android=-fPIC
$(package)_config_opts_x86_64_linux=linux-x86_64
$(package)_config_opts_i686_linux=linux-generic32
$(package)_config_opts_arm_linux=linux-armv4
$(package)_config_opts_aarch64_linux=linux-aarch64
$(package)_config_opts_arm_android=linux-armv4
$(package)_config_opts_aarch64_android=linux-aarch64
$(package)_config_opts_x86_64_darwin=darwin64-x86_64-cc
$(package)_config_opts_x86_64_mingw32=mingw64
endef

define $(package)_config_cmds
  ./Configure $($(package)_config_opts)
endef

# fixed build date for reproducible builds
define $(package)_build_cmds
  SOURCE_DATE_EPOCH=1700000000 $(MAKE) -j$(JOBS) build_libs
endef

define $(package)_stage_cmds
  $(MAKE) DESTDIR=$($(package)_staging_dir) install_dev
endef

define $(package)_postprocess_cmds
  rm -rf share
endef
