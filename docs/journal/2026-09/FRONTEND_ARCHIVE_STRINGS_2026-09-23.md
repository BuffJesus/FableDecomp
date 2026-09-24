# Native archive string decoding - 2026-09-23

Worktree `D:\Documents\FableTLC-native-play`, branch `wip/native-cgame-play`.
Continues [stream reads](FRONTEND_BANK_STREAM_READS_2026-09-23.md).

Three complete readable bodies for valid archive inputs:

| Address | Routine | Compiled / retail bytes |
| --- | --- | --- |
| 00411910 | Zero-initialized byte-vector construction | 87 / 77 |
| 009D2AF0 | Zero-initialized word-pair-vector construction | 90 / 94 |
| 00996390 | Length-prefixed narrow-string read | 312 / 334 |

All are functional DIFFER. Array storage is begin/end/capacity pointers and uses
00BFEA0E / 00BFEA14, distinct from the narrow string-buffer allocator. Zero counts
skip allocation. The pair constructor checks each destination address for null;
a failed single-pair allocation leaves Begin null and End/Capacity equal to 8.
That peculiar non-crashing case is preserved and tested, not generalized into
allocation-failure recovery for larger arrays.

The string reader consumes a four-byte length, obtains a zero-filled length+1
temporary, consumes the full declared byte range, and appends a NUL. It constructs
and copies a real CCharString, destroys the temporary string and releases the byte
vector. Length zero constructs an empty result without the byte-vector allocation.
Embedded NUL truncates the resulting CCharString while the stream still consumes
the full declared range. High bytes are copied without conversion. Successful
return increments the string-instance count once; destroying the result balances it.

The source preserves retail's 0x7FFFFFFF position bound. A length prefix skipped
by that bound leaves an undefined local in retail; malformed lengths, unbounded
allocations, exhausted files and crashing allocation-failure paths are **not**
covered or claimed safe. No invented malformed-archive recovery was added.

## Verification and connection

```powershell
python tools/decomp_pipeline/check_ui_bank_decode.py
python tools/decomp_pipeline/check_ui_bank_open.py
```

Both pass: **1,536 new decoder-dependency cases**, comprising 256 byte-vector,
256 pair-vector and 1,024 string cases; **2,048 connected opening cases**.
The independent gate links real narrow-string lifetime and all four recovered
slow/refill/direct stream readers. It compares allocated bytes, guard bytes,
record fields/refcounts, stream fields, refill contents, file position and service
ordering. Cases vary lengths through 127 bytes, prefix/payload splits across
buffer boundaries, initial buffered bytes, 4/8/16/32-byte buffers, embedded NUL,
high bytes and non-crashing string-record allocation failure.

At the opening fixture's still-external entry-decoder boundary, a representative
four-byte field is now followed by a real length-prefixed string read and cleanup.
The retail boundary thunk invokes actual 00993CA0, 00996390 and 0099EAE0. File data
is synthetic and seek/read callbacks are controlled. This connects the readers to
opening but does **not** recover the full entry loop or prove archive disk loading.

Bootstrap includes the new independent gate. Function identities and resume docs
updated. Sixteen saved passing gate families total **17,833 cases**; older families
were not all rerun. No full bootstrap, GPU/game/presenter launch or commit in this
increment. Original checkout preserved; all changes saved in native-play.

Reports/disassembly: `work/ui_bank_decode_check/`, `work/ui_bank_open_check/`.
Oracle SHA-256:
`41dc91090ae853715ac06d2e9fc96e5d545381d197ed55d624c642f34509ac10`.

## Next: bank storage initialization and the entry loop

009CFBC0 still needs **009CEAE0**, which sets bank Size, clears/resizes RuntimeData,
clears Symbols and conditionally reserves them (OpenFlags bit 8), clears/resizes
Checksums (bit 0x10), clears/resizes UpdateData (bit 0x20), and empties two maps.
Its dependent bodies include:

- 009D3D90 runtime-data vector clear, calling 009D1C30 before freeing storage.
- 009D4010 runtime-data resize, using 009D1C30 to shrink and 009D3A20 to grow.
- 009D3FB0 symbol vector clear, calling 0043336A and real string destruction.
- 009D3DF0 update vector resize, calling 009D3300 to grow.
- 0049B760 symbol reserve, 00464931 checksum resize and 00579435 tree cleanup.

These are reconnaissance identities, not implemented/tested recoveries. The pair
table constructor and archive string reader are now available for the entry loop.
Entry helpers 009D2160 / 009D21D0 / 009CE050, finalization and threaded file Open
0098E1E0 remain, followed by frontend Run/actions and persistent rendering. Native
exception bindings, registry/mode alias and the previous live D3D failure remain
open. Standalone gameplay is unfinished.
