## Ubuntu 16.04 Installing dependencies

sudo apt-get -y update		
sudo apt-get -y install language-pack-en-base		
sudo dpkg-reconfigure locales		
sudo apt-get -y install software-properties-common		

sudo apt-get -y install llvm-3.7-dev		
sudo apt-get -y update		
sudo apt-get -y upgrade		

sudo apt-get -y install libjsonrpccpp-dev		

sudo apt-get -y install build-essential git cmake libboost-all-dev libgmp-dev libleveldb-dev libminiupnpc-dev libreadline-dev libncurses5-dev libcurl4-openssl-dev libmicrohttpd-dev libjsoncpp-dev libargtable2-dev libedit-dev mesa-common-dev ocl-icd-libopencl1 opencl-headers libgoogle-perftools-dev qtbase5-dev qt5-default qtdeclarative5-dev libqt5webkit5-dev ocl-icd-dev libv8-dev libz-dev		

sudo apt-get -y install qml-module-qtquick-controls qml-module-qtquick-privatewidgets qml-module-qtquick-dialogs qml-module-qt-labs-settings qml-module-qtgraphicaleffects		

sudo apt update		
sudo apt upgrade		

* Move to path = webthree-umbrella\dependency\ubuntu_16.04     

sudo apt-get -y install rpm		
sudo apt-get -f install		
sudo apt-get install alien		

sudo dpkg -i libqt5core5a_5.5.1+dfsg-16ubuntu7_amd64.deb		
sudo dpkg -i libqt5qml5_5.5.1-2ubuntu6_amd64.deb		
sudo dpkg -i libjpeg62-turbo_1.5.0-1_amd64.deb		
sudo dpkg -i libqt5webchannel5_5.5.1-59.1_amd64.deb		
sudo dpkg -i libqt5webengine5_5.5.1-1_amd64.deb		
sudo dpkg -i qtwebengine5-dev_5.5.1-1_amd64.deb		
sudo dpkg -i qml-module-qtwebengine_5.5.1-1_amd64.deb		

sudo dpkg -i libcryptopp-dev_5.6.2+0wily-0ubuntu1_amd64.deb		
sudo dpkg -i libqt5core5a_5.5.1+dfsg-16ubuntu7_amd64.deb		
sudo dpkg -i libqt5qml5_5.5.1-2ubuntu6_amd64.deb		
