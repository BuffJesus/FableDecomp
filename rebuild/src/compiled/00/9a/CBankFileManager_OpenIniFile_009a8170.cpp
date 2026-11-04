#include "fable_data_input_stream.h"
#include "fable_memory_file.h"
#include "fable_retail_banks.h"

// CBankFileManager::OpenIniFile(const CWideString&) @ 0x009A8170.
// Shape from the FableWin debug build (0x2FCFA60). Callees by retail bytes:
// 0x9AA7C0 map erase (clear() inlined), 0x99A2F0/0x99A300 CAFile ctor/dtor
// (CMemoryFile's ctor, dtor and GetPathname inlined), 0x9E94A0/0x9E9390
// CMemoryFile::Open/Close, 0x994700/0x994780 CFileDataInputStream,
// 0x994B50 ReadLine, 0x99F560 RemoveWhiteSpaceBeginEnd, 0x995F50
// CMemoryDataInputStream ctor, 0x993B10 its dtor (folded with ~CDataStream),
// 0x994DC0 ReadUntilCharacter, 0x993CA0 ReadWithSrcChunkOverflow (ReadCBYTE
// inlined), 0x9A77D0 ReadQuotedString, 0x993BC0 SetPosition, 0x994E90
// ReadUntilWhiteSpace, 0x99EBF0 CCharString(const char*, long), _stricmp
// (EqualsCaseInsensitive inlined), 0x99E480 ToWideString, 0x99BE70 wide
// operator+, 0x99B7D0/0x99B800 wide operator=, 0x9AC5D0 map operator[],
// 0x997620 CAFile::SplitPath.
//
// Two source facts decide the registers: the keywords reach
// EqualsCaseInsensitive through the implicit literal conversion, and the dev
// bank path is a named local (both make retail address the temporaries from
// their stack slots instead of the call's return register).
void CBankFileManager::OpenIniFile(const CWideString& pathname)
{
    Banks.clear();

    CMemoryFile file;
    file.Open(pathname, FileAccessType(1), 2);

    CWideString retailBankDirectory;
    CWideString devHeaderDirectory;
    CWideString retailHeaderDirectory;

    CFileDataInputStream stream(&file, 0x4000);
    while (!stream.IsEOF())
    {
        CCharString line = stream.ReadLine();
        line.RemoveWhiteSpaceBeginEnd();
        CMemoryDataInputStream lineStream(line);

        bool found;
        CCharString bankName = lineStream.ReadUntilCharacter('=', found);
        if (found)
        {
            lineStream.ReadCBYTE();
            CCharString bankFile;
            if (ReadQuotedString(bankFile, lineStream))
            {
                CCharString headerFile;
                CWideString devHeaderPath;
                CWideString retailHeaderPath;
                if (ReadQuotedString(headerFile, lineStream) &&
                    headerFile.GetLength() > 0)
                {
                    devHeaderPath =
                        devHeaderDirectory + headerFile.ToWideString();
                    retailHeaderPath =
                        retailHeaderDirectory + headerFile.ToWideString();
                }
                CWideString devBankFilePath = bankFile.ToWideString();
                Banks[bankName] = CBankInfo(
                    devBankFilePath,
                    retailBankDirectory,
                    devHeaderPath,
                    retailHeaderPath);
            }
        }
        else
        {
            lineStream.SetPosition(0);
            CCharString keyword = lineStream.ReadUntilWhiteSpace();
            if (keyword.EqualsCaseInsensitive("BeginRetailFile"))
            {
                CCharString directory;
                ReadQuotedString(directory, lineStream);
                retailBankDirectory = directory.ToWideString();
            }
            else if (keyword.EqualsCaseInsensitive("EndRetailFile"))
            {
                retailBankDirectory = L"";
            }
            else if (keyword.EqualsCaseInsensitive("DevHeaderDirectory"))
            {
                CCharString directory;
                ReadQuotedString(directory, lineStream);
                devHeaderDirectory = directory.ToWideString();
            }
            else if (keyword.EqualsCaseInsensitive("RetailHeaderDirectory"))
            {
                CCharString directory;
                ReadQuotedString(directory, lineStream);
                retailHeaderDirectory = directory.ToWideString();
            }
        }
    }

    CWideString iniPathname = file.GetPathname();
    CWideString directory;
    CAFile::SplitPath(iniPathname, 0, &directory, 0, 0);
    BasePath() = directory;
}
