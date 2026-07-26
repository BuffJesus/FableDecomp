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
    matching_grades = {"MATCH", "EXACT", "RELOCATION_MATCH"}
    proven_calls = sum(
        row.get("validated_grade") in matching_grades for row in gfmain_calls
    )
    phase3_calls = [row for row in gfmain_calls if row.get("phase") == "3"]
    phase3_proven = sum(
        row.get("validated_grade") in matching_grades for row in phase3_calls
    )
    authored_phases = sum(
        (root / "rebuild" / "integration" / f"gfmain_phase{phase}.cpp").exists()
        for phase in range(1, len(gfmain_phases) + 1)
    )

    lines = [
        "# Runnable reconstruction",
        "",
        "This dashboard tracks the shortest honest path from independently verified",
        "functions to a reconstructed executable. It does not count a diagnostic",
        "bootstrap as a running game.",
        "",
        "## Boot-path progress",
        "",
        "| Measure | Proven | Scope |",
        "|---|---:|---|",
        (
            f"| GFMain direct-call sites | {proven_calls}/{len(gfmain_calls)} "
            f"({100.0 * proven_calls / len(gfmain_calls):.2f}%) | "
            "All ten call clusters; repeated call sites count separately |"
        ),
        (
            f"| Callable authored GFMain phases | {authored_phases}/{len(gfmain_phases)} "
            f"({100.0 * authored_phases / len(gfmain_phases):.2f}%) | "
            "Integration checkpoints, not retail GFMain byte parity |"
        ),
        (
            f"| Current Phase 3 direct calls | {phase3_proven}/{len(phase3_calls)} "
            f"({100.0 * phase3_proven / len(phase3_calls):.2f}%) | "
            "Settings, persistence, and IME cluster |"
        ),
        "",
        "These are dependency counters, not an estimate of engineering time or total",
        "game completion. Whole-executable verified parity remains the stricter public",
        "percentage in the root README.",
        "",
        "## Current executable milestone",
        "",
        "**The Stage 3 checkpoint now runs GFMain Phases 1 and 2:** VC7.1 links and runs a",
        "Win32 GUI executable through the recovered `WinMain @ 0x00403480`",
        "and authored integration units for retail `0x00402510-0x00402668`.",
        "Its 141-byte body is an exact relocation-normalized retail match, and",
        "the fixture proves both first-instance and duplicate-instance paths,",
        "the 200 KiB MicroThread stack handoff, and the fastcall GFMain ABI.",
        "Phase 1 constructs the recovered system defaults, establishes the",
        "executable/project path with promoted retail functions, crosses a",
        "counted console-variable boundary, then continues through the basic-install",
        "and failure-policy sequence to the Phase 3 boundary.",
        "",
        "**A visible authored checkpoint is now runnable:**",
        "`FableTLC-Reconstruction-VisualCheckpoint.exe` follows that same matched",
        "`WinMain` and reconstructed Phase 1/2 path, invokes the retail-matched full",
        "`GFInitialise` coordinator and its retail-matched progress-display leaf through",
        "an explicit engine boundary, then opens a responsive 1280x720 Win32 window.",
        "When a retail `frontend.big` is available, the build decodes",
        "`FRONTEND_BACKDROP_01` through the recovered Lionhead-LZO/DXT1 tooling,",
        "crops it to its authored 640x480 frame, and embeds it in the executable.",
        "The authored project image remains a dependency-safe fallback. This is the",
        "first genuine game-derived image in the reconstructed process. An authored",
        "D3D9 bridge now uploads it as a managed texture. Two retail-shaped triangle",
        "records pass through the recovered Render2D batch planner, whose triangle-list",
        "flush is executed on a real Win32 D3D9 device; GDI is retained only as a",
        "failure fallback. Texture binding now executes the exact recovered 79-byte",
        "`CRenderManagerCore::AttachTextureToStage` body, including its cache and",
        "active-stage bookkeeping. The exact 167-byte",
        "`CRenderStateManager::RealiseRenderState` body also drains ten queued render,",
        "texture-stage, and sampler states for the live draw. Other dependency dispatch",
        "now includes the recovered 11-byte `CRenderManagerCore::SetAWindow` ownership",
        "hop into a compact D3D9 viewport endpoint. Remaining dispatch is still authored",
        "rather than the complete Lionhead coordinator, runtime archive loader, or game",
        "loop.",
        "",
        "The first parent-coordinator seam is now canonical too:",
        "`FableBuildRender2DBatchPlan` models the recovered 0x3C-byte draw-record",
        "layout and exact normal/text batching decisions from",
        "`CRenderManager2D::Render2DDrawList @ 0x009DA9F0`. Its VC7.1 fixture covers",
        "empty and stable queues, state/topology splits, text interruption and shader",
        "reapplication, output overflow, and retail's exact 2001+1 primitive edge.",
        "This is a behavior-proven integration seam, not a byte-parity claim for the",
        "3,344-byte parent coordinator.",
        "",
        "A second behavior seam now gates its surrounding renderer lifecycle: the",
        "empty-list epilogue, 13 tracked state requests, conditional shader changes,",
        "dirty uploads, attach/realise/DrawPrimitiveUP flush order, queue cleanup,",
        "capture/layout restoration, and texture ownership teardown. The gate also",
        "locks the recovered combined-projection dirty bit to `0x80`.",
        "The visible D3D9 checkpoint now executes both seams: planner output is",
        "translated into lifecycle flushes, and successful presentation requires the",
        "adapter to observe and complete a real `DrawPrimitiveUP` event.",
        "",
        "The full GFInitialise coordinator is promoted and connected:",
        "`GFInitialise @ 0x004022B0` is a 311-byte relocation-normalized match with",
        "root discovery, texture-depth fallback, display clamping, player registration,",
        "progress setup, and both return paths covered. Its 128-byte",
        "`GFInitialise_SetupProgressDisplay @ 0x00413120` leaf independently matches",
        "and covers allocation, null-allocation, reference release, and rejected",
        "initialization. Authored boundary objects still stand in for the unrecovered",
        "engine singleton graph and renderer.",
        "",
        "GFMain Phase 1 now has its filesystem pair promoted:",
        "`CAFile::GetProjectPath @ 0x00997510` (146 bytes) and",
        "`CAFile::SetCurrentPath @ 0x009974F0` (30 bytes) are",
        "relocation-normalized matches. Focused fixtures prove executable-directory",
        "discovery and the recovered update ordering: the OS working directory",
        "changes before the engine updates its cached current-path string.",
        "The folded `CWideString` and `CCharString` lifetime pairs are proven",
        "too and grouped in a shared string-domain header. The PDB lineage",
        "resolves the one-byte retail no-op as `NProfileTimer::StartProfile`,",
        "bringing Phase 1 to seven of nine direct callees at relocation match.",
        "The nested default `CCharString` constructor is retail-matched, while",
        "`CSystemManagerInit` now passes a focused layout/defaults/construction-order",
        "fixture. Its 265-byte object has one documented instruction-scheduling",
        "residue: `lea ecx,[esi+0x5c]` is emitted earlier than retail.",
        "Phase 2 now has all seven direct calls proven: the repeated string/profile",
        "leaves, `NProfileTimer::EndProfile`, the async failure-policy encoder,",
        "the TLC startup-latch clear, and the 69-byte counted `CFileInstaller`",
        "singleton retrieval path.",
        "The authored Phase 2 integration fixture proves setting propagation,",
        "optional installer setup, startup-latch handling, async failure policy,",
        "and balanced counted ownership on both enabled and skipped paths.",
        "Phase 3 has 21 of 34 direct calls proven. The first correction replaces",
        "a false donor `GetActionName` label with a TLC-specific no-argument",
        "default-language factory. Its readable 19-byte body constructs `\"English\"`",
        "in the fastcall hidden return slot and now has a real lifetime fixture.",
        "The next corrected call is `CCharString::ToWideString`: its 45-byte body",
        "and the 13-byte conversion factory plus 66-byte counted-storage copy",
        "constructor all relocation-match and pass focused ownership fixtures.",
        "The shared string/profile targets, two seven-byte text-alignment setters,",
        "and two folded seven-byte CBase vtable restores remain proven as well.",
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
        "`FABLETLC_WIDE_STRING_DESTRUCTOR_BEHAVIOR PASS`,",
        "`FABLETLC_CHAR_STRING_CONSTRUCTOR_BEHAVIOR PASS`,",
        "`FABLETLC_CHAR_STRING_DESTRUCTOR_BEHAVIOR PASS`,",
        "`FABLETLC_PROFILE_START_BEHAVIOR PASS`,",
        "`FABLETLC_CHAR_STRING_DEFAULT_CONSTRUCTOR_BEHAVIOR PASS`,",
        "`FABLETLC_SYSTEM_MANAGER_INIT_BEHAVIOR PASS`,",
        "`FABLETLC_PROFILE_END_BEHAVIOR PASS`,",
        "`FABLETLC_ASYNC_FAILURE_HANDLING_BEHAVIOR PASS`,",
        "`FABLETLC_STARTUP_LATCH_BEHAVIOR PASS`,",
        "`FABLETLC_FILE_INSTALLER_GET_BEHAVIOR PASS`,",
        "`FABLETLC_DEFAULT_LANGUAGE_NAME_BEHAVIOR PASS`,",
        "`FABLETLC_WIDE_STRING_FROM_CHAR_FACTORY_BEHAVIOR PASS`,",
        "`FABLETLC_WIDE_STRING_COPY_CONSTRUCTOR_BEHAVIOR PASS`,",
        "`FABLETLC_CHAR_TO_WIDE_STRING_BEHAVIOR PASS`,",
        "`FABLETLC_PRIMARY_LEFT_ALIGNMENT_BEHAVIOR PASS`,",
        "`FABLETLC_SECONDARY_LEFT_ALIGNMENT_BEHAVIOR PASS`,",
        "`FABLETLC_CBASE_RESTORE_A_BEHAVIOR PASS`,",
        "`FABLETLC_CBASE_RESTORE_B_BEHAVIOR PASS`,",
        "`FABLETLC_GFMAIN_PHASE1_BEHAVIOR PASS`,",
        "`FABLETLC_GFMAIN_PHASE2_BEHAVIOR PASS`,",
        "`FABLETLC_GFINITIALISE_PROGRESS_PHASE_BEHAVIOR PASS`,",
        "`FABLETLC_VISUAL_BOOT_BEHAVIOR PASS`,",
        "`FABLETLC_RENDER2D_BATCH_PLAN PASS`,",
        "`FABLETLC_RENDER2D_DRAW_LIST_ADAPTER PASS`,",
        "`STAGE3_STARTUP PASS`, and",
        "`GFINITIALISE_PROGRESS_INTEGRATION PASS`, and",
        "`VISUAL_BOOT_CHECKPOINT PASS`.",
        "Generated products stay under the ignored `rebuild/build/` tree.",
        "",
        "After the Release build, the live GPU presentation gate is:",
        "",
        "```powershell",
        "powershell -ExecutionPolicy Bypass -File rebuild/smoke_visual_checkpoint.ps1",
        "```",
        "",
        "It requires a `D3D9 Presented` window title, sends `WM_CLOSE`, and requires",
        "a clean zero exit.",
        "",
        "Stages 2 and 3 use explicit integration boundaries and are not claimed as a",
        "retail-matching GFMain. The visual checkpoint can now present one build-time",
        "extracted retail asset through D3D9, but it does **not** yet submit that image",
        "through the recovered Lionhead renderer, initialize the complete engine-service",
        "graph, load archives at runtime, play video, or enter the game loop.",
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
            item.get("validated_grade") in {"MATCH", "RELOCATION_MATCH"}
            for item in phase_calls
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
            "  **The Phase 1 checkpoint and an authored visual shell are implemented;",
            "  recovered retail progress-display wiring remains ahead.**",
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
