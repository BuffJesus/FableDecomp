#!/usr/bin/env python3
"""Snapshot and compile-gate generated auto-RE C++ candidates.

This is deliberately a strict translation-unit gate. It supplies standard C++
headers and calling-convention spellings, but never invents engine classes or
methods merely to make a candidate compile.
"""
from __future__ import annotations

import argparse
import csv
import hashlib
import json
import re
import shutil
import subprocess
from collections import Counter
from dataclasses import dataclass
from datetime import datetime
from pathlib import Path


ADDRESS_RE = re.compile(r"^0x([0-9a-fA-F]{8})_")
ERROR_RE = re.compile(r"\berror:\s*(.+)")
MISSING_PATTERNS = (
    re.compile(r"['\u2018]([^'\u2019]+)['\u2019] does not name a type"),
    re.compile(r"['\u2018]([^'\u2019]+)['\u2019] has not been declared"),
    re.compile(r"['\u2018]([^'\u2019]+)['\u2019] was not declared in this scope"),
    re.compile(r"invalid use of incomplete type [^'\u2018]*['\u2018]([^'\u2019]+)['\u2019]"),
)
VC71_FEATURES = {
    "auto": re.compile(r"\bauto\b"),
    "decltype": re.compile(r"\bdecltype\b"),
    "lambda": re.compile(r"\[[^\]\r\n]*\]\s*(?:\([^)]*\))?\s*(?:mutable\s*)?(?:->[^\{]+)?\{"),
    "nullptr": re.compile(r"\bnullptr\b"),
    "range-for": re.compile(r"\bfor\s*\([^;:()]+:[^;()]+\)"),
    "static_assert": re.compile(r"\bstatic_assert\s*\("),
    "std-fixed-width": re.compile(r"\bstd::(?:u?int(?:8|16|32|64)_t|byte|uintptr_t)\b"),
    "using-alias": re.compile(r"^\s*using\s+\w+\s*=", re.MULTILINE),
}


@dataclass(frozen=True)
class Candidate:
    address: str
    source: Path
    origin: str


def write_tsv(path: Path, rows: list[dict[str, object]]) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    fields = list(rows[0]) if rows else []
    temp = path.with_suffix(path.suffix + ".tmp")
    with temp.open("w", encoding="utf-8", newline="") as stream:
        writer = csv.DictWriter(stream, delimiter="\t", fieldnames=fields, lineterminator="\n")
        writer.writeheader()
        writer.writerows(rows)
    temp.replace(path)


def discover(root: Path) -> list[Candidate]:
    # Later directories win, so a retry snapshot supersedes its primary draft.
    selected: dict[str, Candidate] = {}
    locations = (
        ("primary", root / "lift" / "reports" / "primary" / "code"),
        ("retry", root / "lift" / "reports" / "retry" / "code"),
        ("wave2", root / "lift" / "reports" / "wave2" / "code"),
        ("wave3", root / "lift" / "reports" / "wave3" / "code"),
    )
    for origin, directory in locations:
        if not directory.exists():
            continue
        for source in sorted(directory.glob("*.cpp")):
            match = ADDRESS_RE.match(source.name)
            if match:
                selected[match.group(1).lower()] = Candidate(match.group(1).lower(), source.resolve(), origin)
    return [selected[key] for key in sorted(selected)]


def strip_comments_and_literals(text: str) -> str:
    result: list[str] = []
    index = 0
    state = "code"
    while index < len(text):
        char = text[index]
        nxt = text[index + 1] if index + 1 < len(text) else ""
        if state == "code":
            if char == "/" and nxt == "/":
                state = "line"
                result.extend("  ")
                index += 2
                continue
            if char == "/" and nxt == "*":
                state = "block"
                result.extend("  ")
                index += 2
                continue
            if char in ('"', "'"):
                state = "string" if char == '"' else "char"
                result.append(" ")
            else:
                result.append(char)
        elif state == "line":
            result.append("\n" if char == "\n" else " ")
            if char == "\n":
                state = "code"
        elif state == "block":
            result.append("\n" if char == "\n" else " ")
            if char == "*" and nxt == "/":
                result.append(" ")
                index += 1
                state = "code"
        else:
            result.append("\n" if char == "\n" else " ")
            if char == "\\":
                if index + 1 < len(text):
                    result.append(" ")
                    index += 1
            elif (state == "string" and char == '"') or (state == "char" and char == "'"):
                state = "code"
        index += 1
    return "".join(result)


def integrity(text: str) -> tuple[bool, str]:
    if not text.strip():
        return False, "empty source"
    if "```" in text:
        return False, "markdown fence in source"
    clean = strip_comments_and_literals(text)
    stack: list[str] = []
    pairs = {"}": "{", ")": "(", "]": "["}
    for char in clean:
        if char in "{([":
            stack.append(char)
        elif char in pairs:
            if not stack or stack.pop() != pairs[char]:
                return False, f"unbalanced {char}"
    if stack:
        return False, f"unclosed {stack[-1]}"
    return True, ""


def extract_missing(stderr: str) -> list[str]:
    missing: set[str] = set()
    for pattern in MISSING_PATTERNS:
        missing.update(match.group(1) for match in pattern.finditer(stderr))
    return sorted(missing, key=str.casefold)


def compiler_errors(stderr: str) -> list[str]:
    return [match.group(1).strip() for line in stderr.splitlines() if (match := ERROR_RE.search(line))]


def vc71_features(text: str) -> dict[str, int]:
    clean = strip_comments_and_literals(text)
    return {name: len(pattern.findall(clean)) for name, pattern in VC71_FEATURES.items() if pattern.search(clean)}


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--root", type=Path, default=Path(__file__).resolve().parents[1])
    parser.add_argument("--compiler", default="g++")
    parser.add_argument("--timeout", type=int, default=45)
    args = parser.parse_args()

    root = args.root.resolve()
    rebuild = root / "rebuild"
    gate_dir = rebuild / "compile-gate"
    snapshot_dir = rebuild / "candidates" / "snapshots"
    snapshot_dir.mkdir(parents=True, exist_ok=True)
    gate_dir.mkdir(parents=True, exist_ok=True)
    prelude = rebuild / "include" / "candidate_prelude.hpp"
    compiler = shutil.which(args.compiler)
    if compiler is None:
        raise SystemExit(f"compiler not found: {args.compiler}")

    manifest_rows: dict[str, dict[str, str]] = {}
    manifest = rebuild / "manifest" / "functions.tsv"
    if manifest.exists():
        with manifest.open("r", encoding="utf-8-sig", newline="") as stream:
            manifest_rows = {row["address"].lower(): row for row in csv.DictReader(stream, delimiter="\t")}

    rows: list[dict[str, object]] = []
    dependency_counts: Counter[str] = Counter()
    for candidate in discover(root):
        payload = candidate.source.read_bytes()
        text = payload.decode("utf-8-sig", errors="replace")
        digest = hashlib.sha256(payload).hexdigest()
        snapshot = snapshot_dir / candidate.source.name
        if not snapshot.exists() or snapshot.read_bytes() != payload:
            snapshot.write_bytes(payload)
        integrity_ok, integrity_detail = integrity(text)
        command = [
            compiler,
            "-m32",
            "-std=gnu++20",
            "-fsyntax-only",
            "-fmax-errors=96",
            "-include",
            str(prelude),
        ]
        declaration_header = rebuild / "include" / "candidates" / f"{candidate.address}.hpp"
        if declaration_header.exists():
            command.extend(["-include", str(declaration_header)])
        command.append(str(snapshot))
        try:
            process = subprocess.run(command, cwd=root, capture_output=True, text=True, timeout=args.timeout)
            stderr = process.stderr
            errors = compiler_errors(stderr)
            host_pass = process.returncode == 0
        except subprocess.TimeoutExpired as exc:
            stderr = (exc.stderr or "") if isinstance(exc.stderr, str) else ""
            errors = [f"compiler timeout after {args.timeout}s"]
            host_pass = False
        missing = extract_missing(stderr)
        dependency_counts.update(missing)
        features = vc71_features(text)
        vc71_count = sum(features.values())
        source_row = manifest_rows.get(candidate.address, {})
        rows.append(
            {
                "address": candidate.address,
                "name": source_row.get("name", ""),
                "module": source_row.get("module", ""),
                "checker_verdict": source_row.get("agent_verdict", ""),
                "origin": candidate.origin,
                "snapshot": str(snapshot.resolve()),
                "declaration_header": str(declaration_header.resolve()) if declaration_header.exists() else "",
                "sha256": digest,
                "bytes": len(payload),
                "integrity": "PASS" if integrity_ok else "FAIL",
                "integrity_detail": integrity_detail,
                "host_cpp20_syntax": "PASS" if host_pass else "FAIL",
                "compiler_errors": len(errors),
                "first_blocker": errors[0] if errors else "",
                "missing_dependencies": ";".join(missing),
                "vc71_incompatibilities": vc71_count,
                "vc71_features": ";".join(f"{key}:{value}" for key, value in sorted(features.items())),
                "compile_ready": int(integrity_ok and host_pass and vc71_count == 0),
            }
        )

    write_tsv(gate_dir / "candidates.tsv", rows)
    summary = {
        "generated_at": datetime.now().astimezone().isoformat(timespec="seconds"),
        "compiler": compiler,
        "candidates": len(rows),
        "integrity_pass": sum(row["integrity"] == "PASS" for row in rows),
        "host_cpp20_syntax_pass": sum(row["host_cpp20_syntax"] == "PASS" for row in rows),
        "vc71_compatible": sum(int(row["vc71_incompatibilities"]) == 0 for row in rows),
        "compile_ready": sum(int(row["compile_ready"]) for row in rows),
        "unique_missing_dependencies": len(dependency_counts),
    }
    temp_json = gate_dir / "summary.json.tmp"
    temp_json.write_text(json.dumps(summary, indent=2) + "\n", encoding="utf-8")
    temp_json.replace(gate_dir / "summary.json")

    lines = [
        "# Auto-RE candidate compile gate",
        "",
        f"Generated: `{summary['generated_at']}`",
        "",
        "| Gate | Passing | Total |",
        "|---|---:|---:|",
        f"| Source integrity | {summary['integrity_pass']} | {summary['candidates']} |",
        f"| 32-bit host C++20 syntax | {summary['host_cpp20_syntax_pass']} | {summary['candidates']} |",
        f"| No detected VC7.1 language blockers | {summary['vc71_compatible']} | {summary['candidates']} |",
        f"| Ready for VC7.1 module compilation | {summary['compile_ready']} | {summary['candidates']} |",
        "",
        "A checker PASS is not a compiler PASS. The host syntax gate supplies only standard-library and calling-convention compatibility; missing engine declarations must be resolved through real module headers.",
        "",
        "## Most common missing declarations",
        "",
        "| Declaration | Candidates |",
        "|---|---:|",
    ]
    lines.extend(f"| `{name}` | {count} |" for name, count in dependency_counts.most_common(30))
    lines.extend(["", "Full per-candidate results are in `candidates.tsv`.", ""])
    temp_md = gate_dir / "README.md.tmp"
    temp_md.write_text("\n".join(lines), encoding="utf-8")
    temp_md.replace(gate_dir / "README.md")
    print(json.dumps(summary, indent=2))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
