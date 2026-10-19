package=termcap
$(package)_version=1.3.1
$(package)_download_path=https://ftp.gnu.org/gnu/termcap
$(package)_file_name=$(package)-$($(package)_version).tar.gz
$(package)_sha256_hash=91a0e22e5387ca4467b5bcb18edf1c51b930262fd466d5fda396dd9d26719100
# readline for Windows needs a termcap library, built like MSYS2 does (mingw-w64-termcap).
# Its configure script predates cross compiling, the three sources are compiled directly.
$(package)_patches=0001-tparam-replace-write-with-fprintf.patch

define $(package)_preprocess_cmds
  patch -Nbp1 < $($(package)_patch_dir)/0001-tparam-replace-write-with-fprintf.patch
endef

define $(package)_build_cmds
  $($(package)_cc) $($(package)_cflags) $($(package)_cppflags) -std=gnu11 -DHAVE_STRING_H=1 -DHAVE_UNISTD_H=1 -DSTDC_HEADERS=1 -c termcap.c tparam.c version.c && \
  $($(package)_ar) rcs libtermcap.a termcap.o tparam.o version.o
endef

define $(package)_stage_cmds
  mkdir -p $($(package)_staging_prefix_dir)/lib $($(package)_staging_prefix_dir)/include && \
  cp libtermcap.a $($(package)_staging_prefix_dir)/lib/ && \
  cp termcap.h $($(package)_staging_prefix_dir)/include/
endef
