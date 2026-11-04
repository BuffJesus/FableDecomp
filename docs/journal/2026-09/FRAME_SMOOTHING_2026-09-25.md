# Frame smoothing recovery, 2026-09-25

Continues land/boot-debake's main-component call-chain work. This does not wire Play
or establish playable world entry.

CFrameRateSmoother::AddFrameTime (0049b9e0, 63 bytes) and CalculateSmoothedTime
(0049b9a0, 59 bytes) are now genuine C++ RELOCATION_MATCHes. AddFrameTime overwrites
the oldest sample when full, advances circular indices, and calls the averaging
method. CalculateSmoothedTime walks NoTimes samples starting at FirstTime, wraps by
MaxNoTimes, and stores their arithmetic mean. The empty-buffer result remains NaN,
matching retail's division by zero; no invented guard changes the behavior.

The shared class now models its 12-byte STLport float vector and the PDB-named
counters, with retail's offsets corrected for the smaller vector. SmoothedTime is
at 0x28 and total size is still 0x30. The first eight base-class bytes remain opaque.
The recovered averaging accumulator is float; VC7.1 retains it on the x87 stack.
Using double changes the constant-load width and fails exact parity.

Verification:

- Both leaf fixtures pass. They check partial/full buffers, wraparound, capacity one,
  negative samples, call ordering, untouched bytes, and the empty-buffer result.
- gate_frame_smoothing.py separately compiles and links all three real methods
  (AddFrameTime, CalculateSmoothedTime, GetSmoothedTime). Seven consecutive samples
  verify filling and repeated overwrites, final ring contents and mean, and guards.
- Mutating the full-buffer count update or removing the averaging division makes
  the corresponding fixture fail.
- All 118 boot leaves and all component ABI checks pass; zero regressions across
  35 header-dependent catalog entries.

Full unmodified Release bootstrap exits zero, including WinMain, GFMain phases,
Stage 3 and visual checkpoint. Next substantial work
remains component initialization/run and frame-smoother allocation/initialization.
