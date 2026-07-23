#!/usr/bin/env python3
"""Render the multi-stage full-decompilation dashboard from rebuild status."""
from __future__ import annotations

import argparse
import json
from datetime import datetime, timezone
from pathlib import Path


def pct(value: int, total: int) -> str:
    return f"{(100.0 * value / total):.3f}%" if total else "n/a"


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--root", type=Path, default=Path(__file__).resolve().parents[1])
    args = parser.parse_args()
    root = args.root.resolve()
    rebuild = root / "rebuild"
    status_path = rebuild / "manifest" / "status.json"
    status = json.loads(status_path.read_text(encoding="utf-8"))
    gate_summary_path = rebuild / "compile-gate" / "summary.json"
    gate = json.loads(gate_summary_path.read_text(encoding="utf-8")) if gate_summary_path.exists() else {}
    signature_path = rebuild / "compile-gate" / "signature-audit.json"
    signature = json.loads(signature_path.read_text(encoding="utf-8")) if signature_path.exists() else {}
    naming_triage_path = root / "ghidra_out" / "naming_stragglers" / "quality_triage.json"
    naming_triage = json.loads(naming_triage_path.read_text(encoding="utf-8")) if naming_triage_path.exists() else {}
    total = status["catalog_functions"]
    accepted_names = total - status["naming_quality_review"]
    verified = (
        status["lift_matching"]
        + status["lift_functional"]
        + status.get("candidate_retail_matches", 0)
        + status.get("candidate_relocation_matches", 0)
    )
    byte_identical = status["lift_matching"] + status.get("candidate_retail_matches", 0)
    structurally_different = (
        status.get("behavior_tested_candidates", 0)
        - status.get("candidate_retail_matches", 0)
        - status.get("candidate_relocation_matches", 0)
    )
    timestamp = datetime.now().astimezone().isoformat(timespec="seconds")
    dashboard = {
        "generated_at": timestamp,
        **status,
        "mechanically_named": total,
        "accepted_name_quality": accepted_names,
        "verified_lifted": verified,
        "candidate_integrity_pass": gate.get("integrity_pass", 0),
        "candidate_host_syntax_pass": gate.get("host_cpp20_syntax_pass", 0),
    }
    json_temp = rebuild / "COVERAGE.json.tmp"
    json_temp.write_text(json.dumps(dashboard, indent=2) + "\n", encoding="utf-8")
    json_temp.replace(rebuild / "COVERAGE.json")

    lines = [
        "# Full-decompilation coverage dashboard",
        "",
        f"Generated: `{timestamp}`",
        "",
        "| Stage | Complete | Denominator | Coverage | Meaning |",
        "|---|---:|---:|---:|---|",
        f"| Clean Ghidra pseudo-C (measured sweep) | {status['clean_decompile_pass']:,} | {status['clean_decompile_measured']:,} | {pct(status['clean_decompile_pass'], status['clean_decompile_measured'])} | Decompiler completed; not necessarily compilable C++ |",
        f"| Mechanically named | {total:,} | {total:,} | 100.000% | No default `FUN_*` labels |",
        f"| Accepted naming quality | {accepted_names:,} | {total:,} | {pct(accepted_names, total)} | Excludes current low-confidence/address-suffixed review tail |",
        f"| Usable reconstruction/navigation names | {naming_triage.get('usable_navigation_names', accepted_names):,} | {total:,} | {pct(naming_triage.get('usable_navigation_names', accepted_names), total)} | Includes explicitly classified compiler/template family labels; does not claim original symbols |",
        f"| Calling convention known | {status['calling_convention_known']:,} | {total:,} | {pct(status['calling_convention_known'], total)} | Required for ABI-correct declarations |",
        f"| Complete non-undefined prototype | {status['prototype_complete']:,} | {total:,} | {pct(status['prototype_complete'], total)} | Known CC, return, and parameter types |",
        f"| Auto-RE candidate source | {status['agent_candidates']:,} | {total:,} | {pct(status['agent_candidates'], total)} | Generated/reviewed candidate; not compile-verified |",
        f"| Auto-RE checker PASS | {status['agent_pass']:,} | {total:,} | {pct(status['agent_pass'], total)} | Structural checker grade only |",
        f"| Auto-RE source integrity | {gate.get('integrity_pass', 0):,} | {gate.get('candidates', status['agent_candidates']):,} | {pct(gate.get('integrity_pass', 0), gate.get('candidates', status['agent_candidates']))} | Balanced, non-empty source snapshots |",
        f"| Host C++20 syntax pass | {gate.get('host_cpp20_syntax_pass', 0):,} | {gate.get('candidates', status['agent_candidates']):,} | {pct(gate.get('host_cpp20_syntax_pass', 0), gate.get('candidates', status['agent_candidates']))} | Strict 32-bit translation unit with real declarations |",
        f"| Candidate signature audit clean | {signature.get('pass', 0):,} | {signature.get('candidates', status['agent_candidates']):,} | {pct(signature.get('pass', 0), signature.get('candidates', status['agent_candidates']))} | Definition agrees with manifest owner/name/shape and has unique linkage |",
        f"| VC7.1 compiled candidate | {status.get('compiled_candidates', 0):,} | {total:,} | {pct(status.get('compiled_candidates', 0), total)} | Compiles to an object; behavioral parity not yet claimed |",
        f"| Behavior-tested candidate | {status.get('behavior_tested_candidates', 0):,} | {total:,} | {pct(status.get('behavior_tested_candidates', 0), total)} | Local branch/layout oracle passes; retail parity not yet claimed |",
        f"| Candidate retail `.text` match | {status.get('candidate_retail_matches', 0):,} | {total:,} | {pct(status.get('candidate_retail_matches', 0), total)} | Generated-candidate track exact matches awaiting promotion |",
        f"| Candidate relocation-masked `.text` match | {status.get('candidate_relocation_matches', 0):,} | {total:,} | {pct(status.get('candidate_relocation_matches', 0), total)} | Exact instruction bytes/layout after masking expected COFF linker fields |",
        f"| Verified functional or matching C++ | {verified:,} | {total:,} | {pct(verified, total)} | Compiled under VC7.1 and checked against retail |",
        f"| Byte-identical C++ | {byte_identical:,} | {total:,} | {pct(byte_identical, total)} | Exact `.text` match |",
        f"| Lifted modules | {status['lift_modules']:,} | {status['modules']:,} | {pct(status['lift_modules'], status['modules'])} | Module-level reconstruction proof |",
        "",
        "## Interpretation",
        "",
        "The reverse-engineering database is near complete, while functional source reconstruction remains at proof-of-pipeline scale. Auto-RE output is deliberately not counted as reconstructed source until it compiles.",
        "",
        "The clean-decompile sweep predates later function discovery, so its denominator is 49,082 while the current catalog contains 49,553 functions. The dashboard does not silently extrapolate the missing 471 measurements.",
        "",
        "## Current gates",
        "",
        f"- {total - status['prototype_complete']:,} functions still lack a complete non-undefined prototype.",
        f"- {total - status['calling_convention_known']:,} functions still have an unknown calling convention.",
        f"- {naming_triage.get('remaining_semantic_review', status['naming_quality_review']):,} names remain in the semantic/accessor review tail; donor proposals and the invalid boundary are tracked separately.",
        f"- {status['agent_candidates'] - status.get('compiled_candidates', 0):,} auto-RE candidates still need a VC7.1-compatible translation unit.",
        f"- {signature.get('review', 0):,} original generated candidate definitions still disagree with corrected manifest identities or parameter shapes; curated replacements are tracked separately.",
        f"- {structurally_different:,} behavior-tested candidates still differ structurally from the retail parity oracle.",
    ]
    if status.get("vc71_sdk_probe_pass"):
        lines.append("- VC7.1 + Win32 + Direct3D 9 compile/link/runtime SDK probe: **resolved and passing**.")
    else:
        lines.append("- Real Win32/DirectX engine translation units still need a passing VC7.1 Platform SDK probe.")
    md_temp = rebuild / "COVERAGE.md.tmp"
    md_temp.write_text("\n".join(lines) + "\n", encoding="utf-8")
    md_temp.replace(rebuild / "COVERAGE.md")
    print(f"wrote {rebuild / 'COVERAGE.md'}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
