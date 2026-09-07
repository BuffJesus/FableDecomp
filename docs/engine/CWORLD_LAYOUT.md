# CWorld object layout (recovered from the ctor 0x004a67d0)

The `CWorld` object created by `CMainGameComponent::InitWorld` (docs/engine/GAME_ENTRY_PATHS.md#new-game-path-child-prologue).
Fully decoded from the 727B ctor `CWorld::CWorld(WorldCtx&) @ 0x004a67d0` (__thiscall,
returns this, ret 4). Object size = **0x198 (408 bytes)** (matches InitWorld's
`operator new(0x198)`). This is the field map; the ctor itself is a byte-parity **DEFER**
(see bottom).

## Construction sequence
1. `call 0x99a2f0` — **CBase base-class ctor**.
2. **Two vtable pointers** (multiple inheritance): `[+0] = 0x12390f0`, `[+4] = 0x12390dc`
   (with an intermediate `[+4] = 0x1238b78` write — the MI construction-vtable dance).
3. Copy 3 dwords from the WorldCtx arg: `[+8] = ctx[0]`, `[+0xc] = ctx[8]`, `[+0x10] = ctx[0xc]`.
4. Zero `[+0x14 .. +0x7c]` (26 dwords).
5. `CCharString` sub-object @ **+0x84** (`byte[+0x80]=0` first; ctor 0x99e4b0).
6. Doubles: `[+0x90] = <const 0x12390d0>`, `[+0x98] = <const 0x122ed70>`; `[+0x88]=[+0x8c]=0`.
7. Zero `[+0xa0]`, bytes `[+0xa4..+0xa8]=0`, dwords `[+0xac..+0xd0]=0`.
8. Bool block `[+0xd4..+0xe1]`: mostly **1**, with **0** at +0xd8, +0xdc; `byte[+0xe2]=0`.
9. `[+0xe4]=0`, `[+0xe8] = 0xffffffff`, `[+0xf8]=0`.
10. `CCharString` sub-object @ **+0xfc** (default ctor 0x99aed0).
11. Bytes `[+0x100..+0x103]` (=1 at +0x103), zero `[+0x104..+0x128]`, bytes `[+0x12c..+0x133]`
    (=1 at +0x12f).
12. `[+0x134]=[+0x138]=0`.
13. `CCharString` sub-object @ **+0x13c** (default ctor 0x99aed0).
14. Zero `[+0x140..+0x170]`.
15. **`[+0x174]` = a 0x14-byte node** `malloc(0x14)` (0xbfea0e), self-linked
    (`node->next = node->prev = node`) — an empty intrusive-list sentinel.
16. `[+0x178]=[+0x17c]=0`, `byte[+0x190]=0`.
17. `[+0x180] = GFGetTime()` (double, 0x9e1bc0) — creation timestamp.
18. `[+0x188] = <const 0x122ed70>` (double).
19. return this.

## Notable members
- `+0x84 / +0xfc / +0x13c` — three `CCharString`s (names/paths?).
- `+0x90 / +0x98 / +0x188` — doubles (two = const 0x122ed70; likely default rates/scales).
- `+0x180` — creation time (`GFGetTime`).
- `+0x174` — intrusive-list sentinel (a self-linked 0x14 node; the world's entity/region list head?).
- `+0xd4..+0xe1` — a run of ~14 bool flags (default-on except two).
- `+0xe8 = -1` — a slot/index initialised to "none".
- `+0x8`, `+0xc`, `+0x10` — copied from the InitWorld context (comp/f1c/defmgr-derived).

## Parity status: DEFER (MI-ctor tier)
Byte-exact reconstruction is blocked on the **two-vptr multiple-inheritance construction
sequence** (the intermediate `[+4]` vtable write + base-ctor ordering) and the exact
compiler ordering of the ~100 member inits interleaved with 3 sub-object ctors — not
cleanly source-reproducible under VC7.1. Semantics are fully recovered (above). Revisit
if a register/construction-level tool lands, or model the exact CBase/MI hierarchy. Next
chain link after this: `CWorld::PostInit @ 0x4a6550` (625B) and the level stream
(`CWorldMap::LoadLevel @ 0x502620`).
