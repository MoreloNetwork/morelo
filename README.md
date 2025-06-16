# Morelo Network

**Empowering Privacy, Security, and Untraceability in Digital Currency**

[![GitHub issues](https://img.shields.io/github/issues/MoreloNetwork/morelo)](https://github.com/MoreloNetwork/morelo/issues)
[![GitHub contributors](https://img.shields.io/github/contributors/MoreloNetwork/morelo)](https://github.com/MoreloNetwork/morelo/graphs/contributors)
[![GitHub last commit](https://img.shields.io/github/last-commit/MoreloNetwork/morelo)](https://github.com/MoreloNetwork/morelo/commits/master)

## Introduction
Welcome to Morelo Network, a revolutionary digital currency platform designed to prioritize privacy, security, and untraceability. With Morelo, users can take control of their finances in a decentralized ecosystem while ensuring their transactions remain confidential.

### Privacy
Morelo employs a robust cryptographic system to safeguard user transactions from prying eyes. By utilizing this technology, all financial activities, including purchases and transfers, remain private by default, protecting sensitive information from unauthorized access.

### Security
The security of Morelo transactions is fortified by a distributed peer-to-peer consensus network. Each wallet is equipped with a 25-word mnemonic seed for added security, while wallet files are encrypted with a passphrase, ensuring assets remain safe even in the event of theft.

### Untraceability
Through innovative ring signatures, Morelo ensures transactions are untraceable and introduces an element of ambiguity, preventing them from being linked back to individual users or computers. This pioneering approach to anonymity ensures user privacy is maintained at all times.

### Contributions and Collaboration
We welcome contributions from the community to enhance the Morelo Network. Whether it's a small fix or a significant code change, individuals are encouraged to submit their contributions via pull requests to the master branch. Collaboration and discussion are vital to shaping the platform and ensuring it remains at the forefront of privacy-centric digital currency.

## Morelo Network resources

- **Website:** [morelonetwork.pl](http://morelonetwork.pl)
- **Explorer:** [explorer.morelonetwork.pl](http://explorer.morelonetwork.pl)
- **Official Devs Pool:** [pool.morelonetwork.pl](http://pool.morelonetwork.pl)
- **Paper Wallet:** [generate.morelonetwork.pl](https://generate.morelonetwork.pl)
- **Mining Pool Stats:** [miningpoolstats.stream/morelo](https://miningpoolstats.stream/morelo)
- **Download:** [github.com/MoreloNetwork/morelo/releases](https://github.com/MoreloNetwork/morelo/releases) 

## Morelo Network social media

- **Discord:** [discord.morelonetwork.pl](https://discord.morelonetwork.pl)
- **X / Twitter:** [twitter.com/MoreloNetwork](https://twitter.com/MoreloNetwork)
- **Facebook:** [www.facebook.com/profile.php?id=61551589695937](https://www.facebook.com/profile.php?id=61551589695937)
- **Bitcointalk:** [bitcointalk.org/index.php?topic=5233023](https://bitcointalk.org/index.php?topic=5233023)
- **Telegram:** [telegram.morelonetwork.pl](https://telegram.morelonetwork.pl)

Feel free to explore and engage with the Morelo community through these channels and platforms! :rocket:

## SSL

As a network, Morelo supports complete, cryptographically secured connections at all levels. This includes, but is not limited to Morelo Network Nodes (Full nodes), Remote Nodes and all wallets - CLI and GUI for desktop, and Android and iOS [ iOS is under development].    

Morelo Network will be consistently implementing the highest security protocols to achieve the greatest privacy for all transactions, as well as all communications made over the Morelo Network.

The use of SSL connections means that there will not be any possibility to use the Morelo Network with unsecured or tampered connections (daemons), and that your privacy will remain a feature built in a protocol level.

 * Below is an example how to generate SSL Keys with openssl

`$ openssl genrsa -out /tmp/KEY 4096`    
`$ openssl req -new -key /tmp/KEY -out /tmp/REQ`    
`$ openssl x509 -req -days 999999 -sha256 -in /tmp/REQ -signkey /tmp/KEY -out /tmp/CERT`    

 * Above example will generate 4096bit SSL Cert at /tmp (which can be changed)*

# Compiling Morelo from source

## IMPORTANT

That build is from the master branch, which is used for active development and can be either unstable or incompatible with release software. Please compile release branches.

### Dependencies

#### In order to sucessfully compile Morelo You need Boost version 1.73.0 - 1.83.0

##### [Download Boost 1.83.0 here](https://archives.boost.io/release/1.83.0/source/)

### Example installation of Boost from source:
```
wget https://archives.boost.io/release/1.83.0/source/boost_1_83_0.tar.bz2
tar xvjf boost_1_83_0.tar.gz
cd boost_1_83_0
./bootstrap.sh
sudo ./b2 install
```

##### Morelo build has been tested on Ubuntu Server 22.04.5 LTS Jammy Jellyfish and 24.04.2 LTS Noble Numbat.

The following table summarizes the tools and libraries required to build. A
few of the libraries are also included in this repository (marked as
"Vendored"). By default, the build uses the library installed on the system,
and ignores the vendored sources. However, if no library is found installed on
the system, then the vendored source will be built and used. The vendored
sources are also used for statically-linked builds because distribution
packages often include only shared library binaries (`.so`) but not static
library archives (`.a`).

| Dep          | Min. version  | Vendored | Debian/Ubuntu pkg  | Arch pkg     | Fedora            | Optional | Purpose        |
| ------------ | ------------- | -------- | ------------------ | ------------ | ----------------- | -------- | -------------- |
| GCC          | 7.3.0         | NO       | `build-essential`  | `base-devel` | `gcc`             | NO       |                |
| CMake        | 3.17.3        | NO       | `cmake`            | `cmake`      | `cmake`           | NO       |                |
| pkg-config   | any           | NO       | `pkg-config`       | `base-devel` | `pkgconf`         | NO       |                |
| Boost        | 1.73.0        | NO       | `libboost-all-dev` | `boost`      | `boost-devel`     | NO       | C++ libraries  |
| OpenSSL      | 1.1.1         | NO       | `libssl-dev`       | `openssl`    | `openssl-devel`   | NO       | sha256 sum     |
| libsodium    | 1.0.16        | NO       | `libsodium-dev`    | ?            | `libsodium-devel` | NO       | Cryptography   |
| libunwind    | any           | NO       | `libunwind8-dev`   | `libunwind`  | `libunwind-devel` | YES      | Stack traces   |
| liblzma      | any           | NO       | `liblzma-dev`      | `xz`         | `xz-devel`        | YES      | For libunwind  |
| libreadline  | 6.3.0         | NO       | `libreadline6-dev` | `readline`   | `readline-devel`  | YES      | Input editing  |
| ldns         | 1.6.17        | NO       | `libldns-dev`      | `ldns`       | `ldns-devel`      | YES      | SSL toolkit    |
| expat        | 1.1           | NO       | `libexpat1-dev`    | `expat`      | `expat-devel`     | YES      | XML parsing    |
| GTest        | 1.5           | YES      | `libgtest-dev`[1]  | `gtest`      | `gtest-devel`     | YES      | Test suite     |
| Doxygen      | any           | NO       | `doxygen`          | `doxygen`    | `doxygen`         | YES      | Documentation  |
| Graphviz     | any           | NO       | `graphviz`         | `graphviz`   | `graphviz`        | YES      | Documentation  |
| HIDAPI       | any           | NO       | `libhidapi-dev`    | ``           | ``                | NO       | for Device     |
| libusb-1.0   | any           | NO       | `libusb-1.0-0-dev` | ``           | ``                | NO       |                |
| libudev      | any           | NO       | `libudev-dev`      | ``           | ``                | NO       |                |
-------------------------------------------------------------------------------------------------------------------------------


[1] On Debian/Ubuntu `libgtest-dev` only includes sources and headers. You must
build the library binary manually. This can be done with the following command:

`$ sudo apt-get install libgtest-dev && cd /usr/src/gtest && sudo cmake . && sudo make && sudo mv libg* /usr/lib/`

Debian / Ubuntu one-liner for all dependencies (excluding `libboost-all-dev`)

`$ sudo apt update && sudo apt install -y git build-essential cmake pkg-config libssl-dev libsodium-dev libunwind-dev liblzma-dev libreadline-dev libldns-dev libexpat1-dev libudev-dev libusb-1.0-0-dev libhidapi-dev xsltproc gperf autoconf automake libtool-bin miniupnpc doxygen graphviz libunbound-dev libzmq3-dev libnorm-dev libpgm-dev libprotobuf-dev protobuf-compiler ccache`

Install all dependencies at once on OS X:

`$ brew update && brew bundle --file=contrib/apple/brew`

### Cloning the repository

Clone Morelo recursively to pull-in needed submodule(s):

`$ git clone --recursive https://github.com/MoreloNetwork/morelo.git && cd morelo`

Change branch to the latest stable release:

`$ git checkout 0.1.1.0`

### Build instructions

Morelo uses the CMake build system and a top-level [Makefile](Makefile) that
invokes cmake commands as needed.

#### On Linux and OS X

* Install the dependencies using the one-liner or the table above.

* Build Boost from source using instructions above.

* Build binaries:

	`$ make release`

	*Optional*: If your machine has several cores and enough memory, enable parallel build by running `make -j<number of threads>` instead of `make`. For this to be worthwhile, the machine should have one core and about 2GB of RAM available per thread.

* The resulting executables can be found in `build/release/bin`

* Add `PATH="$PATH:$HOME/morelo/build/release/bin"` to `.profile`

* Run Morelo with `morelod --detach`

* **Optional**: build and run the test suite to verify the binaries:

	`$ make release-test`

    *NOTE*: `core_tests` test may take a few hours to complete.

* **Optional**: to build binaries suitable for debugging:

	`$ make debug`

* **Optional**: to build statically-linked binaries:

	`$ make release-static`

Dependencies need to be built with -fPIC. Static libraries usually aren't, so you may have to build them yourself with `-fPIC`. Refer to their documentation for how to build them.

* **Optional**: build documentation in `doc/html` (omit `HAVE_DOT=YES` if `graphviz` is not installed):

	HAVE_DOT=YES doxygen Doxyfile

#### On the Raspberry Pi

Tested on a Raspberry Pi Zero with a clean install of minimal Raspbian Jessie from https://www.raspberrypi.org/downloads/raspbian/.

* Install the dependencies using the 'Debian / Ubuntu' one-liner above.

* Build Boost from source using instructions above.

* Increase the system swap size:

`$ sudo /etc/init.d/dphys-swapfile stop`
`$ sudo nano /etc/dphys-swapfile`
`$ CONF_SWAPSIZE=1024`
`$sudo /etc/init.d/dphys-swapfile start`

* Clone Morelo recursively to pull-in needed submodule(s):

`$ git clone --recursive https://github.com/MoreloNetwork/morelo.git && cd morelo`

* Change branch to the latest stable release:

`$ git checkout 0.1.1.0`

* Build binaries:

`$ make release`

* Wait 4-6 hours

* The resulting executables can be found in `build/release/bin`

* Add `PATH="$PATH:$HOME/morelo/build/release/bin"` to `.profile`

* Run Morelo with `morelod --detach`

* You may wish to reduce the size of the swap file after the build has finished, and delete the boost directory from your home directory

#### On Windows:

Binaries for Windows are built on Windows using the MinGW toolchain within
[MSYS2 environment](http://msys2.github.io). The MSYS2 environment emulates a
POSIX system. The toolchain runs within the environment and *cross-compiles*
binaries that can run outside of the environment as a regular Windows
application.

**Preparing the build environment**

1. Download and install the [MSYS2 installer](http://msys2.github.io).

2. Open the MSYS2 shell via the `MSYS2 MSYS` shortcut from the Start Menu

3. Update packages using pacman:

`$ pacman -Syu`

4. After updating close the MSYS2 shell using Alt+F4 or by clicking X at top-right corner. It is Very Important to not exit to the shell!!.

5. Open `MSYS2 MinGW 64-bit` from the Start Menu

6. Update packages again using pacman:

`$ pacman -Syu`

7. Install dependencies (excluding `mingw-w64-x86_64-boost`):

`$ pacman -S git mingw-w64-x86_64-toolchain mingw-w64-x86_64-autotools mingw-w64-x86_64-doxygen mingw-w64-x86_64-cmake mingw-w64-x86_64-ccache mingw-w64-x86_64-openssl mingw-w64-x86_64-zeromq mingw-w64-x86_64-libsodium mingw-w64-x86_64-hidapi mingw-w64-x86_64-protobuf mingw-w64-x86_64-libusb mingw-w64-x86_64-unbound mingw-w64-x86_64-graphviz git mingw-w64-x86_64-pkgconf mingw-w64-x86_64-ccache mingw-w64-x86_64-libunwind mingw-w64-x86_64-gperf mingw-w64-x86_64-miniupnpc mingw-w64-x86_64-zeromq mingw-w64-x86_64-cppzmq mingw-w64-x86_64-gpgme mingw-w64-x86_64-dlfcn patch`

**Building**

* Clone Morelo recursively to pull-in needed submodule(s):

	`$ git clone --recursive https://github.com/morelo/morelo && cd morelo`

* Change branch to the latest stable release:

	`$ git checkout 0.1.1.0`

* Install Boost:

  `$ pacman -U contrib/mingw-w64-x86_64-boost-1.83.0-2-any.pkg.tar.zst`

* Build binaries:

  `$ USE_SINGLE_BUILDDIR=1 make release-static-win-x86_64`

* The resulting executables can be found in `build/release/bin`

* **Optional**: to build binaries suitable for debugging, run:

  `$ make debug-static-win-x86_64`

* The resulting executables can be found in `build/debug/bin`

*** Morelo does Not support 32-bit Windows anymore ***

### On FreeBSD:

The project can be built from scratch by following instructions for Linux above. If you are running morelo in a jail you need to add the flag: `allow.sysvipc=1` to your jail configuration, otherwise lmdb will throw the error message: `Failed to open lmdb environment: Function not implemented`.

We expect to add Morelo into the ports tree in the near future, which will aid in managing installations using ports or packages.

### On OpenBSD:

#### OpenBSD < 6.2

This has been tested on OpenBSD 5.8.

You will need to add a few packages to your system. `pkg_add db cmake gcc gcc-libs g++ miniupnpc gtest`.

The doxygen and graphviz packages are optional and require the xbase set.

The Boost package has a bug that will prevent librpc.a from building correctly. In order to fix this, you will have to Build boost yourself from scratch. Follow the directions here (under "Building Boost"):
https://github.com/bitcoin/bitcoin/blob/master/doc/build-openbsd.md

You will have to add the serialization, date_time, and regex modules to Boost when building as they are needed by Morelo.

To build: `$ env CC=egcc CXX=eg++ CPP=ecpp DEVELOPER_LOCAL_TOOLS=1 BOOST_ROOT=/path/to/the/boost/you/built make release-static-64`

#### OpenBSD >= 6.2

You will need to add a few packages to your system. `$ pkg_add cmake miniupnpc zeromq libiconv`.

The doxygen and graphviz packages are optional and require the xbase set.
s
Build the Boost library using clang. This guide is derived from: https://github.com/bitcoin/bitcoin/blob/master/doc/build-openbsd.md

We assume you are compiling with a non-root user and you have `doas` enabled.

Note: do not use the boost package provided by OpenBSD, as we are installing boost to `/usr/local`.

### Create boost building directory
`$ mkdir ~/boost`
`$ cd ~/boost`

### Fetch boost source
`$ ftp -o boost_1_64_0.tar.bz2 https://netcologne.dl.sourceforge.net/project/boost/boost/1.64.0/boost_1_64_0.tar.bz2`

### MUST output: (SHA256) boost_1_64_0.tar.bz2: OK
`$ echo "7bcc5caace97baa948931d712ea5f37038dbb1c5d89b43ad4def4ed7cb683332 boost_1_64_0.tar.bz2" | sha256 -c`
`$ tar xfj boost_1_64_0.tar.bz2`

### Fetch and apply boost patches, required for OpenBSD
`$ ftp -o boost_test_impl_execution_monitor_ipp.patch https://raw.githubusercontent.com/openbsd/ports/bee9e6df517077a7269ff0dfd57995f5c6a10379/devel/boost/patches/patch-boost_test_impl_execution_monitor_ipp`
`$ ftp -o boost_config_platform_bsd_hpp.patch https://raw.githubusercontent.com/openbsd/ports/90658284fb786f5a60dd9d6e8d14500c167bdaa0/devel/boost/patches/patch-boost_config_platform_bsd_hpp`

### MUST output: (SHA256) boost_config_platform_bsd_hpp.patch: OK
`$ echo "1f5e59d1154f16ee1e0cc169395f30d5e7d22a5bd9f86358f738b0ccaea5e51d boost_config_platform_bsd_hpp.patch" | sha256 -c`
### MUST output: (SHA256) boost_test_impl_execution_monitor_ipp.patch: OK
`$ echo "30cec182a1437d40c3e0bd9a866ab5ddc1400a56185b7e671bb3782634ed0206 boost_test_impl_execution_monitor_ipp.patch" | sha256 -c`

`$ cd boost_1_64_0`
`$ patch -p0 < ../boost_test_impl_execution_monitor_ipp.patch`
`$ patch -p0 < ../boost_config_platform_bsd_hpp.patch`

### Start building boost
`$ echo 'using clang : : c++ : <cxxflags>"-fvisibility=hidden -fPIC" <linkflags>"" <archiver>"ar" <striper>"strip"  <ranlib>"ranlib" <rc>"" : ;' > user-config.jam`
`$ ./bootstrap.sh --without-icu --with-libraries=chrono,filesystem,program_options,system,thread,test,date_time,regex,serialization,locale --with-toolset=clang`
`$ ./b2 toolset=clang cxxflags="-stdlib=libc++" linkflags="-stdlib=libc++" -sICONV_PATH=/usr/local
$ doas ./b2 -d0 runtime-link=shared threadapi=pthread threading=multi link=static variant=release --layout=tagged --build-type=complete --user-config=user-config.jam -sNO_BZIP2=1 -sICONV_PATH=/usr/local --prefix=/usr/local install
`

### On Solaris:

The default Solaris linker can't be used, you have to install GNU ld, then run cmake manually with the path to your copy of GNU ld:

`$ mkdir -p build/release`
`$ cd build/release`
`$ cmake -DCMAKE_LINKER=/path/to/ld -D CMAKE_BUILD_TYPE=Release ../..`
`$ cd ../..`
`$ make`

### On Linux for Android (using docker):

### Build image
`docker build -f utils/build_scripts/android32.Dockerfile -t morelo-android .`
### Create container
`docker create -it --name morelo-android morelo-android bash`
### Get binaries
`docker cp morelo-android:/opt/android/morelo/build/release/bin .`

### Building portable statically linked binaries

By default, in either dynamically or statically linked builds, binaries target the specific host processor on which the build happens and are not portable to other processors. Portable binaries can be built using the following targets:

* `make release-static-linux-x86_64` builds binaries on Linux on x86_64 portable across POSIX systems on x86_64 processors
* `make release-static-linux-armv8` builds binaries on Linux portable across POSIX systems on armv8 processors
* `make release-static-linux-armv7` builds binaries on Linux portable across POSIX systems on armv7 processors
* `make release-static-linux-armv6` builds binaries on Linux portable across POSIX systems on armv6 processors
* `make release-static-win` builds binaries on 64-bit Windows portable across 64-bit Windows systems

### Cross Compiling

You can also cross-compile Morelo static binaries on Linux for Windows and macOS with the `depends` system.

* `make depends target=x86_64-linux-gnu` for 64-bit linux binaries.
* `make depends target=x86_64-w64-mingw32` for 64-bit windows binaries. Requires `g++-mingw-w64-x86-64`
* `make depends target=x86_64-apple-darwin19.2.0` for macOS binaries. Requires `cmake imagemagick libcap-dev librsvg2-bin libz-dev libbz2-dev libtiff-tools curl bsdmainutils python3-setuptools`
* `make depends target=arm-linux-gnueabihf` for armv7 binaries. Requires `g++-arm-linux-gnueabihf`
* `make depends target=aarch64-linux-gnu` for armv8 binaries. Requires `g++-aarch64-linux-gnu`

*** For `x86_64-apple-darwin19.2.0` you need to download SDK first ***        

The required packages are the names for each toolchain on apt. Depending on your OS Distribution, they may have different names.

Using `depends` might also be easier to compile Morelo on Windows than using MSYS. Activate Windows Subsystem for Linux (WSL) with a distribution (for example Ubuntu), install the apt build-essentials and follow the `depends` steps as stated above.

### Compability with older Linux Versions < GLIBC_2.25

* `make depends-compat target=x86_64-linux-gnu` for 64-bit linux binaries.


## Running morelod

The build places the binary in `bin/` sub-directory within the build directory
from which cmake was invoked (repository root by default). To run in
foreground:

`$ ./bin/morelod`

To list all available options, run `./bin/morelod --help`.  Options can be
specified either on the command line or in a configuration file passed by the
`--config-file` argument.  To specify an option in the configuration file, add
a line with the syntax `argumentname=value`, where `argumentname` is the name
of the argument without the leading dashes, for example `log-level=1`.

To run in background:

`$ ./bin/morelod --log-file morelod.log --detach`

To run as a systemd service, copy
[morelod.service](utils/systemd/morelod.service) to `/etc/systemd/system/` and
[morelod.conf](utils/conf/morelod.conf) to `/etc/`. The [example
service](utils/systemd/morelod.service) assumes that the user `morelo` exists
and its home is the data directory specified in the [example
config](utils/conf/morelod.conf).

If you're on Mac, you may need to add the `--max-concurrency 1` option to
morelo-wallet-cli, and possibly morelod, if you get crashes refreshing.

## Internationalization

See [README.i18n.md](README.i18n.md).

## Using Tor

> There is a new, still experimental, [integration with Tor](ANONYMITY_NETWORKS.md). The
> feature allows connecting over IPv4 and Tor simultaneously - IPv4 is used for
> relaying blocks and relaying transactions received by peers whereas Tor is
> used solely for relaying transactions received over local RPC. This provides
> privacy and better protection against surrounding node (sybil) attacks.

While Morelo isn't made to integrate with Tor, it can be used wrapped with torsocks, by
setting the following configuration parameters and environment variables:

* `--p2p-bind-ip 127.0.0.1` on the command line or `p2p-bind-ip=127.0.0.1` in
  morelod.conf to disable listening for connections on external interfaces.
* `--no-igd` on the command line or `no-igd=1` in morelod.conf to disable IGD
  (UPnP port forwarding negotiation), which is pointless with Tor.
* `DNS_PUBLIC=tcp` or `DNS_PUBLIC=tcp://x.x.x.x` where x.x.x.x is the IP of the
  desired DNS server, for DNS requests to go over TCP, so that they are routed
  through Tor. When IP is not specified, morelod uses the default list of
  servers defined in [src/common/dns_utils.cpp](src/common/dns_utils.cpp).
* `TORSOCKS_ALLOW_INBOUND=1` to tell torsocks to allow morelod to bind to interfaces
   to accept connections from the wallet. On some Linux systems, torsocks
   allows binding to localhost by default, so setting this variable is only
   necessary to allow binding to local LAN/VPN interfaces to allow wallets to
   connect from remote hosts. On other systems, it may be needed for local wallets
   as well.
* Do NOT pass `--detach` when running through torsocks with systemd, (see
  [utils/systemd/morelod.service](utils/systemd/morelod.service) for details).
* If you use the wallet with a Tor daemon via the loopback IP (eg, 127.0.0.1:9050),
  then use `--untrusted-daemon` unless it is your own hidden service.

Example command line to start morelod through Tor:

`DNS_PUBLIC=tcp torsocks morelod --p2p-bind-ip 127.0.0.1 --no-igd`

### Using Tor on Tails

TAILS ships with a very restrictive set of firewall rules. Therefore, you need
to add a rule to allow this connection too, in addition to telling torsocks to
allow inbound connections. Full example:

`$ sudo iptables -I OUTPUT 2 -p tcp -d 127.0.0.1 -m tcp --dport 19994 -j ACCEPT`
`$ DNS_PUBLIC=tcp torsocks ./morelod --p2p-bind-ip 127.0.0.1 --no-igd --rpc-bind-ip 127.0.0.1 \
        --data-dir /home/amnesia/Persistent/your/directory/to/the/blockchain`
## Debugging

This section contains general instructions for debugging failed installs or problems encountered with Morelo. First ensure you are running the latest version built from the Github repository.

### Obtaining stack traces and core dumps on Unix systems

We generally use the tool `gdb` (GNU debugger) to provide stack trace functionality, and `ulimit` to provide core dumps in builds which crash or segfault.

* To use gdb in order to obtain a stack trace for a build that has stalled:

Run the build.

Once it stalls, enter the following command:

`$ gdb /path/to/morelod `pidof morelod``

Type `thread apply all bt` within gdb in order to obtain the stack trace

* If however the core dumps or segfaults:

Enter `ulimit -c unlimited` on the command line to enable unlimited filesizes for core dumps

Enter `echo core | sudo tee /proc/sys/kernel/core_pattern` to stop cores from being hijacked by other tools

Run the build.

When it terminates with an output along the lines of "Segmentation fault (core dumped)", there should be a core dump file in the same directory as morelod. It may be named just `core`, or `core.xxxx` with numbers appended.

You can now analyse this core dump with `gdb` as follows:

`$ gdb /path/to/morelod /path/to/dumpfile`

Print the stack trace with `bt`

* To run morelo within gdb:

Type `$gdb /path/to/morelod`

Pass command-line options with `--args` followed by the relevant arguments

Type `run` to run morelod

### Analysing memory corruption

We use the tool `valgrind` for this.

Run with `valgrind /path/to/morelod`. It will be slow.

### LMDB

Instructions for debugging suspected blockchain corruption as per @HYC

There is an `mdb_stat` command in the LMDB source that can print statistics about the database but it's not routinely built. This can be built with the following command:

`$ cd ~/morelo/external/liblmdb && make`

The output of `mdb_stat -ea <path to blockchain dir>` will indicate inconsistencies in the blocks, block_heights and block_info table.

The output of `mdb_dump -s blocks <path to blockchain dir>` and `mdb_dump -s block_info <path to blockchain dir>` is useful for indicating whether blocks and block_info contain the same keys.

These records are dumped as hex data, where the first line is the key and the second line is the data.

# Disclaimer:

**Important Notice:**
Morelo Network is provided as-is without any warranties or guarantees. While every effort has been made to ensure the security and reliability of the platform, users are advised to use Morelo at their own discretion. The Morelo development team and contributors cannot be held liable for any loss or damage resulting from the use of Morelo Network.

## License:

Morelo Network is licensed under the [MIT License](https://opensource.org/licenses/MIT). See the LICENSE file for details.
For inquiries, feedback, or collaboration opportunities, please contact our team.

**Together, let's continue to innovate and shape the future of privacy-centric finance.**
