# Xbox TLC disc — assessment for the PC RE project (2026-08-09)

Source: `D:\Downloads\Fable - The Lost Chapters (USA) (En,Es).7z` → `.iso` (3.51 GB, redump-style
XGD1 image, XDVDFS partition base `0x0`). Extracted to `D:\tmp\fable_xbox\` (not git-tracked).

**Bottom line:** the disc is the *original-platform sibling build* of the exact same source tree.
The single biggest asset is **`default.xbe` — an x86-32 executable of the same game**, so it is a
second byte-level reference for the PC decomp crawl. Several data files are directly useful for the
input/redefine and frontend-parity lanes. It does **not** hand us the one thing we most wanted (a
full `EGameAction` ordinal→label table as plain strings) — that lives in code, same as on PC — but it
gives us a clean place to *decompile* that mapping from a known-good build.

## How to read the disc (tooling, all in `tools/xbox_re/`)
7-Zip does NOT read original-Xbox ISOs. Home-grown parsers instead (Python, stdlib + capstone):
- `xdvdfs.py <iso>` — enumerate the XDVDFS filesystem (binary-tree dir table, 2048-byte sectors).
- `xextract.py <iso> <substr> <outdir>` — extract every file whose path contains `<substr>`.
- `xbe_info.py <default.xbe>` — parse XBE header (base/entry/sections) + keyword string scan.
- `xbe_xref.py [strings…]` — find `.text` immediate-refs to a string and disassemble around them.
- Full file list: `refs/xbox/disc_filesystem_listing.txt` (452 entries).

## `default.xbe` structure (the reference binary)
- `XBEH`, **BaseAddr `0x00010000`**, EntryPoint `0x001E6A74` (retail XOR key `0xA8FC57AB`).
- **135 sections.** `.text` = `0x00013000`, ~6 MB. The rest are demand-paged overlays
  (`xuserst.ini: EnableCodeSectionLoading(true)`): `S_Q*` = per-quest code, `S_V*` = subsystems.
  This independently corroborates our PC finding: **quest logic is compiled C++, no quest VM.**
- Disassembles cleanly with **capstone 5.0.7** (x86-32) — no Ghidra XBE loader required. Proven:
  decompiled the input-action test harness at `0x005C4392` (drives the input/script-interface vtable
  at `[esi+4]`, slots `+0x1c/+0x104/+0x1c0/+0x800`; pushes the `GAME_ACTION_*` debug strings).
- Same VC7.1-era toolchain as PC retail ⇒ **BSim / structural function matching between the XBE and
  our named PC Ghidra DB is viable** and is the highest-leverage follow-up (port names both ways;
  recover inlining/boundary decisions where PC parity is stuck).

## Input / redefine-keys lane — what the disc gives us
The critical PC blocker is the `EGameAction` ordinal→display-label mapping/order for the redefine
menu (see `redefine-keys-name-gap`, `REDEFINE_INPUT_SYSTEM.md`, `GAMEPAD_REDEFINE_PATCH.md`).
- **NOT solved by strings:** the XBE embeds only 4 `GAME_ACTION_*` *identifiers* (the debug-print
  ones); the full enum-name table is stripped, same as PC.
- **Label universe recovered:** `refs/xbox/headers/text_xbox.h` lists **48 `TEXT_GUI_ACTION_*`** tags
  with their text-table IDs (e.g. `TEXT_GUI_ACTION_LOCK_TARGET = 4673`,
  `..._UNSHEATHE_RANGED_WEAPON = 12930`, `..._TOGGLE_FIRST_PERSON_TARGETING = 16577`) plus the full
  `TEXT_GUI_CONTROL_*` / `TEXT_GUI_MENU_*` control-UI set (`MAP_KEYS`, `REDEFINE_KEYS`, `WASD`,
  `ARROWS`, `INVERT_MOUSE`, `ASSIGN/DELETE/PRESS_BUTTON`, `UNDEFINED_CONTROLS_WARNING`, MB4–MB8 +
  abbreviations, mouse-wheel up/down…). This is the superset of PC+console action labels.
- **The remaining piece — the ordered `EGameAction`→`TEXT_GUI_ACTION_*` list — is in code**, and the
  XBE is the cleanest place to decompile it (find the redefine-screen/list builder via xref to the
  action text-IDs or to `TEXT_GUI_MENU_REDEFINE_KEYS`=4689, then read the action-order array). This
  is the concrete next RE task the disc unblocks.
- `xuser.ini` runs `RunScript("joystick.ini")` — confirms a data-driven joystick binding script, but
  `joystick.ini` is not a loose file on the disc (baked into a bank/XBE); not yet located.

## Frontend / gamepad-UI lane — what the disc gives us
- `refs/xbox/headers/front_end_bank_xbox.h` — **full named frontend sprite index (326 entries)**
  incl. the on-screen controller glyphs the gamepad-UI lane lacks: `HUD_ABXY_BIG_A/B/Y_FE`,
  `UI_THUMB_STICK_*` (U/D/L/R off + centre), plus `FRONTEND_BUTTON_L/R/M`, slot/table/minimap sprites.
  (Note: `banks_dvd.ini` shows the game ships **parallel `pc\` and `xbox\` banks**; the PC frontend
  bank has its own header — treat this as the Xbox layout, cross-check before assuming PC parity.)
- `refs/xbox/headers/gui_bank_xbox.h` — editor/GUI icon bank enum.
- Actual glyph pixels live in `/Data/graphics/xbox/frontend.biz` (7.2 MB, Xbox `.biz` bank, swizzled
  textures) — extractable if we want reference art for ABXY/stick glyphs.
- `/Media/Fable.uix` (448 KB) — custom `XSK0`/UIX container (sub-resource offset/size table) = the
  Xbox UI skin package; format is bespoke, not yet parsed.

## Other corroborating data (lower priority)
- `/Data/CompiledDefs/{game,names,script,xfrontend}.bin` — Xbox compiled defs to diff vs PC.
  `xfrontend.bin` is a packed crc0 blob (no plaintext) — needs the def loader.
- `xuserst.ini` = the retail boot script (resolution 640×480×16, `UseRetailBanks`, `RunFromDVD`,
  `SetSkipFrontend(FALSE)`, `UseCompiled*`) — a clean reference for engine init flags.
- `banks_dvd.ini` — the authoritative bank→file→header manifest (PC and Xbox side by side).

## Recommended next steps (highest leverage first)
1. **Import `default.xbe` into Ghidra** (raw x86-32 @ `0x10000`, or via an XBE loader) and run BSim
   against the named PC DB — port names/structure, accelerate the parity crawl with a 2nd reference.
2. **Decompile the redefine-list builder in the XBE** to extract the exact `EGameAction` action order
   + text-tag per row — this is the specific thing that unblocks the 31-action redefine screen.
3. If we want gamepad glyphs on-screen: extract `frontend.biz` ABXY/thumb-stick sprites as reference.

Staged in-repo: `refs/xbox/` (headers, inis, filesystem listing), `tools/xbox_re/` (parsers).
Full extraction kept at `D:\tmp\fable_xbox\` (XBE + big data files; not git-tracked).
