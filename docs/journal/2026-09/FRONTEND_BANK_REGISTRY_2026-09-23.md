# Native archive registry lookup - 2026-09-23

Worktree: `D:\Documents\FableTLC-native-play`, branch `wip/native-cgame-play`.
Continues [bank opening and streams](FRONTEND_BANK_OPEN_AND_STREAMS_2026-09-23.md).
Five complete readable recoveries replace lookup stand-ins:

| Function | Retail address | Compiled / retail bytes |
| --- | --- | --- |
| Signed string-data comparison | 00429950 | 53 / 88 |
| Contained-bank map find | 009AB4F0 | 17 / 110, plus shared body |
| Bank-path map find | 009AB560 | 17 / 110, plus shared body |
| Bank-alias map find | 009AB5D0 | 17 / 110, plus shared body |
| Registered-bank lookup | 009A7F80 | 244 / 270 |

All are functional DIFFER. The three 17-byte wrappers invoke a **121-byte shared
tree-search body**; their wrapper sizes are not full implementation sizes. The
report records that body's fingerprint separately. There is no new byte-match
claim or manual generated-coverage change.

## Contracts

String comparison uses signed bytes and ends at NUL, regardless of stored length.
Map nodes put their key at +0x10 and value at +0x14, with root in header +4 and
left/right at +8/+0x0C. Null storage sorts before every allocated string,
including an allocated empty string. Equal text in different storage compares
equivalent; embedded-NUL suffixes do not participate. The recovered search is a
lower-bound walk followed by the reverse comparison, returning the header on miss.

Registry lookup resolves one alias, copies the resulting name, traverses the
registered-file list in order and returns the first containing bank. It copies
the entire 20-byte bank header and assigns the output counted reference. A miss
leaves both output arguments untouched. Each visited registered object is retained
while searching, then released. Cleanup uses the locally retained control record,
even when destruction callbacks mutate the list node's Info pointer.

`fable_ui_bank_registry.h` supplies explicit views and offset assertions. These
are recovered prefixes, not claims that every registry/file member is understood.
The registry receiver is 013CA79C: paths +4, file-list head +0x10, mode byte +0x14,
aliases +0x18, wide base path +0x24. Registered objects have the bank map at +0x18.
The existing global Open-mode address **013CA7B0 aliases registry +0x14**. Future
production global bindings must preserve that identity; isolated fixtures still
provide the individual external symbols their linked routines use.

## Verification and connection

```powershell
python tools/decomp_pipeline/check_ui_bank_registry.py
python tools/decomp_pipeline/check_ui_bank_open.py
python tools/decomp_pipeline/check_ui_manager_construction.py
```

All pass: **1,657 registry cases**, **2,048 connected opening cases**, and **324
full manager cases**. The registry gate comprises 121 byte-comparison pairs,
768 tree scenarios each invoking all three map functions, and 768 registry
scenarios. It checks rotating tree shapes, missing/present keys, aliases,
null/empty/high-byte/embedded-NUL keys, different string storage, first-bank
precedence, unchanged outputs on miss, old-owner destruction and callbacks
mutating node ownership. The resolved name uses actual recovered string lifetime
code; counted assignment/release uses the existing real ownership implementation.

The opening fixture now contains real registry/list/map state rather than a
lookup callback. Progress can remove a bank before the second lookup, exercising
failure after base opening succeeds. A previous fixture supplied a non-null bank
Data with null Info even to a null output reference. Actual registry assignment
compares Info and leaves Data unchanged when both Info pointers are null; the
opener would then dereference null. End-to-end fixtures now supply a valid control
record for registered objects. Null-Info behavior remains tested directly at the
registry layer; no recovery from that invalid opening state is claimed.

Manager startup now uses the actual empty-registry search instead of its prior
missing-bank lookup stub. Its unavailable-backing-file outcome is still supplied
by the unresolved path backend. Successful archive byte loading is not proved.

Twelve saved gate families now total **13,145 cases** (1,657 newly added).
The registry gate is in bootstrap before bank opening and manager construction.
Reports and disassemblies: `work/ui_bank_registry_check/`,
`work/ui_bank_open_check/`, `work/ui_manager_construction_check/`.
Oracle executable SHA-256:
`41dc91090ae853715ac06d2e9fc96e5d545381d197ed55d624c642f34509ac10`.
Full bootstrap and live GPU tests were not run in this increment. No game/presenter
launched. Work is saved uncommitted; previous work and main checkout preserved.

## Next dependencies

Path resolution **009A7CA0** can now use real alias/path map searches. Its success
branch calls wide-string concatenation **0099BE70** with registry BasePath and
node value. The old checkout has a recovered source for that concatenation:
`rebuild/src/compiled/00/99/CWideString_AddWideStrings_0099be70.cpp`.
Inspect and reconnect its actual dependencies before using it in this worktree.

Missing path resolution constructs `<name> bank not found!` via **0099F600**,
extracts characters via **0099E4C0**, then invokes the exception runtime thunk
**00BFEB84** (IAT slot 01440104). This is not a false return or an empty path.
The narrow concat helper depends on append **0099F100**. Wide concat depends on
append **0099B8D0**, wide unassign **0099B4D0** and associated storage management.
Current path and wide-destruction services remain explicit boundaries.

Archive entry decoding **009CFBC0**, bank finalization and real threaded Win32
opening **0098E1E0** remain unfinished, followed by frontend Run/actions and
persistent renderer integration. The prior live D3D DEVICELOST result is still
unresolved. This checkpoint is not a playable standalone game.
