# Installing

You can compile the SimpleSquirrel from the source code (see below), or [use one of the automatically built releases from GitHub Releases](https://github.com/matusnovak/simplesquirrel/releases).

## Build on Windows

The following steps below will build the SimpleSquirrel library with `MinSizeRel` config on Windows. The Squirrel dependency will be built automatically for you. See the alternative **Building with custom Squirrel library** steps at the bottom of this document.

```bash
# Clone the repository
git clone https://github.com/matusnovak/simplesquirrel
cd simplesquirrel

# Download the dependencies
git submodule update --init

# Create build folder and run cmake
mkdir build
cd build

# You can replace the "Visual Studio 15 2017" with 
# "Visual Studio 15 2017 Win64" to compile for 64-bit applications.
# You can also replace it with -G "MinGW Makefiles"
cmake .. -G "Visual Studio 15 2017" -DCMAKE_INSTALL_PREFIX=C:/... -DCMAKE_BUILD_TYPE=MinSizeRel

# Optional CMake args:
#    -DBUILD_TESTS=OFF
#    -DBUILD_EXAMPLES=OFF

# Build using cmake (or open it in Visual Studio IDE)
# Make sure the "--config" matches "-DCMAKE_BUILD_TYPE" !
cmake --build . --target ALL_BUILD --config MinSizeRel

# Optionally run the INSTALL step
# Make sure the "--config" matches "-DCMAKE_BUILD_TYPE" !
cmake --build . --target INSTALL --config MinSizeRel
```

To build the **Debug** version, simply rerun the steps with `-DCMAKE_BUILD_TYPE=Debug`

## Build on Linux or OSX

The following steps below will build the SimpleSquirrel library with `MinSizeRel` config on Linux or OSX. The Squirrel dependency will be built automatically for you. See the alternative **Building with custom Squirrel library** steps at the bottom of this document.

```bash
# Clone the repository
git clone https://github.com/matusnovak/simplesquirrel
cd simplesquirrel

# Download the dependencies
git submodule update --init

# Create build folder and run cmake
mkdir build
cd build

# You can add -G "Unix Makefiles" right after .. if you wish to
cmake ..  -DCMAKE_INSTALL_PREFIX=/usr/local -DCMAKE_BUILD_TYPE=MinSizeRel

# Optional CMake args:
#    -DBUILD_TESTS=OFF
#    -DBUILD_EXAMPLES=OFF

# Build
make all

# Optionally run the INSTALL step
sudo make install
```

## (Optional) Building Squirrel from scratch

**Linux:**

```bash
git clone https://github.com/albertodemichelis/squirrel.git
cd squirrel
mkdir build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/usr/local -DCMAKE_C_FLAGS="-fPIC"

# Build and install into the install prefix folder
sudo make install

# Copy header files (This is missing from cmake instructions in squirrel library)
sudo cp -rfv ../include* /usr/local/include
find . -name "/usr/local/include/sq*.h" -exec chmod 644 {} \;
```

**Windows:**

```bash
git clone https://github.com/albertodemichelis/squirrel.git
cd squirrel
mkdir build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX=C:/whatever/folder/you/want
make install
```

## (Alternative) Building with custom Squirrel library

Use the `SQUIRREL_LIBRARIES`, `SQSTDLIB_LIBRARIES`, and `SQUIRREL_INCLUDE_DIR` optional CMake args if you wish to compile this library with your already existing Squirrel library. If you do not supply these arguments, the Squirrel library will be built automatically for you (using git submodules)!

```bash
cmake .. -G "Visual Studio 15 2017" \
    -DCMAKE_INSTALL_PREFIX=C:/whatever/folder/you/want \
    -DSQUIRREL_INCLUDE_DIR=C:/Users/username/Documents/squirrel/include \
    -SQUIRREL_LIBRARIES=C:/Users/username/Documents/squirrel/build/squirrel/MinSizeRel/squirrel_static.lib \
    -SQSTDLIB_LIBRARIES=C:/Users/username/Documents/squirrel/build/sqstdlib/MinSizeRel/sqstdlib_static.lib \
    -DCMAKE_BUILD_TYPE=MinSizeRel \
    -DBUILD_TESTS=OFF \
    -DBUILD_EXAMPLES=OFF
```
