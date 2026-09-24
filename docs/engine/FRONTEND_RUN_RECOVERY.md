# Retail frontend Run recovery

> 2026-09-23: archive entry reader 009CFBC0 now has a readable body and 512
> connected retail comparisons; storage/list dependencies add 6,400 comparisons.
> Index registration, finalization, real file I/O and bank callbacks remain open.
> See [entry checkpoint](../journal/2026-09/FRONTEND_BANK_ENTRIES_2026-09-23.md).

Target: CNewFrontendGameComponent::Run, 0042EC7C through 0042F50B (2,191 bytes).
This is an evidence map, not a completed implementation. Integration still
substitutes Run and destructors; no renderer/main-game runtime closure is claimed.

Evidence: installed retail executable SHA256
41dc91090ae853715ac06d2e9fc96e5d545381d197ed55d624c642f34509ac10,
retail disassembly and Ego_r PDB fields. Ignored local disassembly:
work/frontend_component/0042ec7c.asm. Propagated labels are not authoritative:
0042DD28 is called with the frontend receiver despite a label naming another
class. Recover its behavior before declaring a typed receiver.

| Retail address | Responsibility and remaining dependency work |
| --- | --- |
| 0042EC7C | Samples initial update/render times; sets up three boot movies. Playback depends on two global flags. System Update result 2 during movies returns false before normal frontend startup. Recover movie-call ABI and error-string ownership. |
| Before 0042EF33 | Conditional frontend bank opening with temporary wide strings. Recover path getters and bank ownership; inherited labels can be misleading. |
| 0042EF33 | Sets Running, calls Init2 and InitialiseEngine, allocates movie buffers, loads font data, clears/swaps display, starts menu state and attract timer, resets dimensions. |
| 0042F041 | Checks Quit, pending autosave sound, input-delay expiration, Input, Update, interpolation, Draw and attract movie playback. Attract playback resets its timer, clears input events, starts input delay and replays autosave sound. |
| 0042F20F | System Update precedes the first frame and every subsequent frame. Result 2 exits; otherwise the top-of-loop Quit check may exit before input/render work. |
| 0042F224 | Sets Quit true and Running false, deletes movie buffers if present, releases XMV code, clears frontend singleton, calls unresolved receiver helper 0042DD28. |
| 0042F24C | CreateDevFrontEnd has priority: Shutdown, allocate/construct legacy frontend (0x1E60), virtual Init, publish output component, retire this, return true. |
| 0042F297 | If m_bCreateGame: disables boot-movie flag, stops sound with argument 500, conditionally changes banks, prepares initializer, Shutdown, constructs main component (0x161E8), virtual Init, publishes replacement, retires this, destroys local initializer, returns true. |
| 0042F4EC | Ordinary quit: clears singleton, Shutdown, retires this, returns false. |

Do not consolidate the boot-movie early exit with normal shutdown: side effects
differ. Replacement branches call Init before publishing the output pointer.
Retail dereferences the replacement even when allocation returns null; an added
allocation fallback would be a behavior change.

The clock dependency is now implemented: CStopWatch constructor, GetTicks, Start,
StartZero, Stop, Reset and GetElapsedSeconds. check_stopwatch.py validates reviewed
fingerprints, retail hashes and deterministic clock success/failure scenarios.
Startup integrates the real constructor.

Next recover Init2 and 0042DD28, then system-manager and interpolation contracts
needed to test loop ordering. Keep unresolved movies, banks, rendering and
component destruction explicit until recovered. An empty dependency substitute
is not a working engine implementation.
