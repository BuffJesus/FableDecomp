#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_checksum_cache_file.h"
#include "fable_file_data_output_stream.h"
#include "fable_disk_file.h"
class CFileInformation { public: unsigned char Bytes[0x58]; };
static CChecksumCacheFile* cache;
static unsigned opens, closes, streamDtors, existsCalls, stringWrites, dateWrites, crcWrites, signedWrites, seeks;
static long signedValues[4]; static unsigned long crcValues[3];
static const CDateAndTime* dates[3];
static void require(bool condition) { if (!condition) { puts("fixture mismatch"); exit(1); } }
CBaseClass::CBaseClass() {} CBaseClass::~CBaseClass() {}
CBaseClassNonCopyable::CBaseClassNonCopyable() {} CBaseClassNonCopyable::~CBaseClassNonCopyable() {}
CWideString::CWideString() : storage_(0) {}
CWideString::CWideString(const CWideString& other) : storage_(other.storage_) {}
CWideString::~CWideString() {}
CCharString::CCharString() : storage_(0) {}
CCharString::CCharString(const char*, long) : storage_(0) {}
CCharString::CCharString(const CCharString& other) : storage_(other.storage_) {}
CCharString::~CCharString() {}
CCharString CWideString::ToCharString() const { return CCharString("entry", 5); }
CDateAndTime::CDateAndTime() { memset(this, 0, sizeof(*this)); }
CFileInformation CAFile::GetFileInformation() { CFileInformation value; memset(&value, 0, sizeof(value)); return value; }
bool __fastcall CAFile::PathExists(const CWideString&) { return ++existsCalls != 2; }
void CDiskFileWin32::Open(const CWideString& path, FileAccessType access, unsigned long flags) {
    require(&path == &cache->Filename && access == FILE_ACCESS_CREATE && flags == 2);
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
CFileDataOutputStream::CFileDataOutputStream(CAFile* file) : File(file) { require(file->IsOpen()); }
CFileDataOutputStream::~CFileDataOutputStream() { ++streamDtors; }
void CFileDataOutputStream::SetPosition(unsigned long value) { require(value == 17); ++seeks; }
unsigned long CFileDataOutputStream::GetPosition() const { return 17; }
void CFileDataOutputStream::Close() { abort(); }
void CFileDataOutputStream::Write(const void*, long) { abort(); }
void CDataOutputStream::WriteSLONG(long value) { require(signedWrites < 4); signedValues[signedWrites++] = value; }
void CDataOutputStream::WriteULONG(unsigned long value) { require(crcWrites < 3); crcValues[crcWrites++] = value; }
void CDataOutputStream::WriteDateAndTime(const CDateAndTime& value) { require(dateWrites < 3); dates[dateWrites++] = &value; }
void CDataOutputStream::WriteNullTerminatedString(const CCharString&) { ++stringWrites; }
// Test entry into the private Save method; no cleanup is modeled here.
void CChecksumCacheFile::Close() { Save(); }
typedef _STL::map<CWideString, CChecksumCacheFile::CEntry>::value_type CacheValue;
typedef _STL::_Rb_tree_node<CacheValue> CacheNode;
int main() {
    for (unsigned mode = 0; mode < 3; ++mode) {
        unsigned char raw[sizeof(CChecksumCacheFile)]; memset(raw, 0, sizeof(raw));
        cache = reinterpret_cast<CChecksumCacheFile*>(raw);
        _STL::_Rb_tree_node_base header; memset(&header, 0, sizeof(header));
        header._M_left = header._M_right = &header;
        cache->Entries._M_t._M_header._M_data = reinterpret_cast<CacheNode*>(&header);
        cache->NeedsSaving = mode != 0;
        CacheNode nodes[3];
        if (mode == 2) {
            for (unsigned i = 0; i < 3; ++i) {
                nodes[i]._M_left = nodes[i]._M_right = 0;
                nodes[i]._M_parent = &nodes[1];
                nodes[i]._M_value_field.second.Checksum = 100 + i;
            }
            nodes[1]._M_left = &nodes[0]; nodes[1]._M_right = &nodes[2]; nodes[1]._M_parent = &header;
            header._M_parent = &nodes[1]; header._M_left = &nodes[0]; header._M_right = &nodes[2];
            cache->Entries._M_t._M_node_count = 3;
        }
        opens = closes = streamDtors = existsCalls = stringWrites = dateWrites = crcWrites = signedWrites = seeks = 0;
        cache->Close();
        if (mode == 0) require(!opens && !closes && !signedWrites && !existsCalls && !streamDtors);
        else {
            require(opens == 1 && closes == 2 && streamDtors == 1 && seeks == 1 && !cache->NeedsSaving);
            require(signedWrites == 4 && signedValues[0] == 0 && signedValues[1] == 0 && signedValues[2] == 4 && signedValues[3] == (mode == 2 ? 2 : 0));
            require(existsCalls == (mode == 2 ? 3 : 0) && stringWrites == (mode == 2 ? 2 : 0) && dateWrites == stringWrites && crcWrites == stringWrites);
            if (mode == 2) require(crcValues[0] == 100 && crcValues[1] == 102 && dates[0] == &nodes[0]._M_value_field.second.TimeStamp && dates[1] == &nodes[2]._M_value_field.second.TimeStamp);
            cache->Close(); require(opens == 1 && signedWrites == 4);
        }
    }
    puts("CHECKSUM_CACHE_SAVE_00a60aa0_PASS"); return 0;
}
