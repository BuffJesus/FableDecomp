"""Fork steps 1-2 readiness: compile every reconstructed TU with a current MSVC, x86 and x64.

Evidence for docs/modern/MODERN_COMPILE_CENSUS.md (+ .tsv). Each non-test source under
rebuild/src/compiled/ is syntax-checked (`cl /Zs`, front end only: templates, static asserts and
the pointer-width diagnostics all run there) in two configurations:

  x86   current MSVC, 32-bit, /permissive, headers as-is. Step 1 ("modern compiler, still x86"):
        what VC7.1-era spelling a current front end rejects.
  x64   current MSVC, 64-bit. rebuild_abi.h hard-requires 4-byte pointers and every engine header
        asserts its 32-bit PDB layout, so this pass compiles against a copy of rebuild/include
        whose rebuild_abi.h has the pointer check and FABLE_STATIC_ASSERT disabled. What is left is
        code that cannot compile for x64 at all (inline asm, pointer/int casts that are errors) and
        the C4311/C4312/C4302 pointer-truncation warnings. Layout drift is a separate question:
        `--layouts` compiles each engine header alone with the asserts live and lists the classes
        whose PDB size no longer holds on x64 (the fork's own-types worklist).

Nothing in the parity tree is modified; the patched include copies live under work/modern_census/.

  python tools/modern/modern_compile_census.py            # both passes over all TUs
  python tools/modern/modern_compile_census.py --limit 200 --jobs 8
  python tools/modern/modern_compile_census.py --layouts  # header layout pass only
"""
from __future__ import annotations

import argparse
import collections
import concurrent.futures
import json
import os
import re
import shutil
import subprocess
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
SRC = ROOT / "rebuild" / "src" / "compiled"
INCLUDE = ROOT / "rebuild" / "include"
WORK = ROOT / "work" / "modern_census"
OUT_MD = ROOT / "docs" / "modern" / "MODERN_COMPILE_CENSUS.md"
OUT_TSV = ROOT / "docs" / "modern" / "MODERN_COMPILE_CENSUS.tsv"
DIAGS = WORK / "diagnostics.json"
LAYOUT_SUMMARY = WORK / "layout_summary.json"
LAYOUT_SUMMARY = WORK / "layout_summary.json"
OUT_LAYOUT_TSV = ROOT / "docs" / "modern" / "X64_LAYOUT_DRIFT.tsv"
VCVARS = Path(r"C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat")

ASM_RE = re.compile(r"__asm\b|__declspec\s*\(\s*naked\s*\)|\b_emit\b")
DIAG_RE = re.compile(r"^(?P<file>.+?)\((?P<line>\d+)(?:,\d+)?\)\s*:\s*(?P<kind>fatal error|error|warning)\s+(?P<code>C\d{4})\s*:\s*(?P<msg>.*)$")
RAW_ASSERT_RE = re.compile(
    r"typedef\s+char\s+(?P<name>\w+)\s*\[\s*(?P<cond>(?:[^\[\];]|\[[^\[\];]*\])*?)\s*\?\s*1\s*:\s*-1\s*\]\s*;", re.S)
# Front-end diagnostics that mean "this value does not survive 64-bit pointers".
PTR_WARNINGS = {"C4311", "C4312", "C4302", "C4267"}
COMMON_FLAGS = ["/nologo", "/Zs", "/TP", "/EHsc", "/std:c++17", "/permissive", "/W3", "/w14311",
                "/w14312", "/w14302", "/w14267", "/Zc:wchar_t-", "/D_CRT_SECURE_NO_WARNINGS", "/DFABLE_MODERN_CENSUS",
                "/diagnostics:classic"]
BATCH = 24
# A failing `typedef char x[cond ? 1 : -1]` (C2118) or static_assert (C2338).
LAYOUT_ERRORS = {"C2118", "C2338"}


def vc_env(arch: str) -> dict[str, str]:
    cache = WORK / f"env_{arch}.json"
    if cache.exists():
        return json.loads(cache.read_text())
    out = subprocess.run(f'"{VCVARS}" {arch} >nul && set', shell=True, capture_output=True, text=True,
                         check=True).stdout
    env = dict(line.split("=", 1) for line in out.splitlines() if "=" in line)
    WORK.mkdir(parents=True, exist_ok=True)
    cache.write_text(json.dumps(env))
    return env


def patched_include_tree(name: str, assert_macro: str) -> Path:
    """A copy of rebuild/include whose rebuild_abi.h drops the 4-byte pointer requirement.

    A copy, not an overlay directory: MSVC resolves `#include "rebuild_abi.h"` against the
    including header's own directory before any /I path, so only replacing the file works.
    """
    tree = WORK / name
    if tree.exists():
        shutil.rmtree(tree)
    shutil.copytree(INCLUDE, tree)
    text = (INCLUDE / "rebuild_abi.h").read_text()
    text = text.replace("typedef char fable_require_ptr32[sizeof(void*) == 4 ? 1 : -1];",
                        "/* census copy: pointer width unconstrained */")
    text = re.sub(r"#define FABLE_STATIC_ASSERT\(expression\)\s*\\\n[^\n]*\n",
                  lambda _: f"#define FABLE_STATIC_ASSERT(expression) {assert_macro}\n", text)
    if assert_macro not in text:
        raise SystemExit("rebuild_abi.h changed shape; update patched_include_tree()")
    (tree / "rebuild_abi.h").write_text(text)
    # Headers also spell layout asserts by hand: `typedef char Name[(cond) ? 1 : -1];`.
    live = assert_macro.startswith("static_assert")
    for h in tree.rglob("*.h"):
        src = h.read_text(errors="replace")
        out = RAW_ASSERT_RE.sub(lambda m: f'static_assert({m.group("cond")}, "{m.group("name")}");' if live
                                else "/* census copy: layout assert off */", src)
        if out != src:
            h.write_text(out)
    return tree


def write_overlay() -> Path:
    return patched_include_tree("include_x64", "/* census copy: layout asserts off */")


def sources() -> list[Path]:
    return sorted(p for p in SRC.rglob("*.cpp") if not p.name.endswith("_test.cpp"))


def compile_batch(files: list[Path], env: dict[str, str], includes: list[Path]) -> dict[str, list[dict]]:
    """One cl run over several TUs; diagnostics are attributed via cl's per-file banner line."""
    cl = shutil.which("cl", path=env["PATH"])
    cmd = [cl, *COMMON_FLAGS, *[f"/I{p}" for p in includes], *[str(f) for f in files]]
    proc = subprocess.run(cmd, env=env, capture_output=True, text=True, errors="replace", cwd=WORK)
    names = {f.name: str(f) for f in files}
    diags: dict[str, list[dict]] = {str(f): [] for f in files}
    current = None
    for line in (proc.stdout + proc.stderr).splitlines():
        line = line.strip()
        if line in names:
            current = names[line]
            continue
        m = DIAG_RE.match(line)
        if m and current:
            d = m.groupdict()
            d["kind"] = "error" if "error" in d["kind"] else "warning"
            diags[current].append(d)
    return diags


def classify(diags: list[dict]) -> tuple[str, str]:
    errors = [d for d in diags if d["kind"] == "error"]
    code = [d for d in errors if d["code"] not in LAYOUT_ERRORS]
    if code:
        return "error", code[0]["code"]
    if errors:
        # Only a layout assert fired: the code compiles, a PDB size/offset does not hold here.
        return "layout", header_of(errors[0]["file"])
    ptr = sorted({d["code"] for d in diags if d["code"] in PTR_WARNINGS})
    if ptr:
        return "ptr_warning", ",".join(ptr)
    return "clean", ""


def run_pass(name: str, arch: str, includes: list[Path], files: list[Path], jobs: int) -> dict[str, list[dict]]:
    env = vc_env(arch)
    batches = [files[i:i + BATCH] for i in range(0, len(files), BATCH)]
    result: dict[str, list[dict]] = {}
    with concurrent.futures.ThreadPoolExecutor(jobs) as pool:
        for n, part in enumerate(pool.map(lambda b: compile_batch(b, env, includes), batches), 1):
            result.update(part)
            if n % 50 == 0:
                print(f"  {name}: {n}/{len(batches)} batches", flush=True)
    return result


def header_of(diag_file: str) -> str:
    p = diag_file.replace("\\", "/")
    for root in ("rebuild/include/", "modern_census/include_x64/", "modern_census/include_layout/"):
        i = p.find(root)
        if i >= 0:
            return p[i + len(root):]
    return Path(p).name


def is_shared_header(diag_file: str) -> bool:
    return diag_file.lower().endswith((".h", ".hpp", ".inl"))


def census(limit: int | None, jobs: int) -> None:
    files = sources()[:limit] if limit else sources()
    asm = {str(f) for f in files if ASM_RE.search(f.read_text(errors="replace"))}
    portable = [f for f in files if str(f) not in asm]
    print(f"{len(files)} TUs, {len(asm)} with inline asm (not x64-compilable)")
    x86 = run_pass("x86", "x86", [INCLUDE], files, jobs)
    x64 = run_pass("x64", "x64", [write_overlay()], portable, jobs)
    DIAGS.write_text(json.dumps({"limit": limit, "asm": sorted(asm), "files": [str(f) for f in files],
                                 "x86": x86, "x64": x64}))
    report_from(DIAGS)


def report_from(path: Path) -> None:
    """Rebuild the TSV and report from saved diagnostics (no recompilation)."""
    data = json.loads(path.read_text())
    files, asm, x86, x64, limit = [Path(f) for f in data["files"]], set(data["asm"]), data["x86"], data["x64"], data["limit"]

    rows = []
    for f in files:
        key = str(f)
        c86, d86 = classify(x86[key])
        if key in asm:
            c64, d64 = "inline_asm", ""
        else:
            c64, d64 = classify(x64[key])
        first = next((d for d in x86[key] + x64.get(key, []) if d["kind"] == "error"), None)
        rows.append({"tu": f.relative_to(SRC).as_posix(), "x86": c86, "x86_detail": d86, "x64": c64,
                     "x64_detail": d64,
                     "first_error": f"{header_of(first['file'])}:{first['line']} {first['code']} {first['msg'][:120]}" if first else ""})
    OUT_TSV.write_text("tu\tx86\tx86_detail\tx64\tx64_detail\tfirst_error\n" + "".join(
        f"{r['tu']}\t{r['x86']}\t{r['x86_detail']}\t{r['x64']}\t{r['x64_detail']}\t{r['first_error']}\n" for r in rows
        if (r["x86"], r["x64"]) != ("clean", "clean")), encoding="utf-8", newline="\n")
    write_report(rows, x86, x64, limit)


def top(counter: collections.Counter, n: int = 15) -> list[tuple[str, int]]:
    return counter.most_common(n)


def write_report(rows: list[dict], x86: dict, x64: dict, limit: int | None) -> None:
    total = len(rows)
    s86 = collections.Counter(r["x86"] for r in rows)
    s64 = collections.Counter(r["x64"] for r in rows)
    both = sum(1 for r in rows if r["x86"] in ("clean", "ptr_warning") and r["x64"] in ("clean", "ptr_warning"))
    err_codes86 = collections.Counter(r["x86_detail"] for r in rows if r["x86"] == "error")
    err_codes64 = collections.Counter(r["x64_detail"] for r in rows if r["x64"] == "error")
    # Where errors originate: shared headers fix many TUs at once.
    origin86 = collections.Counter()
    origin64 = collections.Counter()
    for diags, origin in ((x86, origin86), (x64, origin64)):
        for tu, ds in diags.items():
            seen = {header_of(d["file"]) for d in ds if d["kind"] == "error" and is_shared_header(d["file"])}
            origin.update(seen)
    ptr_sites = collections.Counter()
    ptr_origin = collections.Counter()        # diagnostics per origin file (a header counts once per TU)
    ptr_origin_tus = collections.Counter()
    ptr_unique = collections.defaultdict(set)  # distinct (line, code) sites per origin file
    for tu, ds in x64.items():
        tu_origins = set()
        for d in ds:
            if d["code"] in PTR_WARNINGS:
                ptr_sites[d["code"]] += 1
                origin = header_of(d["file"]) if is_shared_header(d["file"]) else "(the TU's own source)"
                ptr_origin[origin] += 1
                ptr_unique[origin].add((d["file"].lower() if origin.startswith("(") else "", d["line"], d["code"]))
                tu_origins.add(origin)
        ptr_origin_tus.update(tu_origins)
    pct = lambda n: f"{100 * n / total:.1f}%" if total else "-"
    lines = [
        "# Modern compiler census (fork steps 1-2)",
        "",
        "Generated by `tools/modern/modern_compile_census.py`; do not hand-edit. Row data:",
        "[MODERN_COMPILE_CENSUS.tsv](MODERN_COMPILE_CENSUS.tsv) (every TU not clean in both configurations).",
        "",
        "Every non-test TU under `rebuild/src/compiled/` syntax-checked (`cl /Zs`, VS 2022, `/std:c++17",
        "/permissive /Zc:wchar_t-`, the parity tree's `wchar_t == unsigned short`). The x64 pass uses a",
        "census-only copy of `rebuild/include` whose `rebuild_abi.h` drops the 4-byte pointer requirement",
        "and the PDB layout asserts, so it measures code, not layouts (see `--layouts` for those).",
        "This is a readiness measurement, not a port: a clean x64 TU still runs on 32-bit layouts.",
        "",
        f"TUs: **{total}**" + (f" (first {limit} only)" if limit else ""),
        "",
        "| Result | x86 (step 1) | x64 (step 2) |",
        "|---|---:|---:|",
    ]
    for k in ("clean", "ptr_warning", "layout", "error", "inline_asm"):
        lines.append(f"| {k} | {s86.get(k, 0)} ({pct(s86.get(k, 0))}) | {s64.get(k, 0)} ({pct(s64.get(k, 0))}) |")
    lines += ["", f"Compile in both configurations: **{both}** ({pct(both)}).", ""]
    # What x64 actually needs, by kind of fix (each line is one piece of work, not one TU).
    top_ptr = ptr_origin.most_common(1)[0] if ptr_origin else ("-", 0)
    own = "(the TU's own source)"
    lines += ["## x64 readiness by fix", "",
              f"- **Errors:** {s64.get('error', 0)} TUs; {err_codes64.get('C2821', 0)} are `operator new(unsigned int)`"
              " in shared headers (one `size_t` spelling each, see below).",
              f"- **Pointer-width warnings:** {sum(ptr_origin.values())} diagnostics; {top_ptr[1]} come from"
              f" {len(ptr_unique[top_ptr[0]])} lines of `{top_ptr[0]}` and {len(ptr_unique[own])} sites from"
              f" {ptr_origin_tus[own]} TUs' own sources.",
              f"- **Inline asm:** {s64.get('inline_asm', 0)} TUs, the asm-bake (de-bake) backlog; x64 has no inline asm.",
              "- **Layouts:** see `--layouts` below; the fork's own types replace the PDB layouts.",
              "",
              "## x86 errors by first code", "", "| Code | TUs |", "|---|---:|"]
    lines += [f"| {c} | {n} |" for c, n in top(err_codes86)]
    lines += ["", "## x64 errors by first code", "", "| Code | TUs |", "|---|---:|"]
    lines += [f"| {c} | {n} |" for c, n in top(err_codes64)]
    lines += ["", "## Shared headers that cause errors (TUs affected)", "",
              "Fixing one of these fixes every TU listed against it.", "",
              "| Header | x86 TUs | x64 TUs |", "|---|---:|---:|"]
    for h in sorted(set(origin86) | set(origin64), key=lambda h: -(origin86[h] + origin64[h]))[:25]:
        lines.append(f"| `{h}` | {origin86[h]} | {origin64[h]} |")
    lines += ["", "## x64 pointer-width diagnostics", "", "| Warning | Sites |", "|---|---:|"]
    lines += [f"| {c} | {n} |" for c, n in sorted(ptr_sites.items())]
    lines += ["", "By origin. A header's diagnostics repeat in every TU that includes it, so one fix there",
              "clears them all:", "", "| Origin | Diagnostics | Distinct sites | TUs |", "|---|---:|---:|---:|"]
    for origin, n in ptr_origin.most_common(8):
        lines.append(f"| `{origin}` | {n} | {len(ptr_unique[origin])} | {ptr_origin_tus[origin]} |")
    code_errors = [r for r in rows if r["x86"] == "error" or (r["x64"] == "error" and r["x64_detail"] != "C2821")]
    lines += ["", "## Code errors (excluding the shared `operator new(unsigned int)` headers)", "",
              "| TU | x86 | x64 | First error |", "|---|---|---|---|"]
    lines += [f"| `{r['tu']}` | {r['x86_detail'] or r['x86']} | {r['x64_detail'] or r['x64']} | "
              f"{r['first_error'].replace('|', '/')} |" for r in code_errors]
    if LAYOUT_SUMMARY.exists():
        s = json.loads(LAYOUT_SUMMARY.read_text())
        lines += ["", "## x64 layout drift (`--layouts`)", "",
                  f"Each of the {s['headers']} `rebuild/include/engine/*.h` headers compiled alone for x64 with its",
                  f"PDB asserts live: **{s['failing']} of {s['asserts']}** size-asserted classes fail (vtables or",
                  "pointer members). The fork needs its own types for these; persistence is unaffected because",
                  "saves and defs go member by member ([PERSISTED_TYPES](PERSISTED_TYPES.md)). List:",
                  "[X64_LAYOUT_DRIFT.tsv](X64_LAYOUT_DRIFT.tsv)."]
    x86_tsv, x86_summary = layout_paths("x86")
    if x86_summary.exists():
        s = json.loads(x86_summary.read_text())
        lines += ["", "## x86 layout drift (`--layouts --arch x86`)", "",
                  f"The same headers compiled for **x86** with the current compiler: **{s['failing']} of {s['asserts']}**",
                  "size-asserted classes change size at the same pointer width, with "
                  f"{s['other_errors']} headers failing otherwise. Any class listed in",
                  f"[{x86_tsv.name}]({x86_tsv.name}) embeds a VC7.1 standard-library type whose current layout differs.",
                  "The generated engine headers model library members by their PDB size, so they do not depend on the",
                  "current library. The step-1 layout breaks seen in the TU pass come from hand-written shared headers",
                  "(`std::bitset<274>` in `fable_world.h`)."]
    lines.append("")
    OUT_MD.write_text("\n".join(lines), encoding="utf-8", newline="\n")
    print("\n".join(lines[10:22]))


def layout_paths(arch: str) -> tuple[Path, Path]:
    """(drift TSV, summary JSON) for one architecture; x64 keeps its original file names."""
    if arch == "x64":
        return OUT_LAYOUT_TSV, LAYOUT_SUMMARY
    return ROOT / "docs" / "modern" / f"{arch.upper()}_LAYOUT_DRIFT.tsv", WORK / f"layout_summary_{arch}.json"


def layouts(jobs: int, arch: str = "x64") -> None:
    """Compile each engine header alone with the layout asserts live.

    x64 measures pointer-width drift. x86 measures what a current compiler changes at the same
    pointer width: VC7.1 standard-library layouts (fork step 1)."""
    out_tsv, out_summary = layout_paths(arch)
    overlay = patched_include_tree("include_layout", "static_assert(expression, #expression)")
    stubs = WORK / "layout_tus"
    stubs.mkdir(parents=True, exist_ok=True)
    headers = sorted((INCLUDE / "engine").glob("*.h"))
    tus = []
    for h in headers:
        tu = stubs / f"{h.stem}.cpp"
        tu.write_text(f'#include "engine/{h.name}"\n')
        tus.append(tu)
    diags = run_pass(f"layouts-{arch}", arch, [overlay], tus, jobs)
    assert_re = re.compile(r"sizeof\((\w+)\)\s*==\s*(0x[0-9a-fA-F]+|\d+)")
    drift = {}
    broken = 0
    for tu, ds in diags.items():
        own = Path(tu).stem
        for d in ds:
            if d["code"] == "C2338" or "static_assert" in d["msg"] or "static assertion" in d["msg"]:
                m = assert_re.search(d["msg"])
                if m:
                    drift[m.group(1)] = int(m.group(2), 0)
        if any(d["kind"] == "error" and d["code"] not in ("C2338", "C2607") for d in ds):
            broken += 1
    out_tsv.write_text("class\tpdb_size_x86\n" + "".join(f"{c}\t0x{s:x}\n" for c, s in sorted(drift.items())),
                              encoding="utf-8", newline="\n")
    # Denominator: every class with a size assert anywhere in the tree (engine headers pull in others).
    asserted = set()
    for h in INCLUDE.rglob("*.h"):
        asserted.update(re.findall(r"FABLE_STATIC_ASSERT\(\s*sizeof\((\w+)\)\s*==", h.read_text(errors="replace")))
    out_summary.write_text(json.dumps({"headers": len(headers), "asserts": len(asserted),
                                       "failing": len(drift), "other_errors": broken}))
    print(f"{len(headers)} engine headers; {len(drift)} classes change size on {arch}; {broken} headers have other errors")


def main() -> None:
    ap = argparse.ArgumentParser()
    ap.add_argument("--limit", type=int)
    ap.add_argument("--jobs", type=int, default=max(1, (os.cpu_count() or 4) - 2))
    ap.add_argument("--layouts", action="store_true")
    ap.add_argument("--arch", choices=("x86", "x64"), default="x64", help="architecture for --layouts")
    ap.add_argument("--report", action="store_true", help="rebuild the report from the last run's diagnostics")
    args = ap.parse_args()
    WORK.mkdir(parents=True, exist_ok=True)
    if args.report:
        report_from(DIAGS)
    elif args.layouts:
        layouts(args.jobs, args.arch)
    else:
        census(args.limit, args.jobs)


if __name__ == "__main__":
    main()
