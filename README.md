# Example repository for vcpkg-glfw3 failing to compile on linux

## Setup

1. Install [vcpkg](https://github.com/Microsoft/vcpkg) by following the instructions.
1. `vcpkg install glfw3`
1. `cd build`
1. `cmake .. "-DCMAKE_TOOLCHAIN_FILE=[vcpkg root]\scripts\buildsystems\vcpkg.cmake]"`
1. `make`

## Versions Used

* cmake 3.11.2
* vcpkg 0.0.113-unknownhash

## Output Produced

```
$ cmake .. "-DCMAKE_TOOLCHAIN_FILE=[vcpkg root]\scripts\buildsystems\vcpkg.cmake]"
-- The C compiler identification is GNU 8.1.0
-- The CXX compiler identification is GNU 8.1.0
-- Check for working C compiler: /usr/bin/cc
-- Check for working C compiler: /usr/bin/cc -- works
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Detecting C compile features
-- Detecting C compile features - done
-- Check for working CXX compiler: /usr/bin/c++
-- Check for working CXX compiler: /usr/bin/c++ -- works
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Configuring done
-- Generating done
-- Build files have been written to: [project root]/build
```

```
$ make
[ 50%] Building CXX object CMakeFiles/main.dir/main.cpp.o
make[2]: *** No rule to make target '/usr/lib/librt.so/usr/lib/libm.sodl/usr/lib/libX11.so-lpthread/usr/lib/libXrandr.so/usr/lib/libXinerama.so/usr/lib/libXxf86vm.so/usr/lib/libXcursor.so', needed by 'main'.  Stop.
make[1]: *** [CMakeFiles/Makefile2:68: CMakeFiles/main.dir/all] Error 2
make: *** [Makefile:84: all] Error 2
```

## Investigation

The problem seems to be with `gflw3Target.cmake:12`

glfw from source
```
set_target_properties(glfw PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES "${_IMPORT_PREFIX}/include"
  INTERFACE_LINK_LIBRARIES "/usr/lib/librt.so;/usr/lib/libm.so;dl;/usr/lib/libX11.so;-lpthread;/usr/lib/libXrandr.so;/usr/lib/libXinerama.so;/usr/lib/libXxf86vm.so;/usr/lib/libXcursor.so"
)
```
vcpkg glfw
```
set_target_properties(glfw PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES "${_IMPORT_PREFIX}/include"
  INTERFACE_LINK_LIBRARIES "/usr/lib/librt.so/usr/lib/libm.sodl/usr/lib/libX11.so-lpthread/usr/lib/libXrandr.so/usr/lib/libXinerama.so/usr/lib/libXxf86vm.so/usr/lib/libXcursor.so"
)
```

This is due to `glfw3/portfile.cmake:46`
```
file(READ ${CURRENT_PACKAGES_DIR}/share/glfw3/glfw3Targets.cmake _contents)
set(pattern "get_filename_component(_IMPORT_PREFIX \"\${_IMPORT_PREFIX}\" PATH)\n")
string(REPLACE "${pattern}${pattern}${pattern}" "${pattern}${pattern}" _contents "${_contents}")
file(WRITE ${CURRENT_PACKAGES_DIR}/share/glfw3/glfw3Targets.cmake ${_contents}) # without quotes this strips the semicolons
```
Without quoting `${_contents}` the semicolons in the list are stripped, so this is easily fixed by adding semicolons:

```
file(READ ${CURRENT_PACKAGES_DIR}/share/glfw3/glfw3Targets.cmake _contents)
set(pattern "get_filename_component(_IMPORT_PREFIX \"\${_IMPORT_PREFIX}\" PATH)\n")
string(REPLACE "${pattern}${pattern}${pattern}" "${pattern}${pattern}" _contents "${_contents}")
file(WRITE ${CURRENT_PACKAGES_DIR}/share/glfw3/glfw3Targets.cmake "${_contents}")
```
