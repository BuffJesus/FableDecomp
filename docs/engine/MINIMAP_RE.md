# Minimap subsystem RE (byte-pure reconstruction feed)

Reverse-engineering the retail minimap render path so it can be reconstructed
byte-exact (the north-star follow-up to the functional `frontend_minimap.cpp`
oracle — see docs/journal/HANDOFF_ARCHIVE.md and memory `byte-purity-policy`). Small accessors
are authored + locally verified with `verify_and_land.py` (no workflow agents).

## Landed (byte-exact) — 14 functions
Hand-authored (local verify_and_land):
- **`CDrawMiniMap::ClearPrimitivesIfNecessary` @ 0x00643de5** (RELOCATION_MATCH):
  `if (this->f_18 == 0) this->sub_0x28.Clear(); if (this->f_eb == 0) this->vslot4();`
  — the second (void member) call is the LAST statement so VC7.1 tail-jmps it
  (`mov ecx,esi; pop esi; jmp [vptr+4]`). Needs `#pragma optimize("s",on)`.
- **`CDrawMiniMap::SetRegion` @ 0x0064dfde** (RELOCATION_MATCH): forwarder →
  disp.SetRegionDisp(r) / (if ds:0x13b8780) disp.InitialiseOnRegionLoad(f_c+0x84)
  / this->ApplyRegion(r).

Landed via the minimap decomp workflow (wg5w1pyu3): SetAsActive (MATCH),
Initialise, InitialiseSurface, InitialiseOnRegionLoad, InitialiseRegionMinimapSize,
GetRelativePosOnMap, GetRelativeMapSize, GetRingAlpha, RemoveAllDeadMarkers,
ClearPrimitiveHandles (×2 @0xa46d90/0x443cf0), **LoadRegionMinimapTGAFile**
(the byte-pure region-TGA asset read — high value).

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
Remaining small minimap fns are all the DEFERRED codegen-idiom class (FPU x87
schedule / shared-xor flag idiom that VC7.1 RTM 3077 won't emit): IsActive
0x643e09, IsFinishedClosing 0x6441be, IsFinishedOpening 0x644179, SetMarkerGraphic
0x82a340, GetNonAutoMarkers 0x82a390, UpdateMarkersInternal 0x82a960,
InitialiseRegionBox 0x66c7b0, GetWorldCoordinatesFromTGAPosition 0x66c810,
GetRegionMapExtents 0x49c700, SetRegionMapExtents 0x66c630, ToggleCentering
0x64e019. Semantics recovered (behaviour PASS) — need a full-regalloc permuter or
QFE-4035 compiler to close the byte match.

Next frontend RE lane: the 2D-render core (CShaderRenderManager 56 fns,
CEnginePrimitiveRenderer2D, CEnginePrimitive2DViewportManager) — the byte-pure
Render2D path underlying the whole frontend.

## Frontend lane increment (2026-08-16, round 2)
- **`CEnginePrimitiveManager2DSprites::Render2DPrimitive` @ 0x00baced0** (MATCH):
  virtual-dispatch forwarder — `if (this->active) { prim->f8==0x23 ? vslot13 :
  vslot14 }(prim, chain, subtype)`. Modeled the two targets as virtuals at slots
  13/14 (13 padding virtuals) so VC7.1 emits `call [vptr+0x34]` / `[vptr+0x38]`
  thiscall with no edx. Fields: active byte @+0x08; prim->f8 @+0x08.
DEFERRED (compiler-gated aliasing class, 24-instance clone family):
- **`std::_Move<CCountedPointer<CGuiVarTransferBase>*>` @ 0x00448300** (+23 more:
  535000 6962c0 6f1410 7646c0 764bc0 7f3ce0 84f330 8b2140 8b25c0 a2b340 a46460
  a5fda0 a95c00 a95d90 a96430 aa8f40 …). Semantics fully recovered (behaviour
  PASS) = std::copy over CCountedPointer with release(old)/addref(new):
  `for (n=last-first; n>0; --n,++first,++dest) if (dest->ctr!=first->ctr){
  release dest->ctr (--count==0 → ctr->destroy(ctr->self); operator delete);
  dest->obj=first->obj; dest->ctr=first->ctr; if(dest->ctr) ++count; }`.
  Counter{count@0, destroy@4 (thiscall via ecx=self@8)}; CountedPtr{obj@0,ctr@4};
  the two trailing STL iterator-category tags make it `ret 0xC` (3 stack args).
  BLOCKER: retail RELOADS `dest->ctr` from memory 3× (conservative aliasing —
  it treats `--dest->ctr->count` as possibly clobbering the pointer); VC7.1 RTM
  3077 at /O2 caches it in a register (DIFFER 102v115). The /GS,/Oa flag sweep
  can't force *more* reloads. Needs QFE-4035 or a full-regalloc permuter. Best
  source in scratchpad. If ever landed, clone-sweep the 23 via harvest_skeleton.py.

## Frontend lane increment (2026-08-16)
Landed 2 byte-exact Render2D/frontend fns (local verify_and_land, artifacts on
disk in rebuild/src+tests + build_candidates.ps1 catalog + oracle rows; NOT yet
committed — working tree was mid-flight from a concurrent crawl session rewriting
`auto-re-candidates.tsv`, so a clean scoped commit was deferred to avoid
entangling that work):
- **`CEnginePrimitive2DViewportManager::RenderPrimitive` @ 0x00be1b90** (MATCH):
  bare virtual stub `xor eax,eax / ret 0xC` — `ERendered(prim&, layer, chain&)`
  returning 0.
- **`CTCInGameMenu::Construct` @ 0x004d559d** (RELOCATION_MATCH, `/O1 s`): factory
  `p = operator new(0xF8); if (p) return p->Ctor(parent); return 0;`. Modeled the
  ctor as a real member (implicit __thiscall) so no `xor edx,edx` is emitted, and
  the value-returning forwarder keeps retail's `call;ret`.
DEFERRED (compiler-gated, same-length DIFFER 47v47) — the QFE-4035 class:
- **`CNewFrontendGameComponent::Init` @ 0x0042f75e**: semantics fully recovered
  (behaviour PASS) = `g_frontendComponent=this; Step1(); Step2();
  this->f_b1=1; s=MakeSub(0x60,0x29,this->f_10); s->Activate();`. VC7.1 RTM 3077
  tail-jmps the final void member call (`pop esi; jmp`) and load-hoists the
  `push [esi+0x10]` arg; retail (QFE-4035) keeps `mov ecx,eax; call; pop esi; ret`
  and pushes the memory operand directly. Not recoverable under RTM — needs the
  permuter or QFE compiler. Fields: field10 @+0x10, initialised byte @+0xb1;
  global `g_frontendComponent` @ 0x013b871c.
Remaining small targets in these classes are destructors (fragile member-cleanup
chains) or 900B+ fns; Render2DPrimitive (factory+FPU), RenderSubPrimitive
(dead-store local idiom), GetSupportedTypes (STL vector push_back) are the
fragile idiom class — defer.

## Frontend lane resume point (2026-08-14 EOD)
Session landed 29 frontend byte-exact fns: 14 minimap + 15 Render2D
(CShaderRenderManager/CEnginePrimitiveRenderer2D/CShaderResource). Commits
717b711, 9208c2b, 74d6201 (+ SetRegion in d073a48).
WALL HIT: CGuiControlTreePane's 202 "SortTreeRecursively" are a mislabeled
std::list::clear clone family — loop-alignment + regalloc idiom sensitive
(got 47v47 same-length but not byte-exact), same class the crawl defers. Skip it.
RESUME lane 2 at the real frontend UI logic (not STL-clone families):
CTCInGameMenu (22), CNewFrontendGameComponent (9), CFrontEndManager (8),
CEnginePrimitive2DViewportManager (13), remaining CShaderRenderManager setters.
Recipe: build targets+oracle (Render2D python in scratchpad), run the bpc_*.js
minimap/render2d-style workflow, extract/recover-placeholders/land vs oracle.
