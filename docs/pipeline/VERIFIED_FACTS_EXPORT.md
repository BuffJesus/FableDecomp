# Recorded parity facts export

Run `python tools/decomp_pipeline/export_verified_facts.py` from a checkout to
write its three legacy SDK JSON files under `rebuild/sdk/`. Use `--root PATH`
to select another checkout explicitly. The exporter reads that checkout's
manifest, recorded parity audit, compiled sources and optional binding queue.
It does not compile candidates, read retail inputs or run behavior tests.

A function needs one manifest identity, one source identity and an explicit
`EXACT` or `RELOC` audit row identifying the same source filename. Missing rows,
ambiguous addresses, other verdicts and sources rejected by `crawl.purity` are
excluded. Merely placing a file in `src/compiled` never supplies a `MATCH*` proof.
Binding exports use this same eligible address set and manifest signatures.

Function names, class labels and signatures remain manifest metadata. Each
function includes its `label_trust` reason; a recorded matching address does not
make a propagated class name authoritative.

Struct inference uses only those eligible source files, grouped by the existing
`label_trust` class policy. Unaudited declarations cannot contribute fields to
an otherwise eligible class. Conflicting field declarations and sizes smaller
than the complete field extent are excluded. These declarations remain inferred
layout information; byte parity alone does not prove every field declaration.

The legacy filenames and `verified_methods` key are retained for compatibility.
Every payload includes `assurance` explaining that the evidence is **recorded
parity**, not fresh source/dependency verification, signature certification or
behavioral proof. Exported source hashes identify the current snapshot; they
do not retroactively bind older audit rows to those bytes. Consumers requiring
fresh or behavior-certified facts need additional evidence. This export does
not repair the audit producer's missing dependency/proof freshness contract.

Mirroring is **off by default**. The old `--no-mirror` flag remains valid.
To copy all three files elsewhere, pass an explicit existing destination with
`--mirror-dir PATH`; repeat it for multiple destinations. No sibling repository
paths are discovered or written automatically. Required input failures leave
existing outputs untouched because all payloads are built before writing.
Observed source changes during struct inference also abort before writing.
