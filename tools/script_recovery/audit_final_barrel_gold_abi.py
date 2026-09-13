#!/usr/bin/env python3
"""Certify the final-barrel gold path from retail dispatch through Forge and Lua."""
from __future__ import annotations

import hashlib
import json
import re
import struct
from pathlib import Path


VTABLE_BASE = 0x01260F0C
SLOT = 0x924
TARGET = 0x0089E780
EXE_SHA256 = "41DC91090AE853715AC06D2E9FC96E5D545381D197ED55D624C642F34509AC10"


def _read_u32_at_va(exe: Path, va: int) -> int | None:
    data = exe.read_bytes()
    pe = struct.unpack_from("<I", data, 0x3C)[0]
    section_count = struct.unpack_from("<H", data, pe + 6)[0]
    optional_size = struct.unpack_from("<H", data, pe + 20)[0]
    image_base = struct.unpack_from("<I", data, pe + 24 + 28)[0]
    rva = va - image_base
    for index in range(section_count):
        offset = pe + 24 + optional_size + index * 40
        virtual_size, section_rva, raw_size, raw_offset = struct.unpack_from("<IIII", data, offset + 8)
        if section_rva <= rva < section_rva + max(virtual_size, raw_size):
            raw = raw_offset + rva - section_rva
            return struct.unpack_from("<I", data, raw)[0]
    return None


def audit(workspace: Path, forge: Path = Path(r"D:\Code\ForgeFSE-retail-shadow"),
          exe: Path = Path(r"C:\Programs\Steam\steamapps\common\Fable The Lost Chapters\Fable.exe")) -> dict:
    errors: list[str] = []
    retail = json.loads((workspace / "ghidra_out/script_recovery/new_oakvale_watch_barrels_retail_bytes.json").read_text(encoding="utf-8-sig"))
    lua = (workspace / "refs/script_recovery/reconstructed/NewOakValeIntro/FSE/NewOakValeIntro/NewOakValeIntro.lua").read_text(encoding="utf-8")
    catalog = (workspace / "ghidra_out/gamescriptinterface_catalog.tsv").read_text(encoding="utf-8-sig")
    engine_api = (workspace / "ghidra_out/engine_api.tsv").read_text(encoding="utf-8-sig")
    header = (forge / "FableScriptExtender/GameInterface.h").read_text(encoding="utf-8-sig")
    bindings = (forge / "FableScriptExtender/GameInterface.cpp").read_text(encoding="utf-8-sig")
    wrapper = (forge / "FableScriptExtender/LuaQuestState.cpp").read_text(encoding="utf-8-sig")

    calls = {(entry["site"], entry["slot"]) for entry in retail.get("indirectCalls", [])}
    if ("0x00DBEA2F", "0x924") not in calls or retail.get("goldBreakIndex") != "total-1":
        errors.append("retail WatchBarrels no longer proves the total-1 slot-0x924 dispatch")
    if not re.search(r"^0089e780\tAddItemToContainer$", catalog, re.MULTILINE):
        errors.append("retail target catalog does not name 0x0089E780 AddItemToContainer")
    signature = "?AddItemToContainer@CGameScriptInterface@@UBEXABVCScriptThing@@ABVCCharString@@@Z"
    if not any(line.startswith("0089e780\t") and signature in line for line in engine_api.splitlines()):
        errors.append("PDB-derived signature does not prove container-before-item operands")
    if "typedef void(__thiscall* tAddItemToContainer)(CGameScriptInterfaceBase* This, const CScriptThing* pContainer, const CCharString* pItemDefName);" not in header:
        errors.append("Forge AddItemToContainer typedef differs from the retail ABI")
    if "AddItemToContainer_API = (tAddItemToContainer)pVTable[585]; // 0x924" not in bindings:
        errors.append("Forge does not bind AddItemToContainer to slot 585/0x924")
    wrapper_order = re.compile(r"CScriptThing\* pContainer = spContainer\.get\(\);.*?FableString fsItem\(itemDefName\.c_str\(\)\);.*?AddItemToContainer_API\(m_pGameInterface, pContainer, fsItem\.get\(\)\);", re.DOTALL)
    if not wrapper_order.search(wrapper):
        errors.append("Forge wrapper does not marshal container then CCharString item")
    body = lua[lua.index("function WatchBarrels("):lua.index("function WatchForGotGold(")]
    lua_fragments = ["broken == total - 1", "local last_barrel = quest:GetThingWithScriptName(NOVI.SCRIPT_BARREL)",
                     "quest:AddItemToContainer(last_barrel, NOVI.OBJECT_GOLD_1)",
                     "NOVI_PROBE WatchBarrels GOLD_INSERT_RETURNED item=", "NOVI.OBJECT_GOLD_1"]
    cursor = 0
    for fragment in lua_fragments:
        found = body.find(fragment, cursor)
        if found < 0:
            errors.append(f"Lua final-gold path missing or out of order: {fragment}")
            break
        cursor = found + len(fragment)

    slot_target = None
    exe_hash = None
    if exe.exists():
        exe_hash = hashlib.sha256(exe.read_bytes()).hexdigest().upper()
        if exe_hash != EXE_SHA256:
            errors.append(f"installed executable hash differs: {exe_hash}")
        else:
            slot_target = _read_u32_at_va(exe, VTABLE_BASE + SLOT)
            if slot_target != TARGET:
                errors.append(f"retail vtable slot 0x924 resolves to {slot_target!r}, not 0x0089E780")

    return {
        "schema": "new-oakvale-final-barrel-gold-abi-audit/0.1",
        "retailCallSite": "0x00DBEA2F", "vtableSlot": "0x924", "vtableIndex": 585,
        "nativeTarget": f"0x{TARGET:08X}", "installedSlotTarget": None if slot_target is None else f"0x{slot_target:08X}",
        "installedExeSha256": exe_hash, "errors": errors, "ok": not errors,
    }


if __name__ == "__main__":
    print(json.dumps(audit(Path(__file__).resolve().parents[2]), indent=2))
