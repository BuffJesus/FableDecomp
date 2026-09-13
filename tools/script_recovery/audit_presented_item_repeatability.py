#!/usr/bin/env python3
"""Prove that retail and Forge preserve presented-item messages across immediate reads."""
from __future__ import annotations

import argparse
import json
from pathlib import Path


def audit(pdb_names: Path, native_c: Path, native_disassembly: Path,
          theresa_disassembly: Path, lua_script: Path, forge_source: Path) -> dict:
    pdb = pdb_names.read_text(encoding="utf-8-sig")
    native = native_c.read_text(encoding="utf-8-sig")
    native_asm = native_disassembly.read_text(encoding="utf-8-sig").lower()
    theresa = theresa_disassembly.read_text(encoding="utf-8-sig").lower()
    lua = lua_script.read_text(encoding="utf-8-sig")
    forge = forge_source.read_text(encoding="utf-8-sig")

    first = theresa.find("00dba499: ff908c000000")
    second = theresa.find("00dba4d1: ff928c000000")
    between_queries = theresa[theresa.find("\n", first) + 1:second] if 0 <= first < second else ""
    native_body = native.split(
        "CGameScriptThing::_MsgIsPresentedWithItem_CGameScriptThing__UBE_NAAVCCharString___Z", 1
    )[-1]
    forge_body = forge.split("bool LuaEntityAPI::MsgIsPresentedWithItem", 1)[-1].split("\n}", 1)[0]
    checks = {
        "pdbNamesConstScriptAndGameAccessors": all(name in pdb for name in (
            "?MsgIsPresentedWithItem@CScriptThing@@UBE_NAAVCCharString@@@Z",
            "?MsgIsPresentedWithItem@CGameScriptThing@@UBE_NAAVCCharString@@@Z")),
        "scriptThingDelegatesToImplementationSlot8c": all(x in native_asm for x in (
            "004aac60: 8b4904", "004aac6e: ffa08c000000")),
        "gameAccessorScansInterface42EventList": all(x in native_asm for x in (
            "008d2443: c744241c42000000", "008d2464: 8b38", "008d248f: 8b3f")),
        "gameAccessorAcceptsPresentationEventTypes10And11": all(x in native_asm for x in (
            "008d2487: 83e80a", "008d248c: 48", "008d24e2: b001", "008d252b: b001")),
        "gameAccessorOnlyAssignsCallerOutput": all(x in native_body for x in (
            "CThing::GetDefName", "CDefStringTable::GetString", "CCharString::operator=(param_1"))
            and not any(x in native_body.lower() for x in ("erase(", "remove(", "pop_", "clear(")),
        "theresaImmediatelyQueriesSlot8cTwice": 0 <= first < second,
        "noFrameBetweenTheresaQueries": "0x00f35b30" not in between_queries,
        "forgeCallsNativeOnEveryInvocation": "pVTable->MsgIsPresentedWithItem(pMe, &result)" in forge_body
            and "static " not in forge_body and "cache" not in forge_body.lower(),
        "forgePublishesReturnedNameWithoutConsumingEvent": all(x in forge_body for x in (
            'lua.set("g_PresentedItemName"', "CCharString_Destroy(&result)")),
        "luaFaithfullyRequeriesForNonChocolateBranch": lua.count("presented, item = presented_item(me)") == 2
            and "if presented and item ~= OBJECT_CHOCOLATE_BOX then" in lua,
    }
    return {"schema": "presented-item-repeatability-audit/0.1", "ok": all(checks.values()), "checks": checks}


def main() -> int:
    parser = argparse.ArgumentParser()
    for name in ("pdb_names", "native_c", "native_disassembly", "theresa_disassembly", "lua_script", "forge_source"):
        parser.add_argument("--" + name.replace("_", "-"), type=Path, required=True)
    args = parser.parse_args()
    result = audit(args.pdb_names, args.native_c, args.native_disassembly,
                   args.theresa_disassembly, args.lua_script, args.forge_source)
    print(json.dumps(result, indent=2))
    return 0 if result["ok"] else 1


if __name__ == "__main__":
    raise SystemExit(main())
