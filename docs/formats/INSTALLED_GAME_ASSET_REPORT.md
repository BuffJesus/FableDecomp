# Installed Game Asset Report

*Generated from installed game indexes on 2026-07-18.*

## Install

- Game root: `C:\Programs\Steam\steamapps\common\Fable The Lost Chapters`
- Fable.exe present: `True`
- FSE installed: `True` / folder `True`
- FinalAlbion WAD present: `True`
- Loose TNGs: `397`
- Loose LEVs: `0`

## File Types

| Extension | Count | Bytes |
|---|---:|---:|
| `.tng` | 397 | 15185788 |
| `.ogg` | 68 | 111355313 |
| `.bncfg` | 60 | 96871 |
| `.lug` | 50 | 143763972 |
| `.met` | 50 | 638775 |
| `.wmv` | 47 | 278904009 |
| `.dds` | 38 | 2685856 |
| `.txt` | 22 | 15351 |
| `.h` | 19 | 2840268 |
| `.dll` | 12 | 9802656 |
| `.bin` | 11 | 1840961 |
| `.cab` | 9 | 34999784 |
| `.big` | 8 | 872857732 |
| `.lua` | 8 | 22043 |
| `.rtf` | 7 | 2074616 |
| `.bmp` | 6 | 4854096 |
| `.ini` | 4 | 13222 |
| `.bak` | 4 | 4957602 |
| `.exe` | 4 | 17476304 |
| `.lut` | 4 | 672614100 |

## BIG Banks

| Bank | Entries | Bytes |
|---|---:|---:|
| `data\graphics\graphics.big` | 8112 | 233474128 |
| `data\graphics\pc\frontend.big` | 394 | 13287366 |
| `data\graphics\pc\textures.big` | 6290 | 525271140 |
| `data\lang\English\dialogue.big` | 12134 | 6247716 |
| `data\lang\English\fonts.big` | 8 | 1393834 |
| `data\lang\English\text.big` | 28913 | 4224305 |
| `data\Misc\pc\effects.big` | 1165 | 2262411 |
| `data\shaders\pc\shaders.big` | 112 | 13357 |

## BIG Entry Types

Names below are from EgoCore's GetEntryTypeName mapping where the bank context is unambiguous.

| Type | Likely Meaning | Entries | Bytes |
|---|---|---:|---:|
| `0` | textures/frontend: Graphic Single; text: Text Entry; effects: Particle Entry; shaders: Vertex Shader; fonts: PC Font | 34253 | 497433095 |
| `1` | graphics: Static Mesh; textures/frontend: Graphic Sequence; text: Group Text Entry; dialogue: Lipsync Entry; shaders: Pixel Shader; fonts: Xbox Font | 16939 | 110690992 |
| `6` | graphics: Animation | 3272 | 85700599 |
| `3` | graphics: Physics (BBM); textures/frontend: Bumpmap Sequence | 1383 | 6297506 |
| `5` | graphics: Animated Mesh; textures/frontend: Sprite Sheet | 534 | 33611346 |
| `2` | graphics: Repeated Mesh; textures/frontend: Bumpmap; text: Narrator List; fonts: GlyphData | 428 | 47563900 |
| `4` | graphics: Particle Mesh; textures/frontend: Volume Texture | 156 | 3415137 |
| `9` | graphics: Partial Animation | 107 | 831234 |
| `7` | graphics: Delta Animation | 56 | 630448 |

## FinalAlbion WAD

| Extension | Count | Bytes |
|---|---:|---:|
| `.lev` | 398 | 157522738 |
| `.tng` | 398 | 15236110 |

## FinalAlbion RT STB

| Extension | Count | Bytes |
|---|---:|---:|
| `(none)` | 26 | 3941086 |
| `.lev` | 398 | 593399586 |

## Largest Loose TNGs

| File | Things | ScriptNames | Size |
|---|---:|---:|---:|
| `HeroGuildComplexInside.tng` | 1110 | 1142 | 787564 |
| `BowerstoneSlums_v2.tng` | 1058 | 1098 | 772043 |
| `StartOakValeWest.tng` | 874 | 892 | 576997 |
| `OakValeWest_v2.tng` | 836 | 859 | 554296 |
| `HookCoast.tng` | 821 | 875 | 628461 |
| `SnowspireVillage.tng` | 735 | 753 | 480592 |
| `KnotholeGladeMainTown.tng` | 728 | 775 | 539777 |
| `Graveyard_1.tng` | 592 | 592 | 438712 |
| `NorthernWastes2.tng` | 557 | 557 | 321433 |
| `OakValeEast_v2.tng` | 495 | 496 | 319745 |
| `BowerstonePosh.tng` | 476 | 496 | 318086 |
| `OrchardFarm.tng` | 473 | 473 | 349266 |
| `FrescoDome.tng` | 413 | 413 | 339283 |
| `Arena.tng` | 355 | 355 | 269693 |
| `NorthernWastes3.tng` | 340 | 340 | 241013 |
| `HauntedHouse.tng` | 337 | 337 | 208032 |
| `Greatwood_1.tng` | 313 | 313 | 195539 |
| `BanditCampResidential.tng` | 301 | 301 | 205802 |
| `Greatwood_3.tng` | 290 | 290 | 177373 |
| `LookoutPoint.tng` | 288 | 288 | 197526 |

Loose TNG total: `21764` things and `22083` ScriptName entries.

## Editor Implications

- WAD-backed LEV data is the missing level/terrain half; loose TNG data is already present for entity discovery.
- BIG indexes provide the asset browser backbone: meshes, textures, animations, text, dialogue, fonts, effects, and shaders.
- TNG summaries identify high-density regions that should become early stress tests for editor performance.
- EgoCore should remain the read/compile oracle for BIG and resource banks.
- SilverChest bridges should provide safe WAD/TNG/LEV operations until native behavior is validated against FableWin.
