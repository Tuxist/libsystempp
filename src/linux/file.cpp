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
#include "sysconsole.h"

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
    char           d_name[];  /* Filename (null-terminated) */
};

libsystempp::FileDescriptor::FileDescriptor(){
    _FD=-1;
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
    unsigned short umod=0;
    if((_FD=syscall3(__NR_open,(unsigned long)path,opt,umod))<0)
        throw excep[SystemException::Error] << "Can't open file: " << path;
}

int libsystempp::FileDescriptor::read(void *buf, int bufsize){
    SystemException excep;
    unsigned long ret=-1;
    if((ret=syscall3(__NR_read,(unsigned long)_FD,(unsigned long)buf,(long)bufsize))<0)
        throw excep[SystemException::Error] << "Can't read file from filedescriptor: " << _FD;
    return ret;
}

int libsystempp::FileDescriptor::write(void* buf, int bufsize){
    SystemException excep;
    unsigned long ret=-1;
    if((ret=syscall3(__NR_write,_FD,(unsigned long)buf,bufsize))<0)
        throw excep[SystemException::Error] << "Can't write file from filedescriptor: " << _FD;
    return ret;
}

int libsystempp::FileDescriptor::setFcntl(int opt){
    SystemException excep;
    unsigned long ret=-1;
    if((ret=(int)syscall3(__NR_fcntl, _FD, F_SETFL, opt))<0)
        throw excep[SystemException::Error] << "Can't setfnctl file from filedescriptor: " << _FD;
    return ret;
}

int libsystempp::FileDescriptor::getFcntl(){
    SystemException excep;
    unsigned long ret=-1;
    if((ret=(int)syscall3(__NR_fcntl, _FD, F_GETFL, 0))<0)
        throw excep[SystemException::Error] << "Can't write getfnctl from filedescriptor: " << _FD;
    return ret;
}

void libsystempp::FileDescriptor::close(){
    SystemException excep;
    if(syscall1(__NR_open,(unsigned long)_FD)<0)
        throw excep[SystemException::Error] << "Can't close file socket: " << _FD;
}

libsystempp::File::File(const char* path) {
    SystemException excep;
    _nextFile=nullptr;
    _Path=path;
    int ppos=rfind(path,getlen(path),'/')-1;
    char *tname=nullptr;
    libsystempp::substr(path,&tname,ppos,getlen(path));
    _Name=tname;
    delete[] tname;   
}

libsystempp::File::File(const char* path,const char *name) {
    SystemException excep;
    _nextFile=nullptr;
    _Path=path;
    _Name=name;
}

libsystempp::File::~File(){
    delete _nextFile;
}

const char * libsystempp::File::getName(){
    return _Name.c_str();
}

const char * libsystempp::File::getPath(){
    return _Path.c_str();
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

libsystempp::File *libsystempp::File::nextFile(){
    return _nextFile;
}

libsystempp::Directory::Directory(const char* path, const char* name){
    SystemException excep;
    if(!path || !name)
        throw excep[SystemException::Error] << "Directory path or name can't be null!";
    _Path=path;
    _Name=name;
    _firstFile=nullptr;
    _lastFile=nullptr;
    _firstDirectory=nullptr;
    _lastDirectory=nullptr;
    _nextDirectory=nullptr;
    if(_Path[_Path.size()-1]!='/')
        _Path+="/";
}

libsystempp::Directory::Directory(const char *path){
    SystemException excep;
    unsigned long ppos=0;
    bool start=true;
    
    for(unsigned long i=0; i<getlen(path); ++i){
        switch(path[i]){
            case '/':
                start=true;
                break;
            default:
                if(start)
                    ppos=i;
                start=false;
                break;
        }
    }
    
    if(!path)
        throw excep[SystemException::Error] << "Directory Path wrong !"; 
    char *tname=nullptr;
    libsystempp::substr(path,&tname,ppos,getlen(path));
    if(!path || !tname)
        throw excep[SystemException::Error] << "Directory path or name can't be null!";
    _Path=path;
    _Name=tname;
    _firstFile=nullptr;
    _lastFile=nullptr;
    _firstDirectory=nullptr;
    _lastDirectory=nullptr;
    _nextDirectory=nullptr;
    if(_Path[_Path.size()-1]!='/')
        _Path+="/";
    delete[] tname;
}

libsystempp::Directory::Directory(){
    SystemException excep;
    char *dir = new char[1024];
    unsigned long ppos=0;
    bool start=true;
    
    if(syscall2(__NR_getcwd,(unsigned long)dir,1024)<0)
        throw excep[SystemException::Error] << "getcwd can't change into workdir";
    
    for(unsigned long i=0; i<getlen(dir); ++i){
        switch(dir[i]){
            case '/':
                start=true;
                break;
            default:
                if(start)
                    ppos=i;
                start=false;
                break;
        }
    }
    

    if(!dir)
        throw excep[SystemException::Error] << "Directory Path wrong !"; 
    char *tname=nullptr;
    libsystempp::substr(dir,&tname,ppos,getlen(dir));
    
    if(!dir || !tname)
        throw excep[SystemException::Error] << "Directory path or name can't be null!";
    _Path=dir;
    _Name=tname;
    _firstFile=nullptr;
    _lastFile=nullptr;
    _firstDirectory=nullptr;
    _lastDirectory=nullptr;
    _nextDirectory=nullptr;
    if(_Path[_Path.size()-1]!='/')
        _Path+="/";
    delete[] dir;
    delete[] tname;
}

void libsystempp::Directory::list(){
    FileDescriptor fd;
    fd.open(_Path.c_str(),O_RDONLY | O_DIRECTORY);
    char buf[1024];
    SystemException excep;
    struct linux_dirent *d;
    long nread =0;
    for (;;) {
        nread = syscall3(__NR_getdents,(unsigned long)fd._FD,(unsigned long)buf, 1024);
        if (nread < 0)
            throw excep[SystemException::Error] << "Directory getdents failed!";
        else if(nread==0)
            break;
        for (long bpos = 0; bpos < nread;) {
            d = (struct linux_dirent *) (buf + bpos);
            char d_type = *(buf + bpos + d->d_reclen - 1);
            CharArray cpath=_Path;
            cpath+=d->d_name;
            switch(d_type){
                case DT::REG:
                    if(_lastFile){
                        _lastFile->_nextFile= new File(cpath.c_str(),d->d_name);
                        _lastFile=_lastFile->_nextFile;
                    }else{
                        _firstFile=new File(cpath.c_str(),d->d_name);
                        _lastFile=_firstFile;
                    }
                    break;
                case DT::DIR:
                    if(_lastDirectory){
                        _lastDirectory->_nextDirectory= new Directory(cpath.c_str(),d->d_name);
                        _lastDirectory=_lastDirectory->_nextDirectory;
                    }else{
                        _firstDirectory=new Directory(cpath.c_str(),d->d_name);
                        _lastDirectory=_firstDirectory;
                    }
                    break;
            };
            bpos += d->d_reclen;
        }
    }
}


libsystempp::Directory::~Directory(){
    delete _firstFile;
}

const char * libsystempp::Directory::getName(){
    return _Name.c_str();
}

const char * libsystempp::Directory::getPath(){
    return _Path.c_str();
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
