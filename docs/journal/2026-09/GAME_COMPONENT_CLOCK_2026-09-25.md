# Game-time call-chain recovery, 2026-09-25

Continues the timing increment on land/boot-debake. Play is still unwired in the
GFMain integration boundary; this increment recovers dependencies, not world entry.

## Recovered source and corrected identities

- GFGetTime (009e1bc0), 168 bytes: RELOCATION_MATCH. Named address-bound globals
  represent initialization, selected clock backend, frequency and starting tick.
  It probes QueryPerformanceCounter once, queries frequency, and then reads either
  the performance counter or WINMM timeGetTime. Frequency failure and subsequent
  counter-query failure handling are preserved as retail implements them.
- CMainGameComponent::UpdateAverageFrameDuration (00416202), 23 bytes:
  RELOCATION_MATCH. Replaces a raw-byte bake incorrectly labeled operator_new.
  Calls CFrameRateSmoother::AddFrameTime on the member at 0x16178.
- CMainGameComponent::GetCurrentGameTime const (00416231), 13 bytes:
  RELOCATION_MATCH. Calls the real zero-argument GFGetTime and subtracts GameStartTime.
  The old local type incorrectly supplied this as an argument to a fabricated helper.
- CMainGameComponent::GetPredictedTimeSinceLastRenderFrame const (00416258), 16
  bytes: RELOCATION_MATCH. Calls GetCurrentGameTime and subtracts LastRenderFrameStartTime.
  Previously mislabeled as a second GetCurrentGameTime.
- CFrameRateSmoother::GetSmoothedTime const (0049b990), 4 bytes: MATCH. Reads the
  double at retail offset 0x28, rather than the propagated CMap::DrawGetSeaLevel name.
  Remaining frame-smoother members are still opaque; its total size remains 0x30.

FableWin decorated symbols confirm the method names, constness and protected/public
access. Retail call sites and member offsets distinguish the mispropagated labels.

## Verification

All five source objects pass parity and linked behavioral fixtures. The four wrapper
fixtures no longer substitute their own implementation or merely print PASS. They
check receiver offsets, arguments, signed elapsed results, and untouched object bytes.

GFGetTime tests cover first/repeated calls, backend selection, the exact TRUE (1)
comparison, millisecond fallback, and 64-bit counter subtraction. Its query-result=2
case rejects a mutation that treats any nonzero value as success. Two further negative
controls invert the game-time subtraction and are rejected by the wrapper fixtures.

All 118 boot leaves and the component ABI gate pass. Shared-header gate: zero
regressions across 32 dependent entries. Full unmodified Release bootstrap exits zero, including WinMain, GFMain phases,
Stage 3 and visual checkpoint.

## Parked candidate

GetAverageFrameDuration (00416219) forwards to GetSmoothedTime. The current C++
candidate emits an 11-byte add/jmp; retail uses a 12-byte add/call/ret. It is saved in
rebuild/backlog/near_miss and is not promoted. Its existing assembly catalog entry is
unchanged. The next substantial dependency is CFrameRateSmoother::AddFrameTime,
followed by the rest of the main component's Init/Run call graph.
