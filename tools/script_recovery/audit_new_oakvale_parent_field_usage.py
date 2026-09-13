#!/usr/bin/env python3
"""Cross-check entity Lua state access against declared New Oakvale parent fields."""
from __future__ import annotations

import json
import re
from pathlib import Path

try:
    from tools.script_recovery.validate_reconstructed_package import strip_lua_comments
except ModuleNotFoundError:
    from validate_reconstructed_package import strip_lua_comments

READ_RE = re.compile(r"\bF\.(?:get|get_int)\s*\(\s*quest\s*,\s*F\.([A-Za-z_][A-Za-z0-9_]*)")
WRITE_RE = re.compile(r"\bF\.set\s*\(\s*quest\s*,\s*F\.([A-Za-z_][A-Za-z0-9_]*)")
INCREMENT_RE = re.compile(r"\bF\.increment\s*\(\s*quest\s*,\s*F\.([A-Za-z_][A-Za-z0-9_]*)")
MASTER_READ_RE = re.compile(r"\bF\.get_master\s*\(\s*quest\s*,\s*F\.master\.([A-Za-z_][A-Za-z0-9_]*)")
MASTER_WRITE_RE = re.compile(r"\bF\.set_master\s*\(\s*quest\s*,\s*F\.master\.([A-Za-z_][A-Za-z0-9_]*)")
MASTER_WRITE_DETAIL_RE = re.compile(
    r'\bF\.set_master\s*\(\s*quest\s*,\s*F\.master\.([A-Za-z_][A-Za-z0-9_]*)\s*,\s*["\']([^"\']*)["\']'
)
FIELD_RE = re.compile(r"^F\.([A-Za-z_][A-Za-z0-9_]*)\s*=\s*(?:key\(|\{)", re.MULTILINE)
MASTER_FIELD_RE = re.compile(r'^\s+([A-Za-z_][A-Za-z0-9_]*)\s*=\s*\{\s*name\s*=\s*"[^"]+"', re.MULTILINE)
GOOD_DEED_RE = re.compile(r"\bDeeds\.add_good\s*\(")
BAD_DEED_RE = re.compile(r"\bDeeds\.add_bad\s*\(")
STATIC_PARENT_DATA = {
    f"VillagerSpeech{sex}{reputation}"
    for sex in ("Male", "Female")
    for reputation in ("Good", "Bad", "Both", "Neither")
}


def declared_bases(values: list[str]) -> set[str]:
    """Reduce annotated declarations and array ranges to their canonical base names."""
    result = set()
    for value in values:
        for component in value.split("/"):
            match = re.match(r"([A-Za-z_][A-Za-z0-9_]*)", component.strip())
            if match:
                result.add(match.group(1))
    return result


def audit(entities_dir: Path, lua_entities_dir: Path, fields_file: Path) -> dict:
    errors: list[str] = []
    fields_source = strip_lua_comments(fields_file.read_text(encoding="utf-8-sig"))
    canonical = set(FIELD_RE.findall(fields_source))
    canonical.discard("master")  # namespace for separate CQ_SunnyvaleMasterData keys
    canonical_master = set(MASTER_FIELD_RE.findall(fields_source))
    direct_reads: set[str] = set()
    direct_writes: set[str] = set()
    master_reads: set[str] = set()
    master_writes: set[str] = set()
    master_write_sites = 0
    for inventory in sorted(entities_dir.glob("*.json")):
        data = json.loads(inventory.read_text(encoding="utf-8-sig"))
        if data.get("kind") == "quest":
            continue
        lua_file = lua_entities_dir / f"{inventory.stem}.lua"
        source = strip_lua_comments(lua_file.read_text(encoding="utf-8-sig"))
        increments = set(INCREMENT_RE.findall(source))
        reads = set(READ_RE.findall(source)) | increments
        writes = set(WRITE_RE.findall(source)) | increments
        direct_reads |= reads
        direct_writes |= writes
        declared_reads = declared_bases(data.get("parentFieldsRead", []))
        declared_writes = declared_bases(data.get("parentFieldsWritten", []))
        lua_master_reads = set(MASTER_READ_RE.findall(source))
        lua_master_writes = set(MASTER_WRITE_RE.findall(source))
        lua_master_details = sorted(MASTER_WRITE_DETAIL_RE.findall(source))
        declared_master_reads = {
            row.get("field") for row in data.get("masterDataAccess", []) if row.get("op") == "read"
        }
        declared_master_writes = {
            row.get("field") for row in data.get("masterDataAccess", []) if row.get("op") == "write"
        }
        declared_master_details = sorted(
            (row.get("field"), str(row.get("value")))
            for row in data.get("masterDataAccess", []) if row.get("op") == "write"
        )
        master_reads |= lua_master_reads
        master_writes |= lua_master_writes
        master_write_sites += len(lua_master_details)
        for field in sorted(reads - declared_reads):
            errors.append(f"{inventory.name}: undeclared direct read F.{field}")
        for field in sorted(writes - declared_writes):
            errors.append(f"{inventory.name}: undeclared direct write F.{field}")
        for field in sorted((reads | writes) - canonical):
            errors.append(f"{inventory.name}: direct access to undefined F.{field}")
        for field in sorted((declared_reads | declared_writes) - canonical - STATIC_PARENT_DATA):
            errors.append(f"{inventory.name}: declaration names unknown parent field/data {field}")
        required_indirect_writes = set()
        if GOOD_DEED_RE.search(source):
            required_indirect_writes.add("GoodDeedsPerformed")
        if BAD_DEED_RE.search(source):
            required_indirect_writes.update({"BadDeedsPerformed", "WhichBadDeedsPerformed"})
        for field in sorted(required_indirect_writes - declared_writes):
            errors.append(f"{inventory.name}: undeclared deed-helper write {field}")
        if lua_master_reads != declared_master_reads:
            errors.append(f"{inventory.name}: master-data read declaration differs from Lua")
        if lua_master_writes != declared_master_writes:
            errors.append(f"{inventory.name}: master-data write declaration differs from Lua")
        if lua_master_details != declared_master_details:
            errors.append(f"{inventory.name}: master-data write values/sites differ from Lua")
        for field in sorted((lua_master_reads | lua_master_writes) - canonical_master):
            errors.append(f"{inventory.name}: access to undefined F.master.{field}")

    package_reads: set[str] = set()
    package_writes: set[str] = set()
    for lua_file in sorted(lua_entities_dir.parent.rglob("*.lua")):
        source = strip_lua_comments(lua_file.read_text(encoding="utf-8-sig"))
        increments = set(INCREMENT_RE.findall(source))
        reads = set(READ_RE.findall(source)) | increments
        writes = set(WRITE_RE.findall(source)) | increments
        package_reads |= reads
        package_writes |= writes
        for field in sorted((reads | writes) - canonical):
            errors.append(f"{lua_file.name}: package access to undefined F.{field}")
    return {
        "schema": "new-oakvale-parent-field-usage-audit/0.1",
        "canonicalFields": len(canonical),
        "recognizedStaticParentData": len(STATIC_PARENT_DATA),
        "canonicalMasterFields": sorted(canonical_master),
        "masterReadFields": sorted(master_reads),
        "masterWriteFields": sorted(master_writes),
        "masterWriteSites": master_write_sites,
        "directReadFields": sorted(direct_reads),
        "directWriteFields": sorted(direct_writes),
        "packageReadFields": sorted(package_reads),
        "packageWriteFields": sorted(package_writes),
        "errors": errors,
        "ok": not errors,
    }


if __name__ == "__main__":
    root = Path(__file__).resolve().parents[2]
    package = root / "refs/script_recovery/reconstructed/NewOakValeIntro/FSE/NewOakValeIntro"
    print(json.dumps(audit(
        root / "refs/script_recovery/new_oakvale_intro/entities",
        package / "Entities", package / "fields.lua"), indent=2))
