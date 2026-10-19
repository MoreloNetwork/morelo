package=termcap
$(package)_version=1.3.1
$(package)_download_path=https://ftp.gnu.org/gnu/termcap
$(package)_file_name=$(package)-$($(package)_version).tar.gz
$(package)_sha256_hash=91a0e22e5387ca4467b5bcb18edf1c51b930262fd466d5fda396dd9d26719100
# readline for Windows needs a termcap library, built like MSYS2 does (mingw-w64-termcap)
$(package)_patches=0001-tparam-replace-write-with-fprintf.patch

define $(package)_set_vars
  $(package)_cflags+=-std=gnu11
endef

define $(package)_preprocess_cmds
  patch -Nbp1 < $($(package)_patch_dir)/0001-tparam-replace-write-with-fprintf.patch && \
  cp -f $(BASEDIR)/config.guess $(BASEDIR)/config.sub .
endef

define $(package)_config_cmds
  $($(package)_autoconf)
endef

define $(package)_build_cmds
  $(MAKE) AR="$($(package)_ar)" RANLIB="$($(package)_ranlib)"
endef

define $(package)_stage_cmds
  $(MAKE) install prefix=$($(package)_staging_prefix_dir) exec_prefix=$($(package)_staging_prefix_dir) oldincludedir= && \
  rm -rf $($(package)_staging_prefix_dir)/info
endef
