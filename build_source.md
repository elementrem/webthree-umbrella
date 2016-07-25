* [Windows 64bit](#building-for-windows)
* [Ubuntu 64bit 16.04](#building-for-ubuntu-1604)

## Building for Windows   

### Pre-requisites
- [Visual Studio 2015](https://www.visualstudio.com/products/vs-2015-product-editions)    
- [CMake](https://cmake.org/download/)    

### Source clone
`git clone --recursive https://github.com/elementrem/webthree-umbrella`

### Get dependencies
```
cd webthree-umbrella
cmake -P webthree-helpers/deps/install_deps.cmake
```
You can also manually install dependency. |
------ | ------
boost-1.61|
cryptopp-5.6.2|
curl-7.4.2|
json-rpc-cpp-0.5.0|
jsoncpp-1.6.2|
opencl-icd-loader-0.0|
leveldb-1.2|
llvm-3.8.0|
microhttpd-0.9.2|
miniupnpc-1.9|
qt-5.6|

### Generate Visual Studio project files
```
mkdir build
cd build
cmake -G "Visual Studio 14 2015 Win64" ..
```
Which should result in the creation of `cpp-elementrem.sln` in that build directory.

## Building for Ubuntu 16.04

### Pre-requisites
[Ubuntu Pre-requisites click here](ubuntu_dependency.md)

### Build on the command-line
```
mkdir build
cd build
cmake ..
make
```
