#!/usr/bin/env python3
"""Offline CGame::Play behavior and retail-parity check; no GUI/game launch.

The default gate permits only the documented two-byte functional residue.
--require-byte-match rejects that residue too. --baseline checks the archived
assembly oracle with the same behavior scenarios.
All generated products stay under work/cgame_play_check/.
"""
import argparse
import csv
import hashlib
import json
import os
from pathlib import Path
import subprocess
import sys

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "tools"))
os.environ.setdefault("LANDVERIFY_WORK", str(ROOT / "work/cgame_play_check/helpers"))
import pe_oracle
import verify_and_land as parity
from check_boot_object import CGAME_PLAY_RELOCATIONS, is_cgame_play_register_residue

# The older helper defaults to the shared checkout; keep this worktree isolated.
parity.ROOT = ROOT
ADDRESS = 0x00412F90


def run(command, env):
    result = subprocess.run(
        [str(arg) for arg in command], env=env, capture_output=True,
        text=True, timeout=60,
        creationflags=subprocess.CREATE_NO_WINDOW if os.name == "nt" else 0,
    )
    if result.returncode:
        raise RuntimeError(result.stdout + result.stderr)
    return result.stdout


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--baseline", action="store_true")
    parser.add_argument("--require-byte-match", action="store_true")
    parser.add_argument("--retail-exe", type=Path, default=pe_oracle.EXE)
    args = parser.parse_args()
    mode = "baseline" if args.baseline else "readable"
    directory = ROOT / "work/cgame_play_check" / mode
    directory.mkdir(parents=True, exist_ok=True)
    relative = "src/asm_bake" if args.baseline else "src/compiled"
    source = ROOT / "rebuild" / relative / "00/41/CGame_Play_00412f90.cpp"
    fixture = ROOT / "rebuild/tests/00/41/CGame_Play_00412f90_test.cpp"
    with (ROOT / "rebuild/integration/boot_oracles.tsv").open(
            encoding="utf-8-sig", newline="") as stream:
        oracle = next(row for row in csv.DictReader(stream, delimiter="\t")
                      if row["address"] == "00412f90")
    retail = bytes.fromhex(oracle["bytes"])
    executable = args.retail_exe.read_bytes()
    offset = pe_oracle.va_to_off(pe_oracle.pe_sections(executable), ADDRESS)
    if len(retail) != 394 or offset is None or executable[offset:offset+394] != retail:
        raise RuntimeError("Checked oracle disagrees with the installed retail executable")

    env = parity.env()
    options = ["/nologo", "/c", "/W3", "/MT", "/GS", "/O2", "/Oy"]
    obj = directory / "play.obj"
    test_obj = directory / "behavior.obj"
    test_exe = directory / "behavior.exe"
    run([parity.CL_EXE, *options, "/Fo" + str(obj), source], env)
    test_options = ["/DFABLETLC_CGAME_PLAY_ASM_BASELINE"] if args.baseline else []
    run([parity.CL_EXE, *options, *test_options, "/Fo" + str(test_obj), fixture], env)
    run([parity.VC / "bin/link.exe", "/nologo", "/subsystem:console",
         "/out:" + str(test_exe), obj, test_obj], env)
    behavior = run([test_exe], env).strip()
    if "FABLETLC_CGAME_PLAY_BEHAVIOR PASS" not in behavior:
        raise RuntimeError("Behavior pass marker missing: " + behavior)

    built, section, symbol = parity.obj_text(obj, "?Play@CGame")
    relocations = parity.obj_relocs(obj, section)
    # Reject a changed relocation map rather than masking arbitrary differences.
    relocation_map_matches = relocations == CGAME_PLAY_RELOCATIONS
    actual = parity.mask(built, relocations)
    expected = parity.mask(retail, relocations)
    differences = [i for i, (a, b) in enumerate(zip(actual, expected)) if a != b]
    matched = relocation_map_matches and actual == expected
    status = "RELOCATION_MATCH" if matched else "DIFFER"
    functional_residue = is_cgame_play_register_residue(
        "00412f90", expected, actual, relocations)
    accepted = matched or (functional_residue and not args.require_byte_match)
    disassembly = run([parity.OBJDUMP, "-dr", obj], env)
    (directory / "play-disassembly.txt").write_text(disassembly, encoding="utf-8")
    report = {
        "address": "00412f90", "mode": mode,
        "source": str(source.relative_to(ROOT)),
        "source_sha256": hashlib.sha256(source.read_bytes()).hexdigest(),
        "fixture_sha256": hashlib.sha256(fixture.read_bytes()).hexdigest(),
        "retail_sha256": hashlib.sha256(executable).hexdigest(),
        "oracle_matches_installed_retail": True,
        "symbol": symbol, "retail_bytes": len(retail), "compiled_bytes": len(built),
        "relocations": relocations, "relocation_map_matches": relocation_map_matches,
        "different_byte_offsets": [f"0x{i:X}" for i in differences],
        "behavior": behavior, "status": status,
        "functional_register_residue": functional_residue,
        "accepted": accepted, "requires_byte_match": args.require_byte_match,
        "compiler": str(parity.CL_EXE), "options": options,
        "grade": "asm_bake" if args.baseline else "functional",
    }
    report_path = directory / ("strict-report.json" if args.require_byte_match else "report.json")
    report_path.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(behavior)
    print(f"CGAME_PLAY {status} mode={mode} bytes={len(built)}/{len(retail)} "
          f"differences={report['different_byte_offsets']} relocations={len(relocations)}")
    print(report_path)
    print(f"CGAME_PLAY_ACCEPTANCE {'PASS' if accepted else 'FAIL'}")
    return 0 if accepted else 1


if __name__ == "__main__":
    sys.exit(main())
