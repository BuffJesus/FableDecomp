#pragma once

// CWorldMap / CRegion as CMainGameComponent::Run 0x004189C2 uses them.
//
// Only the non-virtual members retail code calls are declared; CWorldMap's
// layout is not modelled (it is only ever reached by reference). Retail RTTI:
// CWorldMap : CBaseClassNonCopyable, CIDrawWorldMap (+0x04).
//
// CRegion is an element of CWorldMap::Regions (retail +0x2C); retail
// sizeof(CRegion) is 0x58 (CWorldMap::GetRegion 0x004FC180 indexes with
// `imul eax, eax, 0x58`). Ego_r.pdb's is 0x64: its two vector<long> members
// are 12-byte STLport vectors in retail, so PDef sits at +0x24 (Ego_r +0x2C).

#include "fable_game.h"
#include "fable_stlport_vector.h"
#include "rebuild_abi.h"

// Retail RTTI: CRegionDef : CParentDefClassBase : CDefClassBase :
// CDefPointeeBase. Only the lock/unlock base is modelled.
class CRegionDef : public CDefPointeeBase
{
};

class CRegion
{
public:
    // FableWin ?GetPRegionDef@CRegion@@QBE?AV?$CDefPointer@$$CBVCRegionDef@@@@XZ;
    // inline in retail (a CDefPointer copy: lock, and unlock when the copy dies).
    CDefPointer<const CRegionDef> GetPRegionDef() const
    {
        return PDef;
    }

private:
    fable_u8 unknown00_[0x24];              // Maps, VisMaps, Name, DisplayName, RegionDefName
    CDefPointer<const CRegionDef> PDef;     // +0x24
    fable_u8 unknown28_[0x58 - 0x28];
};

class CWorldMap
{
public:
    // FableWin ?GetLoadedRegion@CWorldMap@@QBEJXZ, retail 0x004FB150
    // (`mov eax,[ecx+0x9C]`; the manifest's CWorld::DrawGetEnvironment label
    // is a byte-identical-body propagation).
    long GetLoadedRegion() const;
    // FableWin ?GetRegion@CWorldMap@@QBEABVCRegion@@J@Z / QAEAAV...; retail
    // 0x004FC180 (Regions[index]).
    const CRegion& GetRegion(long index) const;
    CRegion& GetRegion(long index);

    // Retail offsets of the two members the accessors above read (Ego_r has
    // them at +0x34 / +0xBC). Everything else, vptrs included, stays opaque;
    // the retail size is not modelled.
private:
    fable_u8 unknown00_[0x2C];              // vptrs, World, Component, PThingMapManager, LoadedScriptedMapBrushes, Maps
    _STL::vector<CRegion> Regions;          // +0x2C (STLport, 12 bytes)
    fable_u8 unknown38_[0x9C - 0x38];       // RegionTime ... HeightChangedAreas, LoadedMaps
    long LoadedRegion;                      // +0x9C
};

FABLE_STATIC_ASSERT(sizeof(CRegion) == 0x58);
