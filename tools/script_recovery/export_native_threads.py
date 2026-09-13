#!/usr/bin/env python3
"""Export thread-body decompiles for the benchmark quest scripts (offline job + Ghidra headless).

Phase 1 (offline, retail exe only): for every benchmark script, resolve the CreateThread body
addresses the lifter recorded in refs/script_recovery/lifted/<Pkg>/LIFT_REPORT.json. The lifter
only knows them by Ghidra label or LAB_ address; the retail bytes know them exactly: the thread
allocator stores the body pointer with `mov dword ptr [reg+0x34], imm32` (C7 4x 34 imm32) inside
Init/Main/RegisterMain. The stores are matched to the recorded threads in code order and
cross-checked (LAB_ addresses must agree; symbol names must agree with functions.tsv).

Phase 2 (Ghidra headless, read-only): ExportScriptThreadBodies.java takes the job TSV, adds every
direct callee of the five lifecycle functions whose entry lies inside the script's own address
range (the compiler emits one script class contiguously), decompiles the set and writes
refs/script_recovery/native_threads/<Script>.json.

    python tools/script_recovery/export_native_threads.py --build-job      # phase 1 only
    python tools/script_recovery/export_native_threads.py                  # phase 1 + headless
"""
from __future__ import annotations

import argparse
import json
import re
import struct
import subprocess
import sys
import time
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT))
from tools.script_recovery.benchmark_lifter import QUEST_PAIRS  # noqa: E402
from tools.script_recovery.lift_native_lua import CLUSTERS  # noqa: E402

RETAIL_EXE = Path(r"C:\Programs\Steam\steamapps\common\Fable The Lost Chapters\Fable.exe")
FUNCTIONS_TSV = ROOT / "rebuild" / "manifest" / "functions.tsv"
LIFTED = ROOT / "refs" / "script_recovery" / "lifted"
OUT_DIR = ROOT / "refs" / "script_recovery" / "native_threads"
GHIDRA_HOME = Path(r"D:\Subuwu\tools\ghidra-public")
PROJECT_DIR = ROOT / "ghidra_proj"
SCRIPT_DIR = ROOT / "tools" / "ghidra_scripts"
JOB_FILE = ROOT / "ghidra_out" / "script_recovery" / "native_threads_job.tsv"
LIFECYCLE_ROLES = ("destructor", "RegisterMain", "Main", "Init", "OnPersist")
# 0x00CBD4E0 is the shared empty OnPersist; it belongs to no script and must not bound a range.
SHARED_LIFECYCLE = {0x00CBD4E0}
STORE_RE = re.compile(rb"\xc7[\x40-\x47]\x34(.{4})", re.S)


class Image:
    def __init__(self, exe: Path = RETAIL_EXE):
        data = exe.read_bytes()
        pe = struct.unpack_from("<I", data, 0x3C)[0]
        nsec = struct.unpack_from("<H", data, pe + 6)[0]
        opt = struct.unpack_from("<H", data, pe + 20)[0]
        self.base = struct.unpack_from("<I", data, pe + 24 + 28)[0]
        self.data = data
        self.secs = []
        for i in range(nsec):
            off = pe + 24 + opt + i * 40
            vsize, va, rsize, raw = struct.unpack_from("<IIII", data, off + 8)
            self.secs.append((va, max(vsize, rsize), raw))
        text = self.secs[0]
        self.text_lo = self.base + text[0]
        self.text_hi = self.text_lo + text[1]

    def bytes_at(self, va: int, size: int) -> bytes:
        rva = va - self.base
        for sva, ssize, raw in self.secs:
            if sva <= rva < sva + ssize:
                off = raw + (rva - sva)
                return self.data[off:off + size]
        return b""


def load_functions() -> tuple[list[int], dict[int, tuple[str, str]]]:
    starts: list[int] = []
    names: dict[int, tuple[str, str]] = {}
    with FUNCTIONS_TSV.open(encoding="utf-8", errors="replace") as fh:
        header = fh.readline().rstrip("\n").split("\t")
        i_addr, i_name, i_mod = header.index("address"), header.index("name"), header.index("module")
        for line in fh:
            cols = line.rstrip("\n").split("\t")
            if len(cols) <= i_mod:
                continue
            addr = int(cols[i_addr], 16)
            starts.append(addr)
            names[addr] = (cols[i_name], cols[i_mod])
    starts.sort()
    return starts, names


def function_extent(addr: int, starts: list[int]) -> int:
    """Upper bound = next known function start (over-capture is harmless for the store scan)."""
    import bisect
    i = bisect.bisect_right(starts, addr)
    return starts[i] if i < len(starts) else addr + 0x2000


def thread_stores(image: Image, addr: int, starts: list[int]) -> list[int]:
    end = function_extent(addr, starts)
    blob = image.bytes_at(addr, end - addr)
    found = []
    for m in STORE_RE.finditer(blob):
        imm = struct.unpack("<I", m.group(1))[0]
        if image.text_lo <= imm < image.text_hi:
            found.append(imm)
    return found


def build_job(scripts: list[tuple[str, str]]) -> list[dict]:
    image = Image()
    starts, names = load_functions()
    jobs = []
    for script, package in scripts:
        cluster = json.loads((CLUSTERS / f"{script}.json").read_text(encoding="utf-8-sig"))
        report_path = LIFTED / package / "LIFT_REPORT.json"
        threads = json.loads(report_path.read_text(encoding="utf-8"))["threads"] if report_path.is_file() else []
        life = {l["role"]: int(l["address"], 16) for l in cluster["lifecycle"] if l["role"] in LIFECYCLE_ROLES}
        own = [a for a in life.values() if a not in SHARED_LIFECYCLE]
        lo, hi = min(own), max(own)
        stores: list[int] = []
        for role in ("Init", "RegisterMain", "Main"):
            if role in life:
                stores += thread_stores(image, life[role], starts)
        # de-duplicate while keeping code order (a body stored twice is one thread body) and keep
        # only bodies inside the script's own range: every RegisterMain also stores the shared
        # engine body 0x00CDD440 (the Main-thread trampoline), which is not a script thread.
        seen: set[int] = set()
        shared_stores = sorted({a for a in stores if not (lo <= a <= hi)})
        stores = [a for a in stores if lo <= a <= hi and not (a in seen or seen.add(a))]
        resolved = []
        problems = []
        wanted = []
        for t in threads:
            body = t["body"]
            if body.startswith("0x"):
                wanted.append((t["name"], int(body, 16), None))
            else:
                wanted.append((t["name"], None, body.split("::")[-1]))
        # Top-level stores provide an independent byte cross-check. Nested worker threads are
        # discovered only after lifting their parent and therefore are resolved from their literal
        # LAB address or a unique functions.tsv symbol inside this script's range.
        resolved_addrs: list[int] = []
        for name, addr, sym in wanted:
            if addr is not None:
                store = addr
            else:
                candidates = [a for a, (n, _module) in names.items() if lo <= a <= hi and n == sym]
                if len(candidates) != 1:
                    problems.append(f"thread {name}: symbol {sym} has {len(candidates)} matches in script range")
                    continue
                store = candidates[0]
            entry = {"thread": name, "address": f"0x{store:08X}", "recordedBody": name and next(
                t["body"] for t in threads if t["name"] == name)}
            if sym is not None:
                tsv = names.get(store)
                entry["functionsTsvName"] = tsv[0] if tsv else None
                if tsv is None or tsv[0] != sym:
                    problems.append(f"thread {name}: symbol {sym} vs functions.tsv {tsv} at {store:#010x}")
            if not (lo <= store <= hi):
                problems.append(f"thread {name}: body {store:#010x} outside script range {lo:#010x}-{hi:#010x}")
            resolved.append(entry)
            resolved_addrs.append(store)
        for extra in stores:
            if extra in resolved_addrs:
                continue
            resolved.append({"thread": None, "address": f"0x{extra:08X}", "recordedBody": None,
                             "note": "body store not recorded by the lifter"})
        jobs.append({"script": script, "package": package, "class": f"C{script}Script",
                     "rangeLo": f"0x{lo:08X}", "rangeHi": f"0x{hi:08X}",
                     "lifecycle": {r: f"0x{a:08X}" for r, a in life.items()},
                     "threads": resolved, "sharedBodyStores": [f"0x{a:08X}" for a in shared_stores],
                     "problems": problems,
                     "output": str(OUT_DIR / f"{script}.json")})
    return jobs


def write_job_tsv(jobs: list[dict], path: Path) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    lines = ["# script\tclass\trangeLo\trangeHi\tlifecycle(role=addr,...)\tthreads(name=addr,...)\toutput"]
    for j in jobs:
        life = ",".join(f"{r}={a}" for r, a in j["lifecycle"].items())
        threads = ",".join(f"{t['thread'] or 'unrecorded'}={t['address']}" for t in j["threads"])
        lines.append("\t".join([j["script"], j["class"], j["rangeLo"], j["rangeHi"], life, threads or "-", j["output"]]))
    path.write_text("\n".join(lines) + "\n", encoding="utf-8")


def run_headless(job_tsv: Path, log: Path) -> int:
    headless = GHIDRA_HOME / "support" / "analyzeHeadless.bat"
    command = [str(headless), str(PROJECT_DIR), "FableTLC", "-process", "Fable.exe", "-readOnly",
               "-noanalysis", "-scriptPath", str(SCRIPT_DIR), "-postScript", "ExportScriptThreadBodies.java",
               str(job_tsv)]
    print("RUN", " ".join(command), flush=True)
    started = time.time()
    with log.open("w", encoding="utf-8") as fh:
        proc = subprocess.run(command, stdout=fh, stderr=subprocess.STDOUT, text=True)
    print(f"headless exit {proc.returncode} after {time.time() - started:.0f}s; log {log}")
    return proc.returncode


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--build-job", action="store_true", help="only resolve addresses and write the job TSV")
    parser.add_argument("--job", type=Path, default=JOB_FILE)
    parser.add_argument("--script", action="append", help="restrict to these cluster names")
    args = parser.parse_args()
    scripts = [p for p in QUEST_PAIRS if not args.script or p[0] in args.script]
    jobs = build_job(scripts)
    OUT_DIR.mkdir(parents=True, exist_ok=True)
    write_job_tsv(jobs, args.job)
    args.job.with_suffix(".json").write_text(json.dumps(jobs, indent=2) + "\n", encoding="utf-8")
    for j in jobs:
        flag = "OK " if not j["problems"] else "!! "
        print(f"{flag}{j['script']}: range {j['rangeLo']}-{j['rangeHi']} threads "
              f"{[(t['thread'], t['address']) for t in j['threads']]}")
        for p in j["problems"]:
            print(f"     problem: {p}")
    print(f"job -> {args.job}")
    if args.build_job:
        return 0
    return run_headless(args.job, args.job.with_suffix(".log"))


if __name__ == "__main__":
    raise SystemExit(main())
