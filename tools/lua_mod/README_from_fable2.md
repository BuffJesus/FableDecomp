# Fable II In-Game Lua Mod Toolchain

Built: 2026-07-15

Pure-Python BNK modding plus Lua 5.1 script RE helpers. Proven end-to-end: injects in-game Lua
through `gamescripts_r.bnk`, survives save-load, and currently unlocks the real ancestor's chest by
flipping the original entitlement gates.

## Tools

- `bnk_repack.py` - read/extract and repack Fable II BNK v3 archives.
  Round-trip verified on `gamescripts_r.bnk` at 554 entries with zero content mismatches.
- `luadis.py` - Lua 5.1 bytecode disassembler. It resolves enough GETGLOBAL/GETTABLE/SELF/CALL
  context to read game scripts and API usage.
- `script_index.py` - list, search, extract, and disassemble Lua scripts directly from
  `gamescripts_r.bnk`, including large 32 KiB compressed-block entries.
- `myconsolehook0.lua` - current ancestor's chest entitlement hook. It is plaintext Lua; the game
  compiles it at load time.
- `apply_mod.py` - applies the mod to `gamescripts_r.bnk` by hijacking the `AppearanceEnum` slot in
  `generalsetupscript`.

## How Injection Works

- In-game scripts live in `data/gamescripts_r.bnk`. The game opens the release BNK, so loose script
  override is not enough for this path.
- `generalsetupscript.lua` is a `RunScript(...)` list. We byte-patch one same-length script name:
  `miscellaneous/AppearanceEnum.lua` -> `miscellaneous/MyConsoleHook0.lua`.
- The hook runs the original `AppearanceEnum.lua`, so no setup script is skipped.
- `GeneralScriptManager.AddScript{Update=fn}` creates and resumes an update coroutine each frame.
- `LoadFromSave` wipes `CurrentlyRunningScripts`, so init-time registrations die on save load. The
  `GeneralScriptManager.Update` method itself survives, so persistent hooks wrap that method.

## Ancestor's Chest Hook

See `docs/ANCESTORS_CHEST_RE.md` for the full RE note.

The real Chamber-of-Fate ancestor's chest is `SpecialChest.CustomUpdate` in
`scripts\quests\generictriggers.lua`. Its content is already scripted by the game, but gated behind:

- `Stats.IsCollectorsEdition(GetPlayerHero())` for the Spartan set and Master Chief title.
- `PlayerWebsiteUnlocks.IsItemUnlocked(...)` for the mascot/chicken suit, expression book, Hero Doll,
  rare pink dye, and Lionhead tattoos.
- `XboxLive.GetYetToCollectUnlockedGold() > 0` for the online gold bag.

If nothing qualifies, it closes the chest and displays `GUI_WEBSITE_GOLD`. The current hook patches
those gates so the game's own code fills and unlocks the chest. It does not globally grant the items.

## Useful Commands

```powershell
python tools/lua_mod/apply_mod.py Fable2Recomp/assets/game/data/gamescripts_r.bnk
python tools/lua_mod/script_index.py list
python tools/lua_mod/script_index.py search SpecialChest OnlineArmour
python tools/lua_mod/script_index.py disasm generictriggers > tools/lua_mod/generictriggers.dis.txt
```

Revert the BNK:

```powershell
Copy-Item -Force Fable2Recomp/assets/game/data/gamescripts_r.bnk.orig_backup `
  Fable2Recomp/assets/game/data/gamescripts_r.bnk
```

Launch the game with GPU enabled:

```powershell
Fable2.exe --allow_game_relative_writes true --game_data_root D:\Documents\Fable2RE\Fable2Recomp\assets\game --gpu_plugin xenos --mnk_mode true
```
