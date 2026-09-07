#!/usr/bin/env python3
"""Rank native script IR for evidence-gated ForgeFSE Lua reconstruction."""

from __future__ import annotations

import argparse
import csv
import json
import re
from collections import Counter
from pathlib import Path
from typing import Any


OPAQUE_RE = re.compile(r"^(?:FUN_|LAB_|SUB\d*$|thunk_FUN_)", re.IGNORECASE)
INFRASTRUCTURE_RE = re.compile(
    r"^(?:operator_(?:new|delete)|CCharString::|std::|CGuiVarTransferStruct::Add|"
    r"CScriptBase::(?:IsActiveThreadTerminating|AddEntityScriptBinding)|"
    r"CSpawnedFunc<|Compare$|CONCAT\d+|CTimer$)"
)
LUA_QUEST_BINDING_RE = re.compile(
    r'questState_type\s*\[\s*"([A-Za-z_][A-Za-z0-9_]*)"\s*\]'
)
RUNTIME_ALIASES = {
    "PostAddScriptedEntities": "FinalizeEntityBindings",
}
HOST_MANAGED_METHODS = {
    "StartScriptingEntity",
}
RUNTIME_ABI_BLOCKERS = {
    "MsgOnLevelLoaded": (
        "retail method writes std::list<CCharString>; ForgeFSE uses a newer MSVC STL ABI"
    ),
}
RUNTIME_SUPPORT_NAMES = {
    "free", "malloc", "rand", "resize", "push_back", "strstr", "__ftol2",
}


def native_helper_category(name: str, roles: set[str] | None = None) -> str:
    """Classify conversion work without suppressing the underlying native evidence."""
    if roles == {"destructor"}:
        return "lifecycle-cleanup"
    if "::~" in name or re.search(r"(?:__?Dtor(?:_\d+)?|_Destructor|Destructor|_Cleanup)$", name):
        return "object-lifetime"
    parts = name.split("::")
    if len(parts) >= 2:
        owner = re.sub(r"<.*$", "", parts[-2])
        method = re.sub(r"<.*$", "", parts[-1])
        if owner == method:
            return "object-lifetime"
    if (name in RUNTIME_SUPPORT_NAMES or name.startswith(("std_", "StdMap_", "List_",
                                                          "DoublyLinkedList_"))):
        return "runtime-support"
    if name.startswith("CScriptBase::") or name.startswith("CSpawnedFunc"):
        return "script-runtime"
    return "engine-or-script-helper"


def is_infrastructure_call(name: str) -> bool:
    """Recognize native-only mechanics whose operation remains explicit in IR."""
    if INFRASTRUCTURE_RE.match(name):
        return True
    # The decompiler flattens the CCharString owner separator at a subset of
    # call sites. These are the same constructors/comparisons/mutations already
    # treated as native string mechanics under the scoped spelling above; a Lua
    # reconstruction translates the retained call operation instead of lifting
    # the retail storage helper as a standalone script helper.
    return name.startswith("CCharString__")


def api_matcher(api_names: list[str]) -> re.Pattern[str]:
    alternatives = "|".join(re.escape(name) for name in sorted(api_names, key=len, reverse=True))
    return re.compile(rf"(?:^|::|_)({alternatives})(?=$|_)")


def api_match(callee: str, api_names: list[str]) -> str | None:
    match = api_matcher(api_names).search(callee)
    return match.group(1) if match else None


def load_fse_address_names(path: Path | None) -> dict[int, str]:
    """address -> FSE API name, from build_fse_address_map.py.

    The native helper backlog is keyed by target address but *named* from
    Ghidra/BSim, which mislabels the 0x00CBxxxx script-interface thunk band - e.g.
    0x00CBE87F arrives as `CSubtitleRenderer::SetText` (BSim similarity 0.51) when
    two independent FSE trees both bind it as AddLogbookStoryEntry. A call with a
    correct API name is not an unlifted helper at all: a Lua reconstruction calls
    the API directly. Renaming those callees therefore moves them out of the
    helper backlog and into the ordinary API-call bucket.

    Only unambiguous entries are used. An address bound to more than one name
    across the FSE trees is skipped rather than guessed.
    """
    if not path or not path.exists():
        return {}
    payload = json.loads(path.read_text(encoding="utf-8-sig"))
    names: dict[int, str] = {}
    for entry in payload["entries"]:
        if entry.get("ambiguous"):
            continue
        fse = entry.get("fseNames") or []
        if len(fse) != 1:
            continue
        names[int(entry["address"], 16)] = fse[0]
    return names


def apply_fse_names(ir: dict[str, Any], names: dict[int, str],
                    api_names: set[str]) -> int:
    """Rewrite call callees in place where FSE names the target address AND that
    name is a real FSE API. Returns the number of call sites renamed; keeps the
    old name as `bsimName`.

    The API-name filter is load-bearing, not a nicety. FSE also binds addresses
    for the allocator, the CRT and the STL (Game_free, Game_malloc, rand,
    __ftol2, StdMap_Construct, CCharString_OperatorPlus, ...). Those callees are
    deliberately excluded from the helper backlog upstream of here by
    OPAQUE_RE / is_infrastructure_call, which match on the *retail* names -
    renaming them defeats both classifiers. Doing this unfiltered moved 7,410
    call sites and pushed the unresolved-helper count from 584 to 7,364, i.e. it
    reclassified all of the allocator/CRT traffic as unlifted script helpers.
    Renaming is therefore confined to addresses whose FSE name is one the Lua
    reconstruction can actually call.
    """
    if not names or not api_names:
        return 0
    renamed = 0
    for life in ir["lifecycle"]:
        for call in life["calls"]:
            addr = call.get("targetAddress")
            if not addr:
                continue
            fse = names.get(int(addr, 16))
            if not fse or fse == call["callee"] or fse not in api_names:
                continue
            call["bsimName"] = call["callee"]
            call["fseName"] = fse
            call["callee"] = fse
            renamed += 1
    return renamed


def load_interface_methods(slots_path: Path | None, catalog_path: Path | None) -> dict[str, dict[str, str]]:
    if not slots_path or not catalog_path:
        return {}
    addresses = {}
    with catalog_path.open(encoding="utf-8-sig") as stream:
        for line in stream:
            address, name = line.rstrip("\r\n").split("\t", 1)
            addresses[address.lower()] = name
    with slots_path.open(encoding="utf-8-sig", newline="") as stream:
        result = {}
        for row in csv.DictReader(stream, delimiter="\t"):
            if row["executable"].lower() != "true":
                continue
            name = addresses.get(row["target_address"].lower(), row["current_name"])
            decorated = re.match(r"^\?([^@]+)@CGameScriptInterface@@", name)
            if decorated:
                name = decorated.group(1)
            result[row["offset"].lower()] = {"name": name,
                                    "targetAddress": f"0x{row['target_address'].upper()}",
                                    "slotAddress": f"0x{row['slot_address'].upper()}"}
        return result


def analyze(catalog_path: Path, ir_dir: Path, manifest_path: Path,
            slots_path: Path | None = None, interface_catalog_path: Path | None = None,
            interface_field_evidence_path: Path | None = None,
            lua_manager_path: Path | None = None,
            helper_ir_path: Path | None = None,
            fse_address_map_path: Path | None = None) -> dict[str, Any]:
    catalog = json.loads(catalog_path.read_text(encoding="utf-8-sig"))
    manifest = json.loads(manifest_path.read_text(encoding="utf-8-sig"))
    api_names = sorted({row["name"] for row in manifest["functions"]}, key=len, reverse=True)
    match_api = api_matcher(api_names)
    runtime_bindings = set()
    if lua_manager_path:
        runtime_bindings = set(LUA_QUEST_BINDING_RE.findall(
            lua_manager_path.read_text(encoding="utf-8-sig", errors="replace")
        ))
    interface_methods = load_interface_methods(slots_path, interface_catalog_path)
    ready_helper_targets = set()
    if helper_ir_path:
        helper_ir = json.loads(helper_ir_path.read_text(encoding="utf-8-sig"))
        ready_helper_targets = {
            row["targetAddress"] for row in helper_ir["helpers"] if row.get("luaEmissionReady")
        }
    verified_interface_fields = set()
    if interface_field_evidence_path:
        with interface_field_evidence_path.open(encoding="utf-8-sig", newline="") as stream:
            verified_interface_fields = {
                row["script"] for row in csv.DictReader(stream, delimiter="\t")
                if row["verified"].lower() == "true"
            }
    kinds = {row["name"]: row["kind"] for row in catalog["scripts"]}
    fse_names = load_fse_address_names(fse_address_map_path)
    fse_renamed_calls = 0
    fse_renamed_scripts = 0
    rows = []
    for path in sorted(ir_dir.glob("*.json")):
        ir = json.loads(path.read_text(encoding="utf-8-sig"))
        renamed = apply_fse_names(ir, fse_names, set(api_names))
        if renamed:
            fse_renamed_calls += renamed
            fse_renamed_scripts += 1
        calls = [call["callee"] for life in ir["lifecycle"] for call in life["calls"]]
        indirect = [call for life in ir["lifecycle"] for call in life.get("indirectCalls", [])]
        direct_targets = [dict(call, role=life["role"])
                          for life in ir["lifecycle"]
                          for call in life.get("directCallTargets", [])]
        entity_bindings = [binding for life in ir["lifecycle"]
                           for binding in life.get("entityBindings", [])]
        interface_calls = []
        for call in indirect:
            method = interface_methods.get(call.get("vtableOffset"))
            provenance = call.get("interfaceProvenance")
            singleton_provenance = provenance in {
                "direct-gamescriptinterface-singleton",
                "local-copy-of-gamescriptinterface-singleton",
            }
            field_provenance = provenance in {
                "script-instance-gamescriptinterface-field",
                "local-copy-of-script-interface-vtable",
            } and ir["script"] in verified_interface_fields
            if (singleton_provenance or field_provenance) and method:
                runtime_name = RUNTIME_ALIASES.get(method["name"], method["name"])
                host_managed = method["name"] in HOST_MANAGED_METHODS
                abi_blocker = RUNTIME_ABI_BLOCKERS.get(method["name"])
                interface_calls.append({**call, **method,
                                        "forgeManifestMatch": method["name"] in api_names,
                                        "forgeRuntimeName": runtime_name,
                                        "forgeRuntimeMatch": runtime_name in runtime_bindings,
                                        "forgeHostManaged": host_managed,
                                        "forgeRuntimeAbiBlocker": abi_blocker})
        opaque = sorted({callee for callee in calls if OPAQUE_RE.match(callee)})
        matches = {callee: match_api.search(callee) for callee in set(calls)}
        mapped = sorted({match.group(1) for match in matches.values() if match})
        infrastructure = {callee for callee in calls if is_infrastructure_call(callee)}
        helper_candidates = set(calls) - set(opaque) - infrastructure - {
            callee for callee, match in matches.items() if match}
        resolved_native = []
        for helper in sorted(helper_candidates):
            instances = [call for life in ir["lifecycle"] for call in life["calls"]
                         if call["callee"] == helper]
            if instances and all(call.get("targetAddress") in ready_helper_targets
                                 for call in instances):
                resolved_native.append(helper)
        unresolved = sorted(helper_candidates - set(resolved_native))
        resolved_native_counts = Counter(callee for callee in calls if callee in resolved_native)
        resolved_native_targets = {
            helper: sorted({call["targetAddress"] for life in ir["lifecycle"]
                            for call in life["calls"] if call["callee"] == helper})
            for helper in resolved_native
        }
        unresolved_counts = Counter(callee for callee in calls if callee in unresolved)
        unresolved_roles = {
            helper: sorted({life["role"] for life in ir["lifecycle"]
                            if any(call["callee"] == helper for call in life["calls"])})
            for helper in unresolved
        }
        unresolved_targets = {
            helper: sorted({call["targetAddress"] for life in ir["lifecycle"]
                            for call in life["calls"]
                            if call["callee"] == helper and call.get("targetAddress")})
            for helper in unresolved
        }
        unresolved_targeted_counts = {
            helper: sum(call.get("targetAddress") is not None for life in ir["lifecycle"]
                        for call in life["calls"] if call["callee"] == helper)
            for helper in unresolved
        }
        decompile_complete = len(ir["lifecycle"]) == 5 and all(
            life.get("address") and isinstance(life.get("calls"), list) for life in ir["lifecycle"]
        )
        if not decompile_complete:
            stage = "repair-native-ir"
        elif opaque:
            stage = "resolve-opaque-callees"
        elif unresolved:
            stage = "map-native-helpers"
        else:
            stage = "manual-lua-reconstruction"
        rows.append({
            "name": ir["script"], "kind": kinds.get(ir["script"], "unknown"),
            "allocatorAddress": ir["allocatorAddress"], "vtableAddress": ir["vtableAddress"],
            "anchored": bool(ir.get("evidenceAnchors")), "lifecycleFunctions": len(ir["lifecycle"]),
            "calls": len(calls), "distinctCalls": len(set(calls)), "mappedForgeApis": mapped,
            "indirectCalls": len(indirect),
            "nativeDirectCalls": len(direct_targets),
            "nativeDirectCallTargets": direct_targets,
            "entityBindings": entity_bindings,
            "entityBindingCount": len(entity_bindings),
            "completeEntityBindings": sum(binding.get("complete", False)
                                            for binding in entity_bindings),
            "resolvedInterfaceCalls": interface_calls,
            "resolvedInterfaceMethods": sorted({call["name"] for call in interface_calls}),
            "indirectVtableOffsets": sorted({call["vtableOffset"] for call in indirect
                                              if call.get("vtableOffset")}),
            "opaqueCallees": opaque, "unresolvedNativeHelpers": unresolved, "stage": stage,
            "resolvedNativeHelpers": resolved_native,
            "resolvedNativeHelperCallCounts": dict(sorted(resolved_native_counts.items())),
            "resolvedNativeHelperTargetAddresses": resolved_native_targets,
            "unresolvedNativeHelperCallCounts": dict(sorted(unresolved_counts.items())),
            "unresolvedNativeHelperRoles": unresolved_roles,
            "unresolvedNativeHelperTargetAddresses": unresolved_targets,
            "unresolvedNativeHelperTargetedCallCounts": unresolved_targeted_counts,
            "nativeIr": str(path.resolve()),
        })
    rows.sort(key=lambda row: (
        {"manual-lua-reconstruction": 0, "map-native-helpers": 1,
         "resolve-opaque-callees": 2, "repair-native-ir": 3}[row["stage"]],
        len(row["opaqueCallees"]) + len(row["unresolvedNativeHelpers"]), row["name"]
    ))
    stages: dict[str, int] = {}
    for row in rows:
        stages[row["stage"]] = stages.get(row["stage"], 0) + 1
    resolved_methods = {call["name"] for row in rows for call in row["resolvedInterfaceCalls"]}
    resolved_helper_methods = {helper for row in rows for helper in row["resolvedNativeHelpers"]}
    missing_methods = {call["name"] for row in rows for call in row["resolvedInterfaceCalls"]
                       if not call["forgeManifestMatch"]}
    missing_runtime_methods = {call["name"] for row in rows for call in row["resolvedInterfaceCalls"]
                               if not call["forgeRuntimeMatch"] and not call["forgeHostManaged"]}
    host_managed_methods = {call["name"] for row in rows for call in row["resolvedInterfaceCalls"]
                            if call["forgeHostManaged"]}
    abi_blocked_methods = {
        call["name"]: call["forgeRuntimeAbiBlocker"]
        for row in rows for call in row["resolvedInterfaceCalls"]
        if call["forgeRuntimeAbiBlocker"]
    }
    helper_consumers: dict[str, set[str]] = {}
    helper_kinds: dict[str, set[str]] = {}
    helper_calls: Counter[str] = Counter()
    helper_roles: dict[str, set[str]] = {}
    helper_targets: dict[str, set[str]] = {}
    helper_targeted_calls: Counter[str] = Counter()
    for row in rows:
        for helper, count in row["unresolvedNativeHelperCallCounts"].items():
            helper_calls[helper] += count
            helper_consumers.setdefault(helper, set()).add(row["name"])
            helper_kinds.setdefault(helper, set()).add(row["kind"])
            helper_roles.setdefault(helper, set()).update(row["unresolvedNativeHelperRoles"][helper])
            helper_targets.setdefault(helper, set()).update(
                row["unresolvedNativeHelperTargetAddresses"][helper])
            helper_targeted_calls[helper] += row["unresolvedNativeHelperTargetedCallCounts"][helper]
    helper_backlog = [
        {"name": helper, "calls": helper_calls[helper],
         "targetedCalls": helper_targeted_calls[helper],
         "category": native_helper_category(helper, helper_roles[helper]),
         "scripts": len(helper_consumers[helper]),
         "roles": sorted(helper_roles[helper]),
         "targetAddresses": sorted(helper_targets[helper]),
         "kinds": sorted(helper_kinds[helper]),
         "consumers": sorted(helper_consumers[helper])}
        for helper in helper_calls
    ]
    helper_backlog.sort(key=lambda row: (-row["scripts"], -row["calls"], row["name"]))
    category_summary = {}
    for row in helper_backlog:
        category = category_summary.setdefault(row["category"], {"methods": 0, "calls": 0})
        category["methods"] += 1
        category["calls"] += row["calls"]
    target_rows: dict[str, dict[str, Any]] = {}
    for row in rows:
        for call in row["nativeDirectCallTargets"]:
            address = call["target"].upper().replace("0X", "0x")
            target = target_rows.setdefault(address, {
                "targetAddress": address, "calls": 0, "currentNames": set(),
                "consumers": set(), "roles": set(), "kinds": set(),
            })
            target["calls"] += 1
            target["currentNames"].add(call["currentName"])
            target["consumers"].add(row["name"])
            target["roles"].add(call["role"])
            target["kinds"].add(row["kind"])
    direct_target_backlog = []
    for target in target_rows.values():
        names = sorted(target["currentNames"])
        roles = sorted(target["roles"])
        representative = names[0] if names else target["targetAddress"]
        direct_target_backlog.append({
            "targetAddress": target["targetAddress"], "calls": target["calls"],
            "currentNames": names,
            "category": native_helper_category(representative, set(roles)),
            "scripts": len(target["consumers"]), "roles": roles,
            "kinds": sorted(target["kinds"]), "consumers": sorted(target["consumers"]),
        })
    direct_target_backlog.sort(
        key=lambda row: (-row["scripts"], -row["calls"], row["targetAddress"]))
    # The call-site evidence already lives in the per-script IR. Keep this report
    # useful for prioritization instead of duplicating all 15k records a second time.
    for row in rows:
        del row["nativeDirectCallTargets"]
    return {
        "schema": "forgefse-native-conversion-readiness/0.1",
        "sources": {"catalog": str(catalog_path.resolve()), "ir": str(ir_dir.resolve()),
                    "vtableSlots": str(slots_path.resolve()) if slots_path else None,
                    "interfaceCatalog": str(interface_catalog_path.resolve()) if interface_catalog_path else None,
                    "fseAddressMap": str(fse_address_map_path.resolve()) if fse_address_map_path else None,
                    "apiManifest": str(manifest_path.resolve()),
                    "interfaceFieldEvidence": (str(interface_field_evidence_path.resolve())
                                               if interface_field_evidence_path else None),
                    "luaManager": str(lua_manager_path.resolve()) if lua_manager_path else None,
                    "helperIr": str(helper_ir_path.resolve()) if helper_ir_path else None},
        "summary": {"scripts": len(rows), "anchored": sum(row["anchored"] for row in rows),
                    "fseRenamedCalls": fse_renamed_calls,
                    "fseRenamedScripts": fse_renamed_scripts,
                    "fseNamedAddresses": len(fse_names),
                    "lifecycleComplete": sum(row["lifecycleFunctions"] == 5 for row in rows),
                    "verifiedScriptInterfaceFields": len(verified_interface_fields),
                    "resolvedInterfaceCalls": sum(len(row["resolvedInterfaceCalls"]) for row in rows),
                    "resolvedForgeInterfaceCalls": sum(
                        sum(call["forgeManifestMatch"] for call in row["resolvedInterfaceCalls"])
                        for row in rows),
                    "resolvedForgeRuntimeCalls": sum(
                        sum(call["forgeRuntimeMatch"] or call["forgeHostManaged"]
                            for call in row["resolvedInterfaceCalls"])
                        for row in rows),
                    "resolvedInterfaceMethods": len(resolved_methods),
                    "missingForgeInterfaceMethods": sorted(missing_methods),
                    "missingForgeRuntimeMethods": sorted(missing_runtime_methods),
                    "hostManagedInterfaceMethods": sorted(host_managed_methods),
                    "abiBlockedInterfaceMethods": dict(sorted(abi_blocked_methods.items())),
                    "resolvedNativeHelperMethods": len(resolved_helper_methods),
                    "resolvedNativeHelperCalls": sum(
                        sum(row["resolvedNativeHelperCallCounts"].values()) for row in rows),
                    "scriptsWithResolvedNativeHelpers": sum(bool(row["resolvedNativeHelpers"])
                                                            for row in rows),
                    "unresolvedNativeHelperMethods": len(helper_backlog),
                    "unresolvedNativeHelperCalls": sum(helper_calls.values()),
                    "nativeHelperMethodsWithTargetAddress": sum(
                        bool(row["targetAddresses"]) for row in helper_backlog),
                    "nativeHelperCallsWithTargetAddress": sum(helper_targeted_calls.values()),
                    "nativeDirectCalls": sum(row["nativeDirectCalls"] for row in rows),
                    "nativeDirectCallTargets": len(direct_target_backlog),
                    "nativeHelperCategories": dict(sorted(category_summary.items())),
                    "scriptsWithResolvedInterfaceCalls": sum(bool(row["resolvedInterfaceCalls"]) for row in rows),
                    "entityBindings": sum(row["entityBindingCount"] for row in rows),
                    "completeEntityBindings": sum(row["completeEntityBindings"] for row in rows),
                    "scriptsWithEntityBindings": sum(bool(row["entityBindingCount"]) for row in rows),
                    "stages": dict(sorted(stages.items()))},
        "nativeHelperBacklog": helper_backlog,
        "nativeDirectCallTargetBacklog": direct_target_backlog,
        "scripts": rows,
    }


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--catalog", type=Path, required=True)
    parser.add_argument("--ir", type=Path, required=True)
    parser.add_argument("--api-manifest", type=Path, required=True)
    parser.add_argument("--vtable-slots", type=Path)
    parser.add_argument("--interface-catalog", type=Path)
    parser.add_argument("--interface-field-evidence", type=Path)
    parser.add_argument("--lua-manager", type=Path)
    parser.add_argument("--helper-ir", type=Path)
    parser.add_argument("--fse-address-map", type=Path,
                        help="build_fse_address_map.py output; renames helper callees "
                             "whose target address FSE names authoritatively")
    parser.add_argument("--output", type=Path, required=True)
    args = parser.parse_args()
    result = analyze(args.catalog, args.ir, args.api_manifest, args.vtable_slots,
                     args.interface_catalog, args.interface_field_evidence, args.lua_manager,
                     args.helper_ir, args.fse_address_map)
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(result, indent=2) + "\n", encoding="utf-8")
    print(json.dumps(result["summary"], sort_keys=True))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
