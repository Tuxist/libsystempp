
#include "include/fs.h"

// libsytempp::MountPoint::MountPoint(){
//     _nextMountPoint=nullptr;
// }
// 
// libsytempp::MountPoint::~MountPoint(){
// }
// 
// libsytempp::FsInfo::FsInfo(){
//     _firstMountPoint=NULL;
//     _lastMountPoint=NULL;
//     std::fstream mountinfo;
//     mountinfo.open("/proc/self/mountinfo",std::fstream::in);
//     char buffer[1024];
//     if(mountinfo.is_open()){
//         NEXTFSTABLINE:
//         if(mountinfo.getline(buffer,1024)){
//             int entrypos=0,entry=0;
//             bool ne=false;
//             char split[10][FSINFOMAXLEN];
//             for(int i=0; i<1024; ++i){
//                 switch(buffer[i]){
//                     case ' ':{
//                         if(!ne){
//                             split[entry][entrypos]='\0';
//                             ++entry;
//                             entrypos=0;
//                         }
//                         ne=true;
//                     }break;
//                     case '\0':{
//                         MountPoint *curm=addMountpoint();
//                         scopy(split[9],split[9]+FSINFOMAXLEN,curm->_Device);
//                         goto NEXTFSTABLINE;
//                     }break;
//                     default:{
//                         split[entry][entrypos]=buffer[i];
//                         ++entrypos;
//                         ne=false;
//                     }break;
//                 }
//             }
//         }
//     }else{
//         HTTPException httpexception;
//         httpexception[HTTPException::Error] << "FsInfo"
//                                             <<"Could not open Fstab";
//     }
// }
// 
// libsytempp::FsInfo::~FsInfo()
// {
// }
// 
// libsytempp::MountPoint * libsytempp::FsInfo::addMountpoint(){
//     if(_firstMountPoint){
//         _lastMountPoint->_nextMountPoint=new MountPoint();
//         _lastMountPoint=_lastMountPoint->_nextMountPoint;
//     }else{
//         _firstMountPoint=new MountPoint();
//         _lastMountPoint=_firstMountPoint;
//     }
//     return _lastMountPoint;
// }
// 
// libsytempp::MountPoint * libsytempp::FsInfo::getFirstDevice()
// {
//     return _firstMountPoint;
// }
// 
// libsytempp::MountPoint * libsytempp::MountPoint::nextMountPoint(){
//     return _nextMountPoint;
// }
// 
// const char * libsytempp::MountPoint::getDevice(){
//     return _Device;
// }
