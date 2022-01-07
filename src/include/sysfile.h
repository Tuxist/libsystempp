/*******************************************************************************
 * Copyright (c) 2021, Jan Koester jan.koester@gmx.net
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 * Neither the name of the <organization> nor the
 *      names of its contributors may be used to endorse or promote products
 *      derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *******************************************************************************/

#include "sysarray.h"

#ifndef O_RDONLY
#define O_RDONLY 0x00000000
#endif

#ifndef O_WRONLY
#define O_WRONLY 0x00000001
#endif

#ifndef O_RDWR
#define O_RDWR 0x00000002
#endif

#ifndef O_DIRECTORY
#define O_DIRECTORY 0x00200000
#endif

#pragma once

namespace libsystempp{
    class FileDescriptor {
    public:
        FileDescriptor();
        FileDescriptor(int fd);
        ~FileDescriptor();
        int  write(void *buf,int bufsize);
        int  read(void  *buf,int bufsize);
        void open(const char *path, int opt);
        void close();
        void operator=(int value);
        void operator=(FileDescriptor value);        
    protected:
        int  setFcntl(int opt);
        int  getFcntl();
        int _FD;
        friend class Directory;
    };
    
    class File : FileDescriptor{
    public:
        File(const char *path);
        File(const char *path,const char *name);
        ~File();
        const char     *getName();
        void            chmod(unsigned short perm);
        void            chown(const char *user,const char *grp);
        void            rmfile();
        void            touch(unsigned short perm);
        File           *nextFile();
    private:
        CharArray      _Name;
        CharArray      _Path;
        File          *_nextFile;
        friend class Directory;
    };

    class Symbolic {
        friend class Directory;
    };

    class Directory {
    public:
        Directory(const char *path);
        Directory(const char *path,const char *name);
        ~Directory();
        void            list();
        Directory      *mkdir(const char *name,unsigned short perm);
        void            rmdir();
        Directory      *chdir(const char *name);
        void            chmod(unsigned short perm);
        void            chown(const char *user,const char *grp);
        Directory      *nextDirectory();
        File           *getFile();
    private:
        CharArray        _Name;
        CharArray        _Path;
        
        File            *_firstFile;
        File            *_lastFile;
        
        Directory       *_firstDirectory;
        Directory       *_lastDirectory;
        
        Directory       *_nextDirectory;
    };
};
