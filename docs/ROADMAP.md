# FableDecomp roadmap

Updated October 24, 2026. This is the cross-project priority summary; each active
lane owns its current task and acceptance record. Start at [HANDOFF](HANDOFF.md).

## Ongoing work

- Native C++ resource/font ownership: follow the [native continuation](journal/2026-10/2026-10-24-native-recreation-continuation.md). Streaming font module complete 2026-10-27 (every function reconstructed and gated); static font, bank-loaded UI frame and game-text frame accepted. Range crawl 2026-10-27: 0x9D8000..0xA20000 landed as exact-root gates (font manager, system utilities, engine services/support, navigation; ~290 roots); next range 0xA20000.
- C++23/x64 scheduling and subsystem ownership: follow [modern resume](modern/NEXT_SESSION.md).
- Lua recovery stopped for tonight at user request; W507 accepted/W509 bundle verified, W511 scratch-only next, W499 ABI gate retained: follow the [Lua checkpoint](scripts/LUA_RECOVERY_HANDOFF.md) and [owning journal](journal/2026-10/LUA_RECOVERY_2026-10-24.md).
- Forge work remains in its own checkout and handoff.

## Engineering follow-up

- [ ] Require production-bound behavior assertions and negative controls across admission paths; distinguish parity-only tests.
- [ ] Reconcile unique-address coverage, assembly exclusion, population membership, and proof freshness before claiming new completion percentages.
- [ ] Fail closed on absent evidence and missing tests; give unresolved pinned type conflicts an explicit assurance state.
- [ ] Prove one dependency-complete loading, retained-resource, update/render, and teardown milestone with genuine services.
- [ ] Improve readable ownership, fields, callees, and interfaces along complete call chains.
- [ ] Publish reviewed source slices with complete build/test dependencies and distributable controls.

These are audit follow-ups, not instructions to interrupt current owners or to
promote their scratch work. The [verification contract](pipeline/VERIFICATION_CONTRACT.md)
distinguishes intended standards from gaps in their current enforcement.

## Documentation

Current instructions belong in short lane handoffs; reference facts belong under
their topic; dated measurements and failed attempts belong in the journal.
Keep generated metrics generated. Preserve historical evidence rather than
copying its old statuses into current pages. See [project layout](PROJECT_LAYOUT.md).

<a id="parked--shelved"></a>
## Parked / shelved

Design documents, including the [in-engine modding environment](engine/IN_ENGINE_MODDING_ENVIRONMENT.md),
do not authorize implementation or restart deferred work. Follow each design's
scope and the current user's instructions.

The full earlier checklist and its dated results remain in the
[verbatim pre-cleanup roadmap](journal/2026-10/2026-10-24-before-cleanup-ROADMAP-dc512bdd1e23.txt). Old branch/merge/resume commands
there are historical, not current instructions.
