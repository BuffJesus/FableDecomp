#!/usr/bin/env python3
"""Export recorded parity facts, with explicit assurance limits.

Legacy output filenames are retained. EXACT/RELOC rows are recorded byte-parity
evidence, not source-freshness, signature, behavioral or class-layout certification.
Missing/ambiguous evidence is excluded. Default output stays in this checkout;
copying to another project requires an explicit --mirror-dir destination.
"""
import argparse
from collections import Counter, defaultdict
import csv
import hashlib
import json
from pathlib import Path
import re
import shutil
import sys

sys.path.insert(0, str(Path(__file__).resolve().parent))
import class_struct
import label_trust
from crawl import purity

ROOT = Path(__file__).resolve().parents[2]
ASSURANCE = {
    "basis": "explicit recorded EXACT/RELOC audit rows, matching source filenames, and crawl.purity filter",
    "freshness": "not established: legacy audit rows are not bound to current source/dependency hashes",
    "behavior": "not established by byte parity",
    "signatures": "manifest metadata, not independently verified by this export",
    "labels": "manifest metadata; trusted class labels are required for struct inference",
    "layout": "inferred local declarations with trusted class labels, not independent layout proof",
}


def address(value):
    value = (value or "").strip().lower().removeprefix("0x")
    return value.zfill(8) if re.fullmatch(r"[0-9a-f]{1,8}", value) else None


def basename(value):
    return (value or "").replace("\\", "/").rsplit("/", 1)[-1]


def tsv(path, *, optional=False):
    if optional and not path.exists():
        return []
    with path.open(encoding="utf-8-sig", newline="") as stream:
        return list(csv.DictReader(stream, delimiter="\t"))


def by_address(rows):
    grouped = defaultdict(list)
    for row in rows:
        if key := address(row.get("address")):
            grouped[key].append(row)
    return grouped


def build_exports(root):
    """Read only the selected root; build every payload before writing anything."""
    root = Path(root).resolve()
    manifest = label_trust.Manifest(root / "rebuild/manifest/functions.tsv")
    man = by_address(manifest.rows)
    audit = by_address(tsv(root / "rebuild/compile-gate/parity_audit.tsv"))
    queue = tsv(root / "rebuild/backlog/forgefse-binding-queue.tsv", optional=True)
    sources = defaultdict(list)
    for path in sorted((root / "rebuild/src/compiled").rglob("*.cpp")):
        if key := address(path.stem.rsplit("_", 1)[-1]):
            sources[key].append(path)

    excluded = Counter()
    eligible = {}
    snapshots = {}
    by_class = defaultdict(list)
    for key, paths in sorted(sources.items()):
        if len(paths) != 1 or len(man.get(key, [])) != 1 or len(audit.get(key, [])) > 1:
            excluded["ambiguous_or_missing_identity"] += 1
            continue
        if not audit.get(key):
            excluded["missing_audit"] += 1
            continue
        row = man[key][0]
        proof = audit[key][0]
        path = paths[0]
        if proof.get("verdict") not in ("EXACT", "RELOC"):
            excluded["nonmatching_audit"] += 1
            continue
        if basename(proof.get("file")) != path.name or basename(row.get("compiled_source")) != path.name:
            excluded["source_identity_mismatch"] += 1
            continue
        if not path.resolve().is_relative_to(root):
            excluded["source_outside_root"] += 1
            continue
        try:
            data = path.read_bytes()
        except OSError:
            excluded["unreadable_source"] += 1
            continue
        if not purity.is_genuine(data.decode("utf-8", errors="replace")):
            excluded["assembly_source"] += 1
            continue
        label_reason = manifest.trust_reason(key)
        eligible[key] = {
            "address": f"0x{key}", "name": row.get("name", ""),
            "module": row.get("module", "_global"),
            "calling_convention": row.get("calling_convention", ""),
            "return_type": row.get("return_type", ""),
            "parameter_types": row.get("parameter_types", ""),
            "parity": proof["verdict"],
            "label_trust": label_reason or "TRUSTED_CLASS",
            "source": path.relative_to(root).as_posix(),
            # This identifies the exported snapshot; it does not bind the old audit to it.
            "source_sha256": hashlib.sha256(data).hexdigest(),
        }
        if cls := manifest.trusted_module(key):
            by_class[cls].append(path)
        snapshots[path] = eligible[key]["source_sha256"]

    schema = {}
    excluded_classes = Counter()
    for cls, paths in sorted(by_class.items()):
        provenance = {}
        off_map, size, used = class_struct.facts_for(cls, files=paths, provenance=provenance)
        if used < 1 or (not off_map and not size):
            continue
        if any(len({(typ, name, span) for typ, name, span, _ in rows}) != 1
               for rows in provenance.values()):
            excluded_classes["conflicting_fields"] += 1
            continue
        end = max((offset + rows[0][2] for offset, rows in provenance.items()), default=0)
        if size is not None and size < end:
            excluded_classes["size_conflict"] += 1
            continue
        entry = {"verified_methods": used, "inference": "source_declarations"}
        if size is not None:
            entry["size"] = f"0x{size:x}"
        if off_map:
            entry["fields"] = [
                {"offset": f"0x{offset:x}", "type": off_map[offset][0],
                 "name": off_map[offset][1], "size": f"0x{provenance[offset][0][2]:x}"}
                for offset in sorted(off_map)
            ]
        schema[cls] = entry

    bindings = []
    for row in queue:
        key = address(row.get("address"))
        if key not in eligible:
            continue
        fact = eligible[key]
        bindings.append({
            "address": fact["address"], "api_names": row.get("api_names", ""),
            "qualified_name": row.get("qualified_name", ""),
            "calling_convention": fact["calling_convention"], "return_type": fact["return_type"],
            "bridge_kind": row.get("bridge_kind", ""), "parity": fact["parity"],
        })
    for path, digest in snapshots.items():
        if not path.resolve().is_relative_to(root) or hashlib.sha256(path.read_bytes()).hexdigest() != digest:
            raise ValueError(f"source changed while exporting: {path.relative_to(root)}")
    return {
        "verified_functions.json": {
            "count": len(eligible), "assurance": ASSURANCE,
            "excluded": dict(sorted(excluded.items())), "functions": list(eligible.values()),
        },
        "struct_schema.json": {
            "count": len(schema), "assurance": ASSURANCE,
            "excluded": dict(sorted(excluded_classes.items())), "classes": schema,
        },
        "forgefse_verified_bindings.json": {
            "count": len(bindings), "assurance": ASSURANCE,
            "note": "Recorded parity targets only. Signature, owner/vtable and runtime binding checks remain required.",
            "bindings": bindings,
        },
    }


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--root", type=Path, default=ROOT, help="checkout to read and write (default: this checkout)")
    mirrors = parser.add_mutually_exclusive_group()
    mirrors.add_argument("--no-mirror", action="store_true", help="compatibility flag; local-only is already the default")
    mirrors.add_argument("--mirror-dir", action="append", type=Path, default=[],
                         help="explicit existing destination for all three exports; repeat for multiple destinations")
    args = parser.parse_args(argv)
    root = args.root.resolve()
    sdk = root / "rebuild/sdk"
    for destination in args.mirror_dir:
        if not destination.is_dir() or destination.resolve() == sdk.resolve():
            parser.error(f"mirror destination must be an existing directory distinct from {sdk}: {destination}")
    try:
        exports = build_exports(root)
    except (OSError, ValueError, KeyError) as error:
        print(f"export failed: {error}", file=sys.stderr)
        return 1
    sdk.mkdir(parents=True, exist_ok=True)
    for name, payload in exports.items():
        with (sdk / name).open("w", encoding="utf-8", newline="\n") as stream:
            json.dump(payload, stream, indent=1)
            stream.write("\n")
        for destination in args.mirror_dir:
            shutil.copy2(sdk / name, destination / name)
        print(f"{name}: {payload['count']}")
    print(f"-> {sdk}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
