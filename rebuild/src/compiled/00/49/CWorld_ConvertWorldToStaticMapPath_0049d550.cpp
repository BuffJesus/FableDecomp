// CWorld::ConvertWorldToStaticMapPath(worldName, xbox, useRetailBanks), retail 0x0049D550
// (FableWin ?ConvertWorldToStaticMapPath@CWorld@@QBE?AVCWideString@@ABV2@_N1@Z):
// <static map dir><world file name><suffix>, the suffix picking the retail-bank
// ("_RT") and Xbox ("_XB") variants of the .stb file.
#include "fable_world.h"
#include "fable_filesystem.h"
#include "fable_resource_directories.h"

CWideString CWorld::ConvertWorldToStaticMapPath(const CWideString& worldName, bool xbox, bool useRetailBanks) const
{
    CWideString name;
    CAFile::SplitPath(worldName, 0, 0, &name, 0);
    if (useRetailBanks)
    {
        if (xbox)
            name = NResourceDirectoryNames::GetStaticMapDir() + name + L"_RT_XB.stb";
        else
            name = NResourceDirectoryNames::GetStaticMapDir() + name + L"_RT.stb";
    }
    else
    {
        if (xbox)
            name = NResourceDirectoryNames::GetStaticMapDir() + name + L"_XB.stb";
        else
            name = NResourceDirectoryNames::GetStaticMapDir() + name + L".stb";
    }
    return name;
}