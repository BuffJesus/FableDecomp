// CWorld::ConvertWorldToISMThingyPath(worldName, xbox, useRetailBanks), retail 0x0049D6B0
// (FableWin ?ConvertWorldToISMThingyPath@CWorld@@QBE?AVCWideString@@ABV2@_N1@Z):
// <static map dir><world file name><suffix>, the suffix picking the retail-bank
// ("_RT") and Xbox ("_XB") variants of the .ism file.
#include "fable_world.h"
#include "fable_filesystem.h"
#include "fable_resource_directories.h"

CWideString CWorld::ConvertWorldToISMThingyPath(const CWideString& worldName, bool xbox, bool useRetailBanks) const
{
    CWideString name;
    CAFile::SplitPath(worldName, 0, 0, &name, 0);
    if (useRetailBanks)
    {
        if (xbox)
            name = NResourceDirectoryNames::GetStaticMapDir() + name + L"_RT_XB.ism";
        else
            name = NResourceDirectoryNames::GetStaticMapDir() + name + L"_RT.ism";
    }
    else
    {
        if (xbox)
            name = NResourceDirectoryNames::GetStaticMapDir() + name + L"_XB.ism";
        else
            name = NResourceDirectoryNames::GetStaticMapDir() + name + L".ism";
    }
    return name;
}