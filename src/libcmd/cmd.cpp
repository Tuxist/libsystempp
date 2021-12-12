/*******************************************************************************
Copyright (c) 2021, Jan Koester jan.koester@gmx.net
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

#include <systempp/syssocket.h>
#include <systempp/sysconsole.h>
#include <systempp/sysutils.h>

#include "cmd.h"

#define KTKEY 0
#define KTSKEY 1

libsystempp::Cmd::Cmd() {
    _Key = nullptr;
    _SKey = '\0';
    _Value = nullptr;
    _Help = nullptr;
    _Found = false;
    _Required = false;
    _nextCmd = nullptr;
}

const char *libsystempp::Cmd::getKey() {
    return _Key;
}

const char libsystempp::Cmd::getShortkey() {
    return _SKey;
}

const char *libsystempp::Cmd::getValue() {
    return _Value;
}

size_t libsystempp::Cmd::getValueSize_t() {
    return atoi(_Value);
}

int libsystempp::Cmd::getValueInt() {
    return atoi(_Value);
}

const char *libsystempp::Cmd::getHelp() {
    return _Help;
}

bool libsystempp::Cmd::getFound() {
    return _Found;
}

bool libsystempp::Cmd::getRequired() {
    return _Required;
}

libsystempp::Cmd *libsystempp::Cmd::nextCmd() {
    return _nextCmd;
}

libsystempp::Cmd::~Cmd() {
    delete[] _Key;
    delete[] _Value;
    delete[] _Help;
    delete _nextCmd;
}

libsystempp::CmdController::CmdController() {
    _firstCmd = NULL;
    _lastCmd = NULL;
}

void libsystempp::CmdController::registerCmd(const char *key, const char skey,bool required, const char *defaultvalue, const char *help) {
    SystemException   sysexception;
    if (!key || !skey || !help) {
        sysexception[SystemException::Critical] << "cmd parser key,skey or help not set!";
        throw sysexception;
    }
    /*if key exist overwriting options*/
    for (Cmd *curdcmd = _firstCmd; curdcmd; curdcmd=curdcmd->nextCmd()) {
        if (ncompare(key,getlen(key),curdcmd->getKey(),
            getlen(curdcmd->getKey())) == 0) {
            /*set new shortkey*/
            curdcmd->_SKey = skey;
            /*set reqirement flag*/
            curdcmd->_Required = required;
            /*set new value*/
            delete[] curdcmd->_Value;
            curdcmd->_Value = new char[getlen(defaultvalue)+1];
            scopy(defaultvalue, defaultvalue+getlen(defaultvalue),curdcmd->_Value);
            curdcmd->_Value[getlen(defaultvalue)] = '\0';
            /*set new help*/
            delete[] curdcmd->_Help;
            curdcmd->_Help = new char[getlen(help) + 1];
            scopy(help, help + getlen(help), curdcmd->_Help);
            curdcmd->_Help[getlen(help)] = '\0';
            return;
        }
    }
    /*create new key value store*/
    if (!_firstCmd) {
        _firstCmd = new Cmd;
        _lastCmd = _firstCmd;
    }
    else {
        _lastCmd->_nextCmd = new Cmd;
        _lastCmd = _lastCmd->_nextCmd;
    }
    /*set new key*/
    _lastCmd->_Key = new char[getlen(key) + 1];
    scopy(key,key+getlen(key),_lastCmd->_Key);
    _lastCmd->_Key[getlen(key)] = '\0';
    /*set new shortkey*/
    _lastCmd->_SKey = skey;
    /*set reqirement flag*/
    _lastCmd->_Required = required;
    /*set new value*/
    if (defaultvalue) {
        _lastCmd->_Value = new char[getlen(defaultvalue) + 1];
        scopy(defaultvalue, defaultvalue + getlen(defaultvalue),
                           _lastCmd->_Value);
        _lastCmd->_Value[getlen(defaultvalue)] = '\0';
    }
    /*set new help*/
    _lastCmd->_Help = new char[getlen(help) + 1];
    scopy(help, help + getlen(help), _lastCmd->_Help);
    _lastCmd->_Help[getlen(help)] = '\0';
    
}

void libsystempp::CmdController::registerCmd(const char *key, const char skey, bool required, size_t defaultvalue, const char *help) {
    char buf[255];
    itoa(defaultvalue,buf);
    registerCmd(key,skey,required,buf,help);
}

void libsystempp::CmdController::registerCmd(const char *key, const char skey, bool required, int defaultvalue, const char *help) {
    char buf[255];
    itoa(defaultvalue,buf);
    registerCmd(key, skey, required, buf, help);
}

void libsystempp::CmdController::parseCmd(int argc, char** argv){
    for (int args = 1; args < argc; args++) {
        int keytype = -1;
        if (argv[args][0]=='-' && argv[args][1] == '-') {
            keytype = KTKEY;
        }else if (argv[args][0] == '-'){
            keytype = KTSKEY;
        }else {
            break;
        }
        
        size_t kendpos = getlen(argv[args]);
        for (size_t cmdpos = 0; cmdpos < getlen(argv[args])+1; cmdpos++) {	
            switch (argv[args][cmdpos]) {
                case '=': {
                    kendpos = cmdpos;
                };
            }
        }
        
        char *key = NULL;
        char skey = '0';
        if (keytype == KTKEY) {
            key = new char[kendpos-1];
            scopy(argv[args] +2, argv[args] +kendpos, key);
            key[kendpos - 2] = '\0';
        } else if (keytype == KTSKEY){
            skey = argv[args][1];
        }
        
        for (Cmd *curcmd = _firstCmd; curcmd; curcmd = curcmd->nextCmd()) {
            if (keytype == KTKEY) {
                if (ncompare(key,getlen(key),curcmd->getKey(),
                    getlen(curcmd->getKey())) == 0) {
                    curcmd->_Found = true;
                    int valuesize = (getlen(argv[args]) - (kendpos+1));
                    if (valuesize > 0) {
                        delete[] curcmd->_Value;
                        curcmd->_Value = new char[valuesize+1];
                        scopy(argv[args]+(kendpos+1), argv[args] + getlen(argv[args]),
                                           curcmd->_Value);
                        curcmd->_Value[valuesize] = '\0';
                    }
                }
            } else if (keytype == KTSKEY) {
                if (curcmd->getShortkey()== skey) {
                    curcmd->_Found = true;
                    if (++args<argc) {
                        int valuesize = getlen(argv[args]);
                        delete[] curcmd->_Value;
                        curcmd->_Value = new char[valuesize + 1];
                        scopy(argv[args], argv[args] + getlen(argv[args]), 
                                           curcmd->_Value);
                        curcmd->_Value[valuesize] = '\0';
                    }
                }
            }
        }
        
        delete[] key;
    }
}

bool libsystempp::CmdController::checkRequired() {
    for (Cmd *curdcmd = _firstCmd; curdcmd; curdcmd = curdcmd->nextCmd()) {
        if (curdcmd->getRequired() && !curdcmd->_Found) {
            return false;
        }
    }
    return true;
}

void libsystempp::CmdController::printHelp() {
    for (Cmd *curdcmd = _firstCmd; curdcmd; curdcmd = curdcmd->nextCmd()) {
        Console[SYSOUT] << "--" << curdcmd->getKey() 
                                     << " -" << curdcmd->getShortkey()
                                     << " "  << curdcmd->getHelp() 
                                     << Console[SYSOUT].endl;
    }
}

libsystempp::Cmd *libsystempp::CmdController::getCmdbyKey(const char *key) {
    for (Cmd *curdcmd = _firstCmd; curdcmd; curdcmd = curdcmd->nextCmd()) {
        if (ncompare(key,getlen(key),curdcmd->getKey(),getlen(curdcmd->getKey())) == 0) {
            return curdcmd;
        }
    }
    return nullptr;
}

libsystempp::CmdController::~CmdController() {
    delete _firstCmd;
    _lastCmd = nullptr;
}
