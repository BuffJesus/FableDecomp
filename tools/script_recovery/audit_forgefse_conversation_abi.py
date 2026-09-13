#!/usr/bin/env python3
"""Verify that Forge preserves retail AddLineToConversation operand order."""
from __future__ import annotations

import argparse
import json
import re
from pathlib import Path


def audit(forge_root: Path, pdb_names: Path) -> dict:
    header = (forge_root / "FableScriptExtender" / "GameInterface.h").read_text(encoding="utf-8-sig")
    source = (forge_root / "FableScriptExtender" / "LuaQuestState.cpp").read_text(encoding="utf-8-sig")
    pdb = pdb_names.read_text(encoding="utf-8-sig")
    checks = {
        "pdbOrderIsIdKeyBoolSpeakerListener": bool(re.search(
            r"AddLineToConversation@CGameScriptInterface.*JABVCCharString@@_NABVCScriptThing@@2@Z", pdb)),
        "forgeNativeTypedefMatches": bool(re.search(
            r"tAddLineToConversation\)\(CGameScriptInterfaceBase\* This, int conversationID, "
            r"const CCharString\* pLine, bool \w+, const CScriptThing\* pSpeaker, "
            r"const CScriptThing\* pListener\)", header)),
        "luaBoolForwardedInNativeSlot": bool(re.search(
            r"AddLineToConversation_API\(m_pGameInterface, conversationID, fsKey, "
            r"bShowSubtitle, pSpeaker, pListener\);", source)),
    }
    return {"schema": "forgefse-conversation-abi-audit/0.1", "ok": all(checks.values()), "checks": checks}


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--forge-root", type=Path, required=True)
    parser.add_argument("--pdb-names", type=Path, required=True)
    args = parser.parse_args()
    result = audit(args.forge_root, args.pdb_names)
    print(json.dumps(result, indent=2))
    return 0 if result["ok"] else 1


if __name__ == "__main__":
    raise SystemExit(main())
