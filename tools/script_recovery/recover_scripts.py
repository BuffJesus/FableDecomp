#!/usr/bin/env python3
"""Ingest ForgeFSE Lua packages and emit provenance-aware generic script IR.

This deliberately performs lossless source preservation plus conservative lexical
analysis.  It is not a Lua decompiler/parser, and never upgrades inferred facts
to verified facts.
"""

from __future__ import annotations

import argparse
import hashlib
import json
import re
import shutil
import sys
import zipfile
from dataclasses import dataclass, field
from pathlib import Path, PurePosixPath
from typing import Any, Iterable


FUNCTION_RE = re.compile(r"^\s*function\s+([A-Za-z_][A-Za-z0-9_]*)\s*\(([^)]*)\)", re.M)
CALL_RE = re.compile(r"\b(quest|Quest|me)\s*:\s*([A-Za-z_][A-Za-z0-9_]*)\s*\(")
STRING_RE = re.compile(r"(['\"])(.*?)\1")
STATE_RE = re.compile(r"\b(?:quest|Quest):(?:Get|Set)State(Bool|Int|Float|String)\s*\(\s*(['\"])(.*?)\2")
PERSIST_RE = re.compile(r"\b(?:quest|Quest):PersistTransfer(Bool|Int|Float|String)\s*\(\s*[^,]+,\s*(['\"])(.*?)\2")
BINDING_RE = re.compile(r"\b(?:quest|Quest):AddEntityBinding\s*\(\s*(['\"])(.*?)\1\s*,\s*(['\"])(.*?)\3")
REGISTRY_START_RE = re.compile(r"\b([A-Za-z_][A-Za-z0-9_]*)\s*=\s*\{")


def sha256(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest().upper()


def classify(name: str, *, entity: bool = False, master: bool = False) -> str:
    if entity:
        return "entity"
    if master:
        return "master"
    if name.startswith("Expression_"):
        return "expression"
    if name.startswith("Global_"):
        return "global"
    if name.startswith("PersonalScript"):
        return "personal"
    if name.startswith("CS_"):
        return "cutscene_host"
    if name.startswith("V_"):
        return "village"
    if name.startswith("QR_"):
        return "repeatable_quest"
    if name.startswith(("Q_", "QS_")):
        return "quest"
    if name.startswith("Gameflow"):
        return "global"
    if "test" in name.lower() or name in {"HeroTurningDebugger", "ShowTargetedThingHealth"}:
        return "test"
    return "script"


def line_number(text: str, offset: int) -> int:
    return text.count("\n", 0, offset) + 1


def parse_calls(text: str) -> list[dict[str, Any]]:
    return [
        {
            "receiver": match.group(1),
            "name": match.group(2),
            "line": line_number(text, match.start()),
        }
        for match in CALL_RE.finditer(text)
    ]


def parse_lua_ir(path: str, text: str, kind: str) -> dict[str, Any]:
    functions = []
    matches = list(FUNCTION_RE.finditer(text))
    for index, match in enumerate(matches):
        end = matches[index + 1].start() if index + 1 < len(matches) else len(text)
        body = text[match.start():end]
        functions.append(
            {
                "name": match.group(1),
                "parameters": [x.strip() for x in match.group(2).split(",") if x.strip()],
                "line": line_number(text, match.start()),
                "calls": parse_calls(body),
            }
        )

    states: dict[str, dict[str, Any]] = {}
    for match in STATE_RE.finditer(text):
        state = states.setdefault(match.group(3), {"name": match.group(3), "types": [], "persisted": False})
        state["types"] = sorted(set(state["types"] + [match.group(1).lower()]))
    for match in PERSIST_RE.finditer(text):
        state = states.setdefault(match.group(3), {"name": match.group(3), "types": [], "persisted": False})
        state["types"] = sorted(set(state["types"] + [match.group(1).lower()]))
        state["persisted"] = True

    strings = [match.group(2) for match in STRING_RE.finditer(text)]
    return {
        "schema": "fable-script-ir/0.1",
        "sourcePath": path,
        "kind": kind,
        "evidenceLevel": "reconstructed-source",
        "functions": functions,
        "state": sorted(states.values(), key=lambda row: row["name"]),
        "entityBindings": [
            {"name": m.group(2), "script": m.group(4), "line": line_number(text, m.start())}
            for m in BINDING_RE.finditer(text)
        ],
        "references": {
            "textTags": sorted({x for x in strings if x.startswith("TEXT_")}),
            "cutscenes": sorted({x for x in strings if x.startswith("CS_")}),
            "regions": sorted({x for x in strings if not x.startswith(("TEXT_", "CS_")) and x in {
                "ArenaExterior", "BarrowFields", "BowerstoneSlums", "HauntedHouse",
                "HeroGuildComplexInside", "OakBay"
            }}),
        },
    }


def balanced_block(text: str, opening_brace: int) -> tuple[str, int]:
    depth = 0
    quote = None
    escaped = False
    for index in range(opening_brace, len(text)):
        char = text[index]
        if quote:
            if escaped:
                escaped = False
            elif char == "\\":
                escaped = True
            elif char == quote:
                quote = None
            continue
        if char in {'"', "'"}:
            quote = char
        elif char == "{":
            depth += 1
        elif char == "}":
            depth -= 1
            if depth == 0:
                return text[opening_brace + 1:index], index + 1
    raise ValueError(f"unbalanced Lua table beginning at offset {opening_brace}")


def parse_registry(text: str) -> list[dict[str, Any]]:
    records = []
    quests_match = re.search(r"\bQuests\s*=\s*\{", text)
    if not quests_match:
        return records
    outer_open = text.find("{", quests_match.start())
    outer, _ = balanced_block(text, outer_open)
    cursor = 0
    while True:
        match = REGISTRY_START_RE.search(outer, cursor)
        if not match:
            break
        key = match.group(1)
        body, end = balanced_block(outer, match.end() - 1)
        cursor = end
        name = re.search(r"\bname\s*=\s*(['\"])(.*?)\1", body)
        file_name = re.search(r"\bfile\s*=\s*(['\"])(.*?)\1", body)
        quest_id = re.search(r"\bid\s*=\s*(\d+)", body)
        if not (name and file_name and quest_id):
            continue
        entities = []
        for ent in re.finditer(
            r"\{\s*name\s*=\s*(['\"])(.*?)\1\s*,\s*file\s*=\s*(['\"])(.*?)\3\s*,\s*id\s*=\s*(\d+)",
            body,
        ):
            entities.append({"name": ent.group(2), "file": ent.group(4), "id": int(ent.group(5))})
        records.append(
            {
                "key": key,
                "name": name.group(2),
                "file": file_name.group(2),
                "id": int(quest_id.group(1)),
                "kind": classify(key, master=key == "FSE_Master"),
                "entities": entities,
            }
        )
    return records


def load_api_manifest(path: Path) -> dict[tuple[str, str], dict[str, Any]]:
    payload = json.loads(path.read_text(encoding="utf-8-sig"))
    return {(row["scope"].lower(), row["name"]): row for row in payload["functions"]}


def validate_script(ir: dict[str, Any], api: dict[tuple[str, str], dict[str, Any]]) -> list[dict[str, Any]]:
    findings: list[dict[str, Any]] = []
    acquired = released = 0
    timers = deregistered = 0
    for function in ir["functions"]:
        calls = function["calls"]
        for call in calls:
            scope = "entity" if call["receiver"] == "me" else "quest"
            name = call["name"]
            if (scope, name) not in api and name not in {"Log", "CreateThread", "FinalizeEntityBindings"}:
                other_scope = "quest" if scope == "entity" else "entity"
                if (other_scope, name) in api:
                    findings.append({
                        "severity": "error",
                        "code": "wrong-api-scope",
                        "line": call["line"],
                        "message": f"'{name}' is a {other_scope} API but is called on {call['receiver']}",
                    })
                else:
                    findings.append({
                        "severity": "error",
                        "code": "unknown-api",
                        "line": call["line"],
                        "message": f"{scope} API '{name}' is not present in the ForgeFSE manifest",
                    })
            acquired += name == "AcquireControl"
            released += name == "ReleaseControl"
            timers += name == "RegisterTimer"
            deregistered += name == "DeregisterTimer"
        body_calls = {row["name"] for row in calls}
        if "while" in function["name"].lower():
            pass
        if function["name"] in {"Main", "WatchForBooty"} and not ({"NewScriptFrame", "PlayCutscene", "RunCutscene"} & body_calls):
            findings.append({
                "severity": "warning", "code": "no-obvious-yield", "line": function["line"],
                "message": f"{function['name']} has no recognized yield/blocking operation",
            })
    if acquired > released:
        findings.append({"severity": "warning", "code": "control-balance", "line": 1,
                         "message": f"AcquireControl count {acquired} exceeds ReleaseControl count {released}"})
    if timers > deregistered:
        findings.append({"severity": "warning", "code": "timer-balance", "line": 1,
                         "message": f"RegisterTimer count {timers} exceeds DeregisterTimer count {deregistered}"})
    if any(row["name"] == "OnPersist" for row in ir["functions"]):
        unpersisted = [row["name"] for row in ir["state"] if not row["persisted"]]
        if unpersisted:
            findings.append({"severity": "warning", "code": "partial-persistence", "line": 1,
                             "message": "State referenced but not persisted: " + ", ".join(unpersisted)})
    return findings


@dataclass
class Package:
    archive: Path
    archive_hash: str
    files: dict[str, bytes] = field(default_factory=dict)
    registry: list[dict[str, Any]] = field(default_factory=list)


def read_package(path: Path) -> Package:
    raw = path.read_bytes()
    package = Package(path, sha256(raw))
    with zipfile.ZipFile(path) as zf:
        for info in zf.infolist():
            if info.is_dir():
                continue
            logical = PurePosixPath(info.filename)
            if logical.is_absolute() or ".." in logical.parts:
                raise ValueError(f"unsafe archive path in {path}: {info.filename}")
            package.files[logical.as_posix()] = zf.read(info)
    registry_bytes = package.files.get("FSE/quests.lua")
    if not registry_bytes:
        raise ValueError(f"{path} does not contain FSE/quests.lua")
    package.registry = parse_registry(registry_bytes.decode("utf-8-sig"))
    return package


def emit_combined_registry(records: Iterable[dict[str, Any]]) -> str:
    lines = ["-- Generated by tools/script_recovery/recover_scripts.py", "Quests = {", ""]
    for row in records:
        lines.extend([
            f"    {row['key']} = {{", f"        name = {json.dumps(row['name'])},",
            f"        file = {json.dumps(row['file'])},", f"        id = {row['id']},", "        entity_scripts = {",
        ])
        for ent in row["entities"]:
            lines.append(f"            {{ name = {json.dumps(ent['name'])}, file = {json.dumps(ent['file'])}, id = {ent['id']} }},")
        lines.extend(["        }", "    },", ""])
    lines.extend(["}", ""])
    return "\n".join(lines)


def ingest(archives: list[Path], output: Path, api_manifest: Path) -> dict[str, Any]:
    api = load_api_manifest(api_manifest)
    packages = [read_package(path.resolve()) for path in archives]
    output.mkdir(parents=True, exist_ok=True)
    sources = output / "sources"
    ir_dir = output / "ir"
    sources.mkdir(exist_ok=True)
    ir_dir.mkdir(exist_ok=True)
    all_records: list[dict[str, Any]] = []
    index: dict[str, Any] = {"schema": "fable-script-corpus/0.1", "packages": []}
    validation: list[dict[str, Any]] = []

    for package in packages:
        package_name = package.archive.stem
        package_root = sources / package_name
        package_root.mkdir(exist_ok=True)
        records = [row for row in package.registry if row["key"] != "FSE_Master"]
        all_records.extend(records)
        file_rows = []
        for logical, data in sorted(package.files.items()):
            destination = package_root.joinpath(*PurePosixPath(logical).parts)
            destination.parent.mkdir(parents=True, exist_ok=True)
            destination.write_bytes(data)
            row: dict[str, Any] = {"path": logical, "sha256": sha256(data), "size": len(data)}
            if logical.endswith(".lua") and logical != "FSE/quests.lua":
                text = data.decode("utf-8-sig")
                entity = "/Entities/" in logical
                record = next((x for x in records if logical == f"FSE/{x['file']}.lua"), None)
                kind = classify(record["key"] if record else Path(logical).stem, entity=entity)
                ir = parse_lua_ir(logical, text, kind)
                findings = validate_script(ir, api)
                ir_path = ir_dir / package_name / (logical.replace("/", "__") + ".json")
                ir_path.parent.mkdir(parents=True, exist_ok=True)
                ir_path.write_text(json.dumps(ir, indent=2) + "\n", encoding="utf-8")
                row["ir"] = ir_path.relative_to(output).as_posix()
                row["kind"] = kind
                validation.extend({"package": package_name, "path": logical, **finding} for finding in findings)
            file_rows.append(row)
        index["packages"].append({
            "name": package_name,
            "sourceArchive": str(package.archive),
            "archiveSha256": package.archive_hash,
            "evidenceLevel": "reconstructed-source",
            "registry": records,
            "files": file_rows,
        })

    keys: dict[str, str] = {}
    ids: dict[int, str] = {}
    entity_ids: dict[int, str] = {}
    for row in all_records:
        if row["key"] in keys:
            validation.append({"severity": "error", "code": "duplicate-script-key", "path": "combined_registry.lua",
                               "message": f"Duplicate key {row['key']}"})
        keys[row["key"]] = row["name"]
        if row["id"] in ids:
            validation.append({"severity": "error", "code": "duplicate-script-id", "path": "combined_registry.lua",
                               "message": f"Duplicate script id {row['id']}"})
        ids[row["id"]] = row["key"]
        for ent in row["entities"]:
            if ent["id"] in entity_ids:
                validation.append({"severity": "error", "code": "duplicate-entity-id", "path": "combined_registry.lua",
                                   "message": f"Duplicate entity id {ent['id']}"})
            entity_ids[ent["id"]] = ent["name"]

    (output / "corpus_index.json").write_text(json.dumps(index, indent=2) + "\n", encoding="utf-8")
    (output / "validation.json").write_text(json.dumps(validation, indent=2) + "\n", encoding="utf-8")
    (output / "combined_registry.lua").write_text(emit_combined_registry(all_records), encoding="utf-8")
    return {"packages": len(packages), "scripts": len(all_records), "findings": len(validation),
            "errors": sum(row["severity"] == "error" for row in validation)}


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("archives", nargs="+", type=Path)
    parser.add_argument("--output", type=Path, required=True)
    parser.add_argument("--api-manifest", type=Path, default=Path("refs/fse_api_manifest.json"))
    args = parser.parse_args()
    missing = [str(path) for path in args.archives if not path.is_file()]
    if missing:
        parser.error("missing archive(s): " + ", ".join(missing))
    result = ingest(args.archives, args.output.resolve(), args.api_manifest.resolve())
    print(json.dumps(result, sort_keys=True))
    return 1 if result["errors"] else 0


if __name__ == "__main__":
    sys.exit(main())
