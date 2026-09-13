# Current retail port review — September 12, 2026

Meet Sister and Maze Research are staged as disabled source packages. Each has
seven Lua files, an exact file manifest and a passing package validation.
The shared runtime test record passed six C++/Lua binaries; all 120 recorded
inputs still matched when this review was generated. Engine calls in those
tests are doubles. Nothing here installs, enables or launches either quest.

Reproduce into a new directory:

```powershell
python tools/script_recovery/prepare_retail_port_reviews.py --forge-root D:/Code/ForgeFSE-retail-shadow --runtime-proof work/runtime-conversation-lines-final-20260912/result.json --output work/retail-port-review-NEW
```

The generator rejects a stale runtime proof, changed Maze API baseline, failed
package validation or installed DLLs differing from v23. `verification.json`
hashes its generated outputs. This README is supplementary review guidance.
The older Meet Sister review remains historical; this directory's manifests
capture the newer shared-flag runtime sources.

## Runtime and metadata

`runtime-provenance.json` identifies the existing offline Release DLL and its
build log separately from both installed v23 DLLs. No DLL is bundled. A recorded
file hash identifies a candidate; it does not certify a clean reproducible
binary build or successful live playback.

Meet Sister's metadata proposal defines RetailResources plus optional entity
binding flags. Maze Research additionally requires RetailFlags, three Quest
shared-state accessors and RunMacroWithFlags, plus the borrowed-Thing correction
to SetThingAsUsable. Both proposals are unapplied. Before integrating, compare
their baseline hash with the current shared manifest and coordinate with its
converter owner. Do not replace that owner's newer changes with this snapshot.

Maze's named flag maps and retained Thing wrappers are C++ shared owners held
by the Quest. They may be exposed in separate Lua VMs without transferring Lua
objects. They are transient state, not serialized quest properties. The native
UNLIMBO child task must see the same map supplied to the macro. Its normal and
Escape flag timing must not be replaced with an immediate sword reveal.

## Later installation and acceptance

1. Identify the exact runtime build to test and confirm its source/proof hashes.
   Complete coordinated metadata integration independently of runtime activation.
2. Arrange live timing and a backed-up disposable profile/save. With the game
   closed, capture both installed DLLs and every affected file/configuration,
   including whether each destination existed before the change.
3. Merge only the selected native override into the current configuration.
   Do not copy either standalone staging retail_override.lua over the user's
   existing registry. Preserve the successful New Oakvale setup. Do not add
   legacy quests.lua activation or modify native quest declarations.
4. For Meet Sister, verify native identity QS_MeetSister and entity IDs 220/221.
   For Maze Research, verify V_MazeResearch and IDs 64/65. Check for conflicts in
   the actual merged configuration before enabling either package.
5. Install the selected binary consistently to both DLL locations, copy only
   the reviewed package files, and verify destination hashes and one activation
   authority. Keep a complete operation manifest for rollback.
6. Meet Sister still needs normal/Escape playback, region streaming and resource
   cleanup. Maze Research needs both morality branches, bookcase journal
   cancellation, the concurrent UNLIMBO task, normal/Escape macro playback,
   sword grant/removal, streaming and cleanup. Save/load behavior is unverified.
   Record these separately from offline test results.

## Rollback

Close the game. Restore the exact pre-install DLLs, configuration and files
from the operation manifest; remove only files recorded as newly created by
that installation. Restore the disposable profile/save if needed. Verify every
restored hash and native override identity. Use the captured state rather than
an assumed version label. No rollback action was needed during this review.

New Oakvale's audible dialogue issue, actual rendered fades and restored-v23
Father Escape remain separate live checks. These bundles make no claim that
those reports are resolved.
