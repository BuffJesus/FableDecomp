#!/usr/bin/env python3
"""Validate native entity destructor exports and their shared cleanup body."""
from __future__ import annotations

import json
from pathlib import Path


def audit(workspace: Path, snapshot_path: Path) -> dict:
    snapshot = json.loads(snapshot_path.read_text(encoding="utf-8-sig"))
    cleanup = snapshot["sharedEntityCleanup"]
    expected = {row["destructor"].upper() for row in snapshot["entities"].values()}
    export_dir = workspace / "ghidra_out/script_recovery/native_destructors"
    actual = {path.stem.upper(): path for path in export_dir.glob("0x*.c")}
    errors: list[str] = []
    if set(actual) != expected:
        errors.append(
            f"destructor exports differ: missing={sorted(expected-set(actual))}, "
            f"extra={sorted(set(actual)-expected)}")
    bodies = set()
    for address in sorted(expected & set(actual)):
        text = actual[address].read_text(encoding="utf-8", errors="replace")
        lines = text.splitlines()
        if not lines or f"function entry {address.lower()}" not in lines[0].lower() or "DecompFuncsToDirectory.java" not in lines[0]:
            errors.append(f"{address}: invalid provenance header")
        body = "\n".join(lines[1:]).strip()
        bodies.add(body)
        for fragment in (
                "NUISystem::CFrontEndScreen::~CFrontEndScreen",
                "if ((param_1 & 1) != 0)", "operator_delete(this)", "return this"):
            if fragment not in body:
                errors.append(f"{address}: missing shared entity cleanup fragment {fragment}")
    if len(bodies) != 1:
        errors.append(f"entity destructor bodies are not identical ({len(bodies)} variants)")
    cleanup_path = workspace / "ghidra_out/script_recovery/native_support" / f"{cleanup['address']}.c"
    if not cleanup_path.exists():
        errors.append(f"missing shared cleanup export {cleanup_path.name}")
    else:
        support = cleanup_path.read_text(encoding="utf-8", errors="replace")
        header = support.splitlines()[0] if support.splitlines() else ""
        if cleanup["address"].lower() not in header.lower() or "DecompFuncsToDirectory.java" not in header:
            errors.append("shared cleanup export has invalid provenance header")
        for fragment in (
                cleanup["ghidraDonorName"],
                cleanup["baseVtable"].lower().removeprefix("0x"),
                cleanup["scriptThingVtable"].lower().removeprefix("0x"),
                "*(undefined4 *)(this + 0xc) = 0",
                "*(undefined4 *)(this + 0x10) = 0"):
            if fragment not in support:
                errors.append(f"shared cleanup export missing fragment {fragment}")
    manifest = (workspace / "rebuild/manifest/functions.tsv").read_text(
        encoding="utf-8", errors="replace")
    tail = cleanup["tailTarget"].lower().removeprefix("0x")
    tail_rows = [line for line in manifest.splitlines() if line.lower().startswith(tail + "\t")]
    if len(tail_rows) != 1 or "CBase_RestoreVTable" not in tail_rows[0] or "RELOCATION_MATCH" not in tail_rows[0]:
        errors.append(f"shared cleanup tail target {cleanup['tailTarget']} lacks exact CBase manifest evidence")
    return {
        "schema": "new-oakvale-destructor-audit/0.1",
        "entityDestructors": len(expected),
        "exportedDecompiles": len(actual),
        "uniqueCleanupBodies": len(bodies),
        "sharedCleanupTarget": cleanup["address"],
        "sharedCleanupCallers": len(expected),
        "supportDecompiles": int(cleanup_path.exists()),
        "errors": errors,
        "ok": not errors,
    }


if __name__ == "__main__":
    root = Path(__file__).resolve().parents[2]
    print(json.dumps(audit(
        root, root / "ghidra_out/script_recovery/new_oakvale_entity_vtables.json"), indent=2))
