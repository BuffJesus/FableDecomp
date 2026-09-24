# Native archive entry decoding - 2026-09-23

Worktree `D:\Documents\FableTLC-native-play`, branch `wip/native-cgame-play`.
Continues [bank storage](FRONTEND_BANK_STORAGE_2026-09-23.md).

Eight further readable implementations now pass retail differential gates:

| Address | Behavior | Compiled / retail bytes |
| --- | --- | --- |
| 00464931 | Resize checksum words | 22 / 62, shared helper below |
| 009D3DF0 | Resize update pointers/words | 22 / 87, shared helper below |
| 0049B760 | Resize symbol strings | 368 / 101 plus retail helpers |
| 00579435 | Destroy string-tree subtree | 63 / 53 |
| 009D2110 | Clear filename alias list | 73 / 69 |
| 009D2160 | Resize filename alias list | 120 / 99 |
| 009D21D0 | Copy filename alias list | 70 / 68 |
| 009CFBC0 | Read archive entry list | 1625 / 1969 |

All functional DIFFER, not byte-identical. The two 22-byte word wrappers call a
231-byte `FableUiResizeWordVector` implementation; report records its hash/size
separately. Retail wrappers delegate growth to additional helpers. Entry-reader
size is its named symbol, not a sum of all dependencies or any compiler-outlined
local helpers. These sizes are evidence identifiers, not equivalent total code-size
comparisons.

## Connected behavior

Bank setup now links every recovered resize and tree-cleanup service; only
allocation/free boundaries remain controlled in the setup fixture. **0049B760 is
resize, not reserve:** it constructs empty strings and advances End to count.
This corrects the terminology and test double from the previous checkpoint.
String-tree cleanup recurses right, destroys/frees the current node, then walks
left. Real narrow-string ownership is used throughout.

Alias lists contain a pointer and signed byte count, but their heap allocation
has a separate full-word count cookie. Clear uses that cookie and destroys strings
in reverse order, then resets the pointer/count while preserving padding. Resize
clears first and allocates even for zero elements. Allocation failure leaves a
null pointer with the requested count byte. Copy sign-extends the source count;
its retail self-copy behavior clears the old contents. Valid decoder callers use
small nonnegative counts and successful allocations.

The entry reader prepares Size=its size argument+1, reads a pair table, invokes
bank virtual +0x28, and reads each entry's fields, symbol, checksum, filenames and
extra bytes. It writes runtime offset/size/type/valid state, truncates the type to
a byte in runtime storage while passing the full word to virtual +0x30, and
conditionally stores symbols/checksums/update records under flags 8/0x10/0x20.
Update records copy the actual extra bytes and own a copied alias list.

It registers the symbol and first filename, saves the stream position, invokes
the entry callback, restores the position through the current stream vtable,
and releases temporary strings/arrays. Finally it conditionally sorts CRC entries,
clears the sort flag, compacts CRC storage, conditionally packs read-only runtime
storage, invokes virtual +0x3C, marks FileValid and frees the pair table. The
return type is **bool** (true), correcting the earlier external declaration.
Opening/manager fixtures were updated for that ABI and remain separately scoped.

Disassembly caution: the metadata-copy load at 009D01BE occurs before cleaning
up a pushed allocation argument. Its stack offsets therefore refer to End/Begin,
not Capacity/End. It copies blob bytes, not the vector descriptor for nonempty
blobs. The connected metadata cases verify this interpretation.

## Verification

```powershell
python tools/decomp_pipeline/check_ui_bank_storage_helpers.py
python tools/decomp_pipeline/check_ui_bank_aliases.py
python tools/decomp_pipeline/check_ui_bank_entries.py
python tools/decomp_pipeline/check_ui_bank_storage.py
python tools/decomp_pipeline/check_ui_bank_open.py
python tools/decomp_pipeline/check_ui_manager_construction.py
```

- Storage helpers: **3,328 PASS**, full old/new storage, owners and allocator/free
  order, aliased fill, tree shapes/shared keys, shrinking and growing arrays.
- Alias lists: **3,072 PASS**, cookie/count differences, reverse release, zero to
  255-element resize, failed resize allocation, shared/null records and self-copy.
  Negative-count copy, failed copy allocation and corrupt cookies are excluded.
- Entry reader: **512 PASS**, zero/multiple entries and pairs, optional storage
  flags, empty strings/lists/blobs, meaningful runtime fields, persisted metadata,
  callback order and position restoration, changed finalization flags, real slow
  reads on partial/empty initial windows. Successful allocations and valid indexes.
- Expanded setup/erase gate: **1,024 PASS**, now with real helpers replacing doubles.
- Opening regression: **2,048 PASS** after bool ABI correction. Its decoder remains
  a sample-read double; it is not evidence of a full connected file-open-to-decoder
  path. The new entry gate exercises the actual decoder separately.
- Manager construction regression: **324 PASS** after the declaration correction.

Reports and disassembly: `work/ui_bank_storage_helpers_check/`,
`work/ui_bank_aliases_check/`, `work/ui_bank_entries_check/` and existing gate
folders. All use retail executable SHA-256
`41dc91090ae853715ac06d2e9fc96e5d545381d197ed55d624c642f34509ac10`.

Three new families add **6,912 cases**. Twenty-one saved passing families total
**26,793 cases**, an aggregate of saved reports, not a fresh rerun of all families.
Bootstrap now includes the new gates. No full bootstrap, GPU/game/presenter launch
or commit. Source/tests/docs saved uncommitted in native-play.

## Remaining boundaries and next work

The actual entry reader links real bank setup, vectors, alias lists, string reads
and lifetime, and slow stream reads. Its fixture still controls allocators, direct
backing reads/seek and bank virtual callbacks. These named services remain external:

- **009CE050** symbol/filename/CRC index registration, including 0042D131 map
  insertion, 004014A0 CRC, 009B6300 vector insertion, filename normalization and
  filename-map insertion.
- **009B85A0** CRC sort, **009B7B10** CRC storage compaction, **009CD740** runtime
  packing, bank virtual **+0x28/+0x30/+0x3C** behavior.

Next recover those services and connect the real decoder into the bank-opening
fixture with valid serialized entries and properly initialized storage. Then
real threaded file Open **0098E1E0**, production allocator/exception bindings,
registry/mode-global alias, frontend Run/actions and rendering. Existing live D3D
CreateDevice failure remains unresolved. No playable standalone game is claimed.
