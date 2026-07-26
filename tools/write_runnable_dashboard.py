#!/usr/bin/env python3
"""Write the deterministic runnable-reconstruction readiness dashboard."""

from __future__ import annotations

import argparse
import csv
from pathlib import Path


def read_tsv(path: Path) -> list[dict[str, str]]:
    with path.open("r", encoding="utf-8-sig", newline="") as stream:
        return list(csv.DictReader(stream, delimiter="\t"))


def repository_path(root: Path, value: str) -> str:
    if not value:
        return ""

    path = Path(value)
    if path.is_absolute():
        try:
            path = path.relative_to(root)
        except ValueError:
            return value.replace("\\", "/")
    return path.as_posix()


def markdown_source(path: str) -> str:
    if not path:
        return "—"
    # RUNNABLE.md lives under rebuild/, while manifest paths are repository-root
    # relative. Keep the generated links valid on GitHub and in local viewers.
    return f"[source](../{path})"


def source_state(function: dict[str, str], parity: dict[str, str]) -> str:
    compiled_source = function.get("compiled_source", "")
    address = function["address"].lower()
    if compiled_source:
        retail_status = parity.get(address, function.get("retail_parity", ""))
        if retail_status in {"MATCH", "RELOCATION_MATCH"}:
            return retail_status
        if function.get("behavior_test"):
            return "functional"
        return "compiled"

    if function.get("agent_source_path"):
        verdict = function.get("agent_verdict") or "unreviewed"
        return f"agent-{verdict.lower()}"
    return "not authored"


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--root",
        type=Path,
        default=Path(__file__).resolve().parents[1],
        help="repository root",
    )
    args = parser.parse_args()
    root = args.root.resolve()

    functions = {
        row["address"].lower(): row
        for row in read_tsv(root / "rebuild/manifest/functions.tsv")
    }
    coverage = {
        row["address"].lower(): row
        for row in read_tsv(root / "ghidra_out/coverage.tsv")
    }
    parity = {
        row["address"].lower(): row["status"]
        for row in read_tsv(root / "rebuild/compile-gate/retail-parity.tsv")
    }
    boot_chain = read_tsv(root / "rebuild/integration/boot_chain.tsv")
    gfmain_phases = read_tsv(root / "rebuild/integration/gfmain_phases.tsv")
    gfmain_calls = read_tsv(root / "rebuild/integration/gfmain_calls.tsv")

    lines = [
        "# Runnable reconstruction",
        "",
        "This dashboard tracks the shortest honest path from independently verified",
        "functions to a reconstructed executable. It does not count a diagnostic",
        "bootstrap as a running game.",
        "",
        "## Current executable milestone",
        "",
        "**Stage 1 now reaches the GFMain boundary:** VC7.1 links and runs a",
        "Win32 GUI executable through the recovered `WinMain @ 0x00403480`.",
        "Its 141-byte body is an exact relocation-normalized retail match, and",
        "the fixture proves both first-instance and duplicate-instance paths,",
        "the 200 KiB MicroThread stack handoff, and the fastcall GFMain ABI.",
        "",
        "The first Stage-2 leaf is also promoted:",
        "`GFInitialise_SetupProgressDisplay @ 0x00413120` is a 128-byte",
        "relocation-normalized match with allocation, null-allocation, reference",
        "release, and rejected-initialization behavior covered. It is not yet",
        "reachable through the reconstructed GFMain/GFInitialise path.",
        "",
        "GFMain Phase 1 now has its filesystem pair promoted:",
        "`CAFile::GetProjectPath @ 0x00997510` (146 bytes) and",
        "`CAFile::SetCurrentPath @ 0x009974F0` (30 bytes) are",
        "relocation-normalized matches. Focused fixtures prove executable-directory",
        "discovery and the recovered update ordering: the OS working directory",
        "changes before the engine updates its cached current-path string.",
        "The folded 15-byte `CWideString` default constructor and 68-byte",
        "reference-counted destructor are proven too, bringing Phase 1 to four",
        "of nine direct callees.",
        "",
        "**Stage 0 remains the smallest linker proof:** VC7.1 links a console",
        "PE containing",
        "`MemCmp_Unsigned16 @ 0x00403C60`, whose function body is byte-identical",
        "to retail. The executable invokes that function and verifies its behavior.",
        "",
        "```powershell",
        "powershell -ExecutionPolicy Bypass -File rebuild/build_bootstrap.ps1",
        "```",
        "",
        "Expected terminal markers include `FABLETLC_BOOTSTRAP_STAGE0 PASS`,",
        "`FABLETLC_WINMAIN_BEHAVIOR PASS`,",
        "`FABLETLC_PROGRESS_SETUP_BEHAVIOR PASS`,",
        "`FABLETLC_SET_CURRENT_PATH_BEHAVIOR PASS`,",
        "`FABLETLC_GET_PROJECT_PATH_BEHAVIOR PASS`,",
        "`FABLETLC_WIDE_STRING_CONSTRUCTOR_BEHAVIOR PASS`,",
        "`FABLETLC_WIDE_STRING_DESTRUCTOR_BEHAVIOR PASS`, and",
        "`STAGE1_STARTUP PASS`.",
        "Generated products stay under the ignored `rebuild/build/` tree.",
        "",
        "Stage 1 still stops at an instrumented GFMain stub. It does **not** yet",
        "initialize Lionhead engine services, open the retail window, load assets,",
        "or enter the game loop.",
        "",
        "## Retail boot spine",
        "",
        "| Order | Address | Role | Retail size | Current source grade | Source | Blocking fact |",
        "|---:|---:|---|---:|---|---|---|",
    ]

    for item in boot_chain:
        address = item["address"].lower()
        function = functions.get(address, {"address": address})
        size = coverage.get(address, {}).get("size", "—")
        source = (
            item.get("validated_source")
            or
            function.get("compiled_source")
            or function.get("agent_source_path")
            or ""
        )
        source = repository_path(root, source)
        state = item.get("validated_grade") or source_state(function, parity)
        lines.append(
            "| {order} | `0x{address}` | {role} | {size} | `{state}` | {source} | {blocker} |".format(
                order=item["order"],
                address=address.upper(),
                role=item["role"],
                size=size,
                state=state,
                source=markdown_source(source),
                blocker=item["blocker"],
            )
        )

    lines.extend(
        [
            "",
            "## Next dependency closure",
            "",
        ]
    )
    for item in boot_chain:
        lines.append(
            f"{item['order']}. **{item['role']} (`0x{item['address'].upper()}`):** "
            f"{item['next_action']}"
        )

    lines.extend(
        [
            "",
            "## GFMain dependency phases",
            "",
            "The 3,952-byte coordinator is split by observed retail call clusters.",
            "These are integration units, not invented retail functions.",
            "",
            "| Phase | Address range | Role | Direct calls | Unique targets | Proven | Anchors |",
            "|---:|---|---|---:|---:|---:|---|",
        ]
    )
    for phase in gfmain_phases:
        phase_calls = [
            item for item in gfmain_calls if item["phase"] == phase["phase"]
        ]
        unique_targets = {item["target"] for item in phase_calls}
        proven_calls = sum(
            bool(item.get("validated_grade")) for item in phase_calls
        )
        lines.append(
            "| {phase} | `0x{start}`-`0x{end}` | {role} | {calls} | "
            "{targets} | {proven} | {anchors} |".format(
                phase=phase["phase"],
                start=phase["start"].upper(),
                end=phase["end"].upper(),
                role=phase["role"],
                calls=len(phase_calls),
                targets=len(unique_targets),
                proven=proven_calls,
                anchors=phase["anchors"],
            )
        )

    verified_dependencies = [
        item for item in gfmain_calls if item.get("validated_grade")
    ]
    lines.extend(
        [
            "",
            "### Verified GFMain dependencies",
            "",
            "| Phase | Call site | Target | Function | Grade | Source |",
            "|---:|---:|---:|---|---|---|",
        ]
    )
    for item in verified_dependencies:
        source = repository_path(root, item.get("validated_source") or "")
        lines.append(
            "| {phase} | `0x{call_site}` | `0x{target}` | {name} | "
            "`{grade}` | {source} |".format(
                phase=item["phase"],
                call_site=item["call_site"].upper(),
                target=item["target"].upper(),
                name=item["analysis_name"],
                grade=item["validated_grade"],
                source=markdown_source(source),
            )
        )

    lines.extend(["", "Phase closure order:"])
    for phase in gfmain_phases:
        lines.append(
            f"{phase['phase']}. **{phase['role']}:** {phase['next_action']}"
        )

    lines.extend(
        [
            "",
            "## Milestone definitions",
            "",
            "- **Stage 0 — linked code:** an owned x86 executable runs at least one",
            "  behavior-gated, retail-matching function. **Implemented.**",
            "- **Stage 1 — owned startup:** reconstructed WinMain reaches an",
            "  instrumented GFMain boundary without borrowing retail code.",
            "  **Implemented; exact CRT-entry parity remains a separate task.**",
            "- **Stage 2 — engine bootstrap:** reconstructed GFMain/GFInitialise reaches",
            "  the first visible progress-display state with controlled platform shims.",
            "- **Stage 3 — data bootstrap:** compiled definitions and core archives load",
            "  far enough to create the main game component.",
            "- **Stage 4 — game loop:** the reconstructed process pumps input, updates a",
            "  minimal world, renders frames, and shuts down cleanly.",
            "- **Stage 5 — playable slice:** a controlled region can be loaded and the",
            "  hero can be moved and saved.",
            "",
            "Modern x64 APIs may continue as small oracle-backed experiments, but menus,",
            "installer work, enhancements, and broad port refactors are intentionally",
            "behind Stage 4.",
            "",
            "_Generated by `tools/write_runnable_dashboard.py` from the function, parity,",
            "coverage, and boot-chain manifests._",
            "",
        ]
    )

    output = root / "rebuild/RUNNABLE.md"
    output.write_text("\n".join(lines), encoding="utf-8", newline="\n")
    print(f"WROTE {output}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
