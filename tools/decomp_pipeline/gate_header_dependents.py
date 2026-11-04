#!/usr/bin/env python3
"""Find the crawl-catalog landings a shared-header edit can break, and check them.

For every header under rebuild/include that differs from a git revision (default HEAD),
this collects the catalog entries in rebuild/build_candidates.ps1 whose source or test
includes it (directly or through other rebuild headers). Each entry is graded twice with
verify_and_land's parity_of/behaviour_of: once against the revision's headers and once
against the working tree. Only entries that pass at the revision and fail now are
regressions.

Usage: python tools/decomp_pipeline/gate_header_dependents.py [--rev HEAD] [--jobs 8]
"""
import argparse
import csv
import json
import re
import shutil
import subprocess
import sys
import tempfile
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
INC = ROOT / "rebuild" / "include"
sys.path.insert(0, str(Path(__file__).resolve().parent))
sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
import verify_and_land as V  # noqa: E402

INCLUDE_RE = re.compile(r'^\s*#\s*include\s*"([^"]+)"', re.M)


def git(*args):
    return subprocess.run(["git", *args], cwd=ROOT, capture_output=True, text=True, check=True).stdout


def changed_headers(rev):
    names = git("diff", "--name-only", rev, "--", "rebuild/include").split()
    names += git("ls-files", "--others", "--exclude-standard", "rebuild/include").split()
    return {Path(n).relative_to("rebuild/include").as_posix() for n in names if n.endswith(".h")}


def includes(path):
    try:
        return INCLUDE_RE.findall(path.read_text(encoding="utf-8", errors="replace"))
    except OSError:
        return []


def affected_headers(changed):
    """Headers that are, or transitively include, a changed header."""
    graph = {}
    for h in INC.rglob("*.h"):
        graph[h.relative_to(INC).as_posix()] = set(includes(h))
    hit = set(changed)
    grew = True
    while grew:
        grew = False
        for h, deps in graph.items():
            if h not in hit and deps & hit:
                hit.add(h)
                grew = True
    return hit


def catalog():
    text = (ROOT / "rebuild" / "build_candidates.ps1").read_text(encoding="utf-8")
    out = []
    for block in re.findall(r"\[pscustomobject\]@\{(.*?)\n    \}", text, re.S):
        f = dict(re.findall(r"^\s*(\w+) = '([^']*)'", block, re.M))
        if "Address" in f and "Source" in f:
            out.append(f)
    return out


def head_include_dir(rev, changed, work):
    """rebuild/include as of `rev` for the changed headers (new headers stay visible)."""
    d = work / "head_include"
    shutil.copytree(INC, d)
    for h in changed:
        try:
            content = subprocess.run(["git", "show", f"{rev}:rebuild/include/{h}"], cwd=ROOT,
                                     capture_output=True, check=True).stdout
            target = d / h
            # A deleted header may also have lost its parent directory in the
            # working tree. Restore that directory for the baseline snapshot.
            target.parent.mkdir(parents=True, exist_ok=True)
            target.write_bytes(content)
        except subprocess.CalledProcessError:
            pass  # untracked at rev: leave the working copy
    # Test-support headers can include native .cpp helpers outside include/.
    # Keep those dependencies at their real locations when moving the headers;
    # otherwise both baselines can silently fail to compile their fixtures.
    for header in d.rglob("*.h"):
        text = header.read_text(encoding="utf-8-sig")
        updated = text
        original_parent = INC / header.relative_to(d).parent
        for include in INCLUDE_RE.findall(text):
            target = (original_parent / include).resolve()
            if target.is_file() and not target.is_relative_to(INC.resolve()):
                updated = updated.replace('"' + include + '"', '"' + target.as_posix() + '"')
        if updated != text:
            header.write_text(updated, encoding="utf-8")
    return d


def grade(entry, oracle, include_dir, work):
    # Embedded switch tables need a complete-body validator that separately
    # checks executable instructions, table relocations and dispatch behavior.
    # Keep the registered validators explicit; never silently fall back to
    # treating table bytes as instructions if one fails.
    if entry.get("ValidationGate"):
        gate = entry["ValidationGate"]
        if gate not in ("gate_window_procedure.py", "gate_vertex_declaration_generation.py", "gate_vertex_stream_building.py"):
            return "UNKNOWN_VALIDATION_GATE", "-"
        gate_work = Path(tempfile.mkdtemp(prefix=entry["Address"] + "_gate_", dir=work))
        report = gate_work / "report.json"
        command = [sys.executable, str(Path(__file__).with_name(gate)),
                   "--source", str(ROOT / "rebuild/src/compiled" / entry["Source"]),
                   "--test", str(ROOT / "rebuild/tests" / entry["TestSource"]),
                   "--include", str(include_dir), "--output", str(report)]
        result = subprocess.run(command, capture_output=True, text=True)
        (gate_work / "gate.log").write_text(result.stdout + result.stderr, encoding="utf-8")
        if result.returncode or not report.is_file():
            return "VALIDATION_GATE_FAIL", "-"
        proof = json.loads(report.read_text())
        if proof.get("address") != entry["Address"] or proof.get("grade") not in ("MATCH", "RELOCATION_MATCH", "NEAR_MATCH"):
            return "VALIDATION_GATE_REPORT_FAIL", "-"
        return proof["grade"], "PASS"
    e = V.env()
    e["INCLUDE"] = f"{V.VC / 'include'};{include_dir}"
    addr = entry["Address"].lower()
    src = (ROOT / "rebuild" / "src" / "compiled" / entry["Source"]).read_text(encoding="utf-8", errors="replace")
    tst_path = ROOT / "rebuild" / "tests" / entry.get("TestSource", "")
    row = oracle.get(addr)
    if not row:
        return "NO_ORACLE", "-"
    leaf = (entry.get("Leaf") or row["name"]).rsplit("::", 1)[-1]
    w = Path(tempfile.mkdtemp(prefix=addr + "_", dir=work))
    extra = entry.get("CompilerFlags", "").split()
    parity, _, _ = V.parity_of(V.vc71(src), addr, leaf, bytes.fromhex(row["bytes"]), w, e, extra=extra)
    beh = "-"
    if tst_path.is_file() and entry.get("PassPattern"):
        fixture = tst_path.read_text(encoding="utf-8", errors="replace")
        # A fixture can include a provider through ../src relative to the
        # include search directory. Preserve that provider when the include
        # tree moves, while still resolving ordinary headers in the staged tree.
        for include in INCLUDE_RE.findall(fixture):
            for parent in (tst_path.parent, INC):
                target = (parent / include).resolve()
                if target.is_file():
                    if not target.is_relative_to(INC.resolve()):
                        fixture = fixture.replace('"' + include + '"', '"' + target.as_posix() + '"')
                    break
        beh = V.behaviour_of(V.vc71(fixture),
                             addr, entry["PassPattern"], w, e, w / f"{addr}.obj",
                             require_candidate=parity == "NEAR_MATCH")
    return parity, beh


def grade_passed(result):
    parity, behavior = result
    if parity == "NEAR_MATCH":
        return behavior == "PASS"
    return parity in ("MATCH", "RELOCATION_MATCH") and behavior in ("PASS", "-")


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--rev", default="HEAD")
    ap.add_argument("--jobs", type=int, default=8)
    a = ap.parse_args()
    changed = changed_headers(a.rev)
    hit = affected_headers(changed)
    oracle = {r["address"].lower(): r for r in csv.DictReader(
        open(ROOT / "rebuild" / "oracles" / "auto-re-candidates.tsv", encoding="utf-8-sig"), delimiter="\t")}
    entries = []
    for entry in catalog():
        files = [ROOT / "rebuild" / "src" / "compiled" / entry["Source"]]
        if entry.get("TestSource"):
            files.append(ROOT / "rebuild" / "tests" / entry["TestSource"])
        if any(set(includes(f)) & hit for f in files):
            entries.append(entry)
    print(f"changed headers: {sorted(changed)}")
    print(f"catalog entries depending on them: {len(entries)}")
    work = Path(tempfile.mkdtemp(prefix="hdrgate_"))
    old_inc = head_include_dir(a.rev, changed, work)

    def both(entry):
        return entry, grade(entry, oracle, old_inc, work), grade(entry, oracle, INC, work)

    ok = grade_passed

    regressions = 0
    with ThreadPoolExecutor(a.jobs) as pool:
        for entry, before, after in pool.map(both, entries):
            if ok(before) and not ok(after):
                regressions += 1
                print(f"REGRESSION {entry['Address']} {entry['Source']}: {before} -> {after}")
            elif not ok(before) and ok(after):
                print(f"fixed      {entry['Address']} {entry['Source']}: {before} -> {after}")
            elif not ok(after):
                print(f"still bad  {entry['Address']} {entry['Source']}: {after}")
    print(f"{regressions} regression(s) across {len(entries)} dependent catalog entries")
    return 1 if regressions else 0


if __name__ == "__main__":
    sys.exit(main())
