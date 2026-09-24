#!/usr/bin/env python3
"""Offline retail parity and behavior checks for the frontend's CStopWatch."""
import argparse
import json
from pathlib import Path
import sys

from check_frontend_startup import digest, relocation_symbols
from check_cgame_play import ROOT, parity, pe_oracle, run


def bind_clock_doubles(obj, env, counter=True):
    """Give the fixture strong definitions for Win32 import slots.

    /alternatename is insufficient: the CRT's kernel32 import library can resolve
    the real clocks first. Rename only fixture data symbols, never engine code.
    """
    renames = ["--redefine-sym",
               "_FableTestFrequencyImport=__imp__QueryPerformanceFrequency@4"]
    if counter:
        renames += ["--redefine-sym",
                    "_FableTestCounterImport=__imp__QueryPerformanceCounter@4"]
    run([Path(parity.OBJDUMP).with_name("objcopy.exe"), *renames, obj], env)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--require-byte-match", action="store_true")
    parser.add_argument("--retail-exe", type=Path, default=pe_oracle.EXE)
    args = parser.parse_args()
    contract = json.loads((ROOT / "rebuild/integration/stopwatch_contract.json").read_text())
    directory = ROOT / "work/stopwatch_check"
    directory.mkdir(parents=True, exist_ok=True)
    env, options = parity.env(), contract["compiler_options"]
    image = args.retail_exe.read_bytes()
    sections = pe_oracle.pe_sections(image)
    reports, objects = [], []
    accepted = True
    for target in contract["functions"]:
        source = ROOT / target["source"]
        obj = directory / (target["address"] + ".obj")
        objects.append(obj)
        run([parity.CL_EXE, *options, "/Fo" + str(obj), source], env)
        body, section, symbol = parity.obj_text(obj, target["symbol"])
        offsets = parity.obj_relocs(obj, section)
        asm = run([parity.OBJDUMP, "-dr", obj], env)
        (directory / (target["address"] + ".asm")).write_text(asm)
        offset = pe_oracle.va_to_off(sections, int(target["address"], 16))
        if offset is None:
            raise RuntimeError("Unmapped retail address: " + target["address"])
        retail = image[offset:offset + target["retail_bytes"]]
        if digest(retail) != target["retail_sha256"]:
            raise RuntimeError("Retail oracle changed: " + target["address"])
        normalized = parity.mask(body, offsets)
        matched = normalized == parity.mask(retail, offsets)
        reviewed = (len(body) == target["compiled_bytes"] and
                    digest(normalized) == target["masked_sha256"] and
                    relocation_symbols(asm, symbol) == target["relocation_symbols"])
        approved = reviewed and (matched or not args.require_byte_match)
        if target["grade"] == "matching" and not matched:
            approved = False
        accepted = accepted and approved
        status = "RELOCATION_MATCH" if matched else "DIFFER"
        reports.append({"address": target["address"], "source_sha256": digest(source.read_bytes()),
                        "compiled_bytes": len(body), "retail_bytes": len(retail),
                        "retail_parity": status, "reviewed_fingerprint": reviewed,
                        "accepted": approved})
        print(f"STOPWATCH {target['address']} {status} bytes={len(body)}/{len(retail)} reviewed={reviewed}")
    fixture = ROOT / "rebuild/tests/00/62/CStopWatch_test.cpp"
    test_obj, test_exe = directory / "behavior.obj", directory / "behavior.exe"
    run([parity.CL_EXE, *options, "/Fo" + str(test_obj), fixture], env)
    bind_clock_doubles(test_obj, env)
    run([parity.VC / "bin/link.exe", "/nologo", "/subsystem:console",
         "/out:" + str(test_exe), *objects, test_obj], env)
    behavior = run([test_exe], env).strip()
    if "FABLETLC_STOPWATCH_BEHAVIOR PASS" not in behavior:
        raise RuntimeError("Stopwatch behavior pass marker missing")
    print(behavior)
    report = {"functions": reports, "behavior": behavior, "accepted": accepted,
              "fixture_sha256": digest(fixture.read_bytes()), "retail_sha256": digest(image),
              "require_byte_match": args.require_byte_match,
              "runtime_scope": "offline console fixture; only OS clocks replaced"}
    name = "strict-report.json" if args.require_byte_match else "report.json"
    (directory / name).write_text(json.dumps(report, indent=2) + "\n")
    print("STOPWATCH_GATE " + ("PASS" if accepted else "FAIL"))
    return 0 if accepted else 1


if __name__ == "__main__":
    sys.exit(main())
