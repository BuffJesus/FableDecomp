#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_checksum_cache_file.h"
#include "fable_data_input_stream.h"
#include "fable_disk_file.h"
class CFileInformation {public: unsigned char Bytes[0x58];};
static CChecksumCacheFile* cache;
static const CWideString* inputName;
static unsigned opens,closes,existsCalls,streamDtors,erases,inserts,overflows,finalPosition;
static bool exists,overflowMode;
static unsigned char blob[128];static unsigned blobSize;
static void require(bool value) {if(!value){puts("cache open fixture mismatch");exit(1);}}
CBaseClass::CBaseClass() {} CBaseClass::~CBaseClass() {}
CBaseClassNonCopyable::CBaseClassNonCopyable() {} CBaseClassNonCopyable::~CBaseClassNonCopyable() {}
CWideString::CWideString() : storage_(0) {}
CWideString::CWideString(const CWideString& other) : storage_(other.storage_) {}
CWideString::~CWideString() {}
CCharString::CCharString() : storage_(0) {}
CCharString::CCharString(const char* text, long) : storage_(reinterpret_cast<CCharStringData*>(const_cast<char*>(text))) {}
CCharString::CCharString(const CCharString& other) : storage_(other.storage_) {}
CCharString::~CCharString() {}

CDateAndTime::CDateAndTime() { memset(this, 0, sizeof(*this)); }
CFileInformation CAFile::GetFileInformation() { CFileInformation value; memset(&value, 0, sizeof(value)); return value; }
bool __fastcall CAFile::PathExists(const CWideString& name) { require(&name==inputName); ++existsCalls; return exists; }
void CDiskFileWin32::Open(const CWideString& path, FileAccessType access, unsigned long flags) {
    require(&path == inputName && access == FILE_ACCESS_READ && flags == 2);
    require(!Opened); Opened = true; ++opens;
}
void CDiskFileWin32::Close() { Opened = false; ++closes; }
void CDiskFileWin32::Read(void*, long, CASuspendableProcess*) { abort(); }
void CDiskFileWin32::Write(const void*, long, CASuspendableProcess*) { abort(); }
void CDiskFileWin32::SetPosition(long) { abort(); }
void CDiskFileWin32::MovePosition(long) { abort(); }
long CDiskFileWin32::GetPosition() const { abort(); return 0; }
void CDiskFileWin32::SetLength(long) { abort(); }
CAFile* CDiskFileWin32::Clone() const { abort(); return 0; }
CFileInformation CDiskFileWin32::GetFileInformation() { CFileInformation value; memset(&value, 0, sizeof(value)); return value; }

CWideString& CWideString::operator=(const CWideString& other) {storage_=other.storage_;return *this;}
CWideString CCharString::ToWideString() const {CWideString value;*reinterpret_cast<void**>(&value)=storage_;return value;}
CDataInputStream::CDataInputStream() : StreamPos(0),StreamSize(0),CurrentSourcePtr(0),SourceChunkStreamPos(0),SourceChunkBytesRemaining(0) {}
void CDataInputStream::SetPosition(unsigned long) {abort();}
void CDataInputStream::Close() {abort();}
void CDataInputStream::ReadCompressed(void*,long) {abort();}
void CDataInputStream::ReadZlibCompressed(void*,long) {abort();}
CWideString CDataInputStream::ReadNullTerminatedWString() {abort();}
void CDataInputStream::InternalReadNextSourceChunk(const fable_u8*&,long&) {abort();}
bool CDataInputStream::NeedsBufferedRead(long) {abort();return false;}
void CDataInputStream::InternalReadUnbuffered(fable_u8*,long) {abort();}
void CFileDataInputStream::Close() {abort();}
void CFileDataInputStream::InternalReadNextSourceChunk(const fable_u8*&,long&) {abort();}
bool CFileDataInputStream::NeedsBufferedRead(long) {abort();return false;}
void CFileDataInputStream::InternalReadUnbuffered(fable_u8*,long) {abort();}
CFileDataInputStream::CFileDataInputStream(CAFile* file,unsigned long size) : File(file),Buffer(0),BufferSize(size) {
    require(file->IsOpen() && size==0x4000);StreamSize=blobSize;
    if(!overflowMode){CurrentSourcePtr=blob;SourceChunkBytesRemaining=blobSize;}
}
CFileDataInputStream::~CFileDataInputStream() {++streamDtors;finalPosition=StreamPos;require(File->IsOpen());}
void CDataInputStream::ReadWithSrcChunkOverflow(void* data,long size) {
    require(overflowMode && size>0 && StreamPos+size<=blobSize);
    memcpy(data,blob+StreamPos,size);StreamPos+=size;++overflows;
}
CCharString CDataInputStream::ReadNullTerminatedString() {
    unsigned start=StreamPos;while(ReadValue<unsigned char>()!=0)require(StreamPos<blobSize);
    return CCharString(reinterpret_cast<const char*>(blob+start),StreamPos-start-1);
}
CDateAndTime CDataInputStream::ReadDateAndTime() {
    CDateAndTime value;
    value.Year=ReadValue<long>();value.Month=ReadValue<unsigned char>();value.Day=ReadValue<unsigned char>();
    value.Hour=ReadValue<unsigned char>();value.Minute=ReadValue<unsigned char>();value.Second=ReadValue<unsigned short>();value.Millisecond=ReadValue<unsigned short>();
    return value;
}
typedef _STL::map<CWideString,CChecksumCacheFile::CEntry> CacheMap;
typedef CacheMap::_Rep_type CacheTree;
typedef _STL::_Rb_tree_node<CacheMap::value_type> CacheNode;
static CChecksumCacheFile::CEntry recovered[2];
static char recoveredNames[2][16];static unsigned uniqueNames;
template<> void CacheTree::_M_erase(CacheNode* root) {require(this==&cache->Entries._M_t && root==reinterpret_cast<CacheNode*>(0x1234));++erases;}
template<> CChecksumCacheFile::CEntry& CacheMap::operator[](const CWideString& path) {
    require(this==static_cast<CacheMap*>(&cache->Entries));++inserts;
    const char* name=*reinterpret_cast<const char* const*>(&path);
    for(unsigned i=0;i<uniqueNames;++i)if(!strcmp(name,recoveredNames[i]))return recovered[i];
    require(uniqueNames<2 && strlen(name)<16);strcpy(recoveredNames[uniqueNames],name);
    return recovered[uniqueNames++];
}
static void put32(unsigned long value){memcpy(blob+blobSize,&value,4);blobSize+=4;}
static void put16(unsigned short value){memcpy(blob+blobSize,&value,2);blobSize+=2;}
static void record(const char* name,unsigned index) {
    unsigned n=strlen(name)+1;memcpy(blob+blobSize,name,n);blobSize+=n;
    put32(2000+index);blob[blobSize++]=2;blob[blobSize++]=3;blob[blobSize++]=4;blob[blobSize++]=5;put16(6);put16(789);put32(0xA0000000+index);
}
int main() {
    for(unsigned mode=0;mode<7;++mode)for(unsigned buffered=0;buffered<2;++buffered) {
        unsigned char raw[sizeof(CChecksumCacheFile)];memset(raw,0,sizeof(raw));cache=reinterpret_cast<CChecksumCacheFile*>(raw);
        CacheNode header;memset(&header,0,sizeof(header));header._M_parent=reinterpret_cast<CacheNode*>(0x1234);
        cache->Entries._M_t._M_header._M_data=&header;cache->Entries._M_t._M_node_count=1;cache->NeedsSaving=true;cache->BufferSize=37;
        if(mode==6){cache->Entries._M_t._M_node_count=0;header._M_parent=0;header._M_left=header._M_right=&header;}
        CWideString name;*reinterpret_cast<void**>(&name)=reinterpret_cast<void*>(0x5678);inputName=&name;
        opens=closes=existsCalls=streamDtors=erases=inserts=overflows=finalPosition=uniqueNames=0;exists=mode!=0;overflowMode=buffered!=0;
        blobSize=0;put32(mode==1?3:4);put32(mode==2?0:mode==3?0xFFFFFFFF:2);
        if(mode>=4){record("one",0);record(mode==5?"one":"two",1);}
        cache->Open(name);
        require(erases==(mode==6?0:1) && existsCalls==1 && !cache->NeedsSaving && cache->BufferSize==37 && !cache->Entries._M_t._M_node_count);
        require(header._M_parent==0 && header._M_left==&header && header._M_right==&header);
        require(*reinterpret_cast<void**>(&cache->Filename)==reinterpret_cast<void*>(0x5678));
        if(!exists)require(!opens && !closes && !streamDtors);
        else require(opens==1 && closes==1 && streamDtors==1 && finalPosition==(mode==1?4:mode<4?8:blobSize));
        if(mode<4)require(!inserts && !uniqueNames);
        else {
            require(inserts==2 && uniqueNames==(mode==5?1:2));
            unsigned i=mode==5?0:1;require(recovered[i].Checksum==0xA0000001 && recovered[i].TimeStamp.Year==2001 && recovered[i].TimeStamp.Millisecond==789);
            if(mode!=5)require(recovered[0].Checksum==0xA0000000 && recovered[0].TimeStamp.Year==2000);
        }
        require(overflowMode || !overflows);
    }
    puts("CHECKSUM_CACHE_OPEN_00a60d40_PASS");return 0;
}
