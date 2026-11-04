#include <stdlib.h>
#include <new>
#include "fable_checksum_cache_file.h"
#include "fable_disk_file.h"
#include "fable_crc.h"
inline CAFile::CAFile() {}
unsigned long CChecksumCacheFile::GetChecksum(const CWideString& filename) {
    if(!CAFile::PathExists(filename))return 0;
    _STL::map<CWideString,CEntry>::iterator found=Entries.find(filename);
    CDateAndTime time;
    if(found!=Entries.end()) {
        time=CAFile::GetFileModifiedTime(filename);
        if(time==found->second.TimeStamp)return found->second.Checksum;
    } else time=CAFile::GetFileModifiedTime(filename);
    unsigned char* buffer=new unsigned char[BufferSize];
    unsigned long crc=0xffffffff;
    CDiskFileWin32 file;
    file.Open(filename,FILE_ACCESS_READ,2);
    long length=file.GetLength();
    for(long offset=0;offset<length;offset+=BufferSize) {
        long size=BufferSize < length-offset ? BufferSize : length-offset;
        file.Read(buffer,size,0);
        crc=FableCRC_Calc_004014A0(crc,buffer,size);
    }
    crc=~crc;
    delete[] buffer;
    CEntry entry;
    entry.Checksum=crc;entry.TimeStamp=time;
    Entries[filename]=entry;
    NeedsSaving=true;
    return crc;
}
