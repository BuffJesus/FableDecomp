# Script converter resume checklist

End-of-night checkpoint, 2026-09-12. User requested a stop. Resume only when asked.

## Verified checkpoint

- [x] Whole New Oakvale inventory emitted: 51 functions, zero missing bodies.
- [x] Latest report checked: 47/51 functions, 14/18 files compile; 1,296 TODOs.
- [x] Draft registration remains disabled (`Quests = {}`).
- [x] Husband home movement, question entry, and conversation continuation fixes tested.
- [x] Native resource map verifies 57 resource events and ten temporary Thing lifetimes.
- [x] Dynamic animation-byte loads remain symbolic in argument IR.
- [x] Eight blocking-Speak mismatches remain explicit diagnostics.
- [x] Four-operation resource extension prepared; compiled forwarding harness passes.
- [x] Extension patch passes `git apply --check`; runtime checkout remains unchanged.
- [x] Full suite checkpoint recorded: 870 tests, known Bully two failures/two errors only.
- [x] Later temporary-Thing additions passed ten focused resource/lifetime tests.
- [x] No converter-owned process remains pending.

## Resume in this order

- [ ] Re-read the latest section of [the handoff](../journal/2026-09/SCRIPT_CONVERTER_HANDOFF_2026-09-12.md), current report, and runtime source before relying on old assumptions.
- [ ] Implement a **separate disabled** resource-aware husband Lua candidate. None exists yet.
- [ ] Keep one resource ID across construction, preparation, explicit acquisition attempts, actions, task queries and deterministic destruction.
- [ ] Lower all ten returned Thing wrappers through their real query and destructor; add the missing explicit Thing distance-query capability before replacing home checks.
- [ ] Resolve cached woman/wife and hit-condition wrapper ownership, movie locals and error/cancellation cleanup.
- [ ] Provide a reviewed runtime read of animation byte `0x01375748`; do not freeze it to true.
- [ ] Coordinate integration of `work/man_resource_extension/resource-actions.patch` with the runtime owner. It is **not applied**, built as a full x86 runtime, or deployed.
- [ ] Test real generated resource flow against native traces for normal, retry and cancellation paths. Stub forwarding and snippet tests do not prove engine ABI or gameplay parity.
- [ ] Continue remaining entity/quest/helper recovery, including four syntax-failing entity files.
- [ ] Complete full-scope behavior and integration validation before enabling any draft.

## Boundaries and useful artifacts

Preserve the dirty shared workspace. Do not reset/stash/clean broadly. Runtime checkout,
reconstructed ports, installed game files and shared API manifests belong to the parallel session.
The known Bully failures concern missing camera cleanup after run1 termination; do not modify that
session's port or tests to make the converter suite green.

- Converter entry: `tools/script_recovery/convert_new_oakvale.py`.
- Report: `refs/script_recovery/lifted/NewOakValeIntro/CONVERSION_REPORT.json`.
- Native resource evidence: `tools/script_recovery/native_affair_man_resources_witness.json`.
- Resource mapper: `tools/script_recovery/native_post_attack_resources.py`.
- Speech gap: `tools/script_recovery/native_affair_man_speech.py`.
- Extension generator: `python -m tools.script_recovery.prepare_man_resource_extension`.
- Extension tests: `python -m unittest tools.script_recovery.test_retail_resource_actions`.
- Resource tests: `python -m unittest tools.script_recovery.test_native_affair_man_resources tools.script_recovery.test_native_resource_lifetime`.
- Full checkpoint log: `work/converter_resource_action_ir_tests.log`.

Do not mark the goal complete based on syntax counts, decreasing TODOs or passing narrow tests.
