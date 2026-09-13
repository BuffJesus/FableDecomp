#!/usr/bin/env python3
"""Audit the retail timer ABI, Forge forwarding, and observed countdown semantics."""
from __future__ import annotations

import argparse
import json
import re
from pathlib import Path


def audit(forge_root: Path, pdb_names: Path, slots: Path,
          native_decomp: Path, runtime_log: Path, world_update_decomp: Path) -> dict:
    folder = forge_root / "FableScriptExtender"
    header = (folder / "GameInterface.h").read_text(encoding="utf-8-sig")
    interface = (folder / "GameInterface.cpp").read_text(encoding="utf-8-sig")
    lua = (folder / "LuaQuestState.cpp").read_text(encoding="utf-8-sig")
    pdb = pdb_names.read_text(encoding="utf-8-sig")
    slot_text = slots.read_text(encoding="utf-8-sig")
    native = native_decomp.read_text(encoding="utf-8-sig")
    runtime = runtime_log.read_text(encoding="utf-8-sig")
    world_update = world_update_decomp.read_text(encoding="utf-8-sig")
    values = [int(value) for value in re.findall(
        r"NOVI_PROBE StartBarrelTimer update bar=\d+ value=(-?\d+)", runtime)]
    expected = list(range(45, -1, -1))

    checks = {
        "pdbSignaturesExact": all(name in pdb for name in (
            "?RegisterTimer@CGameScriptInterface@@UAEJXZ",
            "?DeregisterTimer@CGameScriptInterface@@UAEXJ@Z",
            "?SetTimer@CGameScriptInterface@@UAEXJJ@Z",
            "?GetTimer@CGameScriptInterface@@UBEJJ@Z",
        )),
        "retailSlots87Through90": all(pattern in slot_text for pattern in (
            "0x15c\t01261068\t006e79e0\t?RegisterTimer",
            "0x160\t0126106c\t006e7690\t?DeregisterTimer",
            "0x164\t01261070\t006e76e0\t?SetTimer",
            "0x168\t01261074\t006e7710\t?GetTimer",
        )),
        "forgeTypedefsExact": all(line in header for line in (
            "typedef int(__thiscall* tRegisterTimer)(CGameScriptInterfaceBase* This);",
            "typedef void(__thiscall* tDeregisterTimer)(CGameScriptInterfaceBase* This, int timerId);",
            "typedef void(__thiscall* tSetTimer)(CGameScriptInterfaceBase* This, int timerId, int time);",
            "typedef int(__thiscall* tGetTimer)(CGameScriptInterfaceBase* This, int timerId);",
        )),
        "forgeSlotsExact": all(line in interface for line in (
            "RegisterTimer_API = (tRegisterTimer)pVTable[87];",
            "DeregisterTimer_API = (tDeregisterTimer)pVTable[88];",
            "SetTimer_API = (tSetTimer)pVTable[89];",
            "GetTimer_API = (tGetTimer)pVTable[90];",
        )),
        "forgeForwardsOperandsUnchanged": all(line in lua for line in (
            "RegisterTimer_API(m_pGameInterface)",
            "DeregisterTimer_API(m_pGameInterface, timerId);",
            "SetTimer_API(m_pGameInterface, timerId, timeValue);",
            "GetTimer_API(m_pGameInterface, timerId);",
        )),
        "nativeSetWritesValue": "*(undefined4 *)(iStack_4 + 0x14) = param_2;" in native,
        "nativeGetReadsValue": "return *(undefined4 *)(iStack_4 + 0x14);" in native,
        "nativeMissingGetReturnsZero": bool(re.search(
            r"_GetTimer_CGameScriptInterface__UBEJJ_Z[\s\S]+?return 0;", native)),
        "nativeDeregisterRemovesNode": (
            "free(_Memory);" in native and
            "*(int *)(in_ECX + 0x40) = *(int *)(in_ECX + 0x40) + -1;" in native),
        "nativeRegisterInitializesZero": bool(re.search(
            r"_RegisterTimer_CGameScriptInterface__UAEJXZ[\s\S]+?\+ 0x14\) = 0;", native)),
        "runtimeCountsDown45ToZero": values[:len(expected)] == expected,
        "runtimeNeverNegative": bool(values) and all(value >= 0 for value in values),
        "worldUpdateUsesFrameModuloConstantFps": all(fragment in world_update for fragment in (
            "_GetFrame_CWorld__SIJXZ()", "_GetConstantFPS_CMainGameComponent__SIJXZ()",
            "uVar1 = iVar2 / iVar3;", "if (iVar2 % iVar3 == 0)")),
        "worldUpdateDecrementsOnlyPositiveTimers": all(fragment in world_update for fragment in (
            "if (0 < *(int *)(iVar2 + 4))", "*(int *)(iVar2 + 4) = *(int *)(iVar2 + 4) + -1;")),
    }
    return {
        "schema": "forgefse-timer-abi-audit/0.1",
        "ok": all(checks.values()),
        "checks": checks,
        "observedCountdown": values[:len(expected)],
    }


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--forge-root", type=Path, required=True)
    parser.add_argument("--pdb-names", type=Path, required=True)
    parser.add_argument("--slots", type=Path, required=True)
    parser.add_argument("--native-decomp", type=Path, required=True)
    parser.add_argument("--runtime-log", type=Path, required=True)
    parser.add_argument("--world-update-decomp", type=Path, required=True)
    args = parser.parse_args()
    result = audit(args.forge_root, args.pdb_names, args.slots,
                   args.native_decomp, args.runtime_log, args.world_update_decomp)
    print(json.dumps(result, indent=2))
    return 0 if result["ok"] else 1


if __name__ == "__main__":
    raise SystemExit(main())
