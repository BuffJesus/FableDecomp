# Save-Game Signature Enforcement on LOAD — Verdict

**Status:** VERDICT REACHED (decompile-confirmed). **Date:** 2026-07-20.
**Question:** does retail `Fable.exe` ENFORCE the save trailer signature when loading a save?
**Answer: YES — ENFORCED, unconditionally, as a hard gate at the top of the load path.**

Evidence log: `ghidra_out/decomp_save_sig_load1.log` (headless DecompFuncs on retail
`Fable.exe`, project FableTLC, 2026-07-20).

---

## 1. The verifier: `CUserProfileManager::VerifySignature` @ retail `0x00409730`

Named in the DB from the BSim/RTTI port (`?VerifySignature@CUserProfileManager@@SI_NABVCWideString@@@Z`,
`ghidra_out/bsim_port_audit_05final.tsv` row `409730`); body decompiled this session and it does
exactly what the name says. Takes a file *path* (CWideString), opens the file itself, and returns
`bool`. Full decompile in the log; the load-bearing excerpts:

**(a) Magic dispatch** — accepts either container, first 8 bytes:
```c
CDiskFileWin32::Read(file, local_c, 8, 0);
bVar1  = memcmp8(local_c, "FablePro");   // profile container
bVar10 = memcmp8(local_c, "FableSav");   // save container
if (!bVar1 && !bVar10) return false;
```

**(b) Magic tail** — for `FableSav`, the next u32 (file[8..0xB]) must be `0x2165`
(= `65 21 00 00` = `"e!"` + 2 NULs, completing `"FableSave!"` + pad):
```c
if ((!bVar10) || (local_5c == 0x2165)) { ... } else return false;
```

**(c) Signature check — THE gate.** Reads the u32 at file offset `0x0C`
(`total_data_len` = trailer position, per SAVE_WRITER.md), bounds-checks it against the file
size, re-reads the whole prefix `file[0 : total_data_len]` from position 0, reads the stored
u32 trailer at `total_data_len`, and compares against a fresh seed-0 CRC:
```c
if (uVar2 + 4 <= fileSize) {                 // uVar2 = u32 @ 0x0C = total_data_len
    FUN_00411910(uVar2);                     // resize buffer to total_data_len
    CDataInputStream::SetPosition(stream, 0);
    /* read total_data_len bytes into buffer (whole file prefix incl. header) */
    /* read next u32 -> local_5c = stored trailer signature */
    uVar2 = CCRC::Calc(0, buffer, total_data_len);
    if (uVar2 == local_5c) { ... }           // MISMATCH -> falls through -> return false
}
```
This is byte-for-byte the recipe SAVE_WRITER.md confirmed on the write side:
**trailer = reflected CRC-32 (poly 0xEDB88320, zlib table), seed 0, no final inversion, over
`file[0 : total_data_len]`** — the literal on-disk bytes including the `FableSave!` magic and
the 16-byte header, everything up to (not including) the stored 4-byte trailer.

**(d) File size + padding check** — after the CRC matches, the file must be *exactly* the
expected slab size and the slack must be a constant fill byte:
```c
uVar4 = (-(uint)bVar1 & 0xfffb9000) + 0x4b000;   // FablePro -> 0x4000, FableSav -> 0x4B000
if (fileSize == uVar4) {
    /* stream the remaining bytes after the trailer in 0x4000 chunks: */
    if (byte != (!bVar1 - 1U & 0x20))   // FableSav -> every pad byte must be 0x00
        goto LAB_00409b34;              //           (FablePro -> 0x20, i.e. spaces)
    ...
    return true;                        // only exit that returns true
}
```
So for a `FableSave!` file: size must be exactly `0x4B000` (307200) and every byte from
`total_data_len + 4` to EOF must be `0x00`. Profiles (`FablePro`): size `0x4000`, pad `0x20`.

Every other path returns `false`. There is **no config-flag consult inside VerifySignature** —
no read of `UseSaveGameSignatures` or any global; the function validates unconditionally.

## 2. The enforcement site: `CWorld::LoadGameStateInternal` @ retail `0x004A21F0`

First statements of the function (decompile, same log):
```c
bool CWorld::LoadGameStateInternal(CWorld *this, CWideString *param_1, bool param_2)
{
    bVar2 = CUserProfileManager::VerifySignature(param_1);
    if (!bVar2) {
        return false;          // <-- HARD REJECT: load aborts before reading anything
    }
    if (param_2) { NProgressDisplay::StartProgress(...); PrepareForLoad(this); }
    ...                        // only now opens the file and parses FableSav/zlib/persist
}
```
The signature check is the *very first thing* the loader does, before `PrepareForLoad`, before
the zlib chunks are touched. A bad or missing trailer means `LoadGameStateInternal` returns
`false` and the save never loads. No branch bypasses it.

## 3. All callers of `VerifySignature` (xrefs, FindRefs.java)

| call site | caller | role |
|---|---|---|
| `0x0040d38b` | `Load` @ `0x0040d350` (CUserProfileManager) | profile/save load wrapper |
| `0x004a220f` | `CWorld::LoadGameStateInternal` @ `0x004a21f0` | THE save loader (above) |
| `0x005957fd` | `FUN_005957d9` | (see §5 audit) |
| `0x0047ed2e` | `FUN_0047ed0a` | (see §5 audit) |

## 4. Verdict table

| check | on-disk field | load behaviour | verdict |
|---|---|---|---|
| Header dword @ `0x0C` | `total_data_len` (NOT a hash — SAVE_WRITER.md correction stands) | used as the CRC input length + trailer locator; an inconsistent value shifts/fails the CRC or the `+4 <= fileSize` bound | **ENFORCED (structurally)** |
| 4-byte trailer @ `total_data_len` | seed-0 CRC-32 of `file[0:total_data_len]` | computed-vs-stored compare; mismatch → `return false` | **ENFORCED** |
| File size | exactly `0x4B000` | equality check | **ENFORCED** |
| Padding | `0x00` from `total_data_len+4` to EOF | per-byte check | **ENFORCED** |

## 5. Secondary audit (all closed)

**`UseSaveGameSignatures` does NOT exist in retail — there is no off-switch.**
Raw memory scan of the whole retail image (`FindStrRaw.java`, log
`ghidra_out/decomp_save_sig_load3.log` / `..._load4.log`):
- `"UseSaveGameSignatures"` — **0 hits** in retail `Fable.exe`.
- `"UseBinarySaveGames"` — **0 hits**.
- `"UseRetailSaveGameSystem"` — present at `0x0122E85C` (scan sanity check; matches
  SAVEGAME_FORMAT.md §5, proving the scan works).

So the config-flag mitigation suggested in SAVEGAME_FORMAT.md §8 ("flip
`UseSaveGameSignatures`") is **impossible in retail** — those console vars are donor/debug-build
only (donor `FableWin.exe` strings at `0x0402F21C`/`0x0402F208`). Retail validates
unconditionally; VerifySignature reads no global flag.

**The other two callers** (`ghidra_out/decomp_save_sig_load2.log`) are consistent:
- `CUserProfileManager::Load` @ `0x0040D350` — gates **Profile.bin** load the same way:
  `if (VerifySignature(path)) { ... LoadGameState(file); }` — bad profile signature =
  profile silently not loaded. (Profiles use fill byte `0x20` and size `0x4000`, per §1d.)
  Note: `Profile.bin` payload is text-mode persist, but the *container* still carries the
  `FablePro` magic + seed-0 CRC trailer, and it IS checked.
- `FUN_005957d9` @ `0x005957D9` and `FUN_0047ED0A` @ `0x0047ED0A` — twin cached-validity
  helpers: compute `VerifySignature(path)` once, cache `{path, bool}` at `this+0xC4`/`+0xE0`,
  return the cached bool on repeat queries. This is the save-slot/UI "is this save valid?"
  predicate — an unsigned save would not even appear loadable in the UI.

## 6. Implications for the save editor (`forge save edit`)

**An edited save MUST be re-signed — and the proven writer recipe is exactly what the loader
checks.** Requirements for a loadable file (all verified against the loader decompile):
1. `"FableSave!"` + 2 NUL pad (bytes 0..0xB; loader checks 8-byte `FableSav` + u32 `0x2165`).
2. u32 @ `0x0C` = `total_data_len` = byte offset of the trailer = total size of
   header + chunk0 + framing dword + chunk1 (i.e. everything before the trailer).
3. u32 trailer at `total_data_len` = `CCRC::Calc(0, file[0:total_data_len])` — reflected
   CRC-32, poly `0xEDB88320`, seed 0, **no final XOR**, over the literal file bytes.
4. Zero-pad from `total_data_len + 4` to exactly `0x4B000` (307200 bytes). Non-zero slack
   bytes or a different file size are ALSO rejection causes, not cosmetic.

There is **no config bypass in retail** (§5) — re-signing is mandatory, not optional.

`tools/save_edit.py` already does all four (SAVE_WRITER.md gates A–D pass 5/5), so the
round-trip editor's output is load-valid by construction. **`forge save edit` is safe to ship**
provided it reproduces the same four rules. Residual (low) risk: this is a static verdict;
a 30-second live confirmation (load a re-signed edited save in-game) is still worth doing as
the final QA gate, but no *signature* obstacle remains.
