# Existing genuine world-reset provider inside the frontend graph

The existing C++ ResetWorldTransform source at `00988290` compiles under a fresh
guarded O2/MT profile to 131 bytes, 24 instructions and 95.8333% near-match.
Historical native gates were not rerun. Independent static review verifies the
complete original/current body, guard, twelve zero/four one matrix stores, dirty
OR0xC990, identity byte and absence of external dependencies.

Current provider receipt under rendering/00988290/20261014:
`151af94e6385c2f07e15f01a716f6a691701a8630aacfc71872ea8348414cec2`.

Under `work/native-recreation/frontend/0042df9e/20261014/`:

- Readonly union: `13bcf77d4912247325d24f0ec7d864970cdb705da9b0f2252cf50c6e88d3ab36`.
- Independent readiness: `d2328cdd57f65fdf42c4931a3951c311b3f31be8a905b6a372d8c0994771f25b`.
- Draft: `71daa20b0a81bd06e0fb39b83cc9ea5a87607813e81e82227fd768e9f0208060`.
- Compile: `bc63effa27fdf66974cad30c2123dba17d9167e107fa4b75e31a037f677e5742`.
- Link: `68e94fd212d4c27e9f9aa8a760ade8f4d3a6c2a7ee85846ea92500d9f75156f5`.
- Selected sixty contributions: `ee4a10f195f19c4f4002a2f1b298736df660ff8cfc6072e3cb1f369319ca4ffc`.
- Caller/new-provider proof: `a5654ea43c69c0aef10d5b6eaa784f97026eb3104634d1cd5e2fbda34546a8ab`.
- Frozen manifest `world-reset-frozen-review-first/behavior_source_manifest.json`:
  `025ce7b1ab3a9a8e5201ac8491adb963820ba9fad24af55803f0f46ddc669277`.

The graph now has 44 original roots and the same 436 operand sites. Four former
world-reset boundaries become internal calls. The previous mock is removed;
the local Identity helper remains fixture setup. Both matrix identity states
are explicit. For false, one diagonal changes from1 to2 before snapshots, so
relevant parent paths invoke genuine reset. Some empty ordinary-clear frames
skip reset, and parent guards skip the callee's true-identity branch; residency
and invocation remain distinct.

The planned 49,152 cases / 98,304 snapshots multiply the previous finite product
by the new identity axis. Dirty mask0xC990 is preserved and downstream projection
callbacks/event bounds require fresh review; old Record18 disappears with the
mock. Caller/new-provider proof retains 57 unique code spans, 59 comparisons
and eight initialized data sections. No world-reset composition native invocation
has occurred; eighteen independent source checks are pending.

All integer-below-near, float mock, backend/shader/UI/engine mocks, borrowed
lifetimes, CRT/DLL/profile/COMDAT, specular4/stack, masked FP/store ordering and
no-GPU/game/save/x64 limitations persist. No canonical or coverage promotion.
