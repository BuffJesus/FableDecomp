# Native development update - October 24, 2026

Native reconstruction continues locally. The concrete text-bank composition
checkpoint passed 108 bounded processes covering text, group, and narrator
metadata, with 18 native fault controls. These are composition-test results,
not an increase in whole-executable coverage. Decoded payloads and nonempty
resource ownership remain under investigation; no full `gate_all` run or
playable engine is claimed.

Separate C++23/x64 subsystem libraries and offline replay tests also exist
locally. This does not certify a playable modern engine or a complete headless
gameplay session. Scheduling and ownership joins continue through their own
qualification and promotion gates.

This publication updates organization and status only. Newer native and modern
source, fixtures, generated reports, local receipts, and retail-derived inputs
remain outside this change pending dependency-complete review. The published
coverage dashboard keeps its existing snapshot.

See [the handoff](HANDOFF.md), [current priorities](ROADMAP.md), and
[workspace organization](PROJECT_LAYOUT.md) for navigation and safe continuation.

## Historical October 10 checkpoint

The following results and unresolved boundaries describe October 10. Later
bounded composition work above does not establish that all of these earlier
boundaries have been closed.

### Native development update — October 10, 2026

The project is active. Work has continued locally on faithful native PC
C/C++ reconstruction, rather than Lua recovery. The gap in public commits
does not reflect a pause in that work.

This is a status publication, not a merge of the newer local implementation
checkpoint. The source, tests and reports described below remain local pending
a separate dependency-complete publishing review. Existing build instructions
still describe the authored integration checkpoint shipped in `main`.

## Recent verified local progress

- Connected texture-manager allocation, mip reduction, frame initialization,
  byte accounting and lifetime routines into a 20-provider differential test:
  6,912 cases and 129,792 complete memory snapshots. Removing any mandatory
  provider fails linkage; original-compiler parity is checked separately.
- Recovered additional pixel-format accessors, texture queries, resource-list
  operations, frame cleanup/loading routines and original writable data.
  These are native subsystem recoveries, not a replacement game loop.
- Rebuilt the real D3D9 drawing test with 17 mandatory production providers.
  Seven control frames pass full readback comparisons covering 114,688 pixels.
  An earlier device-loss failure no longer reproduces; its cause is unknown.
- Read `FRONTEND_TITLE_01_SPRITE` from a local retail `frontend.big` and rendered
  it through the recovered drawing path. All 46,080 RGB pixels in the test
  target match an independent decoder/reference, including the background.
  Eighteen omission checks cover the production providers and integration
  decoder. This is one title sprite/tile, not the complete title/menu screen.

The latest local native link closure contains 1,792 source files, 442 unresolved
symbols and zero duplicate definitions. The separate native recovery ledger
records 381 exact functions, 71 accepted near-match functions and 43 data
providers. These are checkpoint-specific figures, **not** whole-executable
coverage, playable-game completion percentages or additions to the older
generated dashboard. No full `gate_all` run is claimed for this checkpoint.

## What the visual result does and does not prove

Recovered drawing routines execute on a real GPU with retail content. Bank
reading/decompression use integration ports; window/device creation, texture
upload, test geometry and readback remain harness boundaries. The native bank
`LoadGraphicFrame` path is not yet recovered end to end.

The existing boot-movie and interactive frontend executable is an authored
integration checkpoint. Neither that frontend nor the title-sprite test proves
a complete reconstructed native engine launch, world rendering or gameplay.
The game is not yet playable from reconstructed source.

## Current recovery boundaries

The nested frame loader passes 3,568 bounded original/model/compiled cases and
67,044 complete 64 KiB memory snapshots, but its second candidate reaches only
88.99% instruction similarity. It remains deferred after two parity failures;
behavioral success alone does not make it accepted reconstruction. Its I/O,
threading, allocation and external method boundaries are still explicit.

Surface locking is also deferred after two failed candidates. New evidence
must justify another attempt. A second physical async `GetData` body is a clone
of the existing nonconst routine, not an established const overload and not
extra recovery credit.

## Next steps

1. Review and publish dependency-complete native source/test checkpoints without
   merging unrelated Lua work or local experiments.
2. Continue recovering the native bank-loading and resource-lifetime dependencies
   needed to replace the current integration boundaries.
3. Extend real-content rendering toward a persistent visual launch, then world
   and gameplay initialization. No launch date or completion percentage is promised.
4. Reconcile canonical reports before refreshing whole-project percentages.

Retail executables, archives, artwork, decoded textures, screenshots, PDBs and
scratch byte oracles are not part of this publication. Tests consume assets
from a user-supplied installation; no proprietary assets are being distributed.
