# ImarisReader

ImarisReader is a file reader for the Imaris5 file format (".ims"). The library provides fuctionality to read (possibly very large) image data from ".ims" files in blocks, as well as all related metadata and parameters associated with the dataset.

### Usage

The ImarisReader library has C++, C, Java and Python APIs. Using C++ API pseudocode, an image block can be read from a dataset by specifying the 5-dimensional start and end coordinates of the block (x, y, z, channel and timepoint):

```C++
bpImageReader<bpUInt16> vImageReader(vFileName, vImageIndex, vOptions);
vImageReader.ReadData(vBlockStartIndex, vBlockEndIndex, vResolutionLevel, vBlockData);
```
Full usage examples in C++, C, Java and Python can be found here: https://github.com/imaris/ImarisReaderTest

### Dependencies

1. boost: version >= 1.70: https://www.boost.org/. On macOS and Windows compile boost with:
```
--with-atomic --with-filesystem link=static
```
2. zlib: https://www.zlib.net/ (compile with default options)
3. lz4: https://github.com/lz4/lz4 (compile with default options)
4. hdf5 version >= 1.14.3: https://www.hdfgroup.org/downloads/hdf5/ (compile with default options)

### Build

```bash
mkdir build
cd build
cmake /path/to/ImarisReader -DBoost_ROOT:PATH="/path/to/boost" -DZLIB_ROOT:PATH="/path/to/zlib" -DLZ4_ROOT:PATH="/path/to/lz4" -DHDF5_ROOT:PATH="/path/to/hdf5"
```
  
On Windows, the generated solution files can be opened and compiled with Visual Studio, while on Mac the generated Makefile can be compiled with ```make```. The Visual Studio version should be specified according to the setup of the other libraries, e.g. adding ```-G "Visual Studio 14 Win64"```.
