#!/usr/bin/env python3
"""Cross-check generated candidate definitions against the engine API manifest."""
from __future__ import annotations

import argparse
import csv
import json
import re
from collections import Counter, defaultdict
from datetime import datetime
from pathlib import Path


DEFINITION_RE = re.compile(
    r"(?m)^[ \t]*(?P<return>[^\r\n;{}]+?)[ \t]*(?:\r?\n[ \t]*)?"
    r"(?P<owner>[A-Za-z_]\w*(?:::\w+)*)::(?P<method>~?[A-Za-z_]\w*)[ \t]*"
    r"\((?P<params>[^;{}]*?)\)\s*(?:const\s*)?\{",
    re.DOTALL,
)

GLOBAL_DEFINITION_RE = re.compile(
    r'(?m)^[ \t]*(?P<return>[^\r\n;{}]+?)[ \t]+'
    r'(?P<method>[A-Za-z_$][\w$]*)[ \t]*'
    r'\((?P<params>[^;{}]*?)\)\s*\{',
    re.DOTALL,
)


def read_tsv(path: Path) -> list[dict[str, str]]:
    with path.open("r", encoding="utf-8-sig", newline="") as stream:
        return list(csv.DictReader(stream, delimiter="\t"))


def write_tsv(path: Path, rows: list[dict[str, object]]) -> None:
    fields = list(rows[0]) if rows else []
    temp = path.with_suffix(path.suffix + ".tmp")
    with temp.open("w", encoding="utf-8", newline="") as stream:
        writer = csv.DictWriter(stream, delimiter="\t", fieldnames=fields, lineterminator="\n")
        writer.writeheader()
        writer.writerows(rows)
    temp.replace(path)


def split_parameters(text: str) -> list[str]:
    text = text.strip()
    if not text or text == "void":
        return []
    result: list[str] = []
    start = 0
    depths = {"<": 0, "(": 0, "[": 0}
    closing = {">": "<", ")": "(", "]": "["}
    for index, char in enumerate(text):
        if char in depths:
            depths[char] += 1
        elif char in closing and depths[closing[char]]:
            depths[closing[char]] -= 1
        elif char == "," and not any(depths.values()):
            result.append(text[start:index].strip())
            start = index + 1
    result.append(text[start:].strip())
    return result


def return_shape(value: str) -> str:
    clean = re.sub(r"\b(?:class|struct|const|volatile|__thiscall|__cdecl|__stdcall|__fastcall)\b", "", value)
    clean = re.sub(r"\s+", " ", clean).strip()
    if "&" in clean:
        return "reference"
    if "*" in clean:
        return "pointer"
    return "value"


def expected_explicit_parameters(row: dict[str, str]) -> int:
    count = int(row["parameter_count"])
    types = [part.strip() for part in row["parameter_types"].split(",") if part.strip()]
    if row["calling_convention"] == "__thiscall" and types:
        first = types[0].replace("class ", "").replace("struct ", "")
        if row["module"] in first and "*" in first:
            return max(0, count - 1)
    return count


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--root", type=Path, default=Path(__file__).resolve().parents[1])
    args = parser.parse_args()
    root = args.root.resolve()
    gate = root / "rebuild" / "compile-gate"
    candidates = read_tsv(gate / "candidates.tsv")
    manifest = {row["address"]: row for row in read_tsv(root / "rebuild" / "manifest" / "functions.tsv")}

    duplicate_groups: dict[tuple[str, str, str], list[str]] = defaultdict(list)
    for candidate in candidates:
        row = manifest[candidate["address"]]
        duplicate_groups[(row["module"], row["name"], row["parameter_types"])].append(row["address"])

    rows: list[dict[str, object]] = []
    issue_counts: Counter[str] = Counter()
    for candidate in candidates:
        row = manifest[candidate["address"]]
        text = Path(candidate["snapshot"]).read_text(encoding="utf-8-sig", errors="replace")
        # Storage/code-generation attributes are declarator prefixes, not function names.
        # Removing them before the lightweight parse keeps naked CRT helpers and ordinary
        # generated definitions on the same signature path.
        parse_text = re.sub(r"\b__declspec\s*\([^)]*\)\s*", "", text)
        match = DEFINITION_RE.search(parse_text)
        is_global = row["module"] == "_global"
        if match is None and is_global:
            match = GLOBAL_DEFINITION_RE.search(parse_text)
        issues: list[str] = []
        generated_owner = generated_method = generated_return = ""
        generated_count = -1
        if match is None:
            issues.append("definition-not-parsed")
        else:
            generated_owner = "_global" if is_global else match.group("owner")
            generated_method = match.group("method")
            generated_return = re.sub(r"\s+", " ", match.group("return")).strip()
            generated_count = len(split_parameters(match.group("params")))
            expected_count = expected_explicit_parameters(row)
            if generated_owner != row["module"]:
                issues.append("owner-mismatch")
            expected_method = row["name"].rsplit("::", 1)[-1]
            if generated_method != expected_method:
                issues.append("method-name-mismatch")
            if generated_count != expected_count:
                issues.append("explicit-parameter-count-mismatch")
            if return_shape(generated_return) != return_shape(row["return_type"]):
                issues.append("return-shape-mismatch")
        duplicate_key = (row["module"], row["name"], row["parameter_types"])
        duplicates = duplicate_groups[duplicate_key]
        if len(duplicates) > 1:
            issues.append("duplicate-linkage-signature")
        issue_counts.update(issues)
        rows.append(
            {
                "address": row["address"],
                "module": row["module"],
                "manifest_name": row["name"],
                "checker_verdict": row["agent_verdict"],
                "generated_owner": generated_owner,
                "generated_method": generated_method,
                "manifest_return": row["return_type"],
                "generated_return": generated_return,
                "expected_explicit_parameters": expected_explicit_parameters(row),
                "generated_explicit_parameters": generated_count,
                "duplicate_addresses": ";".join(duplicates) if len(duplicates) > 1 else "",
                "status": "PASS" if not issues else "REVIEW",
                "issues": ";".join(issues),
            }
        )
    write_tsv(gate / "signature-audit.tsv", rows)
    summary = {
        "generated_at": datetime.now().astimezone().isoformat(timespec="seconds"),
        "candidates": len(rows),
        "pass": sum(row["status"] == "PASS" for row in rows),
        "review": sum(row["status"] == "REVIEW" for row in rows),
        "issue_counts": dict(issue_counts.most_common()),
    }
    temp_json = gate / "signature-audit.json.tmp"
    temp_json.write_text(json.dumps(summary, indent=2) + "\n", encoding="utf-8")
    temp_json.replace(gate / "signature-audit.json")
    lines = [
        "# Candidate signature audit",
        "",
        f"Generated: `{summary['generated_at']}`",
        "",
        f"Clean: **{summary['pass']} / {summary['candidates']}**. Review required: **{summary['review']}**.",
        "",
        "| Issue | Candidates |",
        "|---|---:|",
    ]
    lines.extend(f"| `{issue}` | {count} |" for issue, count in issue_counts.most_common())
    lines.extend(["", "## Review queue", "", "| Address | Module | Manifest | Generated | Issues |", "|---|---|---|---|---|"])
    for row in rows:
        if row["status"] == "REVIEW":
            lines.append(
                f"| `{row['address']}` | `{row['module']}` | `{row['manifest_name']}` | `{row['generated_method']}` | {row['issues']} |"
            )
    lines.append("")
    temp_md = gate / "SIGNATURE_AUDIT.md.tmp"
    temp_md.write_text("\n".join(lines), encoding="utf-8")
    temp_md.replace(gate / "SIGNATURE_AUDIT.md")
    print(json.dumps(summary, indent=2))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
