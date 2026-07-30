#!/usr/bin/env python3
"""Create the reproducible FableTLC source-reconstruction workspace.

Generated files are derived from the Ghidra/API exports and lift ledgers. The
script never modifies the Ghidra project or the authoritative lift sources.
"""
from __future__ import annotations

import argparse
import csv
import json
import re
import shutil
from collections import Counter, defaultdict
from dataclasses import dataclass, asdict
from pathlib import Path


UNDEFINED_TYPES = {"", "undefined", "undefined1", "undefined2", "undefined4", "undefined8"}
ADDRESS_RE = re.compile(r"(?:0x)?([0-9a-fA-F]{8})")


@dataclass
class FunctionRow:
    address: str
    name: str
    module: str
    calling_convention: str
    return_type: str
    parameter_count: int
    parameter_types: str
    prototype_complete: bool
    clean_decompile: str
    naming_quality: str
    agent_verdict: str
    agent_source: str
    lift_grade: str
    lift_source: str


def normalize_address(value: str) -> str:
    match = ADDRESS_RE.fullmatch(value.strip())
    if not match:
        raise ValueError(f"invalid address: {value}")
    return match.group(1).lower()


def module_for_name(name: str) -> str:
    clean = name.strip()
    if "::" in clean:
        owner = clean.rsplit("::", 1)[0]
        return owner.replace("\t", " ").replace("\n", " ")

    # A significant part of engine_api.tsv still carries VC7.1 decorated
    # symbols. Recover the owning class/namespace without requiring an external
    # demangler merely to group the rebuild manifest.
    constructor = re.match(r"^\?\?[01]([^@?$]+)@@", clean)
    if constructor:
        return constructor.group(1)
    decorated = re.match(r"^\?[^@]+@(.+?)@@", clean)
    if decorated:
        components = [part for part in decorated.group(1).split("@") if part and not part.startswith("?")]
        if components:
            return "::".join(reversed(components))
    return "_global"


def module_for_api(name: str, calling_convention: str, parameter_types: str) -> str:
    module = module_for_name(name)
    if module != "_global" or calling_convention != "__thiscall" or not parameter_types:
        return module
    # Ghidra often exports a demangled method as a bare function name while its
    # implicit this pointer remains the first typed parameter.
    first = parameter_types.split(",", 1)[0].strip()
    first = re.sub(r"^(?:class|struct|const)\s+", "", first)
    first = re.sub(r"\s+(?:const\s*)?[*&]+\s*$", "", first).strip()
    return first if first else module


def read_engine_api(path: Path) -> list[dict[str, str]]:
    rows: list[dict[str, str]] = []
    with path.open("r", encoding="utf-8-sig", errors="replace", newline="") as stream:
        reader = csv.reader(stream, delimiter="\t")
        for parts in reader:
            if not parts or parts[0].startswith("#") or len(parts) < 7:
                continue
            rows.append(
                {
                    "address": normalize_address(parts[0]),
                    "cc": parts[1],
                    "ret": parts[2],
                    "nparams": parts[4],
                    "params": parts[5],
                    "name": parts[6],
                }
            )
    return rows


def read_function_overrides(path: Path) -> dict[str, dict[str, str]]:
    if not path.exists():
        return {}
    with path.open("r", encoding="utf-8-sig", errors="replace", newline="") as stream:
        return {normalize_address(row["address"]): row for row in csv.DictReader(stream, delimiter="\t")}


def read_function_boundary_exclusions(path: Path) -> set[str]:
    """Read false function starts absorbed into a reviewed retail function body."""
    if not path.exists():
        return set()
    with path.open("r", encoding="utf-8-sig", errors="replace", newline="") as stream:
        return {
            normalize_address(row["address"])
            for row in csv.DictReader(stream, delimiter="\t")
            if row.get("address")
        }


def read_clean_coverage(path: Path) -> dict[str, bool]:
    result: dict[str, bool] = {}
    with path.open("r", encoding="utf-8-sig", errors="replace", newline="") as stream:
        for row in csv.DictReader(stream, delimiter="\t"):
            result[normalize_address(row["address"])] = row.get("ok") == "1"
    return result


def read_quality_tail(path: Path) -> dict[str, str]:
    result: dict[str, str] = {}
    if not path.exists():
        return result
    with path.open("r", encoding="utf-8-sig", errors="replace", newline="") as stream:
        for row in csv.DictReader(stream, delimiter="\t"):
            result[normalize_address(row["address"])] = row.get("confidence", "review")
    return result


def read_agent_sessions(paths: list[tuple[str, Path]]) -> dict[str, tuple[str, str]]:
    result: dict[str, tuple[str, str]] = {}
    for label, path in paths:
        if not path.exists():
            continue
        data = json.loads(path.read_text(encoding="utf-8-sig"))
        for key, value in data.get("functions", {}).items():
            result[normalize_address(key)] = (value.get("verdict", ""), label)
    return result


def index_agent_code(directories: list[Path]) -> dict[str, str]:
    result: dict[str, str] = {}
    for directory in directories:
        if not directory.exists():
            continue
        for path in directory.rglob("*.cpp"):
            match = re.match(r"0x([0-9a-fA-F]{8})_", path.name)
            if match:
                result[match.group(1).lower()] = str(path.resolve())
    return result


def read_compiled_candidates(path: Path) -> dict[str, dict[str, str]]:
    result: dict[str, dict[str, str]] = {}
    if not path.exists():
        return result
    with path.open("r", encoding="utf-8-sig", errors="replace", newline="") as stream:
        for row in csv.DictReader(stream, delimiter="\t"):
            if row.get("status") == "PASS":
                result[normalize_address(row["address"])] = row
    return result


def read_retail_parity(path: Path) -> dict[str, dict[str, str]]:
    if not path.exists():
        return {}
    with path.open("r", encoding="utf-8-sig", errors="replace", newline="") as stream:
        return {normalize_address(row["address"]): row for row in csv.DictReader(stream, delimiter="\t")}


def read_verified_boot_leaves(path: Path, root: Path) -> dict[str, dict[str, str]]:
    """Load independently gated exact leaves that bypass the bulk candidate queue."""
    if not path.exists():
        return {}
    accepted_grades = {"MATCH", "RELOCATION_MATCH"}
    result: dict[str, dict[str, str]] = {}
    with path.open("r", encoding="utf-8-sig", errors="replace", newline="") as stream:
        for row in csv.DictReader(stream, delimiter="\t"):
            grade = row.get("validated_grade", "")
            source = row.get("validated_source", "")
            if grade not in accepted_grades or not source:
                continue
            source_path = Path(source)
            if not source_path.is_absolute():
                source_path = root / source_path
            if not source_path.exists():
                continue
            result[normalize_address(row["target"])] = {
                "status": "PASS",
                "behavior_test": "PASS",
                "source": str(source_path.resolve()),
                "retail_parity": grade,
            }
    return result


def verified_lifts(root: Path) -> dict[str, tuple[str, str]]:
    """Current hand-verified Tier-0 oracle results."""
    proof_source = root / "lift" / "proofs" / "src"
    rows = {
        "00662030": ("matching", proof_source / "air_resistance.cpp"),
        "00ab5f00": ("matching", proof_source / "cmouse_dx.cpp"),
        "00ab5f10": ("matching", proof_source / "cmouse_dx.cpp"),
        "00ab5f20": ("matching", proof_source / "cmouse_dx.cpp"),
        "00ab5f30": ("matching", proof_source / "cmouse_dx.cpp"),
        "00ab5f40": ("matching", proof_source / "cmouse_dx.cpp"),
        "00ab5f50": ("matching", proof_source / "cmouse_dx.cpp"),
        "00ab5f70": ("matching", proof_source / "cmouse_dx.cpp"),
        "00ab5f80": ("matching", proof_source / "cmouse_dx.cpp"),
        "00ab5f90": ("matching", proof_source / "cmouse_dx.cpp"),
        "00ab46e0": ("matching", proof_source / "cmouse_dx.cpp"),
        "00a65d40": ("matching", proof_source / "cmemalloc.cpp"),
        "00a65de0": ("matching", proof_source / "cmemalloc.cpp"),
        "00a65e50": ("matching", proof_source / "cmemalloc.cpp"),
        "00a65e20": ("functional", proof_source / "cmemalloc.cpp"),
        "00a65d50": ("matching", proof_source / "cmemalloc.cpp"),
        "00a65d70": ("functional", proof_source / "cmemalloc.cpp"),
        "00a65df0": ("matching", proof_source / "cmemalloc.cpp"),
    }
    return {address: (grade, str(path.resolve())) for address, (grade, path) in rows.items()}


def write_tsv(path: Path, fieldnames: list[str], rows: list[dict[str, object]]) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    temp = path.with_suffix(path.suffix + ".tmp")
    with temp.open("w", encoding="utf-8", newline="") as stream:
        writer = csv.DictWriter(stream, delimiter="\t", fieldnames=fieldnames, lineterminator="\n")
        writer.writeheader()
        writer.writerows(rows)
    temp.replace(path)


def materialize_tier0(root: Path, rebuild: Path) -> None:
    source_dir = rebuild / "src" / "tier0"
    oracle_dir = rebuild / "oracles"
    source_dir.mkdir(parents=True, exist_ok=True)
    oracle_dir.mkdir(parents=True, exist_ok=True)
    for name in ("air_resistance.cpp", "cmouse_dx.cpp", "cmemalloc.cpp"):
        shutil.copy2(root / "lift" / "proofs" / "src" / name, source_dir / name)
    for name in ("cmouse_dx.expected.tsv", "cmemalloc.expected.tsv"):
        shutil.copy2(root / "lift" / "proofs" / "oracles" / name, oracle_dir / name)


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--root", type=Path, default=Path(__file__).resolve().parents[1])
    parser.add_argument("--out", type=Path)
    args = parser.parse_args()
    root = args.root.resolve()
    rebuild = (args.out or root / "rebuild").resolve()
    manifest_dir = rebuild / "manifest"
    manifest_dir.mkdir(parents=True, exist_ok=True)

    api_rows = read_engine_api(root / "ghidra_out" / "engine_api.tsv")
    generated_overrides = read_function_overrides(
        rebuild / "corrections" / "msvc_prototype_overrides.tsv"
    )
    manual_overrides = read_function_overrides(rebuild / "corrections" / "function_overrides.tsv")
    boundary_exclusions = read_function_boundary_exclusions(
        rebuild / "corrections" / "function_boundary_exclusions.tsv"
    )
    # Generated decorated-name recovery supplies broad ABI coverage; reviewed
    # manual rows remain authoritative for every overlapping address.
    overrides = {**generated_overrides, **manual_overrides}
    clean = read_clean_coverage(root / "ghidra_out" / "coverage.tsv")
    # The original sweep timed out on this giant function. A later 512 MB
    # payload retry completed and its 1,020,610-character output is retained.
    cutscene_retry = root / "ghidra_out" / "runcutscenemacro_full.c"
    if cutscene_retry.exists() and cutscene_retry.stat().st_size > 0:
        clean["00cbfb7d"] = True
    tail = read_quality_tail(root / "ghidra_out" / "naming_stragglers" / "manifest.tsv")
    sessions = read_agent_sessions(
        [
            ("primary", root / "lift" / "state" / "re-agent-progress.json"),
            ("retry", root / "lift" / "state" / "re-agent-retry-progress.json"),
            ("wave2", root / "lift" / "state" / "re-agent-wave2-progress.json"),
            ("wave3", root / "lift" / "state" / "re-agent-wave3-progress.json"),
        ]
    )
    agent_code = index_agent_code(
        [
            root / "lift" / "reports" / "primary" / "code",
            root / "lift" / "reports" / "retry" / "code",
            root / "lift" / "reports" / "wave2" / "code",
            root / "lift" / "reports" / "wave3" / "code",
        ]
    )
    compiled = read_compiled_candidates(rebuild / "compile-gate" / "vc71-compiled.tsv")
    parity = read_retail_parity(rebuild / "compile-gate" / "retail-parity.tsv")
    boot_leaves = read_verified_boot_leaves(
        rebuild / "integration" / "gfmain_calls.tsv", root
    )
    lifts = verified_lifts(root)

    functions: list[FunctionRow] = []
    for api in api_rows:
        address = api["address"]
        if address in boundary_exclusions:
            continue
        if address in overrides:
            override = overrides[address]
            api = {
                **api,
                "name": override.get("name") or api["name"],
                "cc": override.get("calling_convention") or api["cc"],
                "ret": override.get("return_type") or api["ret"],
                # Zero parameters and an empty parameter-type list are valid,
                # intentional corrections. Truthiness fallback silently kept
                # poisoned Ghidra parameters for no-argument overrides.
                "nparams": (
                    override["parameter_count"]
                    if override.get("parameter_count") != ""
                    else api["nparams"]
                ),
                "params": (
                    override.get("parameter_types", "")
                    if (
                        override.get("parameter_types", "") != ""
                        or override.get("parameter_count") == "0"
                    )
                    else api["params"]
                ),
            }
        cc = api["cc"]
        ret = api["ret"]
        params = api["params"]
        prototype_complete = cc not in ("", "unknown") and ret not in UNDEFINED_TYPES and "undefined" not in params
        agent_verdict, agent_source = sessions.get(address, ("", ""))
        lift_grade, lift_source = lifts.get(address, ("", ""))
        module = module_for_api(api["name"], cc, params)
        if address in overrides and overrides[address].get("module"):
            module = overrides[address]["module"]
        module = {
            "00662030": "CTCPhysicsBase",
        }.get(address, module)
        functions.append(
            FunctionRow(
                address=address,
                name=api["name"],
                module=module,
                calling_convention=cc,
                return_type=ret,
                parameter_count=int(api["nparams"]),
                parameter_types=params,
                prototype_complete=prototype_complete,
                clean_decompile="yes" if clean.get(address) is True else "no" if clean.get(address) is False else "unmeasured",
                naming_quality=f"review-{tail[address]}" if address in tail else "accepted",
                agent_verdict=agent_verdict,
                agent_source=agent_source,
                lift_grade=lift_grade,
                lift_source=lift_source,
            )
        )

    # Dataclass construction above deliberately keeps the manifest schema stable;
    # add candidate paths as a separate column below.
    function_rows: list[dict[str, object]] = []
    for row in functions:
        data = asdict(row)
        data["prototype_complete"] = int(row.prototype_complete)
        data["agent_source_path"] = agent_code.get(row.address, "")
        compiled_proof = compiled.get(row.address) or boot_leaves.get(row.address, {})
        parity_status = parity.get(row.address, {}).get("status", "")
        if not parity_status:
            parity_status = boot_leaves.get(row.address, {}).get("retail_parity", "")
        data["compiled_status"] = compiled_proof.get("status", "")
        data["behavior_test"] = compiled_proof.get("behavior_test", "")
        data["retail_parity"] = parity_status
        data["compiled_source"] = compiled_proof.get("source", "")
        function_rows.append(data)
    function_fields = list(function_rows[0].keys()) if function_rows else []
    write_tsv(manifest_dir / "functions.tsv", function_fields, function_rows)

    declared_modules: dict[str, int] = {}
    scaffold = root / "ghidra_out" / "decomp_module_scaffold.tsv"
    with scaffold.open("r", encoding="utf-8-sig", errors="replace") as stream:
        for line in stream:
            if not line.strip() or line.startswith("#"):
                continue
            name, count = line.rstrip("\r\n").split("\t", 1)
            declared_modules[name] = int(count)

    by_module: dict[str, list[FunctionRow]] = defaultdict(list)
    for row in functions:
        by_module[row.module].append(row)
    module_names = sorted(set(declared_modules) | set(by_module))
    module_rows: list[dict[str, object]] = []
    for name in module_names:
        rows = by_module.get(name, [])
        module_rows.append(
            {
                "module": name,
                "scaffold_function_count": declared_modules.get(name, 0),
                "catalog_function_count": len(rows),
                "prototype_complete": sum(row.prototype_complete for row in rows),
                "agent_pass": sum(row.agent_verdict == "PASS" for row in rows),
                "agent_fail": sum(row.agent_verdict == "FAIL" for row in rows),
                "compiled_candidates": sum(row.address in compiled for row in rows),
                "behavior_tested_candidates": sum(compiled.get(row.address, {}).get("behavior_test") == "PASS" for row in rows),
                "candidate_retail_matches": sum(parity.get(row.address, {}).get("status") == "MATCH" for row in rows),
                "candidate_relocation_matches": sum(parity.get(row.address, {}).get("status") == "RELOCATION_MATCH" for row in rows),
                "lift_matching": sum(row.lift_grade == "matching" for row in rows),
                "lift_functional": sum(row.lift_grade == "functional" for row in rows),
            }
        )
    write_tsv(manifest_dir / "modules.tsv", list(module_rows[0].keys()), module_rows)

    materialize_tier0(root, rebuild)
    summary = {
        "catalog_functions": len(functions),
        "modules": len(declared_modules),
        "calling_convention_known": sum(row.calling_convention not in ("", "unknown") for row in functions),
        "prototype_complete": sum(row.prototype_complete for row in functions),
        "naming_quality_review": len(tail),
        "agent_candidates": len(agent_code),
        "agent_pass": sum(row.agent_verdict == "PASS" for row in functions),
        "agent_fail": sum(row.agent_verdict == "FAIL" for row in functions),
        "compiled_candidates": len(compiled),
        "behavior_tested_candidates": sum(row.get("behavior_test") == "PASS" for row in compiled.values()),
        "candidate_retail_matches": sum(row.get("status") == "MATCH" for row in parity.values()),
        "candidate_relocation_matches": sum(row.get("status") == "RELOCATION_MATCH" for row in parity.values()),
        "lift_matching": sum(row.lift_grade == "matching" for row in functions),
        "lift_functional": sum(row.lift_grade == "functional" for row in functions),
        "lift_modules": 3,
        "clean_decompile_measured": len(clean),
        "clean_decompile_pass": sum(clean.values()),
        "vc71_sdk_probe_pass": any(
            "Result: **PASS**" in report.read_text(encoding="utf-8-sig", errors="replace")
            for report in (rebuild / "probes").glob("vc71-sdk-*.md")
        ),
    }
    temp_status = manifest_dir / "status.json.tmp"
    temp_status.write_text(json.dumps(summary, indent=2) + "\n", encoding="utf-8")
    temp_status.replace(manifest_dir / "status.json")
    print(json.dumps(summary, indent=2))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
