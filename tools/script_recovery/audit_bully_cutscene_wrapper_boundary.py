#!/usr/bin/env python3
"""Lock retail cutscene marshalling and prevent replaying movement after a skip."""
from __future__ import annotations

import argparse
import json
from pathlib import Path

try:
    from tools.script_recovery.audit_bully_skip_runtime import audit as audit_skip_capture
except ModuleNotFoundError:
    from audit_bully_skip_runtime import audit as audit_skip_capture


def _ordered(text: str, fragments: tuple[str, ...]) -> bool:
    cursor = 0
    for fragment in fragments:
        cursor = text.find(fragment, cursor)
        if cursor < 0:
            return False
        cursor += len(fragment)
    return True


def audit(workspace: Path, forge_root: Path) -> dict:
    disasm = (workspace / "ghidra_out/script_recovery/bully_main_disasm.txt").read_text(encoding="utf-8-sig").lower()
    interpreter = (workspace / "ghidra_out/runcutscenemacro_full.c").read_text(encoding="utf-8-sig")
    evidence = workspace / "refs/script_recovery/new_oakvale_intro/runtime_evidence"
    skip_capture = audit_skip_capture(evidence / "interactive-20260912-v20-unskipped-success.log",
                                     evidence / "interactive-20260912-v20-early-skip-complete.log",
                                     evidence / "bully-installed-macros-20260912.json")
    lua = (workspace / "refs/script_recovery/reconstructed/NewOakValeIntro/FSE/NewOakValeIntro/Entities/NOVI_Bully.lua").read_text(encoding="utf-8-sig")
    forge = forge_root / "FableScriptExtender"
    header = (forge / "FableAPI.h").read_text(encoding="utf-8-sig")
    quest = (forge / "LuaQuestState.cpp").read_text(encoding="utf-8-sig")
    run_off = lua.split("local function run_off(", 1)[1].split("\nend", 1)[0]

    checks = {
        "retailBullyUsesFastcallRegisterAndStackLayout": _ordered(disasm, (
            "00dbcb77: 6a01", "00dbcb79: 6a00", "00dbcb7b:", "push eax",
            "00dbcb80: 6a00", "00dbcb82:", "lea edx", "00dbcb86:", "lea ecx",
            "00dbcb8a:", "call 0x00cbfb7d")),
        "forgeDeclaresExactFastcallShape": (
            "typedef void(__fastcall* tRunCutsceneMacro)(const CCharString* macro_name, void* entitymap, void* flagmap, void* input_args, bool setupcond, bool skippable);"
            in header),
        "forgeCallsNativeInExactSemanticOrder": (
            "RunCutsceneMacro_Func(cutsceneNameStr, pMapBuffer, pFlagsBuffer, pInputArgsBuffer, bSetupCond, bSkippable);"
            in quest),
        "forgeBorrowsCallerMovie": all(fragment in quest for fragment in (
            "const bool ownsMovieSequence =",
            "callerMovie == m_movieHandlesByLuaState.end() || callerMovie->second == nullptr;",
            'Log("    [RunCutsceneWithSetup] Reusing caller-owned movie sequence.");')),
        "forgeOnlyDestroysOwnedMovie": _ordered(quest, (
            "if (ownsMovieSequence) {",
            "MovieResource_Destroy_API(reinterpret_cast<CScriptGameResourceObjectMovieBase*>(pMovieObject));")),
        "nativeInterpreterRecognizesRunToAndPollsTasks": all(fragment in interpreter for fragment in (
            'CCharString::CCharString(local_4b8,".RunTo",-1);',
            "_IsPerformingScriptTask_CScriptGameResourceObjectScriptedThingBase__UBE_NXZ",
            "IsCutsceneSkipped()")),
        "capturedEarlySkipExplainsNoNativeRunToAndFallback": skip_capture["ok"],
        "luaLeavesMacroMovementAndVisibilityToRetail": all(fragment not in run_off for fragment in (
            ":MoveToPosition(", ":MoveToThing(", ":EntitySetAsDrawable(")),
    }
    return {"schema": "bully-cutscene-wrapper-boundary-audit/0.2", "ok": all(checks.values()), "checks": checks}


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--workspace", type=Path, default=Path(__file__).resolve().parents[2])
    parser.add_argument("--forge-root", type=Path, required=True)
    args = parser.parse_args()
    result = audit(args.workspace.resolve(), args.forge_root.resolve())
    print(json.dumps(result, indent=2))
    return 0 if result["ok"] else 1


if __name__ == "__main__":
    raise SystemExit(main())
