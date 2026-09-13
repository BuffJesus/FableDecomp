#!/usr/bin/env python3
"""Prove Barrel Man phase 1 is unreachable from the class's clean lifecycle."""
from __future__ import annotations

import argparse
import json
from pathlib import Path


def audit(retail_disassembly: Path, donor_decompile: Path, lua_script: Path,
          entity_manifest: Path, layouts: Path) -> dict:
    retail = retail_disassembly.read_text(encoding="utf-8-sig").lower()
    donor = donor_decompile.read_text(encoding="utf-8-sig")
    lua = lua_script.read_text(encoding="utf-8-sig")
    manifest = json.loads(entity_manifest.read_text(encoding="utf-8-sig"))
    layout = layouts.read_text(encoding="utf-8-sig")
    phase_writes = tuple(line.strip() for line in retail.splitlines()
                         if "mov dword ptr [esi + 0x20]" in line)
    checks = {
        "pdbPinsMyPhaseAt20": "CNOVI_BarrelMan\t36\t32\tEMyPhase\tMyPhase" in layout,
        "donorConstructorDoesNotInitializePhase": "in_ECX[6] = param_2;" in donor
            and "in_ECX[7]" not in donor.split("// CALLEES", 1)[0]
            and "in_ECX[8]" not in donor.split("// CALLEES", 1)[0],
        "donorInitSetsPhaseZero": "*(undefined4 *)(in_ECX + 0x20) = 0;" in donor,
        "retailMainPhaseWriteSetIsTwoThroughFive": len(phase_writes) == 5
            and sum(",0x2" in line for line in phase_writes) == 2
            and sum(",0x3" in line for line in phase_writes) == 1
            and sum(",0x4" in line for line in phase_writes) == 1
            and sum(",0x5" in line for line in phase_writes) == 1,
        "retailMainNeverWritesPhaseOne": not any(",0x1" in line for line in phase_writes),
        "luaPreservesDeadCaseWithoutInventingTransition": "WALKING_OFF  = 1" in lua
            and "MyPhase = PHASE.WALKING_OFF" not in lua,
        "luaPreservesDeadCaseNullMarkerFallback": all(x in lua for x in (
            "local ZERO_POSITION            = { x = 0.0, y = 0.0, z = 0.0 }",
            "local target = walkOff and walkOff:GetPos() or ZERO_POSITION",
            "walk_to(quest, me, target)")),
        "manifestClassifiesPhaseOneAsUnreachable": any(
            p.get("id") == "WALKING_OFF(1)" and "unreachable" in p.get("entry", "")
            for p in manifest.get("phases", [])),
        "descriptiveLabelsAreNotBehavioralUncertainty": not manifest.get("uncertainties")
            and all(v.get("nameEvidence") == "inference"
                    for field in manifest["localFields"] if field.get("name") == "MyPhase"
                    for v in field["enumValues"]),
    }
    return {"schema": "barrel-man-phase-reachability-audit/0.1",
            "ok": all(checks.values()), "checks": checks, "retailPhaseWrites": phase_writes}


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--retail-disassembly", type=Path, required=True)
    parser.add_argument("--donor-decompile", type=Path, required=True)
    parser.add_argument("--lua-script", type=Path, required=True)
    parser.add_argument("--entity-manifest", type=Path, required=True)
    parser.add_argument("--layouts", type=Path, required=True)
    args = parser.parse_args()
    result = audit(args.retail_disassembly, args.donor_decompile, args.lua_script,
                   args.entity_manifest, args.layouts)
    print(json.dumps(result, indent=2))
    return 0 if result["ok"] else 1


if __name__ == "__main__":
    raise SystemExit(main())
