# Fable decomp/reimplementation archive audit

Audit date: 2026-07-29

## Source

- Local archive: `D:\Downloads\fable-decomp-and-reimpl.7z`
- SHA-256:
  `BFDB06DDF83BA9BF5592EAC137596D6982294A1EF53433A8CB580A0D62F8852D`
- Compressed size: 8,548,983 bytes
- Contents: 3,306 files, 126,742,707 uncompressed bytes

The archive was inventoried and extracted to a disposable temporary directory.
Nothing was overlaid onto the reconstruction workspace.

## What it contains

The archive has two separate projects:

1. `fable-decomp`: a Rust toolchain that delinks the 2013 Fable Anniversary
   `Ego_r` PE/PDB into per-translation-unit COFF objects, reconstructs CodeView
   types, generates headers, and can feed its sidecar data to Ghidra.
2. `fable-reimpl`: generated headers and Ghidra-decompiled bodies assembled
   from that donor. The archive contains 1,579 headers, 1,583 source files,
   34,274 function records, 31,580 unique mangled symbols, and 26,072 original
   source-line references.

This is the Fable Anniversary 2013 debug donor, not the 2005 TLC retail
executable. It therefore cannot establish retail byte parity.

## Overlap with this repository

This repository already uses the same `Ego_r.pdb` donor and has 43,198 names in
`ghidra_out/egor_pdb_names.tsv`. Exact mangled-symbol comparison found:

- 30,636 archive symbols already present in the existing donor-name table.
- 944 archive symbol strings absent from that table and worth triage.
- 12,562 existing PDB-name entries without an assembled archive body.

The most useful incremental material is not another name dump. It is:

- per-translation-unit grouping and original source-line provenance;
- generated full-class headers and vtable/type relationships;
- donor control-flow bodies for difficult ownership coordinators;
- the reusable PE/PDB-to-COFF and CodeView tooling.

For GFMain specifically, the donor body corroborates one
`CSystemManagerInit` owner spanning startup configuration through system
initialisation, followed by `GFInitialise`, `CGame::Play`, `GFUninitialise`,
registry persistence, and final string cleanup. Retail disassembly and exact
retail leaves remain authoritative.

## Use policy

- Treat all archive output as donor evidence only.
- Verify every claimed TLC function against the retail address, bytes,
  relocations, and behavior fixture before promotion.
- Do not add the generated donor sources to retail parity counts.
- Do not copy the archive wholesale into this repository.
- The two archive roots contain no top-level license or Git metadata. Only the
  nested vendored `pdb` crate carries its own license files. Direct publication
  of archive source should wait for upstream provenance and license
  confirmation.

## Recommendation

Keep the archive available locally. Use targeted queries against it for source
ownership, class layout, and control-flow hypotheses, especially when a retail
boundary is already byte-mapped but its higher-level owner is unclear. The
toolchain itself merits a separate integration experiment after the current
GFMain ownership pass; it may generate richer donor headers than the existing
flat TSV exports.
