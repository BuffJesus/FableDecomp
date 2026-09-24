# Native archive stream reads - 2026-09-23

Worktree `D:\Documents\FableTLC-native-play`, branch `wip/native-cgame-play`.
Continues [path/string recovery](FRONTEND_BANK_PATHS_2026-09-23.md).

Recovered four complete readable stream routines needed by entry decoder 009CFBC0:

| Address | Routine | Compiled / retail bytes | Grade |
| --- | --- | --- | --- |
| 00993CA0 | Slow byte read | 300 / 351 | DIFFER |
| 00994360 | Refill/source acquisition | 77 / 69 | DIFFER |
| 009943B0 | Buffer-use decision | 19 / 19 | RELOCATION_MATCH |
| 009943D0 | Direct file read | 49 / 41 | DIFFER |

The slow reader consumes existing buffered bytes, asks virtual +0x24 whether to
buffer the remainder, then either loops over source chunks (+0x20) or invokes
direct reading (+0x28). Buffer choice uses signed `request < BufferSize`; equality
selects direct reading. Refill uses unsigned min(BufferSize, StreamSize-position),
seeks the file, requests bytes via file virtual +0x0C with its final flag false,
and returns the current Buffer pointer. Direct reading also seeks first.

Callback ordering matters: file and buffer pointers are reloaded after callbacks.
Direct slow-path completion restores position from the pre-callback position plus
requested length and clears the buffered window. Buffered copies preserve the
retail unsigned position bound of 0x7FFFFFFF, including wrapping arithmetic.
The slow reader expects callers to have selected it when existing bytes are
insufficient; it does not clamp the initial available-byte count to the request.
No EOF/zero-progress recovery was invented; invalid streams that cannot replenish
the requested bytes remain outside the exercised valid-stream cases.

## Evidence

```powershell
python tools/decomp_pipeline/check_ui_bank_stream_read.py
python tools/decomp_pipeline/check_ui_bank_open.py
python tools/decomp_pipeline/check_ui_bank_stream.py
python tools/decomp_pipeline/check_ui_manager_construction.py
```

New reader gate: **2,048 cases**, all passing. It compares full stream memory,
guards, buffers, destination bytes, file positions and callback order. Cases
include partial preexisting buffers, zero/exact/large request thresholds, signed
buffer choice, position wrap, callbacks replacing File/Buffer and mutating
StreamPos, and a position callback shrinking capacity to force repeated refills.
File seek/read callbacks remain controlled; no real OS read is claimed.

The **2,048-case bank-opening gate** now links all four bodies. At the still-external
entry-decoder boundary it consumes four representative bytes through the real
slow/refill chain, checks the result and observes the resulting close-time seek.
The oracle boundary thunk calls actual retail 00993CA0; it does not model that
reader in Python. An initial logging-hook offset fired before the reader call;
corrected to the post-call address, then the entire gate passed. This integration
does not implement the entry decoder itself.

The **1,024-case stream lifecycle gate** also passes. The **324-case manager regression** passes
(`work/ui_manager_construction_check/report.json`); the shared vtable
view grew to include the three recovered slots, but manager's unavailable-file
path does not invoke these reads. Bootstrap includes the new independent gate.

Fifteen saved passing construction/bank/string/texture families total **16,297
cases**, including 2,048 new cases. Full bootstrap and live GPU/game launch were
not run. Changes remain uncommitted in the isolated native-play worktree.

Reports and disassemblies: `work/ui_bank_stream_read_check/`,
`work/ui_bank_open_check/`, `work/ui_bank_stream_check/`.
Retail SHA-256 remains
`41dc91090ae853715ac06d2e9fc96e5d545381d197ed55d624c642f34509ac10`.

## Next dependencies

Entry decoder **009CFBC0** starts with **009CEAE0** and temporary pair-array
construction **009D2AF0**, reads an initial count and word pairs, and invokes bank
virtual +0x28. Entry loops read scalar fields and strings via **00996390**, with
helpers **009D2160 / 009D21D0 / 009CE050** and bank virtual callbacks. A partial
disassembly/dependency map is saved as `next-archive-decoder.txt` in the new report
directory; it is reconnaissance, not a recovered decoder implementation.

Then recover bank finalization and threaded file Open **0098E1E0**, followed by
frontend Run/actions and persistent rendering. Production exception bindings,
registry/mode global alias and previous live D3D DEVICELOST remain unresolved.
The standalone game is not yet playable.
