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

The master branch is used for active development and can be unstable or incompatible with release software. Please compile release tags.

Prebuilt binaries for every commit are available as artifacts of the [build workflow](https://github.com/MoreloNetwork/morelo/actions/workflows/build.yml):

| Artifact        | Platform                                     | Notes                                                        |
| --------------- | -------------------------------------------- | ------------------------------------------------------------ |
| `linux-x64`     | 64-bit Linux, glibc 2.27+ (Ubuntu 18.04+, Debian 10+, RHEL 8+) | Everything except glibc is linked statically |
| `win-x64`       | 64-bit Windows                               | Fully static, no DLLs needed                                 |
| `android-armv8` | Android 7.0+ (API 24), arm64-v8a             | Also contains `android-armv8-libwallet.tar.xz` (static `libwallet_api` and its dependencies) |
| `android-armv7` | Android 7.0+ (API 24), armeabi-v7a           | Also contains `android-armv7-libwallet.tar.xz`               |

### Cloning the repository

Clone Morelo recursively to pull in the needed submodules:

`$ git clone --recursive https://github.com/MoreloNetwork/morelo.git && cd morelo`

If you already cloned without `--recursive`, run `git submodule update --init --recursive`.

### Dependencies

Morelo needs a C++14 compiler, CMake and Boost 1.74 or newer (tested up to 1.92).
libzmq, miniupnpc and lmdb are always built from the vendored sources, libunbound is built from the vendored sources when it is not installed.

| Dep          | Min. version | Debian/Ubuntu pkg               | Arch pkg     | Fedora                      | Optional | Purpose                  |
| ------------ | ------------ | ------------------------------- | ------------ | --------------------------- | -------- | ------------------------ |
| GCC          | 7            | `build-essential`               | `base-devel` | `gcc-c++`                   | NO       |                          |
| CMake        | 3.10         | `cmake`                         | `cmake`      | `cmake`                     | NO       |                          |
| pkg-config   | any          | `pkg-config`                    | `base-devel` | `pkgconf`                   | NO       |                          |
| Boost        | 1.74         | `libboost-all-dev`              | `boost`      | `boost-devel`               | NO       | C++ libraries            |
| OpenSSL      | 1.1.1        | `libssl-dev`                    | `openssl`    | `openssl-devel`             | NO       | TLS, hashing             |
| libsodium    | 1.0.9        | `libsodium-dev`                 | `libsodium`  | `libsodium-devel`           | NO       | Cryptography             |
| autotools    | any          | `autoconf automake libtool-bin` | `base-devel` | `autoconf automake libtool` | NO       | Building vendored libzmq |
| libunbound   | 1.4.16       | `libunbound-dev`                | `unbound`    | `unbound-devel`             | YES      | DNS (vendored if missing)|
| libreadline  | 6.3          | `libreadline-dev`               | `readline`   | `readline-devel`            | YES      | Input editing            |
| HIDAPI       | any          | `libhidapi-dev`                 | `hidapi`     | `hidapi-devel`              | YES      | Hardware wallets         |
| libusb-1.0   | any          | `libusb-1.0-0-dev`              | `libusb`     | `libusbx-devel`             | YES      | Hardware wallets         |
| libudev      | any          | `libudev-dev`                   | `systemd`    | `systemd-devel`             | YES      | Hardware wallets         |
| Doxygen      | any          | `doxygen`                       | `doxygen`    | `doxygen`                   | YES      | Documentation            |
| Graphviz     | any          | `graphviz`                      | `graphviz`   | `graphviz`                  | YES      | Documentation            |

Debian / Ubuntu (22.04 or newer) one-liner:

`$ sudo apt update && sudo apt install -y git build-essential cmake pkg-config libboost-all-dev libssl-dev libsodium-dev libunbound-dev libreadline-dev libhidapi-dev libusb-1.0-0-dev libudev-dev autoconf automake libtool-bin ccache`

macOS:

`$ brew update && brew bundle --file=contrib/apple/brew`

### Building on Linux and macOS

* Install the dependencies (see above).

* Build:

	`$ make -j$(nproc) release`

	Each compile job needs about 2GB of RAM, use a lower `-j` value if the machine runs out of memory.

* The executables are in `build/release/bin`.

* Run the daemon with `build/release/bin/morelod --detach`.

* **Optional**: `make debug` builds binaries suitable for debugging.

`make release` optimizes for the CPU of the build machine (`-march=native`), so the binaries may not run on older CPUs. For a portable binary see [depends](#building-portable-release-binaries-with-depends).

#### On the Raspberry Pi

Follow the Linux instructions above. Increase the swap size to at least 2GB (`CONF_SWAPSIZE` in `/etc/dphys-swapfile`) and build with `make -j1 release`; this takes several hours.

### Building on Windows

Windows binaries are built with GCC in the [MSYS2](https://www.msys2.org) **UCRT64** environment. The result is fully static and needs no DLLs.

1. Install [MSYS2](https://www.msys2.org) and open the **MSYS2 UCRT64** shell from the Start Menu.

2. Update the system. Repeat until there is nothing left to update, and restart the shell if pacman asks for it:

	`$ pacman -Syu`

3. Install the dependencies:

	`$ pacman -S --needed git make patch autotools zip mingw-w64-ucrt-x86_64-toolchain mingw-w64-ucrt-x86_64-cmake mingw-w64-ucrt-x86_64-ccache mingw-w64-ucrt-x86_64-boost mingw-w64-ucrt-x86_64-openssl mingw-w64-ucrt-x86_64-libsodium mingw-w64-ucrt-x86_64-hidapi mingw-w64-ucrt-x86_64-libusb mingw-w64-ucrt-x86_64-unbound mingw-w64-ucrt-x86_64-pkgconf mingw-w64-ucrt-x86_64-readline`

4. Clone the repository (see above) and build:

	`$ make -j$(nproc) release-static-win-x86_64`

5. The executables are in `build/release/bin`.

To build binaries suitable for debugging, run `make debug-static-win-x86_64` (output in `build/debug/bin`).

The old `MINGW64` environment no longer provides all the required packages (e.g. hidapi and libusb), use `UCRT64`. 32-bit Windows is not supported.

### Building portable release binaries with depends

The `depends` system builds every library from pinned sources and links it statically. The CI release binaries are built this way, and you can do the same on any x86-64 Linux machine (or WSL):

| Target              | Command                                                                       | Extra requirements |
| ------------------- | ----------------------------------------------------------------------------- | ------------------ |
| Linux x86-64        | `make depends target=x86_64-linux-gnu`                                        | `gperf` |
| Windows x86-64      | `make depends target=x86_64-w64-mingw32`                                      | `g++-mingw-w64-x86-64`, switched to the posix thread model (see below) |
| Android arm64-v8a   | `make depends target=aarch64-linux-android ANDROID_NDK=/path/to/android-ndk`  | [Android NDK](https://developer.android.com/ndk/downloads) r23 or newer |
| Android armeabi-v7a | `make depends target=arm-linux-androideabi ANDROID_NDK=/path/to/android-ndk`  | Android NDK r23 or newer |

All targets also need `build-essential cmake pkg-config autoconf automake libtool-bin curl`. Add `-j$(nproc)` to build in parallel. The executables are in `build/release/bin`. For Android, `make -C build/release wallet_api` additionally builds the static wallet library.

The Windows cross compiler has to use the posix thread model:

`$ sudo update-alternatives --set x86_64-w64-mingw32-g++ $(which x86_64-w64-mingw32-g++-posix)`
`$ sudo update-alternatives --set x86_64-w64-mingw32-gcc $(which x86_64-w64-mingw32-gcc-posix)`

A Linux binary needs at least the glibc version of the machine it was built on. CI builds it in the `quay.io/pypa/manylinux_2_28_x86_64` container, and keeps it compatible with glibc 2.27 and newer.

### On FreeBSD

Follow the Linux instructions with the dependencies installed from packages (`pkg install git gmake cmake pkgconf boost-libs libsodium unbound readline autoconf automake libtool`) and use `gmake` instead of `make`. If you are running morelo in a jail, add `allow.sysvipc=1` to the jail configuration, otherwise lmdb fails with `Failed to open lmdb environment: Function not implemented`.

### On Solaris

The default Solaris linker can't be used, you have to install GNU ld, then run cmake manually with the path to your copy of GNU ld:

`$ mkdir -p build/release && cd build/release`
`$ cmake -DCMAKE_LINKER=/path/to/ld -DCMAKE_BUILD_TYPE=Release ../..`
`$ make`

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
