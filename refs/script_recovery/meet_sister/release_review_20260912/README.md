# Meet Sister review package — not installed or enabled

`package-manifest.json` identifies all seven staged Lua files, the runtime source
snapshot and dependency evidence. It intentionally contains no release DLL:
source validation does not identify a deployable binary. `api-proposal.json`
contains the optional binding-flags update and 13 proposed API entries. Neither
artifact changes the shared API manifest, SDK overlays or installed profile.

Recreate in a **new** directory from the repository root:

```powershell
python tools/script_recovery/prepare_meet_sister_release.py --forge-root D:/Code/ForgeFSE-retail-shadow --output work/meet-sister-release-review-NEW
```

The metadata proposal requires coordinated support for the `RetailResources`
receiver and callback parameter. Do not replace existing metadata entries using
an old baseline if the converter session has changed the manifest.

## Resource contract

The actual package and runtime adapter are the usage example. In particular:

```lua
quest:WithRetailResources(function(r)
    local own = r:NewResource()
    r:PrepareResource(own)
    while not r:TryAcquire(own, me, 4) do
        r:Frame()
        if quest:IsActiveThreadTerminating() then
            r:ReleaseResource(own)
            return
        end
    end
    -- Follow the individual native script's termination and cleanup order.
    r:ReleaseResource(own)
end)
```

`TryAcquire` is one attempt. A failed attempt can still be copied as an empty
resource into an actor map, as required by the actual Meet Sister Hero path.
Resource IDs belong to one scope and have distinct kinds. Release invalidates
the local ID; map copies retain their own references. Explicitly unpause,
destroy movies/maps and release resources at their native positions. Scope exit
closes leftovers in reverse creation order as an error fallback. A retained
scope is unusable after callback exit; GC does not defer native cleanup.

`RunMacro` borrows the map and uses nil flag/input maps. It adds no movie,
camera, acquisition or post-macro termination policy. Pass Lua `false`/`true`
for booleans: numeric zero is truthy through sol. Mixing this resource scope
with legacy cached control/movie helpers is not reviewed.

## Later deployment procedure

These steps are release preparation, not actions performed by this package.

1. Complete the coordinated metadata update and build a Release x86 runtime.
   Record the binary hash/build inputs and rerun the clean resource checks.
2. Establish a separate disposable test profile/save and arrange live timing.
   Keep the New Oakvale test profile/configuration backed up and recoverable.
3. With the game closed, record hashes and backups of both installed DLL copies,
   all profile/config files that will change, and the presence/absence of each
   new package file. Keep the exact original registry and override configuration.
4. Prepare a merged override configuration for review. Do **not** overwrite the
   shared `retail_override.lua` with this standalone staging file or add a legacy
   `quests.lua` identity. The intended native identity is `QS_MeetSister`, with
   Messenger ID 220 and Sister ID 221. Verify collision-free registration against
   the current runtime configuration before activation.
5. Install the identified DLL consistently to both locations and copy only the
   reviewed package files. Verify hashes and single activation authority. Do not
   edit the native FinalAlbion quest declaration to force New Oakvale activation.
6. Test normal and Escape outcomes for both entities, including region streaming
   and cleanup. Record actual outcomes separately from the offline evidence.

## Rollback

Close the game, restore both exact pre-install DLLs and the saved registry,
override and profile files. Restore any files that previously existed; remove
only newly created files listed as absent in the pre-install manifest. Restore
the backed-up disposable profile/save as needed. Verify every restored hash and
activation authority before launching again. Use the captured pre-install
manifest, not an assumed version label.

Installed v23 was not changed by this preparation. Live playback, streaming,
audio and camera behavior are acceptance work still outstanding.
