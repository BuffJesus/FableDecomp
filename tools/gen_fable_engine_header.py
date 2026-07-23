#!/usr/bin/env python3
"""Generate the retail engine address/ABI header consumed by tooling and injectors.

The corrected rebuild manifest is preferred because it layers reviewed and
VC7.1-decorated-name prototype recovery over the raw Ghidra export.
"""
from __future__ import annotations

import argparse
import collections
import csv
import re
from pathlib import Path


VALID_CC = {"__thiscall", "__fastcall", "__cdecl", "__stdcall"}


def sanitize(name: str) -> str:
    value = name.replace("::", "_")
    value = re.sub(r"~", "dtor_", value)
    replacements = {
        "operator==": "op_eq", "operator!=": "op_ne", "operator=": "op_assign",
        "operator[]": "op_index", "operator+=": "op_addeq", "operator<": "op_lt",
        "operator>": "op_gt",
    }
    for source, target in replacements.items():
        value = value.replace(source, target)
    value = re.sub(r"operator", "op_", value)
    value = re.sub(r"<[^>]*>", "", value)
    value = re.sub(r"[^0-9A-Za-z_]", "_", value)
    value = re.sub(r"_+", "_", value).strip("_")
    if not value or value[0].isdigit():
        value = "fn_" + value
    return value


def read_manifest(path: Path) -> list[dict[str, str]]:
    with path.open("r", encoding="utf-8-sig", errors="replace", newline="") as stream:
        return list(csv.DictReader(stream, delimiter="\t"))


def evidence(row: dict[str, str]) -> str:
    items = [
        "prototype=" + ("complete" if row.get("prototype_complete") == "1" else "incomplete"),
        "decompile=" + row.get("clean_decompile", "unmeasured"),
    ]
    for key, label in (
        ("agent_verdict", "agent"), ("compiled_status", "compiled"),
        ("behavior_test", "behavior"), ("retail_parity", "parity"),
        ("lift_grade", "lift"),
    ):
        if row.get(key):
            items.append(f"{label}={row[key]}")
    return "; ".join(items)


def render(rows: list[dict[str, str]], source: Path) -> str:
    seen: collections.Counter[str] = collections.Counter()
    complete = sum(row.get("prototype_complete") == "1" for row in rows)
    output = [
        "// fable_engine.h - GENERATED. Retail Fable.exe (TLC, Steam) engine address/ABI map.",
        f"// Source: {source.as_posix()} ({len(rows)} functions; {complete} complete prototypes).",
        "// Confidence comments distinguish navigation-only entries from compiled/retail-verified lifts.",
        "// Regenerate: python tools/gen_fable_engine_header.py",
        "#pragma once",
        "#include <cstdint>",
        "",
        "// Cast an address constant to a typed engine function pointer:",
        "//   auto f = FABLE_FN(int, __thiscall, fable::CCharString_AssignFromWide, void*, short*);",
        "#define FABLE_FN(ret, cc, addrconst, ...) (reinterpret_cast<ret(cc *)(__VA_ARGS__)>(addrconst))",
        "",
        "namespace fable {",
    ]
    for row in rows:
        address = row["address"].lower().removeprefix("0x")
        symbol = sanitize(row["name"])
        seen[symbol] += 1
        if seen[symbol] > 1:
            symbol = f"{symbol}__{address}"
        cc = row.get("calling_convention", "unknown")
        if cc not in VALID_CC:
            cc = "unknown"
        output.append(
            f"    constexpr std::uintptr_t {symbol} = 0x{address}; // "
            f"{cc} {row.get('return_type', 'undefined')} {row['name']}"
            f"({row.get('parameter_types', '')}) | {evidence(row)}"
        )
    output.append("} // namespace fable")
    return "\n".join(output) + "\n"


def write_if_changed(path: Path, text: str) -> bool:
    path.parent.mkdir(parents=True, exist_ok=True)
    if path.exists() and path.read_text(encoding="utf-8", errors="replace") == text:
        return False
    temp = path.with_suffix(path.suffix + ".tmp")
    temp.write_text(text, encoding="utf-8")
    temp.replace(path)
    return True


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--root", type=Path, default=Path(__file__).resolve().parents[1])
    parser.add_argument("--manifest", type=Path)
    parser.add_argument("--output", type=Path, action="append")
    parser.add_argument("--no-fableforge-mirror", action="store_true")
    args = parser.parse_args()
    root = args.root.resolve()
    source = (args.manifest or root / "rebuild" / "manifest" / "functions.tsv").resolve()
    outputs = args.output or [root / "ghidra_out" / "fable_engine.h"]
    if not args.no_fableforge_mirror and args.output is None:
        outputs.append(Path(r"D:\Code\FableForge\generated\fable_engine.h"))
    rows = read_manifest(source)
    text = render(rows, source)
    changed = [str(path.resolve()) for path in outputs if write_if_changed(path.resolve(), text)]
    print(
        f"fable_engine_header functions={len(rows)} "
        f"complete={sum(row.get('prototype_complete') == '1' for row in rows)} "
        f"changed={len(changed)}"
    )
    for path in changed:
        print(f"  wrote {path}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
