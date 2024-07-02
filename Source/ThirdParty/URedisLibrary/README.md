This module contains the third party libraries:

- redis-plus-plus: https://github.com/sewenew/redis-plus-plus (C++ library)
- hiredis: https://github.com/redis/hiredis (C library)

The headers are shared among all the platforms (include folder). The libraries are pre-built and available for the supported platforms.

Redis Plus Plus has been compiled setting the standard the C++11 (although the project uses C++17).

The version used are:

- redis-plus-plus: 1.3.10
- hiredis: v1.2.0

## Linux & macOS compilation

Refer to: https://pgaleone.eu/2023/06/18/unreal-engine-third-party-linux-sysroot-dependencies/

### Hiredis

```bash
# Hiredis
git clone git@github.com:redis/hiredis.git
cd hiredis
git checkout v1.2.0
mkdir build
cd build
cmake ..
make -j$(nproc)
mkdir /tmp/hr
make DESTDIR=/tmp/hr install
```

### Redis Plus Plus

```bash
git clone https://github.com/sewenew/redis-plus-plus.git
cd redis-plus-plus
git checkout 1.3.10
mkdir build

# This is needed only for the linux compilation, skip it for macOS
cat <<EOF > UEToolchain.cmake.sh
set(ENGINE "/home/pgaleone/ue/engine/")

set(CMAKE_SYSROOT "${ENGINE}/Engine/Extras/ThirdPartyNotUE/SDKs/HostLinux/Linux_x64/v22_clang-16.0.6-centos7/x86_64-unknown-linux-gnu/")
set(CMAKE_C_COMPILER "${ENGINE}/Engine/Extras/ThirdPartyNotUE/SDKs/HostLinux/Linux_x64/v22_clang-16.0.6-centos7/x86_64-unknown-linux-gnu/bin/clang")
set(CMAKE_CXX_COMPILER "${ENGINE}/Engine/Extras/ThirdPartyNotUE/SDKs/HostLinux/Linux_x64/v22_clang-16.0.6-centos7/x86_64-unknown-linux-gnu/bin/clang++")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -nostdinc++ -I${ENGINE}/Engine/Source/ThirdParty/Unix/LibCxx/include/ -I${ENGINE}/Engine/Source/ThirdParty/Unix/LibCxx/include/c++/v1/")

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
EOF

cd build
cmake -DCMAKE_TOOLCHAIN_FILE=../UEToolchain.cmake -DREDIS_PLUS_PLUS_BUILD_TEST=OFF ..
# for macos: cmake -DCMAKE_PREFIX_PATH=/tmp/hr/usr/local/ -DREDIS_PLUS_PLUS_BUILD_TEST=OFF ..
make -j$(nproc)
mkdir /tmp/rd
make DESTDIR=/tmp/rd install
```

