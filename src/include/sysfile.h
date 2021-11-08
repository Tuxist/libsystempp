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

#ifndef SYSFILE_H
#define SYSFILE_H

namespace libsystempp{
    class FileDescriptor {
    public:
        FileDescriptor();
        FileDescriptor(int fd);
        ~FileDescriptor();
        void open(const char *path, int opt);
        void close();
        int  write(void *buf,int bufsize);
        int  read(void  *buf,int bufsize);
        int  setFcntl(int opt);
        int  getFcntl();
        void operator=(int value);
        void operator=(FileDescriptor value);
    protected:
        int _FD;
    };
    
    class File {
    public:
        File(const char *path);
        ~File();
        FileDescriptor *open(int opt);
        void            close();
        void            chmod(long perm);
        void            chown(const char *,const char *grp);
        void            rmfile();
        void            touch(long perm);
    private:
        CharArray      _Name;
        CharArray      _Path;
        FileDescriptor _Fd;
        File          *_nextFile;
        friend class Directory;
    };
    
    class Directory {
    public:
        Directory();
        ~Directory();
        Directory      *mkdir(const char *name);
        void            rmdir(const char *name);
        Directory      *chdir(const char *name);
        void            chmod(long perm);
        void            chown(const char *user,const char *grp);
        File           *getFiles();
        Directory      *getFolders();
    private:
        File            *_firstFileDescriptor;
        File            *_lastFileDescriptor;
        File            *_currentFileDescriptor;
        
        Directory      *_firstFolder;
        Directory      *_lastFolder;
        Directory      *_currentFolder;
        Directory      *_nextFolder;
    };
    
};

#endif
