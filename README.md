# gr-modified_tagged_file_sink
It is a simple 'Tagged File Sink' but modified so you can change filename.

Tested on GNU Radio Companion 3.10.7.0 (Python 3.10.12).

To build/rebuild and test (in terminal):
```
sh rebuild.sh
```
or:
```
mkdir build
cd
build
sudo make uninstall
cd ..
sudo rm -r build
mkdir build 
cd build 
cmake .. 
make -j4 
sudo make install 
sudo ldconfig
gnuradio-companion ../examples/testModifiedTaggedFileSink.grc
```
