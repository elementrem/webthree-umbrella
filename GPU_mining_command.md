## GPU mining Pre-requisites

The algorithm is memory hard and in order to fit the DAG into memory, it needs 1-2GB of RAM on each GPU.    
The GPU miner is implemented in OpenCL. To get openCL for your chipset and platform as follows:   
[AMD SDK openCL](http://developer.amd.com/tools-and-sdks/opencl-zone/amd-accelerated-parallel-processing-app-sdk/)    
[NVIDIA CUDA openCL](https://developer.nvidia.com/cuda-downloads)   

Ubuntu 16.04 does not officially support CUDA, But you can use the `sudo apt install nvidia-cuda-dev`

-----------------------------------------------------

- If you using Windows os, install the following dependency.

verison | SDK link       
--- | ---
windows 7| https://www.microsoft.com/en-us/download/details.aspx?id=8279         
windows 8.1|    https://developer.microsoft.com/en-us/windows/downloads/windows-8-1-sdk         
Windows 10  |   https://dev.windows.com/en-us/downloads/windows-10-sdk      

- If you using Ubuntu os, install the following dependency.            
`git clone --recursive https://github.com/elementrem/webthree-umbrella`       
[Ubuntu Pre-requisites click here](ubuntu_dependency.md)       

------------------------

## Start GPU mining

gele downloads 
[go-elementrem gele](https://github.com/elementrem/go-elementrem/releases) or [Elementrem mist wallet](https://github.com/elementrem/mist/releases)
Mist-wallet gele path = `<Elementrem-Wallet Directory>\resources\node\gele`   

- Run `gele --rpc`       
      ***Do not run Mist first. You must open the gele rpc. Rpc port is set automatically `localhost:7075`***     
      Launch `gele --rpc` followed by Elementrem-mist-wallet    

eleminer download [webthree-umbrella](https://github.com/elementrem/webthree-umbrella/releases)
- Run `eleminer -G` // G option enables the GPU mining.




## GPU mining screenshot
![](https://github.com/elementrem/webthree-umbrella/blob/master/img_helper/windows_gpu_mine_1.png?raw=true)
![](https://github.com/elementrem/webthree-umbrella/blob/master/img_helper/linux_gpu_mine_1.png?raw=true)
![](https://github.com/elementrem/webthree-umbrella/blob/master/img_helper/linux_gpu_mine_2.png?raw=true)
