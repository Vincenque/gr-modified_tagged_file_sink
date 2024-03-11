# gr-add_tag_module
It is a simple module to add a tag (to a 'Tagged File Sink' which will record) when button is pressed to a stream in C++ to make it sure it works as fast as possible.
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
