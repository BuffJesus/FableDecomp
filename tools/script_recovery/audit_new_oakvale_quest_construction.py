#!/usr/bin/env python3
"""Validate the native New Oakvale quest construction and lifecycle chain."""
from __future__ import annotations

import json
from pathlib import Path


def audit(workspace: Path, cluster_path: Path) -> dict:
    cluster = json.loads(cluster_path.read_text(encoding="utf-8-sig"))
    inventory = json.loads((workspace / "refs/script_recovery/new_oakvale_intro/entities/Q_NewOakValeIntro.json").read_text(encoding="utf-8-sig"))
    vtable_snapshot = json.loads((workspace / "ghidra_out/script_recovery/new_oakvale_quest_vtable.json").read_text(encoding="utf-8-sig"))
    ownership = json.loads((workspace / "ghidra_out/script_recovery/new_oakvale_quest_ownership.json").read_text(encoding="utf-8-sig"))
    entity_vtables = json.loads((workspace / "ghidra_out/script_recovery/new_oakvale_entity_vtables.json").read_text(encoding="utf-8-sig"))
    expected = {
        cluster["allocatorAddress"].upper(), cluster["constructorAddress"].upper(),
        cluster["baseConstructorAddress"].upper(),
        cluster["destructorImplementationAddress"].upper(),
        cluster["baseDestructorAddress"].upper(),
        *(row["address"].upper() for row in cluster["lifecycle"]),
    }
    export_dir = workspace / "ghidra_out/script_recovery/native_quest_construction"
    actual = {path.stem.upper(): path for path in export_dir.glob("0x*.c")}
    errors: list[str] = []
    if set(actual) != expected:
        errors.append(f"quest construction exports differ: missing={sorted(expected-set(actual))}, "
                      f"extra={sorted(set(actual)-expected)}")
    bodies: dict[str, str] = {}
    for address, path in actual.items():
        text = path.read_text(encoding="utf-8", errors="replace")
        lines = text.splitlines()
        if not lines or f"function entry {address}".lower() not in lines[0].lower() or "DecompFuncsToDirectory.java" not in lines[0]:
            errors.append(f"{address}: invalid provenance header")
        bodies[address] = text

    allocator = bodies.get(cluster["allocatorAddress"].upper(), "")
    constructor = bodies.get(cluster["constructorAddress"].upper(), "")
    register = next(row for row in cluster["lifecycle"] if row["role"] == "RegisterMain")
    register_body = bodies.get(register["address"].upper(), "")
    deleting_destructor = bodies.get(next(row["address"].upper() for row in cluster["lifecycle"]
                                         if row["role"] == "destructor"), "")
    destructor = bodies.get(cluster["destructorImplementationAddress"].upper(), "")
    base_destructor = bodies.get(cluster["baseDestructorAddress"].upper(), "")
    vtable = cluster["vtableAddress"].lower().removeprefix("0x")
    for fragment in ("operator_new(0x10c)",
                     "CQ_NewOakValeIntroScript::CQ_NewOakValeIntroScript(this,param_1,param_2)"):
        if fragment not in allocator:
            errors.append(f"quest allocator missing {fragment}")
    for fragment in ("CHeroMorphDef::CHeroMorphDef", "this + 0x40", "this + 0x44",
                     f"PTR__vector_deleting_destructor__{vtable}", "this + 0x104", "this + 0x108"):
        if fragment not in constructor:
            errors.append(f"quest constructor missing {fragment}")
    for fragment in ("operator_new(0x3c)", '"Main"', "this_00 + 0x34", "this_00 + 0x38",
                     "CGuiVarTransferStruct::Add"):
        if fragment not in register_body:
            errors.append(f"RegisterMain missing {fragment}")
    if "CParticleEmitter_Dtor_7();" not in deleting_destructor:
        errors.append("deleting destructor does not call the recovered quest destructor implementation")
    for fragment in ("param_1 + 0x108", "param_1 + 0x104", "*DAT_0143e8f8 + 0x160",
                     "param_1 + 0xf0", "param_1 + 0x9c", "DeleteAllParticles(param_1"):
        if fragment not in destructor:
            errors.append(f"quest destructor implementation missing {fragment}")
    for fragment in ("this + 0x34", "this + 0x30", "this + 0x18", "this + 8", "this + 4",
                     "NHeroInformationScreens::CBase::CBase"):
        if fragment not in base_destructor:
            errors.append(f"script-base destructor missing {fragment}")
    roles = [row["role"] for row in cluster["lifecycle"]]
    if roles != ["destructor", "RegisterMain", "Main", "Init", "OnPersist"]:
        errors.append(f"unexpected quest vtable roles {roles}")
    snapshot_slots = [(row["role"], row["address"]) for row in vtable_snapshot["slots"]]
    cluster_slots = [(row["role"], row["address"]) for row in cluster["lifecycle"]]
    if snapshot_slots != cluster_slots:
        errors.append("retail-byte quest vtable differs from native cluster lifecycle")
    if vtable_snapshot["vtable"] != cluster["vtableAddress"]:
        errors.append("retail-byte quest vtable address differs from cluster")
    if vtable_snapshot["deletingDestructorImplementation"] != cluster["destructorImplementationAddress"]:
        errors.append("retail deleting-destructor call target differs from cluster")
    if vtable_snapshot["sourceSha256"] != entity_vtables["sourceSha256"]:
        errors.append("quest and entity vtable snapshots use different retail executables")
    ownership_expected = {
        ("allocator", "address"): cluster["allocatorAddress"],
        ("allocator", "objectSize"): 0x10C,
        ("allocator", "constructorTarget"): cluster["constructorAddress"],
        ("constructor", "address"): cluster["constructorAddress"],
        ("constructor", "baseConstructorTarget"): cluster["baseConstructorAddress"],
        ("constructor", "installedVtable"): cluster["vtableAddress"],
    }
    for (section, key), expected_value in ownership_expected.items():
        if ownership[section].get(key) != expected_value:
            errors.append(f"retail-byte ownership {section}.{key}={ownership[section].get(key)!r}, expected {expected_value!r}")
    if ownership["sourceSha256"] != vtable_snapshot["sourceSha256"]:
        errors.append("ownership and vtable snapshots use different retail executables")
    if ownership["allocator"].get("operatorNewTarget") != "0x00BFEA1A":
        errors.append("retail-byte allocator no longer calls the known operator_new thunk")
    expected_inventory = {
        "vtable": cluster["vtableAddress"],
        "allocator": cluster["allocatorAddress"],
        "constructor": cluster["constructorAddress"],
        "baseConstructor": cluster["baseConstructorAddress"],
        "baseClass": cluster["baseClassSemantic"],
        "objectSize": 0x10C,
    }
    for key, expected_value in expected_inventory.items():
        if inventory.get(key) != expected_value:
            errors.append(f"quest inventory {key}={inventory.get(key)!r}, expected {expected_value!r}")
    return {
        "schema": "new-oakvale-quest-construction-audit/0.1",
        "objectSize": 0x10C,
        "lifecycleSlots": len(cluster["lifecycle"]),
        "exportedFunctions": len(actual),
        "errors": errors,
        "ok": not errors,
    }


if __name__ == "__main__":
    root = Path(__file__).resolve().parents[2]
    print(json.dumps(audit(root, root / "refs/script_recovery/native_clusters/Q_NewOakValeIntro.json"), indent=2))
