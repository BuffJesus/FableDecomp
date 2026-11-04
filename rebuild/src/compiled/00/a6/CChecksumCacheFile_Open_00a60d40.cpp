#include "fable_checksum_cache_file.h"
#include "fable_data_input_stream.h"
#include "fable_disk_file.h"
inline CAFile::CAFile() {}
void CChecksumCacheFile::Open(const CWideString& filename) {
    Filename=filename;
    Entries.clear();
    NeedsSaving=false;
    if(CAFile::PathExists(filename)) {
        CDiskFileWin32 file;
        file.Open(filename,FILE_ACCESS_READ,2);
        CFileDataInputStream input(&file,0x4000);
        if(input.ReadValue<long>()==4) {
            long count=input.ReadValue<long>();
            while(count>0) {
                CWideString path=input.ReadNullTerminatedString().ToWideString();
                CEntry entry;
                entry.TimeStamp=input.ReadDateAndTime();
                entry.Checksum=input.ReadValue<unsigned long>();
                Entries[path]=entry;
                --count;
            }
        }
    }
}
