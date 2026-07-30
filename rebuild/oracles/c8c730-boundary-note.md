# Oracle boundary note — 0x00C8C730 `Batch_InvokeWithStackArgs`

## Summary
The curated true length of `Batch_InvokeWithStackArgs` at `0x00C8C730` is **326 bytes**
(`0x00C8C730` .. its `ret 0xc` epilogue at **`0x00C8C876`**). Parity was verified against
this 326-byte oracle: `check.py` prints
`RESULT: RELOCATION_MATCH masked_byte_diffs=0 built_len=326 pragma=''`.

## The pipeline bug (do NOT "fix" by editing this function's oracle length manually elsewhere)
`pe_oracle.py` computes each function's oracle byte-region by slicing from a function's
manifest address up to the **next manifest address**. For `0x00C8C730` the next *manifested*
address is well past the real epilogue, because the **two trailing thunks that immediately
follow this function are NOT present in the rebuild manifest**:

- Thunk #1 at ~`0x00C8C7Ax` — a short `__thiscall` float thunk.
- Thunk #2 at ~`0x00C8C8xx` — a second short `__thiscall` float thunk.

Because neither thunk has its own manifest row, `pe_oracle.py`'s next-manifest-address
boundary skips past both of them, so the naive oracle for `0x00C8C730` runs long and
**over-captures** — the earlier oracle came out at **393 bytes** (326 real + the two
trailing thunks). The two thunks are separate functions and must NOT be transcribed as
part of `Batch_InvokeWithStackArgs`.

## Consequence / guidance
- Curated / authoritative length for `0x00C8C730` = **326 bytes**. Preserve this.
- Do NOT modify `pe_oracle.py` (its next-manifest-address heuristic is used project-wide).
- The proper long-term fix is to add manifest rows for the two trailing thunks
  (~`0x00C8C7Ax`, ~`0x00C8C8xx`) so the boundary lands correctly; until then this note
  records the true 326-byte length so future re-derivations do not regress to 393 bytes.
- Landed source: `lift/reports/wave3/code/00/c8/0x00C8C730_global_Batch_InvokeWithStackArgs_c8c730.cpp`
  (`__declspec(naked)` full transcription; call targets are reloc-masked externs).
