# New Oakvale compatibility add-on: local-first plan

## Execution checklist ? overnight update

- [x] Steps1?3: unchanged upstream baseline, independent hosts and cooperative loading.
- [x] Steps4?5: executable hook/loader checks and one successful live registration.
- [x] Step6: local no-install payload, hash validation and per-run save backups.
- [x] Step7, childhood portion: user completed childhood; logs record quest completion.
- [ ] Step7 remainder: save/reload, other original-FSE custom quests and audio correction.
- [ ] Distribution: withheld pending remaining local validation.

Latest evidence, candidate distinctions and restart instructions:
[overnight handoff](NEW_OAKVALE_NIGHT_HANDOFF_2026-09-12.md).
User paused for sleep. The current V5 audio hook is diagnostic only; it extends
the baseline registration-only hook set in an isolated candidate, not a release.


## Constraints

Keep original FSE DLL bytes unchanged. No distribution or publication until local
validation. Preserve installed v23 as the rollback baseline and all shared work.
No no-op API fallbacks, altered RNG or duplicate native/Lua quest activation.

## Implementation sequence

1. Pin clean upstream source and build an unmodified local baseline with isolated
   output/intermediate directories. Record that a locally compiled upstream DLL
   is not a verified maintainer release binary.
2. Prove a cooperative loading path. Upstream has no stable plugin export and
   patches the game registration epilogue at CDB355. An add-on must preserve its
   callback and epilogue rather than overwrite them. Validate hook bytes and
   module ownership; reject unexpected versions and double installation.
3. Prefer separate quest hosts/VMs owned by the add-on over exchanging C++/sol
   objects across DLL boundaries. Reuse reviewed runtime behavior for this quest.
   This may require a substantial sidecar runtime, not a tiny binary patch.
   Original FSE continues owning its own quests; add-on owns only the retail
   New Oakvale override. Establish independent configuration/log paths.
4. Restrict add-on hook installation to registration and reviewed quest needs.
   Remove unrelated rendering/map experiments from its installation path.
   Verify original callback runs on each registration/reload and that the retail
   quest record is replaced once while retaining native name/identity.
5. Compile and execute isolated loader/hook tests before game deployment. Check
   rejected inputs leave bytes untouched, forwarding preserves machine state,
   and both hosts run without sharing allocator-owned objects or VM lifetimes.
6. Assemble local-only payload with exact file hashes and reversible configuration
   changes. Back up current v23 and refuse deployment while game is running.
   Check rollback in a temporary install fixture before touching the game.
7. Coordinate live testing: launch, original-FSE initialization, Father intro and
   Escape, Bully skip, deed branches, Theresa/chocolates, raid and save/reload.
   Existing audio bug remains an explicit open issue; use prepared audio observer.

## Gates

Do not equate successful compilation with compatibility. Do not install a sidecar
that silently disables original FSE or merely renames our replacement DLL.
If independent-host coexistence fails, return to a targeted binding-patch design
and document the failure. Report offline and live results separately.

## Initial evidence

Upstream master b2432e82b2ef1e5fd50b8fac2a8809bd2bd107d6. Its InstallHook writes
E9 at CDB355 and MyHook calls InjectCustomScripts then executes the displaced
epilogue. Its DllMain has no plugin registration/export surface. Current
compatibility-name audit reports eight missing required bindings; behavior and
registration also differ (see original-FSE compatibility journal).
