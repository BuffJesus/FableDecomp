#!/usr/bin/env python3
"""Generate disasm bundles + an author-workflow manifest from an oracle TSV + targets JSON.
Usage: python gen_bundles.py <oracle.tsv> <targets.json> <out_bundles_dir> <manifest_out.json> [max_len]
Prints the manifest JSON to stdout (compact) for direct use as Workflow args."""
import csv, json, subprocess, tempfile, os, sys
from pathlib import Path

OBJDUMP = r"C:\Users\Cornelio\AppData\Local\Microsoft\WinGet\Packages\BrechtSanders.WinLibs.POSIX.UCRT_Microsoft.Winget.Source_8wekyb3d8bbwe\mingw64\bin\objdump.exe"

def dis(hexb):
    with tempfile.NamedTemporaryFile(suffix=".bin", delete=False) as t:
        t.write(bytes.fromhex(hexb)); n = t.name
    try:
        return subprocess.run([OBJDUMP, "-D", "-b", "binary", "-m", "i386", "-M", "intel", n],
                              capture_output=True, text=True).stdout
    finally:
        os.unlink(n)

def main():
    oracle, targets, outdir, manifest_out = sys.argv[1:5]
    max_len = int(sys.argv[5]) if len(sys.argv) > 5 else 160
    OD = Path(outdir); OD.mkdir(parents=True, exist_ok=True)
    orc = {r["address"].lower(): r for r in csv.DictReader(open(oracle, encoding="utf-8-sig"), delimiter="\t")}
    tgt = {d["address"].lower(): d for d in json.load(open(targets))}
    man = []
    for a, r in orc.items():
        ln = int(r["length"])
        if ln > max_len or ln == 0:
            continue
        t = tgt.get(a, {})
        (OD / f"{a}.md").write_text(
            f"# {a} {r['name']} ({ln} bytes)\n"
            f"module: {t.get('module','?')}  manifest CC: {t.get('cc','?')}  return: {t.get('ret','?')}  params: {t.get('pc','?')}\n"
            f"retail hex: {r['bytes']}\n\n"
            f"## RETAIL disasm (raw i386; call/jmp rel32 & abs-addr operands are relocation-masked in parity)\n"
            f"```\n{dis(r['bytes'])}```\n", encoding="utf-8")
        man.append({"address": a, "name": r["name"], "len": ln,
                    "module": t.get("module", "_global"), "bundle": str(OD / f"{a}.md")})
    json.dump(man, open(manifest_out, "w"))
    print(f"bundles={len(man)} -> {OD}  manifest -> {manifest_out}", file=sys.stderr)
    # emit batches of the manifest for workflow args
    print(json.dumps(man))

if __name__ == "__main__":
    main()
