package=readline
$(package)_version=8.3
$(package)_download_path=https://ftp.gnu.org/gnu/readline
$(package)_file_name=$(package)-$($(package)_version).tar.gz
$(package)_sha256_hash=fe5383204467828cd495ee8d1d3c037a7eba1389c22bc6a041f627976f9061cc
$(package)_linux_dependencies=ncurses
$(package)_darwin_dependencies=ncurses
$(package)_mingw32_dependencies=termcap
# upstream bug fixes, and the MSYS2 patches for the Windows console (mingw-w64-readline)
$(package)_patches=readline83-001 readline83-002 readline83-003
$(package)_patches+=mingw-0001-sigwinch.patch mingw-0002-event-hook.patch mingw-0003-no-winsize.patch mingw-0004-locale.patch

define $(package)_set_vars
  $(package)_build_opts=CC="$($(package)_cc)"
  $(package)_config_env=AR="$($(package)_ar)" RANLIB="$($(package)_ranlib)" CC="$($(package)_cc)" LDFLAGS="-L$(host_prefix)/lib" ARFLAGS=$($(package)_arflags)
  $(package)_config_env_darwin=RANLIB="$(host_prefix)/native/bin/x86_64-apple-darwin19.2.0-ranlib" AR="$(host_prefix)/native/bin/x86_64-apple-darwin19.2.0-ar" CC="$(host_prefix)/native/bin/$($(package)_cc)"
  $(package)_config_opts+=--prefix=$(host_prefix)
  $(package)_config_opts+=--exec-prefix=$(host_prefix)
  $(package)_config_opts+=--host=$(HOST)
  $(package)_config_opts+=--disable-shared --with-curses
  $(package)_config_opts_mingw32=--disable-shared --without-curses
  $(package)_config_opts_Release=--disable-debug-mode
  $(package)_config_opts_darwin+=RANLIB="$(host_prefix)/native/bin/x86_64-apple-darwin19.2.0-ranlib" AR="$(host_prefix)/native/bin/x86_64-apple-darwin19.2.0-ar" CC="$(host_prefix)/native/bin/$($(package)_cc)"
  # same as the MSYS2 build: the PC variable comes from libtermcap, mingw-w64 provides a dummy alarm()
  $(package)_cppflags_mingw32=-DNEED_EXTERN_PC=1 -D__USE_MINGW_ALARM -D_POSIX
  $(package)_config_env_mingw32=CPPFLAGS="-DNEED_EXTERN_PC=1 -D__USE_MINGW_ALARM -D_POSIX"
  # $$: expand when building, after funcs.mk has added the per-OS flags
  $(package)_build_opts=CFLAGS="$$($(package)_cflags) $$($(package)_cppflags) -fPIC"
endef

define $(package)_preprocess_cmds
  patch -p0 < $($(package)_patch_dir)/readline83-001 && \
  patch -p0 < $($(package)_patch_dir)/readline83-002 && \
  patch -p0 < $($(package)_patch_dir)/readline83-003
endef

ifeq ($(host_os),mingw32)
define $(package)_preprocess_cmds
  patch -p0 < $($(package)_patch_dir)/readline83-001 && \
  patch -p0 < $($(package)_patch_dir)/readline83-002 && \
  patch -p0 < $($(package)_patch_dir)/readline83-003 && \
  patch -Np1 < $($(package)_patch_dir)/mingw-0001-sigwinch.patch && \
  patch -Np1 < $($(package)_patch_dir)/mingw-0002-event-hook.patch && \
  patch -Np1 < $($(package)_patch_dir)/mingw-0003-no-winsize.patch && \
  patch -Np1 < $($(package)_patch_dir)/mingw-0004-locale.patch
endef
endif

define $(package)_config_cmds
  ./configure $($(package)_config_opts)
endef

define $(package)_build_cmds
  $(MAKE) $($(package)_build_opts)
endef

define $(package)_stage_cmds
  $(MAKE) install DESTDIR=$($(package)_staging_dir) prefix=$(host_prefix) exec-prefix=$(host_prefix)
endef
