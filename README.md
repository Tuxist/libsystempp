# Libsystempp
### Status Report:
## Finished:
- Allocator
- Array Class
- Console
- Filedestribtor class
- Exception Class
- Syscalls (x86_64)

## On Work
- Socket class
- confdb
- json class
- Syscalls (Arm)
- Syscalls (x86)

## Todo:
- unwind lib
- cxa implementation
- thread class
- mutex class
- sysinfo class
- signal class
- dir class
- typeinfo class

## Download:
https://github.com/Tuxist/libhsystempp

### Requirements
- kernel 4.19 (lowest that i have testing)
- gcc/clang
- cmake

## Optional
- doxygen (for docu not much at the moment)

### Building Library
cd <libpath> <br/>
mkdir build <br/>
cd build <br/>
cmake ../ <br/>
make <br/>
make install <br/>
