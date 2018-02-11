## GPU mining Pre-requisites

The algorithm is memory hard and in order to fit the DAG into memory, it needs 1-2GB of RAM on each GPU.    
The GPU miner is implemented in OpenCL. To get openCL for your chipset and platform as follows:   
[AMD SDK openCL](https://developer.amd.com/amd-license-agreement-appsdk/)    
[NVIDIA CUDA openCL](https://developer.nvidia.com/cuda-downloads)   

Ubuntu 16.04 does not officially support CUDA, But you can use the `sudo apt install nvidia-cuda-dev`

-----------------------------------------------------

- **If you using Windows os, install the following dependency.**      

Software | link       
--- | ---
C++ Redistributable  | http://www.microsoft.com/en-us/download/details.aspx?id=48145         
Windows SDK  |   https://dev.windows.com/en-us/downloads/windows-10-sdk      

- **If you using Ubuntu os, install the following dependency.**               
```
git clone https://github.com/elementrem/webthree-umbrella
cd webthree-umbrella/dependency/ubuntu_16.04

sudo dpkg -i libqt5core5a_5.4.2+dfsg-2ubuntu9_amd64.deb
sudo dpkg -i libqt5qml5_5.4.2-1ubuntu6_amd64.deb
sudo dpkg -i libqt5webengine5_5.4.1+1wily-0ubuntu1_amd64.deb
sudo dpkg -i libqt5webengine5-dev_5.4.1+1wily-0ubuntu1_amd64.deb
sudo dpkg -i qml-module-qtwebengine_5.4.1+1wily-0ubuntu1_amd64.deb
sudo dpkg -i libcryptopp-dev_5.6.2+0wily-0ubuntu1_amd64.deb
```

------------------------

## Start GPU mining

Method 1
------------------------
gele downloads 
[go-elementrem gele](https://github.com/elementrem/go-elementrem/releases) or [Elementrem mist wallet](https://github.com/elementrem/mist/releases)
Mist-wallet gele path = `<Elementrem-Wallet Directory>\resources\node\gele`   

- Run `gele --rpc`       
      ***Do not run Mist first. You must open the gele rpc. Rpc port is set automatically `localhost:7075`***     
      Launch `gele --rpc` followed by Elementrem-mist-wallet    

eleminer download [webthree-umbrella](https://github.com/elementrem/webthree-umbrella/releases)
- Run `eleminer -G` // G option enables the GPU mining.          

-----------------------

Method 2
-----------------------       
Get [Elementrem mist wallet](https://github.com/elementrem/mist/releases)
- start **Elementrem-mist-Wallet**       
- open CMD(in windows) , terminal(in linux)       
     move to your path = `<Elementrem-Wallet Directory>\resources\node\gele`         
     run `gele attach`        
- in console mode        
     run `> web3.admin.startRPC()` // RPC open command in console    

eleminer download [webthree-umbrella](https://github.com/elementrem/webthree-umbrella/releases)          
- Run `eleminer -G` // G option enables the GPU mining.    

-----------------------

## GPU mining screenshot
![](https://github.com/elementrem/webthree-umbrella/blob/master/img_helper/windows_gpu_mine_1.png?raw=true)
![](https://github.com/elementrem/webthree-umbrella/blob/master/img_helper/linux_gpu_mine_1.png?raw=true)
![](https://github.com/elementrem/webthree-umbrella/blob/master/img_helper/linux_gpu_mine_2.png?raw=true)
