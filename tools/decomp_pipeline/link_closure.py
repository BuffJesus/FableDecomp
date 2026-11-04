#!/usr/bin/env python3
"""Close a runtime over landed reconstructions, then report the frontier.

Starting from seed sources (catalog-relative globs under rebuild/src/compiled, or paths)
plus optional extra sources (a main, boundaries), repeatedly link, look up each
unresolved symbol in rebuild/build/symbol_index.tsv (symbol_index.py), and add the best
landed source that defines it: a strong definition whose file is named after the symbol's
own function first, then any strong definition, then a COMDAT. Stops when a round adds
nothing. What remains unresolved is the frontier a boundary must stand in for.

Usage:
  python tools/decomp_pipeline/link_closure.py --seed "00/41/CGame_Play_00412f90.cpp" \
      [--seed GLOB ...] [--extra path.cpp ...] [--out DIR] [--max-rounds 40] [--jobs 8]
Writes DIR/closure_sources.txt, DIR/frontier.txt, DIR/duplicates.txt.
"""
import argparse
import collections
import csv
import os
import re
import subprocess
import sys
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE))
import verify_and_land as V  # noqa: E402

ROOT = HERE.parents[1]
SRC = ROOT / "rebuild" / "src" / "compiled"
INDEX = ROOT / "rebuild" / "build" / "symbol_index.tsv"
BASE = ["/nologo", "/c", "/O2", "/Oy", "/GS", "/MT"]
UNRESOLVED = re.compile(r"unresolved external symbol (?:\"[^\"]*\" )?\((\S+)\)")
UNRESOLVED_C = re.compile(r"unresolved external symbol ([_@]\S+) referenced")
DUPLICATE = re.compile(r"LNK2005: (?:\"[^\"]*\" )?\(?(\S+?)\)? already defined in (\S+)")


def leaf_of(symbol):
    m = re.match(r"\?+[0-9A-Z_]?([A-Za-z_]\w*)@", symbol)
    return m.group(1) if m else symbol.lstrip("_").split("@")[0]


def load_index():
    table = collections.defaultdict(list)
    with INDEX.open(encoding="utf-8") as f:
        for row in csv.DictReader(f, delimiter="\t"):
            table[row["symbol"]].append(row)
    return table


def rank(symbol, row):
    name = Path(row["source"]).stem
    primary = f"_{leaf_of(symbol)}_" in f"_{name}_"
    return (row["strength"] != "strong", not primary, row["source"])


_FLAGS = None


def flags_for(source):
    global _FLAGS
    if source == '00/99/STLThreadedUIDTree_IteratorErase_00990430.cpp':
        return ['/nologo', '/c', '/O2', '/MT']
    if source in (
        '00/9d/STLAsyncPointerTree_Distance_009d7550.cpp',
        '00/9d/STLAsyncPointerTree_EqualRange_009d7c80.cpp',
        '00/9d/STLAsyncPointerTree_KeyErase_009d7f40.cpp'):
        # Dedicated native gate verifies this full profile, without /Oy /GS.
        return ['/nologo', '/c', '/O2', '/MT']
    if _FLAGS is None:
        _FLAGS = {}
        text = (ROOT / "rebuild" / "build_candidates.ps1").read_text(encoding="utf-8", errors="replace")
        for block in re.findall(r"\[pscustomobject\]@\{(.*?)\n\s*\}", text, re.S):
            fields = dict(re.findall(r"(\w+)\s*=\s*'([^']*)'", block))
            if "Source" in fields and "CompilerFlags" in fields:
                _FLAGS[fields["Source"]] = fields["CompilerFlags"].split()
    flags = _FLAGS.get(source, BASE)
    return flags if "/c" in flags else BASE + [f for f in flags if f not in BASE]


def compile_one(path, out_dir, env, flags=BASE):
    obj = out_dir / (Path(path).stem + ".obj")
    if not obj.exists() or obj.stat().st_mtime < Path(path).stat().st_mtime:
        r = V.cl([*flags, f"/Fo{obj}", str(path)], env)
        if r.returncode:
            raise RuntimeError(f"compile failed: {path}\n{(r.stdout + r.stderr)[-800:]}")
    return obj


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--seed", action="append", default=[])
    ap.add_argument("--seed-file", action="append", default=[],
                    help="newline-separated catalog-relative source paths")
    ap.add_argument("--extra", action="append", default=[])
    ap.add_argument("--out", default=os.path.join(os.environ.get("TEMP", "."), "fable-link-closure"))
    ap.add_argument("--max-rounds", type=int, default=40)
    ap.add_argument("--jobs", type=int, default=1,
                    help="independent compiler processes; linking remains sequential")
    args = ap.parse_args()
    if args.jobs < 1:
        ap.error("--jobs must be at least 1")
    out = Path(args.out)
    (out / "obj").mkdir(parents=True, exist_ok=True)
    env = V.env()
    index = load_index()

    sources = []
    for pattern in args.seed:
        p = Path(pattern)
        sources += [p] if p.is_file() else sorted(SRC.glob(pattern))
    for seed_file in args.seed_file:
        sources += [SRC / line.strip() for line in Path(seed_file).read_text().splitlines()
                    if line.strip() and not line.lstrip().startswith('#')]
    chosen = {str(s.relative_to(SRC)).replace("\\", "/") if SRC in s.parents else str(s) for s in sources}
    extras = [Path(p) for p in args.extra]
    tried = set()
    frontier, duplicates = [], []
    for round_no in range(1, args.max_rounds + 1):
        # Resolve flags in the main thread: flags_for lazily populates a shared
        # catalog cache. Keep map order stable so parallel completion cannot
        # affect provider/link ordering. Legacy basename collisions stay serial.
        inputs = [(SRC / rel if not Path(rel).is_absolute() else Path(rel), flags_for(rel))
                  for rel in sorted(chosen)] + [(p, BASE) for p in extras]
        stems = [path.stem.casefold() for path, _ in inputs]
        def build(item):
            path, flags = item
            return str(compile_one(path, out / "obj", env, flags))
        if args.jobs > 1 and len(set(stems)) == len(stems):
            with ThreadPoolExecutor(max_workers=args.jobs) as pool:
                objs = list(pool.map(build, inputs))
        else:
            objs = [build(item) for item in inputs]
        # Response file: the object list outgrew the 32K command line.
        (out / "closure_objs.rsp").write_text("\n".join('"%s"' % o for o in objs) + "\n")
        link = subprocess.run([str(V.VC / "bin" / "link.exe"), "/nologo", "/subsystem:console",
                               f"/out:{out / 'closure.exe'}", f"@{out / 'closure_objs.rsp'}"],
                              capture_output=True, text=True, env=env)
        unresolved = sorted(set(UNRESOLVED.findall(link.stdout)) | set(UNRESOLVED_C.findall(link.stdout)))
        duplicates = sorted(set(DUPLICATE.findall(link.stdout)))
        added = 0
        added_this_round = set()
        for symbol in unresolved:
            # One newly selected object may define several unresolved symbols.
            # Give that object a compile/link round before choosing another
            # provider for the same symbol. Otherwise a second COMDAT/strong
            # owner can introduce duplicate definitions and unrelated callees.
            if any(r['source'] in added_this_round for r in index.get(symbol, [])):
                continue
            candidates = [r for r in index.get(symbol, []) if r["source"] not in chosen]
            if not candidates or symbol in tried:
                continue
            tried.add(symbol)
            best = min(candidates, key=lambda r: rank(symbol, r))
            chosen.add(best["source"])
            added_this_round.add(best['source'])
            added += 1
        print(f"round {round_no}: {len(chosen)} sources, {len(unresolved)} unresolved, "
              f"{len(duplicates)} duplicate, +{added}", flush=True)
        frontier = unresolved
        if not added:
            break
    (out / "closure_sources.txt").write_text("\n".join(sorted(chosen)) + "\n")
    (out / "frontier.txt").write_text("\n".join(frontier) + "\n")
    (out / "duplicates.txt").write_text("\n".join(f"{s}\t{o}" for s, o in duplicates) + "\n")
    owners = collections.Counter()
    for symbol in frontier:
        m = re.search(r"@(\w+)@@", symbol)
        owners[m.group(1) if m else symbol[:48]] += 1
    print(f"frontier: {len(frontier)} symbols; duplicates: {len(duplicates)}")
    for owner, count in owners.most_common(40):
        print(f"  {count:4} {owner}")


if __name__ == "__main__":
    main()
