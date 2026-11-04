// CWorld::ConvertWorldToStaticMapPath(worldName), retail 0x0049DDD0
// (FableWin ?ConvertWorldToStaticMapPath@CWorld@@QBE?AVCWideString@@ABV2@@Z): the PC path, picking the
// retail-bank variant by GUseRetailBanks (retail 0x013B8616). Retail inlines the three-argument overload
// (out of line at its own address in the same TU); it is repeated here `inline`
// so this object does not define that overload's symbol a second time.
#include "fable_retail_banks.h"
#include "fable_world.h"
#include "fable_filesystem.h"
#include "fable_resource_directories.h"

inline CWideString CWorld::ConvertWorldToStaticMapPath(const CWideString& worldName, bool xbox, bool useRetailBanks) const
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

CWideString CWorld::ConvertWorldToStaticMapPath(const CWideString& worldName) const
{
    return ConvertWorldToStaticMapPath(worldName, false, GUseRetailBanks);
}