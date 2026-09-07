#!/usr/bin/env python3
"""Harvest an address -> FSE-API-name map from FSE source trees.

The native helper backlog is keyed by retail target address but *named* from
Ghidra/BSim, which mislabels the `0x00CBxxxx` script-interface thunk band (e.g.
0x00CBE87F comes through as `CSubtitleRenderer::SetText` at similarity 0.51 when
FSE binds it as AddLogbookStoryEntry). FSE's own initialisers are an independent,
address-keyed naming oracle for exactly that band:

    FadeOutAndKillEntity_API = ASLR<tFadeOutAndKillEntity>(0x00CBE716);

so every `<name>_API = ASLR<t...>(0x...)` line is one address -> name fact. This
tool collects them, reports where they agree or disagree with the manifest, and
writes the map for the readiness analyzer to consume.

Per CLAUDE.md, an FSE ASLR address outranks a low-confidence BSim name when the
two clash - but a clash is reported, never silently resolved.
"""

from __future__ import annotations

import argparse
import csv
import json
import re
from pathlib import Path
from typing import Any

# <lhs>_API|_Func|_func = ASLR<...>(0x...);  and the vtable-slot form
ASLR_RE = re.compile(
    r"\b(?P<sym>[A-Za-z_][A-Za-z0-9_]*)\s*=\s*ASLR\s*<[^>]*>\s*\(\s*(?P<addr>0x[0-9A-Fa-f]+)\s*\)")
SUFFIX_RE = re.compile(r"_(?:API|Func|func)$")

DEFAULT_SOURCES = [
    Path(r"D:\Code\ForgeFSE\FableScriptExtender"),
    Path(r"D:\Code\ForgeFSE-retail-shadow\FableScriptExtender"),
]


def harvest(roots: list[Path]) -> dict[str, dict[str, Any]]:
    """address -> {name, symbols, sources}. An address bound to two different
    API names in different trees is kept with both, flagged `ambiguous`."""
    out: dict[str, dict[str, Any]] = {}
    for root in roots:
        if not root.exists():
            continue
        for path in sorted(root.rglob("*.cpp")) + sorted(root.rglob("*.h")):
            try:
                text = path.read_text(encoding="utf-8-sig", errors="replace")
            except OSError:
                continue
            for m in ASLR_RE.finditer(text):
                sym = m.group("sym")
                api = SUFFIX_RE.sub("", sym)
                addr = f"0x{int(m.group('addr'), 16):08X}"
                row = out.setdefault(addr, {"address": addr, "names": set(),
                                            "symbols": set(), "sources": set()})
                row["names"].add(api)
                row["symbols"].add(sym)
                row["sources"].add(str(path))
    return out


def manifest_names(path: Path) -> dict[str, str]:
    names: dict[str, str] = {}
    if not path.exists():
        return names
    with path.open(encoding="utf-8-sig", newline="") as fh:
        for row in csv.DictReader(fh, delimiter="\t"):
            addr = (row.get("address") or "").strip()
            if not addr:
                continue
            key = f"0x{int(addr, 16):08X}"
            module = (row.get("module") or "").strip()
            name = (row.get("name") or "").strip()
            names[key] = f"{module}::{name}" if module else name
    return names


def helper_backlog(path: Path) -> dict[str, dict[str, str]]:
    rows: dict[str, dict[str, str]] = {}
    if not path.exists():
        return rows
    with path.open(encoding="utf-8-sig", newline="") as fh:
        for row in csv.DictReader(fh, delimiter="\t"):
            addr = (row.get("target_address") or "").strip()
            if addr:
                rows[f"0x{int(addr, 16):08X}"] = row
    return rows


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("--source", type=Path, action="append",
                    help="FSE source root (repeatable); defaults to both ForgeFSE forks")
    ap.add_argument("--manifest", type=Path, default=Path("rebuild/manifest/functions.tsv"))
    ap.add_argument("--helper-queue", type=Path,
                    default=Path("refs/script_recovery/native_helper_target_queue.tsv"))
    ap.add_argument("--output", type=Path, required=True)
    args = ap.parse_args()

    roots = args.source or DEFAULT_SOURCES
    harvested = harvest(roots)
    manifest = manifest_names(args.manifest)
    queue = helper_backlog(args.helper_queue)

    entries = []
    agreements = clashes = manifest_absent = 0
    queue_hits = []
    for addr in sorted(harvested):
        row = harvested[addr]
        names = sorted(row["names"])
        mname = manifest.get(addr)
        leaf = mname.rsplit("::", 1)[-1] if mname else None
        if mname is None:
            verdict = "not-in-manifest"
            manifest_absent += 1
        elif leaf in names:
            verdict = "agrees"
            agreements += 1
        else:
            verdict = "clash"
            clashes += 1
        entry = {
            "address": addr,
            "fseNames": names,
            "fseSymbols": sorted(row["symbols"]),
            "manifestName": mname,
            "verdict": verdict,
            "ambiguous": len(names) > 1,
            "sources": sorted(row["sources"]),
        }
        if addr in queue:
            q = queue[addr]
            entry["helperQueue"] = {
                "category": q.get("category"),
                "calls": int(q.get("calls") or 0),
                "scripts": int(q.get("scripts") or 0),
                "roles": q.get("roles"),
                "helperNames": q.get("helper_names"),
            }
            queue_hits.append(entry)
        entries.append(entry)

    payload = {
        "schema": "fable-fse-address-name-map/0.1",
        "sources": [str(r) for r in roots],
        "summary": {
            "addresses": len(entries),
            "agreesWithManifest": agreements,
            "clashesWithManifest": clashes,
            "notInManifest": manifest_absent,
            "ambiguousAcrossTrees": sum(1 for e in entries if e["ambiguous"]),
            "helperQueueAddresses": len(queue_hits),
            "helperQueueCalls": sum(e["helperQueue"]["calls"] for e in queue_hits),
            "helperQueueScriptsMax": max((e["helperQueue"]["scripts"] for e in queue_hits),
                                         default=0),
            "helperQueueClashes": sum(1 for e in queue_hits if e["verdict"] == "clash"),
        },
        "entries": entries,
    }
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(payload, indent=2) + "\n", encoding="utf-8")
    print(json.dumps(payload["summary"], sort_keys=True))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
