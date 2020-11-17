# HDF5FileTest
CMake test for extending k-wave's HDF5File class 

## Additional file requirements
This repository only provides a code to test an implementation of k-wave's `Hdf5File` class. To run, add the header `Hdf5File.h` to `src/include` and the
source file `HdfFile.cpp` (note the file extension) to `src`.

To compile cleanly, additional header files will be needed in `src/include`. These may include:
1. `DimensionSizes.h`
2. `Messages.h`
3. `MessagesLinux.h`
4. `MessagesWindows.h`
5. `MatrixNames.h`
6. `TpmExceptions.h`
