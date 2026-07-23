# Fable: The Lost Chapters — `Profile.bin` (the save registry / LOAD-list index)

**Status:** CRACKED + validated on 4 real profiles; transfer procedure derived and a
corrected install STAGED (not deployed). **Date:** 2026-07-20.
**Problem solved:** a hand-copied `.sav` in the profile dir does NOT appear in the in-game
LOAD list — the game enumerates *manual* saves from `Profile.bin`, not by scanning the dir.
**Companion:** `docs/SAVE_SIG_ENFORCEMENT.md` (the shared trailer-signature gate covers
BOTH `FableSav` and `FablePro` containers). **Tool:** `tools/save_install.py`.

---

## 0. TL;DR

- `Profile.bin` is a **`FablePro`** container: 16-byte header + **plaintext persist-script**
  (`Name value;\r\n`, latin1) + **4-byte trailer CRC**. No zlib, no padding.
- The manual-save registry is 50 fixed text slots `SaveGameNames1..50`. A slot is shown in
  the LOAD list **iff its string is non-empty**. The string is BOTH the label AND the
  on-disk filename (`SaveGameNames1 "Manual - Save1"` → file `…\Manual - Save1`). No GUID /
  slot-id / embedded save-id — the key is the exact filename.
- `AutoSave` / `AutoSave.qs` are reserved special slots, NOT in the registry; they show
  up regardless. Only **manual** saves are registered here.
- The trailer is a **seed-0 reflected CRC-32** over `file[0:total_data_len]` — the SAME
  primitive as the `.sav` trailer — and it is **hard-enforced on load**
  (`CUserProfileManager::VerifySignature` @ retail `0x00409730`). Editing the text WITHOUT
  recomputing the trailer bricks the whole profile. **This is the only checksum that blocks
  a naive edit.** It does NOT checksum the referenced `.sav` files.
- Per-slot LOAD-screen metadata (hero name, chapter/region, playtime, thumbnail) is read
  lazily FROM THE `.sav` itself at enumerate time — it is not stored in `Profile.bin`. A
  valid edited `.sav` displays correctly with no extra work.

---

## 1. Container format (CONFIRMED on Cornelio / 1 / Default / Test)

```
0x00  8    magic  "FablePro"
0x08  u32  0                       (always 0 in the 4 real profiles)
0x0C  u32  total_data_len          == byte offset of the trailer == filelen - 4
0x10  ..   persist-script text     `Name value;\r\n` grammar, latin1, NUL-free
EOF-4 u32  trailer signature       = CCRC::Calc(seed=0, file[0:total_data_len])
```

- Reflected CRC-32, poly `0xEDB88320`, table = game's `0x0449BE30`, **no final inversion**,
  seed **0** — identical to the `.sav` trailer and to save-stream field tags (SAVE_HEADER_MAP §0).
- **No zlib** anywhere (unlike `.sav`); the payload is raw text.
- **No padding**: `bytes_after_trailer == 0` on all 4 profiles. (The `SAVE_SIG_ENFORCEMENT.md`
  decompile mentions a `0x4000`/`0x20`-pad branch for `FablePro`, but the real profiles are
  variable-length 16 689–18 084 B and unpadded, so that size/pad branch is not applied to
  profiles as written — the **CRC gate is the operative check**. Empirically verified:
  `Calc(0, prefix) == stored` on all four.)

Sample header dwords / trailer verification (all `MATCH=True`):

| profile   | filelen | total_data_len (@0x0C) | trailer   | CRC(seed0) |
|-----------|--------:|-----------------------:|-----------|------------|
| Cornelio  | 18088   | 18084                  | 94ADE8E3  | 94ADE8E3 ✓ |
| 1         | 16693   | 16689                  | 171E6CF1  | 171E6CF1 ✓ |
| Default   | 16890   | 16886                  | 8CEC5F75  | 8CEC5F75 ✓ |
| Test      | 16887   | 16883                  | 676D17E2  | 676D17E2 ✓ |

---

## 2. The text payload

One flat persist-script. Notable blocks, in file order:

- `ProfileName "<name>";` — the profile/hero-profile display name (== the dir name).
- Options: `SubtitlingEnabled`, `VibrationEnabled`, `Camera*Inverted`, `Gamma`, `HUDAlpha`,
  `SoundVolume`/`MusicVolume`/`DialogueVolume`, `CameraSensitivity`, `Antialiasing`,
  `TextureDetail`/`ShadowDetail`/`EffectsDetail`/`MeshDetail`, `Resolution`, `RefreshRate`,
  `ScreenAspectRatio`, `VerticalSyncEnabled`, … (settings-menu state).
- `AssignedInputsPassive[0..N] CActionInputControl(...);` — full input-binding table.
- `GivenTutorialCategories[0..N] <int>;` — tutorial progress.
- **`SaveGameNames1..50 "<name>";`** — the manual-save registry (see §3). Last block in file.

### 2.1 The registry (`SaveGameNames`)

```
SaveGameNames1  "Manual - Save1";     <- occupied  => shown in LOAD list, file "Manual - Save1"
SaveGameNames2  "";                    <- empty     => slot 2 free
...
SaveGameNames50 "";
```

- Exactly **50 slots**, always all present (empty ones are `""`).
- **Occupied ⇔ non-empty string ⇔ appears in the LOAD list.** The string is the display
  label AND the exact filename in the same profile dir.
- Cross-check: profile `1` has an `AutoSave` on disk but **zero** occupied `SaveGameNames`
  slots — confirming autosaves are not registered here and only manual saves are.

---

## 3. Why a hand-copied `.sav` doesn't appear — and the fix

The game builds the LOAD list from the `SaveGameNames` registry, then reads each referenced
file's HEADER for the on-screen metadata. A file dropped in the dir with **no matching
registry entry is invisible**. (Observed: `Manual - Save2`, 307 200 B, present on disk,
missing from the LOAD list; `SaveGameNames2` was `""`.)

### Transfer procedures

**(a) Overwrite an existing listed slot (simplest, no Profile.bin edit).**
Copy the edited `.sav` over the file named by an occupied slot (e.g. overwrite
`Manual - Save1`). `Profile.bin` is untouched — its entry still names the same file and it
does **not** checksum the `.sav`. Zero registry/CRC risk. Downside: you replace an existing
save.

**(b) Add a new slot (keeps existing saves).**
1. Copy the edited `.sav` into the profile dir under a new filename (e.g. `Manual - Save2`).
2. Set the first empty `SaveGameNamesN` to that exact filename.
3. **Recompute the `Profile.bin` trailer CRC** (seed 0, over the new `file[0:total_data_len]`)
   and update the `total_data_len` dword @ `0x0C`. Skipping this → `VerifySignature` fails →
   the entire profile fails to load.

`tools/save_install.py` implements (b) (and (a) when the slot name matches an occupied slot).

### Checksum verdict

- **Yes, one checksum blocks a naive Profile.bin edit:** the seed-0 CRC-32 trailer. It must
  be recomputed after any text change (the tool does this and self-verifies).
- **No checksum blocks copying the `.sav` file** — the registry entry references only the
  filename; nothing in `Profile.bin` hashes the `.sav` contents. Procedure (a) therefore
  needs no `Profile.bin` change at all.

---

## 4. Tool — `tools/save_install.py`

```
python tools/save_install.py inspect  <profileDir>
python tools/save_install.py install  <edited.sav> <profileDir> <slotName> [--out-dir DIR] [--apply]
    <slotName> = "new"            -> first empty slot, file "Manual - SaveN"
               = "<existing name>"-> overwrite that occupied slot's file in place (proc a)
               = "Some New Name"  -> new slot, file+label = "Some New Name" (proc b)
```

- Default (no `--apply`): **STAGES** a corrected `Profile.bin` + the renamed `.sav` +
  `INSTALL.txt` into `--out-dir`; the real profile dir is untouched.
- `--apply`: writes the real profile dir directly (backs up `Profile.bin` → `Profile.bin.bak`
  first). Game must be closed.
- `inspect` reports every occupied slot, whether its file exists, and flags any
  **unregistered** files in the dir (the ones that won't show in-game).
- Every build recomputes + self-verifies the trailer and re-parses the result before writing.

---

## 5. Staged corrected install (ready to deploy)

`work/save_install/` contains the fix for the gold-edited `Manual - Save2`:

| file | note |
|---|---|
| `Profile.bin` (18 102 B) | Cornelio profile with `SaveGameNames2 "Manual - Save2"` added, trailer re-signed `A34E698D` (self-verifies). Slot 1 preserved. |
| `Manual - Save2` (307 200 B) | the gold-edited save (copy of the on-disk file). |
| `INSTALL.txt` | copy-in instructions (game closed; back up the old Profile.bin). |

Deploy = copy both files into `C:\Users\Cornelio\Documents\My Games\Fable\Saves\Cornelio\`
(overwriting `Profile.bin`), with the game closed. Then `Manual - Save2` appears in the
in-game LOAD list.

---

## 6. What still needs a live test

- **In-game confirmation** that the registered slot appears and loads (user was away; real
  profile untouched). This is the single unverified step; the on-disk / signature side is
  fully validated. If the LOAD screen shows a slot but empty metadata, the metadata read is
  from the `.sav` HEADER (already valid in the gold save), not from `Profile.bin`.
- Confirm the game does not *rewrite* `Profile.bin` on entering the LOAD menu in a way that
  drops an externally-added slot before you can load it (unlikely — it rewrites on SAVE, not
  on browse — but only a live run proves it).
- `dw08` (@0x08) is 0 in all 4 profiles; assumed reserved. Not load-bearing for install.
