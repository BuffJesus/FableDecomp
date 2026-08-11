# Xbox `default.xbe` vs PC `Fable.exe` — automated structural diff (2026-08-09)

Both are x86-32, same VC7.1-era toolchain, same source tree (`C:\Dev\BBBProjects\Fable1_5Release\`).
This is the first automated code/structure diff of the two binaries. Reproduce with
`python tools/xbox_re/xbe_pc_diff.py` (stdlib + capstone; reads both files in place).

- PC  `Fable.exe`   : PE32, ImageBase `0x400000`, 16.67 MB, PDB `Fable.pdb`
- Xbox `default.xbe`: XBEH,  BaseAddr   `0x010000`, 10.38 MB, PDB `Ego_xfr.pdb`

## Headline: it is the **same game, a different build configuration** — not a different code path
The two binaries are sibling compiles of one source tree. The differences are almost entirely
**build/config/platform-layer**, not gameplay logic. Four hard, evidence-backed differences:

### 1. Xbox build ships with **RTTI disabled (`/GR-`)**; PC ships with **RTTI on**
- PC emits **2,557** MSVC RTTI type descriptors (`.?AV…@@`); the XBE emits **12**, and all 12 are
  C++ **exception** types (`CExceptionBase`, `CGenericException`, `std::exception`, `std::length_error`,
  `CBBBFileException`, …). MSVC forces type_info for thrown/caught types even under `/GR-`; the total
  absence of vftable-bearing gameplay classes is the exact signature of RTTI-off.
- **Consequence for this project:** the XBE **cannot** be name-ported by our headline lever (RTTI
  vtable-slot identity — `labels_rtti_port.tsv`). This corrects step 1 of `XBOX_DISC_ASSESSMENT.md`:
  RTTI-based matching is dead; only **BSim function-body structural matching** could bridge the two,
  and even that is degraded by items 2–4 below (different inlining, D3D8 vs D3D9, overlay thunks).
  Net: **the XBE is an evidence/reference binary, not a byte-parity oracle for the PC crawl.**

### 2. Code organization: Xbox uses **demand-paged code overlays**; PC is one flat `.text`
- PC: 6 PE sections, a single **14.7 MB `.text`**, everything resident.
- XBE: **135 sections** — a 6.3 MB resident `.text` plus **66 `S_Q*` per-quest** and **43 `S_V*`
  per-subsystem** code overlays, paged from disc via `EnableCodeSectionLoading(true)` (`xuserst.ini`).
  This is the 64 MB-RAM console adaptation; it does not change logic, but it means XBE function
  addresses live in overlay VA ranges and cross-overlay calls route through paging thunks.
- Quest overlays (66): `S_QGT*` (guild-training family), `S_QHS*` (Hero-save family), `S_QBC*`,
  `S_QEG*`, `S_QWB*`, `S_QRV*`, `S_QOFR*`, … — quest logic is **compiled C++, split per quest**.
  Independently re-confirms the PC finding: **no quest VM** (`docs/QUEST_VM_RE.md`).
- Subsystem overlays (43): `S_V*` (e.g. `S_VMR`, `S_VSC`, `S_VTH`, `S_VHB`, `S_VFC`).

### 3. Platform layer differs exactly as expected (console vs PC)
| Area      | Xbox `default.xbe`                                   | PC `Fable.exe`                                  |
|-----------|-----------------------------------------------------|-------------------------------------------------|
| Graphics  | **D3D8** (`D3D8LTCG`, `D3D8I.LIB`), `D3D`/`D3DX`/`XGRPH` overlay sects | **D3D9** (`d3d9.dll`)                  |
| Input     | Xbox gamepad API (no DirectInput)                   | **DirectInput8** (`DirectInput8Create`); `CInputManagerDX`/`CJoystickDX`/`CKeyboardDX`/`CMouseDX` |
| Audio     | `DSOUND`, `WMADEC` (WMA), `DOLBY` overlay sections  | (PC audio libs, no Dolby/WMA sections)          |
| Online    | **Xbox Live**: `XONLINE`/`XNET`/`XON_RD` sections, `XOnlineGameInviteSend`, `*.XBOXLIVE.COM`, UPnP | none (only a dead `UI_XLIVE_DESC` string) |
| Video     | `XMV` (Xbox Media Video) section                    | (PC video path)                                 |

### 4. PC binary carries **dormant Xbox-pad input classes**
Even with RTTI making the class list visible only on the PC side, the PC build **retains** console
input types: `CInputTypeXboxPadButtonEvent`, `CInputTypeXboxPadLeftStickEvent`,
`CInputTypeXboxPadRightStickEvent`. This corroborates the gamepad-redefine lane
(`GAMEPAD_REDEFINE_PATCH.md`): the PC engine still contains the Xbox pad event plumbing, dormant.

## What the XBE is actually good for (revised, evidence-based)
1. **Confirming architecture** (done): no quest VM; per-quest/subsystem code overlays.
2. **The redefine input-order table** — still the concrete unblock. The `EGameAction`→`TEXT_GUI_ACTION_*`
   ordered list lives in code in *both* builds; the XBE `.text` is a clean place to decompile the
   list-builder (xref the action text-IDs from `refs/xbox/headers/text_xbox.h`). Not RTTI-dependent.
3. **Data/content diffs** — `Data/CompiledDefs/{game,names,script,xfrontend}.bin` Xbox vs PC via the
   def loader; frontend glyph banks (`front_end_bank_xbox.h`, `frontend.biz`).
4. **Reference art** for ABXY/thumb-stick glyphs (`frontend.biz`, swizzled).

## What the XBE is NOT good for
- **Not a byte-parity oracle** for the PC decomp crawl (RTTI-off + overlay paging + D3D8 vs D3D9 ⇒
  different inlining and different platform calls; function bodies will not byte-match PC).
- **Not** a source of the stripped `GAME_ACTION_*`/`EGameAction` **name** strings (still code-only,
  same as PC — only 4 debug identifiers embedded).

## Reproduction / outputs
- Script: `tools/xbox_re/xbe_pc_diff.py` (section map, RTTI class-universe diff, string signals).
- Class lists dumped to scratchpad: `classes_pc_only.txt` (2,546), `classes_common.txt` (11),
  `classes_xbe_only.txt` (1). Re-run to regenerate.
