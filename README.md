# Libsystempp
### Status Report:
## Finished:
- Allocator
- Array Class
- Console
- Filedestribtor class
- Exception Class
- Syscalls (x86_64)
- thread class
- mutex class
- Socket class
- cxa implementation
- basic sysinfo class

## On Work
- confdb
- implementation of libc

## Todo:
- cleanup unittests and write doc 
- first alpha release
- add ldap/krb5 support
- Syscalls (Arm)
- Syscalls (x86)

- signal class
- typeinfo class

## Download:
https://tuxist.de/git/jan.koester/systempp

## mirror
https://github.com/Tuxist/libsystempp

### Requirements
- kernel 4.19 (lowest that i have testing)
- clang
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
