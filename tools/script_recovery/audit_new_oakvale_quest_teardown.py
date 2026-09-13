#!/usr/bin/env python3
"""Validate the New Oakvale quest destructor against its recovered layout."""
from __future__ import annotations

import json
from pathlib import Path


def _load(path: Path) -> dict:
    return json.loads(path.read_text(encoding="utf-8-sig"))


def audit(workspace: Path) -> dict:
    cluster = _load(workspace / "refs/script_recovery/native_clusters/Q_NewOakValeIntro.json")
    teardown = _load(workspace / "ghidra_out/script_recovery/new_oakvale_quest_teardown.json")
    layout = _load(workspace / "ghidra_out/script_recovery/new_oakvale_quest_layout.json")
    helper_ir = _load(workspace / "refs/script_recovery/native_helper_operation_ir.json")
    fse_names = _load(workspace / "refs/script_recovery/fse_address_name_map.json")
    export_dir = workspace / "ghidra_out/script_recovery/native_quest_construction"
    implementation = (export_dir / f"{cluster['destructorImplementationAddress'].upper()}.c").read_text(encoding="utf-8")
    deleting = (export_dir / "0x00DBEFA0.c").read_text(encoding="utf-8")
    base = (export_dir / f"{cluster['baseDestructorAddress'].upper()}.c").read_text(encoding="utf-8")
    errors: list[str] = []

    if teardown["deletingDestructor"]["implementation"] != cluster["destructorImplementationAddress"]:
        errors.append("teardown and cluster implementation addresses differ")
    if teardown["implementation"]["baseDestructor"] != cluster["baseDestructorAddress"]:
        errors.append("teardown and cluster base-destructor addresses differ")
    if cluster.get("baseClassSemantic") != "CScriptBase":
        errors.append("cluster does not identify the proven base class")
    if "CParticleEmitter_Dtor_7();" not in deleting or "param_1 & 1" not in deleting or "operator_delete(this)" not in deleting:
        errors.append("deleting-destructor thunk shape changed")

    timer_offsets = teardown["implementation"]["timerDeregistrationOrder"]
    timer_positions = [implementation.find(f"param_1 + {offset.lower()}") for offset in timer_offsets]
    if any(position < 0 for position in timer_positions) or timer_positions != sorted(timer_positions):
        errors.append("timer deregistration order differs from teardown metadata")
    if implementation.count("*DAT_0143e8f8 + 0x160") != 2:
        errors.append("quest destructor must make exactly two timer-interface calls")

    vector_offsets = teardown["implementation"]["vectorDestructionOrder"]
    vector_positions = [implementation.find(f"param_1 + {offset.lower()}") for offset in vector_offsets]
    if any(position < 0 for position in vector_positions) or vector_positions != sorted(vector_positions):
        errors.append("speech-vector destruction order differs from teardown metadata")
    vector_region = next(region for region in layout["regions"] if region["name"] == "villager-speech-vectors")
    expected_offsets = [f"0x{offset:X}" for offset in range(
        int(vector_region["offset"], 0) + vector_region["size"] - 12,
        int(vector_region["offset"], 0) - 1, -12)]
    if vector_offsets != expected_offsets:
        errors.append("teardown vectors do not reverse-cover the complete layout region")
    if implementation.count("free(*(void **)(param_1 +") != 8:
        errors.append("quest destructor must release exactly eight vector buffers")

    helper = next((row for row in helper_ir["helpers"]
                   if row.get("targetAddress") == cluster["destructorImplementationAddress"]), None)
    if helper is None:
        errors.append("quest destructor implementation missing from helper IR")
    else:
        direct_targets = [row["target"] for row in helper.get("directCallTargets", [])]
        if direct_targets[-1:] != [cluster["baseDestructorAddress"]]:
            errors.append("quest destructor does not end in the recorded base cleanup")
        resolved = helper.get("resolvedInterfaceCalls", [])
        if len(resolved) != 2 or any(row.get("name") != "DeregisterTimer" for row in resolved):
            errors.append("both timer-interface calls must resolve to DeregisterTimer")
    for fragment in ("this + 0x34", "this + 0x30", "this + 0x18", "this + 8", "this + 4",
                     "NHeroInformationScreens::CBase::CBase"):
        if fragment not in base:
            errors.append(f"base cleanup missing inherited-storage anchor {fragment}")
    constructor_map = next((row for row in fse_names["entries"]
                            if row.get("address") == cluster["baseConstructorAddress"]), None)
    if constructor_map is None or constructor_map.get("fseNames") != ["CScriptBase_Construct"]:
        errors.append("base constructor is not independently mapped to CScriptBase_Construct")
    base_helper = next((row for row in helper_ir["helpers"]
                        if row.get("targetAddress") == cluster["baseDestructorAddress"]), None)
    expected_consumers = teardown["baseDestructor"]["consumerScriptDestructors"]
    if base_helper is None or base_helper.get("consumerCalls") != expected_consumers:
        errors.append("base cleanup consumer count differs from teardown evidence")
    elif base_helper.get("consumerRoles") != ["destructor"] or any(
            row.get("role") != "destructor" for row in base_helper.get("consumers", [])):
        errors.append("base cleanup has a non-destructor script consumer")
    if not teardown["baseDestructor"].get("exactClassNameResolved"):
        errors.append("base-class identity unexpectedly marked unresolved")

    return {
        "schema": "new-oakvale-quest-teardown-audit/0.1",
        "timersDeregistered": len(timer_offsets),
        "speechVectorsDestroyed": len(vector_offsets),
        "baseCleanup": cluster["baseDestructorAddress"],
        "baseClass": cluster["baseClassSemantic"],
        "baseDestructorConsumers": expected_consumers,
        "errors": errors,
        "ok": not errors,
    }


if __name__ == "__main__":
    print(json.dumps(audit(Path(__file__).resolve().parents[2]), indent=2))
