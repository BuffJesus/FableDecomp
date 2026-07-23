#!/usr/bin/env python3
"""Recover conservative function prototypes from intact VC7.1 decorated names."""
from __future__ import annotations

import argparse
import csv
import json
import re
import subprocess
from pathlib import Path


CC_RE = re.compile(r"__(thiscall|cdecl|stdcall|fastcall)\b")
ACCESS_RE = re.compile(r"^(?:public|protected|private):\s+")
STORAGE_RE = re.compile(r"\b(?:virtual|static)\b")


def read_tsv(path: Path) -> list[dict[str, str]]:
    with path.open("r", encoding="utf-8-sig", newline="") as stream:
        return list(csv.DictReader(stream, delimiter="\t"))


def read_engine_api(path: Path) -> list[dict[str, str]]:
    rows: list[dict[str, str]] = []
    with path.open("r", encoding="utf-8-sig", errors="replace") as stream:
        for line in stream:
            if not line.strip() or line.startswith("#"):
                continue
            parts = line.rstrip("\n").split("\t")
            if len(parts) < 7:
                continue
            rows.append(
                {
                    "address": parts[0].lower(),
                    "calling_convention": parts[1],
                    "name": parts[6],
                }
            )
    return rows


def write_tsv(path: Path, rows: list[dict[str, str]]) -> None:
    fields = [
        "address", "name", "module", "calling_convention", "return_type",
        "parameter_count", "parameter_types", "evidence",
    ]
    temp = path.with_suffix(path.suffix + ".tmp")
    with temp.open("w", encoding="utf-8", newline="") as stream:
        writer = csv.DictWriter(stream, delimiter="\t", fieldnames=fields, lineterminator="\n")
        writer.writeheader()
        writer.writerows(rows)
    if path.exists() and path.read_bytes() == temp.read_bytes():
        temp.unlink()
        return
    temp.replace(path)


def undecorate_many(tool: Path, names: list[str], batch_size: int = 32) -> dict[str, str]:
    decoded: dict[str, str] = {}
    pattern = re.compile(r'Undecoration of :- "([\s\S]*?)"\r?\nis :- "([\s\S]*?)"')
    for start in range(0, len(names), batch_size):
        batch = names[start : start + batch_size]
        process = subprocess.run(
            [str(tool), *batch], capture_output=True, text=True,
            encoding="mbcs", errors="replace", check=False,
        )
        for decorated, result in pattern.findall(process.stdout):
            decoded[decorated] = result.strip()
    return decoded


def split_parameters(payload: str) -> list[str] | None:
    payload = payload.strip()
    if payload in ("", "void"):
        return []
    result: list[str] = []
    begin = 0
    angle = square = paren = 0
    for index, char in enumerate(payload):
        if char == "<":
            angle += 1
        elif char == ">":
            angle = max(0, angle - 1)
        elif char == "[":
            square += 1
        elif char == "]":
            square = max(0, square - 1)
        elif char == "(":
            paren += 1
        elif char == ")":
            paren = max(0, paren - 1)
        elif char == "," and angle == square == paren == 0:
            result.append(payload[begin:index].strip())
            begin = index + 1
    result.append(payload[begin:].strip())
    if any(not item for item in result):
        return None
    return result


def normalize_type(value: str) -> str:
    value = re.sub(r"\b(?:class|struct|enum|union)\s+", "", value)
    value = value.replace("unsigned long", "ulong")
    value = value.replace("unsigned int", "uint")
    value = value.replace("unsigned short", "ushort")
    value = value.replace("unsigned char", "uchar")
    value = re.sub(r"\s+", " ", value).strip()
    return value


def parse_signature(result: str) -> tuple[str, str, str, list[str]] | None:
    if "`" in result or "operator" in result or "[thunk]" in result:
        return None
    match = CC_RE.search(result)
    if match is None:
        return None
    calling_convention = "__" + match.group(1)
    prefix = ACCESS_RE.sub("", result[: match.start()].strip())
    prefix = re.sub(r"\s+", " ", STORAGE_RE.sub("", prefix)).strip()
    suffix = result[match.end() :].strip()
    open_index = suffix.find("(")
    if open_index <= 0:
        return None
    qualified_name = suffix[:open_index].strip()
    if " " in qualified_name or "::" not in qualified_name and calling_convention == "__thiscall":
        return None
    depth = 0
    close_index = -1
    for index in range(open_index, len(suffix)):
        if suffix[index] == "(":
            depth += 1
        elif suffix[index] == ")":
            depth -= 1
            if depth == 0:
                close_index = index
                break
    if close_index < 0:
        return None
    trailing = suffix[close_index + 1 :].strip()
    if trailing not in ("", "const", "volatile", "const volatile"):
        return None
    explicit = split_parameters(suffix[open_index + 1 : close_index])
    if explicit is None:
        return None
    if any("(" in item or ")" in item for item in explicit):
        return None
    owner = qualified_name.rsplit("::", 1)[0] if "::" in qualified_name else ""
    leaf = qualified_name.rsplit("::", 1)[-1]
    owner_leaf = owner.rsplit("::", 1)[-1]
    constructor_or_destructor = leaf in (owner_leaf, "~" + owner_leaf)
    if not prefix and not constructor_or_destructor:
        return None
    return_type = "void" if constructor_or_destructor else normalize_type(prefix)
    if not return_type:
        return None
    parameters = [normalize_type(item) for item in explicit]
    if calling_convention == "__thiscall":
        if not owner:
            return None
        parameters.insert(0, normalize_type(owner) + " *")
    return qualified_name, calling_convention, return_type, parameters


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--root", type=Path, default=Path(__file__).resolve().parents[1])
    parser.add_argument("--undname", type=Path, default=Path(r"D:\Tools\vc71\bin\undname.exe"))
    parser.add_argument("--out", type=Path)
    args = parser.parse_args()
    root = args.root.resolve()
    manifest = read_tsv(root / "rebuild" / "manifest" / "functions.tsv")
    modules = {row["address"].lower(): row["module"] for row in manifest}
    engine_api = read_engine_api(root / "ghidra_out" / "engine_api.tsv")
    manual_path = root / "rebuild" / "corrections" / "function_overrides.tsv"
    manual = {row["address"].lower() for row in read_tsv(manual_path)}
    candidates = [
        {**row, "module": modules.get(row["address"].lower(), "_global")}
        for row in engine_api
        if row["address"].lower() not in manual
        and row["calling_convention"] == "unknown"
        and row["name"].startswith("?")
    ]
    decoded = undecorate_many(args.undname, [row["name"] for row in candidates])
    rows: list[dict[str, str]] = []
    rejected: list[dict[str, str]] = []
    for item in candidates:
        human = decoded.get(item["name"], "")
        parsed = parse_signature(human) if human else None
        if parsed is None:
            rejected.append({"address": item["address"], "name": item["name"], "undecorated": human})
            continue
        name, cc, return_type, parameters = parsed
        rows.append(
            {
                "address": item["address"].lower(),
                "name": name,
                "module": item["module"],
                "calling_convention": cc,
                "return_type": return_type,
                "parameter_count": str(len(parameters)),
                "parameter_types": ",".join(parameters),
                "evidence": f"VC7.1 undname: {human}",
            }
        )
    rows.sort(key=lambda row: row["address"])
    output = (args.out or root / "rebuild" / "corrections" / "msvc_prototype_overrides.tsv").resolve()
    write_tsv(output, rows)
    report = {
        "decorated_unknown_candidates": len(candidates),
        "undecorated": len(decoded),
        "accepted": len(rows),
        "rejected": len(rejected),
        "output": str(output),
        "rejected_examples": rejected[:20],
    }
    report_path = output.with_suffix(".json")
    report_path.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(json.dumps(report, indent=2))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
