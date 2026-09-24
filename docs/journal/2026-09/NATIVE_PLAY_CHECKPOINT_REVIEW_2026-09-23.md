# Standalone checkpoint review - 2026-09-23

User requested a bedtime documentation save, then commit, push and review.
Branch: `wip/native-cgame-play`; remote: `origin` (BuffJesus/FableDecomp).
The checkpoint includes accumulated 2026-09-22/23 standalone frontend/engine work,
not only the latest archive decoder. Existing asm implementations moved to the
explicit archive directory remain historical evidence, not C++ recovery credit.

## Review performed

Self-review focused on the new archive decoder/storage ownership, integration
boundaries and bool ABI, changed shared game/boot declarations, visual animation
integration, bounded CGame::Play parity acceptance, bootstrap gate wiring and
renamed candidate catalog paths. No blocking issue was identified in that scope.
This is not an independent review or a line-by-line audit of every accumulated
recovery; the checkpoint spans hundreds of source/test files.

- Changed candidate catalog source/test paths exist; Python syntax checks for new
  tools pass; no merge markers found in new files; `git diff --check` passes.
- New files are C++/headers, Python, Markdown and JSON. Local builds, retail
  executable/assets, and ignored oracle reports are not included in the commit.
- Six bounded-parity checker tests pass. CGame::Play behavior/acceptance passes
  with only its documented two-byte register-selection difference.
- Frontend startup/stopwatch gates pass. Animation integration passes, including
  colour (1,296), swapping (1,520), random (4,480), fade (260), visibility (1,024)
  and hierarchy (1,034) comparisons; presenter compile check also passes.
- Latest archive helper/list/entry gates and affected setup/opening/manager gates
  passed before checkpoint review; see the entry journal for counts and boundaries.

## Known integration limits

The complete standalone executable has not been built or run in this review.
Frontend Run and real main-game runtime are unfinished. The new decoder's index
registration, sorting/compaction/packing and bank virtual callbacks remain explicit
service boundaries. The opening fixture still uses a sample decoder double. D3D
CreateDevice previously returned DEVICELOST; no live-device retry was attempted.
Passing offline comparisons do not establish playable standalone gameplay.

Next work and exact local paths are in
[NATIVE_PLAY_SESSION_RESUME_2026-09-23.md](NATIVE_PLAY_SESSION_RESUME_2026-09-23.md).
Push this checkpoint to the work branch; no main-branch merge is part of this save.
