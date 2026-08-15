# Minimap subsystem RE (byte-pure reconstruction feed)

Reverse-engineering the retail minimap render path so it can be reconstructed
byte-exact (the north-star follow-up to the functional `frontend_minimap.cpp`
oracle — see docs/HANDOFF.md and memory `byte-purity-policy`). Small accessors
are authored + locally verified with `verify_and_land.py` (no workflow agents).

## Landed (byte-exact)
- **`CDrawMiniMap::ClearPrimitivesIfNecessary` @ 0x00643de5** (RELOCATION_MATCH):
  `if (this->f_18 == 0) this->sub_0x28.Clear(); if (this->f_eb == 0) this->vslot4();`
  — the second (void member) call is the LAST statement so VC7.1 tail-jmps it
  (`mov ecx,esi; pop esi; jmp [vptr+4]`), matching retail. Sub-helper at 0x82a1d0
  is a thiscall method on the subobject at this+0x28. Needs in-source
  `#pragma optimize("s",on)`.

## Semantics recovered, byte-match DEFERRED (flag-comparison idiom class)
These behave identically to retail (verify_and_land behaviour PASS) but VC7.1 RTM
3077 emits `mov al,[mem]; test al,al` / `setne` where retail hoists a single
`xor eax,eax` and reuses `al=0` as the `cmp byte[mem], al` operand across every
branch (the shared-false-zero idiom). Same class the parity crawl defers.

- **`CDrawMiniMap::IsActive` @ 0x00643e09** → bool:
  ```
  if (this->f_eb) return this->f_100;          // returns raw byte at +0x100
  if (this->f_e9 == 0) return false;
  if (this->f_ec == 0) return false;           // unsigned (retail: jbe)
  return true;
  ```
  Fields: f_e9 @+0xe9, f_eb @+0xeb, f_ec @+0xec, f_100 @+0x100 (all bool/byte).
- **`CDrawMiniMap::SetAsActive(bool a)` @ 0x00643e2e** → void, ret 4:
  ```
  this->f_e9 = a;
  if (!a && this->sub_0x100.f0) {              // sub-object at +0x100
      this->sub_0x100.fd /*+0x10d*/ = 0;
      this->sub_0x100.fc /*+0x10c*/ = 1;
  }
  ```
  Retail bases the +0x100 accesses off `lea eax,[ecx+0x100]` (subobject pointer).

## Remaining small minimap targets (un-landed, complete-proto)
CDrawMiniMap: IsFinishedClosing 0x6441be, IsFinishedOpening 0x644179, SetRegion
0x64dfde, GetRingAlpha 0x643e52, ToggleCentering 0x64e019.
CRegionMinimap: InitialiseSurface 0x66c750, LoadRegionMinimapTGAFile 0x66ca80,
GetWorldCoordinatesFromTGAPosition 0x66c810, GetRegionMapExtents 0x49c700,
SetRegionMapExtents 0x66c630, InitialiseRegionBox 0x66c7b0.
CMiniMapDisplay: Initialise 0x82a310, GetRelativePosOnMap 0x829ce0,
SetMarkerGraphic 0x82a340, InitialiseOnRegionLoad 0x82a180, GetNonAutoMarkers
0x82a390, UpdateMarkersInternal 0x82a960, RemoveAllDeadMarkers 0x82a2a0,
GetRelativeMapSize 0x829c70, ClearPrimitiveHandles 0xa46d90 / 0x443cf0.
`CRegionMinimap::LoadRegionMinimapTGAFile` is the highest-value next target (the
byte-pure region-TGA asset read the functional oracle currently hand-ports).
