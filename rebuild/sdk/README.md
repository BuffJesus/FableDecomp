# Historical SDK snapshots

`verified_functions.json`, `struct_schema.json` and
`forgefse_verified_bindings.json` are historical generated snapshots. They
predate the exporter eligibility and assurance checks described in the
[export contract](../../docs/pipeline/VERIFIED_FACTS_EXPORT.md). Their filenames
and older verification labels do not establish current source freshness,
behavioral certification, signature validity or independently proven layouts.

The exporter repair does **not** regenerate or revalidate these checked-in
payloads. In particular, an older `MATCH*` label may reflect the former
missing-audit fallback. Preserve the original snapshots as historical evidence;
do not promote their labels to new verification claims.

New exports require explicit recorded `EXACT`/`RELOC` rows and carry an
`assurance` section documenting their remaining limits. Snapshot source hashes
identify the exported bytes; legacy audit rows still lack a complete current
source/dependency proof binding. Mirroring is opt-in through explicit paths.

`fse_native_overlay.json` is produced separately and is outside this exporter
repair's validation scope.
