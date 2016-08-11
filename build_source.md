* [Windows 64bit](#building-for-windows)
* [Ubuntu 64bit 16.04](#building-for-ubuntu-1604)

## Building for Windows   

### Pre-requisites
- [Visual Studio 2015](https://www.visualstudio.com/products/vs-2015-product-editions)    
- [CMake](https://cmake.org/download/)    

### Source clone
````
git clone --recursive https://github.com/elementrem/webthree-umbrella   
cd webthree-umbrella
git checkout master
git submodule update --recursive
```


### Get dependencies
```
cd webthree-umbrella
cmake -P webthree-helpers/deps/install_deps.cmake
```
[You can also manually install dependency.](https://github.com/elementrem/cpp-dependencies/releases) |
------ | ------
[boost-1.61](https://github.com/elementrem/cpp-dependencies/releases/download/vc140/boost-1.61.tar.gz)  |
[cryptopp-5.6.2](https://github.com/elementrem/cpp-dependencies/releases/download/vc140/cryptopp-5.6.2.tar.gz)  |
[curl-7.4.2](https://github.com/elementrem/cpp-dependencies/releases/download/vc140/curl-7.4.2.tar.gz)  |
[json-rpc-cpp-0.5.0](https://github.com/elementrem/cpp-dependencies/releases/download/vc140/json-rpc-cpp-0.5.0.tar.gz)  |
[jsoncpp-1.6.2](https://github.com/elementrem/cpp-dependencies/releases/download/vc140/jsoncpp-1.6.2.tar.gz) |
[opencl-icd-loader-0.0](https://github.com/elementrem/cpp-dependencies/releases/download/vc140/opencl-icd-loader-0.0.tar.gz) |
[leveldb-1.2](https://github.com/elementrem/cpp-dependencies/releases/download/vc140/leveldb-1.2.tar.gz) |
[llvm-3.8.0](https://github.com/elementrem/cpp-dependencies/releases/download/vc140/llvm-3.8.0.tar.gz)  |
[microhttpd-0.9.2](https://github.com/elementrem/cpp-dependencies/releases/download/vc140/microhttpd-0.9.2.tar.gz)  |
[miniupnpc-1.9](https://github.com/elementrem/cpp-dependencies/releases/download/vc140/miniupnpc-1.9.tar.gz) |
[qt-5.6](https://github.com/elementrem/cpp-dependencies/releases/download/vc140/qt-5.6.tar.gz)  |

### Generate Visual Studio project files
```
mkdir build
cd build
cmake -G "Visual Studio 14 2015 Win64" ..
```
Which should result in the creation of `cpp-elementrem.sln` in that build directory.

## Building for Ubuntu 16.04

### Source clone
````
git clone --recursive https://github.com/elementrem/webthree-umbrella   
cd webthree-umbrella
git checkout master
git submodule update --recursive
```

### Pre-requisites
[Ubuntu Pre-requisites click here](ubuntu_dependency.md)

### Get dependencies
```
cd webthree-umbrella
cmake -P webthree-helpers/deps/install_deps.cmake
```
[You can also manually install dependency.](https://github.com/elementrem/cpp-dependencies/releases) |
------ | ------
[boost-1.61](https://github.com/elementrem/cpp-dependencies/releases/download/vc140/boost-1.61.tar.gz)  |
[cryptopp-5.6.2](https://github.com/elementrem/cpp-dependencies/releases/download/vc140/cryptopp-5.6.2.tar.gz)  |
[curl-7.4.2](https://github.com/elementrem/cpp-dependencies/releases/download/vc140/curl-7.4.2.tar.gz)  |
[json-rpc-cpp-0.5.0](https://github.com/elementrem/cpp-dependencies/releases/download/vc140/json-rpc-cpp-0.5.0.tar.gz)  |
[jsoncpp-1.6.2](https://github.com/elementrem/cpp-dependencies/releases/download/vc140/jsoncpp-1.6.2.tar.gz) |
[opencl-icd-loader-0.0](https://github.com/elementrem/cpp-dependencies/releases/download/vc140/opencl-icd-loader-0.0.tar.gz) |
[leveldb-1.2](https://github.com/elementrem/cpp-dependencies/releases/download/vc140/leveldb-1.2.tar.gz) |
[llvm-3.8.0](https://github.com/elementrem/cpp-dependencies/releases/download/vc140/llvm-3.8.0.tar.gz)  |
[microhttpd-0.9.2](https://github.com/elementrem/cpp-dependencies/releases/download/vc140/microhttpd-0.9.2.tar.gz)  |
[miniupnpc-1.9](https://github.com/elementrem/cpp-dependencies/releases/download/vc140/miniupnpc-1.9.tar.gz) |
[qt-5.6](https://github.com/elementrem/cpp-dependencies/releases/download/vc140/qt-5.6.tar.gz)  |

### Build on the command-line
```
mkdir build
cd build
cmake ..
make
```
