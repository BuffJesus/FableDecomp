# Native archive bank storage - 2026-09-23

> Superseded in part by the [entry-reader continuation](FRONTEND_BANK_ENTRIES_2026-09-23.md): all four remaining storage helpers now recovered and connected; 0049B760 resizes symbols rather than reserving them. The original checkpoint below records its then-current boundaries.

Worktree `D:\Documents\FableTLC-native-play`, branch `wip/native-cgame-play`.
Continues [archive strings](FRONTEND_ARCHIVE_STRINGS_2026-09-23.md).

Six readable implementations recovered and tested:

| Address | Behavior | Compiled / retail bytes |
| --- | --- | --- |
| 00414E00 | Destroy narrow-string range | 35 / 33 |
| 0043336A | Erase string-vector range | 75 / 73 |
| 009D3FB0 | Clear bank symbol storage | 81 / 95 |
| 009D3D90 | Clear runtime-entry storage | 51 / 90 |
| 009CEAE0 | Prepare bank entry storage | 349 / 375 |
| 009D4010 | Resize runtime-entry vector | 407 / 87 plus retail helpers |

All functional DIFFER. The 407-byte resize implementation includes growth and
reallocation behavior that the 87-byte retail wrapper delegates to 009D3A20 and
009D3580. It is not a claim that equivalent complete implementations are 407 vs
87 bytes, nor that those general-purpose insertion helpers are separately recovered.

## Behavior and connection

Bank preparation sets Size, clears and resizes 12-byte RuntimeData entries, clears
Symbols and conditionally reserves them using OpenFlags bit 8, clears/resizes
Checksums under bit 0x10, clears/resizes UpdateData under bit 0x20, then clears
SymbolIndices and FilenameIndices when their counts are nonzero. Flags are read
at each decision, including after callbacks. Tree heads are reloaded after subtree
destruction before sentinel links/count are reset.

String erasure assigns the suffix down over the erased range, destroys the tail
and updates End. Actual string assignment/unassignment/destruction is linked;
shared records retain correct owner counts. Full symbol clear invokes real erase,
resets the vector and frees its storage. Runtime clear uses trivial entries and
does not destroy pointed-to objects; retail's copy-helper call there has identical
source endpoints and copies nothing. Zero-capacity clear preserves the vector.

Runtime resize shrinks without reallocating, does nothing at equal size, fills
available capacity in place, or allocates length+max(length,added) entries. The fill
value may alias the old buffer: old storage remains alive until copying/filling
finishes. Header stores after free overwrite callback changes as retail does.
The bank's empty runtime-entry prototype initializes ten meaningful bytes; its
two padding bytes are unspecified in retail. Native zeroes them and the connected
test masks just those padding positions. Direct resize tests compare all bytes
because their supplied prototype has explicitly initialized padding.

## Verification

```powershell
python tools/decomp_pipeline/check_ui_bank_runtime_vector.py
python tools/decomp_pipeline/check_ui_bank_storage.py
```

Both pass **1,024 cases each**. Runtime-vector cases cover shrink/no-op/grow,
capacity reuse/reallocation, aliased fill values and a free callback overwriting
the vector header. Storage cases comprise 512 setup scenarios and 512 range
erasures, comparing all bank fields, old array storage, new storage, string records,
tree heads, counts and service order. Callbacks can change OpenFlags and replace
tree heads. Allocators remain controlled; allocation failure and invalid vectors
are outside this gate's verified scope.

The storage gate now links actual runtime resize instead of its first development
double. **Symbol reserve 0049B760, checksum resize 00464931, update resize 009D3DF0
and subtree cleanup 00579435 remain explicit boundaries.** Thus this is verified
bank-setup orchestration with real runtime/string storage handling, not a complete
archive loading implementation. It is not yet connected into the still-unrecovered
009CFBC0 entry loop. Earlier bank-opening/string gates are unchanged this increment.

Bootstrap includes both new gates; function identities and resume docs updated.
Eighteen saved passing families total **19,881 cases** (2,048 newly added). This
is an aggregate of saved reports, not a rerun of all older cases. No full bootstrap,
GPU/game/presenter launch or commit. Changes saved uncommitted in native-play.

Reports/disassembly: `work/ui_bank_runtime_vector_check/` and
`work/ui_bank_storage_check/`. Oracle SHA-256 remains
`41dc91090ae853715ac06d2e9fc96e5d545381d197ed55d624c642f34509ac10`.

## Next

Recover the four remaining bank-setup boundaries, then integrate bank setup,
pair-table construction and string reads into **009CFBC0**. Its entry helper
bodies 009D2160 / 009D21D0 / 009CE050 and virtual callbacks still need recovery.
Then bank finalization, real threaded file Open 0098E1E0 and frontend/rendering.
Production exception bindings, registry/mode alias and the prior live D3D failure
remain open. Standalone gameplay remains unfinished.
