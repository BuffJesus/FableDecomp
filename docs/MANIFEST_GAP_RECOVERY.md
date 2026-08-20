# Manifest gap recovery — teaching the manifest the functions it never listed

## The defect
A manifest oracle row spans `[addr, next_manifest_addr)`. When the next *real* function is
absent from the manifest, the row swallows it. VC7.1 packs small functions with no `0xCC`
between them, so there is no padding cue and `trim_overcapture.py` cannot see it. Such a row
can never reach byte parity: its oracle is not the body of the function it names.

Measured on 2026-08-19: **26.3% of the un-landed pool (5,388 rows)** was in this state.

## The pipeline
1. **`crawl/rowtrim.py`** — control-flow trim. Decode the row, track the furthest forward
   branch target *inside* it, cut at the first `ret`/`jmp` nothing branches past.
   **Refuses to trim past any indirect jump** (`jmp [table+eax*4]`): a dense switch's cases
   are unreachable to a linear decoder, and trimming there truncates a real function.
   Also never cuts a tail of `int3`/`nop` (the function's own alignment padding).
   Wired into `pe_oracle.py`, `crawl/next_smallest.py` and all the de-bake tools.
2. **`crawl/manifest_gaps.py`** → `rebuild/manifest/manifest-gaps.tsv`. Every trim point is a
   function start the manifest does not list.
3. **`crawl/xrefs.py`** — evidence filter. Indexes every address that is *entered*: direct
   `call`/`jmp rel32` targets in `.text`, plus every stored dword anywhere in the image that
   lands in `.text` (vtable slots, function-pointer tables). Calibration: **83.7% of known
   manifest functions** are entered by this index, so requiring an xref is conservative —
   it discards some real functions but admits little junk.
4. **`crawl/manifest_add_gaps.py --write`** — appends the xref-confirmed starts to
   `rebuild/manifest/functions.tsv`, tagged `module=_gapscan`, `agent_source=gapscan`,
   `agent_verdict=discovered`. This is the structural fix: a host row's span now *ends* at the
   discovered start, so its oracle is the real body with no trimming needed, and
   `verify_and_land` (which rejects candidates lacking an authoritative manifest start) will
   accept them.
5. **`crawl/shape_author.py --unlanded`** picks them up. For `_gapscan` rows the **shape is the
   prototype**: the classifier derives the signature from the bytes and `verify_and_land`
   proves it byte-for-byte. Ordinary rows still require a complete Ghidra prototype — nothing
   is silently guessed.
   `crawl/gap_author.py` does the same directly from the gaps file (it also splits a gap region
   into its successive functions), and `crawl/shapes.py` holds the classifiers both share.

## Results (2026-08-19)
| step | count |
|------|------:|
| over-captured manifest rows | 13,996 |
| bytes of code they swallowed | ~7.0 MB |
| functions recovered by splitting the gap regions | 49,668 |
| …rejected as unconfirmed (no xref) | 43,780 (88%) |
| **xref-confirmed starts added to the manifest** | **7,529** (1.60 MB attributed) |
| manifest rows | 49,568 → **57,097** |
| over-capture after the merge | 13,996 → 11,582 rows (7.0 → 5.3 MB) |
| `pe_oracle` self-validation vs proven rows | 13,198 → 13,986 / 14,047 |
| **gap functions landed** (shape-classified, byte-verified) | **2,936 / 2,936, zero failures** |
| …of which exact `MATCH` | 2,802 (134 `RELOCATION_MATCH`) |
| parity gate rows | 14,165 → **17,100** compared, `differing` unchanged at 55 |

## Rules
- **Never trim past an indirect jump.** Without the guard the two documented jump-table
  functions are truncated 120→102 (`00557ca0`) and 147→56 (`005578a0`). A too-long oracle
  fails loudly; a too-short one silently lands a body that is not the whole function.
- **A discovered start needs an xref.** Byte parity alone cannot confirm one: a 1-byte `c3`
  "empty function" matches its oracle trivially whether or not that byte begins a function.
- **Provenance stays visible.** `_gapscan` rows are always distinguishable from Ghidra-derived
  ones, and they carry no invented prototype.
