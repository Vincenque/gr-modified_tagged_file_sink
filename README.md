# gr-add_tag_module
It is a simple 'Tagged File Sink' but modified so you can change filename.
Tested on GNU Radio Companion 3.10.7.0 (Python 3.10.12).
To build:
```
mkdir build 
cd build 
cmake .. 
make -j4 
sudo make install 
sudo ldconfig
```


To rebuild (in build folder):
```
sudo make uninstall
cd ..
sudo rm -r build
mkdir build 
cd build 
cmake .. 
make -j4 
sudo make install 
sudo ldconfig
```
