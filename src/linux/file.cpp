/*******************************************************************************
Copyright (c) 2019, Jan Koester jan.koester@gmx.net
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
	* Redistributions of source code must retain the above copyright
	  notice, this list of conditions and the following disclaimer.
	* Redistributions in binary form must reproduce the above copyright
	  notice, this list of conditions and the following disclaimer in the
	  documentation and/or other materials provided with the distribution.
	* Neither the name of the <organization> nor the
	  names of its contributors may be used to endorse or promote products
	  derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*******************************************************************************/

#include <linux/fcntl.h>
#include <linux/types.h>

#include "sysexception.h"
#include "sysfile.h"
#include "syscall.h"
#include "sysbits.h"
#include "sysutils.h"

enum DT{
        UNKNOWN  = 0,
        FIFO     = 1,
        CHR      = 2,
        DIR      = 4,
        BLK      = 6,
        REG      = 8,
        LNK      = 10,
        SOCK     = 12,
        WHT      = 14
};

struct linux_dirent {
    unsigned long  d_ino;     /* Inode number */
    unsigned long  d_off;     /* Offset to next linux_dirent */
    unsigned short d_reclen;  /* Length of this linux_dirent */
    char          *d_name;  /* Filename (null-terminated) */
    /* length is actually (d_reclen - 2 -
     *                                    offsetof(struct linux_dirent, d_name)) */
    char           pad;       // Zero padding byte
    char           d_type;    // File type (only since Linux
    // 2.6.4); offset is (d_reclen - 1)
};

libsystempp::FileDescriptor::FileDescriptor(){
}

libsystempp::FileDescriptor::FileDescriptor(int fd){
    _FD=fd;
}

libsystempp::FileDescriptor::~FileDescriptor(){
}

void libsystempp::FileDescriptor::operator=(libsystempp::FileDescriptor value){
    _FD=value._FD;
}


void libsystempp::FileDescriptor::operator=(int value){
    _FD=value;
}

void libsystempp::FileDescriptor::open(const char *path, int opt){
    SystemException excep;
    if(syscall4(__NR_open,(unsigned long)_FD,(unsigned long)path,0,opt)>0)
        throw excep[SystemException::Error] << "Can't open file: " << path;
}

int libsystempp::FileDescriptor::read(void *buf, int bufsize){
    return syscall3(__NR_read,(unsigned long)_FD,(unsigned long)buf,(long)bufsize);
}

int libsystempp::FileDescriptor::write(void* buf, int bufsize){
    return syscall3(__NR_write,_FD,(unsigned long)buf,bufsize);
}

int libsystempp::FileDescriptor::setFcntl(int opt){
    return (int)syscall3(__NR_fcntl, _FD, F_SETFL, opt);
}

int libsystempp::FileDescriptor::getFcntl(){
    return (int)syscall3(__NR_fcntl, _FD, F_GETFL, 0);
}

void libsystempp::FileDescriptor::close(){
    SystemException excep;
    if(syscall1(__NR_open,(unsigned long)_FD)>0)
        throw excep[SystemException::Error] << "Can't close file socket: " << _FD;
}

libsystempp::File::File(const char* path) {
    SystemException excep;
    _Path=path;
    open(path,O_RDWR);
    int ppos=rfind(path,getlen(path),'/')-1;
    char *tname=new char[getlen(path)-ppos];
    libsystempp::substr(path,&tname,ppos,getlen(path));
    _Name=tname;
    delete[] tname;
    _nextFile=nullptr;
}

libsystempp::File::~File(){
}

const char * libsystempp::File::getName(){
    return _Name.c_str();
}

void libsystempp::File::chown(const char *user, const char* grp){
}

void libsystempp::File::chmod(unsigned short perm){
    SystemException excep;
    if(syscall1(__NR_chmod,perm)<0)
        throw excep[SystemException::Error] << "chmod can't change file permission !";
}

void libsystempp::File::touch(unsigned short perm){
}

void libsystempp::File::rmfile(){
    SystemException excep;
    if(syscall1(__NR_unlink,(unsigned long)_Path.c_str())<0)
        throw excep[SystemException::Error] << "rmdir can't delete file!";    
}

libsystempp::File * libsystempp::File::nextFile(){
    return _nextFile;
}


libsystempp::Directory::Directory(const char *path){
    _Path=path;
    if(_Path[_Path.size()-1]!='/')
        _Path+="/";
}

void libsystempp::Directory::list(){
    FileDescriptor fd;
    fd.open(_Path.c_str(),O_RDONLY | O_DIRECTORY);
    char buf[1024];
    SystemException excep;
    long nread = syscall3(__NR_getdents,fd._FD,(long)&buf, 1024);
    if (nread == -1)
       excep[SystemException::Error] << "Directory getdents failed!";
    else if(nread==0)
        return;  
    for (long bpos = 0; bpos < nread;) {
        struct linux_dirent *d = (struct linux_dirent *) (buf + bpos);
            char d_type = *(buf + bpos + d->d_reclen - 1);

            CharArray cpth= _Path;
            cpth+=d->d_name;
            
            switch(d_type){
                case DT::REG:
                    if(_lastFile){
                        _lastFile->_nextFile= new File(cpth.c_str());
                    }else{
                        _firstFile=new File(cpth.c_str());
                        _lastFile=_firstFile;
                    }
                    break;
                case DT::DIR:
                    if(_lastDirectory){
                        _lastDirectory->_nextDirectory= new Directory(cpth.c_str());
                        _lastDirectory=_lastDirectory->_nextDirectory;
                    }else{
                        _firstDirectory=new Directory(cpth.c_str());
                        _lastDirectory=_firstDirectory;
                    }
                    break;
            };
            bpos += d->d_reclen;
    }
}


libsystempp::Directory::~Directory(){
}

void libsystempp::Directory::chmod(unsigned short perm){
    SystemException excep;
    if(syscall1(__NR_chmod,perm)<0)
        throw excep[SystemException::Error] << "chmod can't change directory permission !";
}

void libsystempp::Directory::chown(const char* user, const char* grp){
}

void libsystempp::Directory::rmdir(){
    SystemException excep;
    if(syscall1(__NR_rmdir,(unsigned long)_Path.c_str())<0)
        throw excep[SystemException::Error] << "rmdir can't delete directory !";
}

libsystempp::Directory * libsystempp::Directory::nextDirectory(){
    return _nextDirectory;
}

libsystempp::Directory * libsystempp::Directory::chdir(const char* name){
    return this;
}

libsystempp::Directory * libsystempp::Directory::mkdir(const char* name,unsigned short perm){
    SystemException excep;
    CharArray newpth=_Path;
    newpth+=name;
    if(syscall2(__NR_mkdir,(unsigned long)newpth.c_str(),perm)<0)
        throw excep[SystemException::Error] << "mkdir can't create directory !";
    if(_lastDirectory){
        _lastDirectory->_nextDirectory= new Directory(newpth.c_str());
        _lastDirectory=_lastDirectory->_nextDirectory;
    }else{
        _firstDirectory=new Directory(newpth.c_str());
        _lastDirectory=_firstDirectory;
    }
    return _lastDirectory;
}

libsystempp::File * libsystempp::Directory::getFile(){
    return _firstFile;
}


