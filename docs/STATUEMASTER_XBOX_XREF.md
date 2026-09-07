# StatueMaster Xbox executable cross-reference

**Date:** 2026-08-29  
**Builds compared:** original Xbox `default.xbe` and retail PC `Fable.exe`

## Result

The original Xbox build does **not** contain a surviving StatueMaster Guild
presentation branch or the five augmentation-inscription behaviors. The Xbox
and PC implementations agree on every relevant control-flow and data-binding
check performed here. Therefore, the missing Guild response and inscription
puzzle were removed or left disconnected before the original Xbox retail
master; they were not cut specifically for the PC port.

This conclusion concerns surviving executable and level-data behavior. It does
not establish what reward, if any, was planned for the Guild direction.

## Correct Xbox overlay

The complete StatueMaster implementation is in Xbox overlay **`S_VSM`**:

- virtual address `0x0081D4E0`
- virtual size `0x9A4` (2,468 bytes)
- file offset `0x009D4000`
- 17 contiguous functions

The engine's script registry binds `V_StatueMaster` to `S_VSM` and its factory
at `0x0081DE46`. The global game-flow overlay activates `V_StatueMaster`, so
this is shipped, reachable quest behavior rather than an unused registration.

Two tempting name guesses are unrelated:

- `S_QSMS` is `QS_MeetSister`.
- `S_PSM` is `PersonalScriptMain`.

`S_QRET` is the repeatable `QR_EscortTrader` quest and supplies the
`TraderToEscort` object used by the StatueMaster cellar-door condition.

## Guild branch

Xbox `CStatueMasterStatue::Main` at `0x0081DB1E` implements the same three-way
selection as retail PC:

- direction 1: Bowerstone, text ID 6730
- direction 3: Greatwood, text ID 5921
- all other values, including direction 2: Nowhere, text ID 4580

There is no direction-2/Guild text arm. `TEXT_QST_061_STATUE_GUILD` (ID 3497)
has no genuine code-immediate reference anywhere in the XBE. Its sole raw
dword occurrence is an incidental `call` displacement, not a text lookup.

The state itself is not wholly absent. Both builds initialize and can return
the `SM_Guild` direction marker, but neither has a presentation or reward
consumer for it. In both builds, the computed Guild direction falls through to
the Nowhere text.

## Text-ID equivalence

Xbox and PC text identifiers are directly comparable:

- Both generated headers contain 28,912 tags.
- All 28,912 names have identical numeric IDs and declaration order.
- After line-ending normalization, the headers are byte-identical.
- Both English text containers contain the same 28,913 entries: 28,912 header
  entries plus the internal narrator record.
- Eighty platform-specific payloads differ in wording, but their IDs remain
  fixed. None belongs to the StatueMaster family.

Thus ID 3497 and the inscription IDs have the same meaning on both platforms;
there is no offset or platform-specific renumbering.

## Orphan inscription tags

The following authored text tags have no executable behavior on either build:

| Tag | ID |
|---|---:|
| `TXT_CLICK_STATUE_MASTER_FIRE` | 5306 |
| `TXT_CLICK_STATUE_MASTER_STEEL` | 6117 |
| `TXT_CLICK_STATUE_MASTER_SILVER` | 6929 |
| `TXT_CLICK_STATUE_MASTER_DIAMOND` | 7312 |
| `TXT_CLICK_STATUE_MASTER_LIGHTNING` | 8789 |

Whole-image immediate and symbolic-reference scans found no relevant consumer
in the Xbox XBE or PC executable. Level-data scans likewise found no binding of
these tags in either shipped world. They are orphan localization evidence, not
a dormant executable puzzle.

## Lookout Point readable

`TXT_CLICK_LOOKOUT_POINT_STATUE_01` (ID 8759) is also unbound on both platforms.
This needed a separate data-layer check because ordinary `TXT_CLICK_*` text is
usually referenced from TNG data rather than executable code.

The Lookout Point StatueMaster entity is placed and script-bound, but its
`CTCActionUseReadable` text field is empty in the identical Xbox and PC level
data. At runtime, `CStatueMasterStatue::Init` assigns the directional readable
text. It is plausible that ID 8759 was an earlier static readable superseded by
the scripted version, but that interpretation is inferential. The proven fact
is that neither retail platform binds it.

## Evidence and method

The comparison used the locally extracted original Xbox executable at
`D:\tmp\fable_xbox\extracted\default.xbe`, its named `S_*` overlays, the Xbox
and PC generated text headers and English text containers, both executables,
and the corresponding world data.

The critical negative searches covered the entire XBE, including every quest
overlay—not merely resident `.text`. Independent verification used separate
XBE/PE parsers, whole-file byte scans, Capstone disassembly, text-container
parsing, and positive-control tags known to be live. Multiple verifier passes
reproduced the Guild, Bowerstone, Greatwood, and Nowhere findings before the
workflow's final synthesis was interrupted by a usage limit.

## Reconstruction boundary

A restored five-statue augmentation puzzle remains a historically informed
recreation, not retail parity. The evidence supports the five elements and a
Guild direction marker, but supplies no surviving strike-handler, persistence
logic, completion sequence, or Guild reward. Any implementation should label
those choices as new design.
