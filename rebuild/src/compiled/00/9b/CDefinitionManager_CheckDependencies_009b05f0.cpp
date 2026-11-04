// CDefinitionManager::CheckDependencies(const vector<CWideString>&) @ 0x009B05F0
// (FableWin ?CheckDependencies@CDefinitionManager@@IAEXABV?$vector@VCWideString@@...@Z).
// With checksums on, every file the path list's patterns match is checksummed
// (through the ".cks" cache beside the compiled file), keyed by its lower-case
// name; the sorted checksums fold into DependencyCheckCRC. Otherwise the CRC
// stays 0.
#include <new>
#include "fable_definition_manager.h"
#include "fable_checksum_cache_file.h"
#include "fable_crc.h"
#include "fable_filesystem.h"
#include "fable_stlport_vector_impl.h"

void CDefinitionManager::CheckDependencies(const _STL::vector<CWideString>& pathList)
{
    DependencyCheckCRC = 0;
    if (!CreateChecksums)
        return;

    CWideString drive;
    CWideString directory;
    CWideString filename;
    CAFile::SplitPath(CompiledFileName, &drive, &directory, &filename, 0);
    CWideString checksumPath = drive + directory + filename + L".cks";
    CChecksumCacheFile checksumCache(0x10000);
    checksumCache.Open(checksumPath);

    CVectorMap<CCharString, unsigned long, CKeyPairCompareLess<CCharString, unsigned long> > checksums;
    for (const CWideString* pattern = pathList.begin(); pattern != pathList.end(); ++pattern)
    {
        CWideString pathname;
        CAFile::CFileScanInfo scanInfo;
        if (CAFile::StartFileScan(*pattern, scanInfo, pathname))
        {
            do
            {
                unsigned long checksum = checksumCache.GetChecksum(pathname);
                checksums.Insert(pathname.ToCharString().GetAsLowerCase(), checksum);
            } while (CAFile::ScanNextFile(*pattern, scanInfo, pathname));
        }
    }
    checksums.Update();

    DependencyCheckCRC = 0;
    for (unsigned long index = 0; index < checksums.size(); ++index)
    {
        unsigned long checksum = checksums[index].second;
        DependencyCheckCRC = CCRC::Calc(DependencyCheckCRC, &checksum, sizeof(checksum));
    }
}