#!/usr/bin/env python3
"""Readability + honesty metrics for the landed corpus -> rebuild/manifest/header_metrics.json.

Counts that the coverage dashboard reports alongside function counts:
  landed_total              landed .cpp translation units (compiled_source set in the manifest)
  genuine                   real compiler input (no __asm / naked / _emit)
  naked_asm / emit_bake     hand-written machine code (asm_bake grade; not reconstruction)
  includes_engine_header    TUs that #include "engine/<Class>.h" (typed onto the PDB layout)
  includes_any_header       TUs with any #include
  generic_struct_decls      local `struct T/Sub/Owner/...` throwaway declarations remaining
  files_with_generic_struct
  gapscan                   address-named gapscan reconstructions (no class label)
  mislabelled_family_files  landed rows whose module label is a propagated dtor/thunk family
  engine_headers_generated / engine_headers_quarantined
  matched_bytes / matched_bytes_genuine   sum of retail bytes covered by MATCH (+RELOC) landings
  text_bytes_total          size of retail .text (from rebuild/manifest/text_section.json if
                            present, else omitted)
"""
from __future__ import annotations

import collections
import csv
import json
import re
import sys
from pathlib import Path

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[1]
sys.path.insert(0, str(HERE))
import label_trust  # noqa: E402
from crawl import purity  # noqa: E402

GENERIC = {"T", "Sub", "Owner", "Slot", "Info", "Obj", "S", "Self", "This", "Inner", "Foo",
           "Holder", "Node", "Entry", "Item", "Mid", "MidObj", "CamObj", "A", "B", "C", "X"}
STRUCT_RE = re.compile(r"\bstruct\s+([A-Za-z_]\w*)\s*\{")
ORACLE = ROOT / "rebuild" / "oracles" / "auto-re-candidates.tsv"


def main() -> int:
    mf = label_trust.Manifest()
    oracle_len = {}
    if ORACLE.exists():
        for r in csv.DictReader(open(ORACLE, encoding="utf-8-sig"), delimiter="\t"):
            try:
                oracle_len[r["address"].lower()] = int(r["length"])
            except ValueError:
                pass
    m = collections.Counter()
    matched_bytes = matched_bytes_genuine = 0
    for r in mf.landed():
        p = Path(r["compiled_source"])
        if not p.exists():
            m["missing_file"] += 1
            continue
        txt = p.read_text(encoding="utf-8", errors="ignore")
        m["landed_total"] += 1
        kind = purity.classify(txt)
        m[kind] += 1
        if '#include "engine/' in txt:
            m["includes_engine_header"] += 1
        if "#include" in txt:
            m["includes_any_header"] += 1
        names = STRUCT_RE.findall(txt)
        g = sum(1 for n in names if n in GENERIC or re.fullmatch(r"[A-Z]_[0-9a-f]{8}", n) or re.fullmatch(r"C_[0-9a-f]{8}", n))
        if g:
            m["generic_struct_decls"] += g
            m["files_with_generic_struct"] += 1
        if r["module"] == "_gapscan" or p.name.startswith("gapscan_"):
            m["gapscan"] += 1
        why = mf.trust_reason(r["address"])
        if why.startswith("MISLABELLED_FAMILY"):
            m["mislabelled_family_files"] += 1
        if r.get("retail_parity", "") in ("MATCH", "RELOCATION_MATCH"):
            if kind != "genuine":
                m["asm_bake_verified"] += 1
                if r["retail_parity"] == "MATCH":
                    m["asm_bake_matching"] += 1
            n = oracle_len.get(r["address"].lower().zfill(8), 0)
            matched_bytes += n
            if kind == "genuine":
                matched_bytes_genuine += n
    # files on disk that the manifest does not know as landed (gapscan rows and strays)
    manifest_files = {Path(r["compiled_source"]).resolve() for r in mf.landed()}
    for d in (ROOT / "rebuild" / "src" / "compiled", ROOT / "rebuild" / "src" / "asm_bake"):
        if not d.exists():
            continue
        for p in d.rglob("*.cpp"):
            m["files_on_disk"] += 1
            if p.name.startswith("gapscan_"):
                m["gapscan"] += 1
            if p.resolve() not in manifest_files:
                m["files_not_in_manifest"] += 1
                kind = purity.classify(p.read_text(encoding="utf-8", errors="ignore"))
                m["unmanifested_" + kind] += 1
    eng = ROOT / "rebuild" / "include" / "engine"
    out = {k: m.get(k, 0) for k in ("landed_total", "files_on_disk", "files_not_in_manifest",
                                    "unmanifested_genuine", "unmanifested_naked_asm", "unmanifested_emit_bake",
                                    "genuine", "naked_asm", "emit_bake",
                                    "includes_engine_header", "includes_any_header",
                                    "generic_struct_decls", "files_with_generic_struct", "gapscan",
                                    "mislabelled_family_files", "missing_file",
                                    "asm_bake_verified", "asm_bake_matching")}
    out["engine_headers_generated"] = len(list(eng.glob("*.h"))) if eng.exists() else 0
    out["engine_headers_quarantined"] = len(list((eng / "_quarantine").glob("*.h"))) if (eng / "_quarantine").exists() else 0
    out["matched_bytes"] = matched_bytes
    out["matched_bytes_genuine"] = matched_bytes_genuine
    ts = ROOT / "rebuild" / "manifest" / "text_section.json"
    if ts.exists():
        out["text_bytes_total"] = json.loads(ts.read_text(encoding="utf-8")).get("size", 0)
    dest = ROOT / "rebuild" / "manifest" / "header_metrics.json"
    dest.write_text(json.dumps(out, indent=2) + "\n", encoding="utf-8")
    print(json.dumps(out, indent=2))
    return 0


if __name__ == "__main__":
    sys.exit(main())
