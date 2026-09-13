#!/usr/bin/env python3
"""Prove the retail logbook helper semantics and Forge entry-point wiring."""
from __future__ import annotations

import argparse
import json
from pathlib import Path


def _section(text: str, address: str) -> str:
    marker = f"@ {address.lower()}"
    start = text.lower().find(marker)
    if start < 0:
        return ""
    end = text.find("//===", start + len(marker))
    return text[start:] if end < 0 else text[start:end]


def audit(disassembly: Path, forge_header: Path, forge_api: Path,
          forge_quest: Path, deeds_lua: Path) -> dict:
    disasm = disassembly.read_text(encoding="utf-8-sig")
    numeric = _section(disasm, "00cbe87f")
    story_key = _section(disasm, "00cbe960")
    tutorial = _section(disasm, "00cbe9ee")
    header = forge_header.read_text(encoding="utf-8-sig")
    api = forge_api.read_text(encoding="utf-8-sig")
    quest = forge_quest.read_text(encoding="utf-8-sig")
    deeds = deeds_lua.read_text(encoding="utf-8-sig")
    checks = {
        "numericHelperConsumesIdFromEcx": "MOV EDX,ECX" in numeric,
        "numericHelperBuildsStoryKey": "s_TEXT_QST_LOG_STORY_" in numeric,
        "numericHelperSubmitsCategoryOne": "PUSH 0x1" in numeric and "+ 0x4d0" in numeric,
        "storyStringHelperUsesNameAndDesc": all(x in story_key for x in ("s__NAME_", "s__DESC_", "PUSH 0x1")),
        "tutorialHelperConsumesKeyFromEcx": "MOV ESI,ECX" in tutorial,
        "tutorialHelperBuildsTitleAndCategoryTwo": all(x in tutorial for x in ("s__TITLE_", "PUSH 0x2", "+ 0x4d0")),
        "tutorialHelperPerformsTrailingYield": "+ 0x1c" in tutorial,
        "forgeDeclaresDedicatedTutorialThunk": all(x in header for x in (
            "tAddLogbookTutorialEntry", "AddLogbookTutorialEntry_Func")),
        "forgeWiresCorrectDistinctAddresses": all(x in api for x in (
            "AddLogbookStoryEntry_Func = ASLR<tAddLogbookStoryEntry>(0xCBE87F)",
            "AddLogbookStoryEntryString_Func = ASLR<tAddLogbookStoryEntryString>(0xCBE960)",
            "AddLogbookTutorialEntry_Func = ASLR<tAddLogbookTutorialEntry>(0xCBE9EE)")),
        "forgeTutorialWrapperCallsNativeThunk": "AddLogbookTutorialEntry_Func(fsKey.get())" in quest,
        "newOakvaleUsesTutorialBinding": "quest:AddLogbookTutorialEntry(Deeds.TEXT_LOG_BASICS)" in deeds,
    }
    return {"schema": "logbook-helper-audit/0.1", "ok": all(checks.values()), "checks": checks}


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--disassembly", type=Path, required=True)
    parser.add_argument("--forge-header", type=Path, required=True)
    parser.add_argument("--forge-api", type=Path, required=True)
    parser.add_argument("--forge-quest", type=Path, required=True)
    parser.add_argument("--deeds-lua", type=Path, required=True)
    args = parser.parse_args()
    result = audit(args.disassembly, args.forge_header, args.forge_api,
                   args.forge_quest, args.deeds_lua)
    print(json.dumps(result, indent=2))
    return 0 if result["ok"] else 1


if __name__ == "__main__":
    raise SystemExit(main())
