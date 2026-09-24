#!/usr/bin/env python3
"""Check the offline Play -> frontend constructor -> Init startup chain.

Real reconstructed functions run with deterministic doubles for external engine
services and the unfinished frontend Run body. This never launches the game/UI.
Functional bodies must retain their reviewed code fingerprint and relocation
targets; matching bodies must also match independently read retail bytes.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re
import sys

from check_cgame_play import ROOT, parity, pe_oracle, run


def digest(data):
    return hashlib.sha256(data).hexdigest()


def relocation_symbols(disassembly, symbol):
    function = disassembly.split(f"<{symbol}>:", 1)[1]
    function = function.split("Disassembly of section", 1)[0]
    return [[int(offset, 16), kind, target.strip()] for offset, kind, target in
            re.findall(r"^\s+([0-9a-fA-F]+):\s+(dir32|DISP32)\s+(.+)$",
                       function, re.MULTILINE)]


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--require-byte-match", action="store_true")
    parser.add_argument("--retail-exe", type=Path, default=pe_oracle.EXE)
    args = parser.parse_args()
    # Local import avoids a module cycle: the stopwatch gate shares the object
    # fingerprint helpers above, and this integration gate consumes its output.
    from check_stopwatch import bind_clock_doubles
    contract_path = ROOT / "rebuild/integration/frontend_startup_contract.json"
    contract = json.loads(contract_path.read_text(encoding="utf-8"))
    directory = ROOT / "work/frontend_startup_check"
    directory.mkdir(parents=True, exist_ok=True)
    env = parity.env()
    options = contract["compiler_options"]
    image = args.retail_exe.read_bytes()
    sections = pe_oracle.pe_sections(image)
    objects = []
    reports = []
    accepted = True
    for target in contract["functions"]:
        address = int(target["address"], 16)
        source = ROOT / target["source"]
        obj = directory / (target["address"] + ".obj")
        run([parity.CL_EXE, *options, "/Fo" + str(obj), source], env)
        objects.append(obj)
        body, section, symbol = parity.obj_text(obj, target["symbol"])
        offsets = parity.obj_relocs(obj, section)
        disassembly = run([parity.OBJDUMP, "-dr", obj], env)
        symbols = relocation_symbols(disassembly, symbol)
        (directory / (target["address"] + ".asm")).write_text(disassembly, encoding="utf-8")
        offset = pe_oracle.va_to_off(sections, address)
        if offset is None:
            raise RuntimeError("Retail address not mapped: " + target["address"])
        retail = image[offset:offset + target["retail_bytes"]]
        if digest(retail) != target["retail_sha256"]:
            raise RuntimeError("Retail oracle changed: " + target["address"])
        normalized = parity.mask(body, offsets)
        matched = normalized == parity.mask(retail, offsets)
        reviewed = (len(body) == target["compiled_bytes"] and
                    digest(normalized) == target["masked_sha256"] and
                    symbols == target["relocation_symbols"])
        approved = reviewed and (matched or not args.require_byte_match)
        if target["grade"] == "matching" and not matched:
            approved = False
        accepted = accepted and approved
        result = {
            "address": target["address"], "source": target["source"],
            "source_sha256": digest(source.read_bytes()),
            "compiled_bytes": len(body), "retail_bytes": len(retail),
            "retail_parity": "RELOCATION_MATCH" if matched else "DIFFER",
            "reviewed_fingerprint": reviewed, "accepted": approved,
            "relocation_symbols": symbols,
        }
        reports.append(result)
        print(f"FRONTEND_STARTUP {target['address']} {result['retail_parity']} "
              f"bytes={len(body)}/{len(retail)} reviewed={reviewed}")

    # Gate Play before linking it into the integration fixture.
    print(run([sys.executable, ROOT / "tools/decomp_pipeline/check_cgame_play.py",
               "--retail-exe", args.retail_exe], env).strip())
    play_obj = directory / "play.obj"
    run([parity.CL_EXE, *options, "/Fo" + str(play_obj),
         ROOT / "rebuild/src/compiled/00/41/CGame_Play_00412f90.cpp"], env)
    objects.append(play_obj)
    print(run([sys.executable, ROOT / "tools/decomp_pipeline/check_stopwatch.py",
               "--retail-exe", args.retail_exe], env).strip())
    objects.append(ROOT / "work/stopwatch_check/0062f740.obj")
    test_obj = directory / "behavior.obj"
    test_exe = directory / "behavior.exe"
    fixture = ROOT / "rebuild/tests/00/42/CNewFrontendGameComponent_startup_test.cpp"
    run([parity.CL_EXE, *options, "/Fo" + str(test_obj), fixture], env)
    bind_clock_doubles(test_obj, env, counter=False)
    run([parity.VC / "bin/link.exe", "/nologo", "/subsystem:console",
         "/out:" + str(test_exe), *objects, test_obj], env)
    behavior = run([test_exe], env).strip()
    if "FABLETLC_FRONTEND_STARTUP_BEHAVIOR PASS" not in behavior:
        raise RuntimeError("Startup behavior pass marker missing")
    print(behavior)
    report = {"functions": reports, "behavior": behavior, "accepted": accepted,
              "fixture_sha256": digest(fixture.read_bytes()),
              "retail_sha256": digest(image),
              "require_byte_match": args.require_byte_match,
              "runtime_scope": "offline fixture; external services and Run are doubles"}
    name = "strict-report.json" if args.require_byte_match else "report.json"
    (directory / name).write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print("FRONTEND_STARTUP_GATE " + ("PASS" if accepted else "FAIL"))
    return 0 if accepted else 1


if __name__ == "__main__":
    sys.exit(main())
