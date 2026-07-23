#!/usr/bin/env python3
"""Generate forge/cutscene_verb_info.hpp from cutscene_verb_args.tsv.

Emits the dispatch order and a heuristic argument hint per verb as vendored
reference data for `forge script verbs`. Regenerate after re-running
extract_cutscene_arg_sigs.py."""
import csv
from pathlib import Path

TSV = Path(r"D:\Documents\FableTLC\ghidra_out\cutscene_verb_args.tsv")
OUT = Path(r"D:\Code\FableForge\libs\forgecore\include\forge\cutscene_verb_info.hpp")

rows = []
with TSV.open(encoding="utf-8-sig", newline="") as f:
    for r in csv.DictReader(f, delimiter="\t"):
        rows.append((r["verb"], int(r["order"]), int(r["argcount"]),
                     r["signature"]))
rows.sort(key=lambda r: r[1])  # dispatch order

lines = []
w = lines.append
w("// Cutscene verb reference data: dispatch order and a HEURISTIC argument hint")
w("// per native verb, derived from the RunCutsceneMacro_Func decompile by")
w("// tools/extract_cutscene_arg_sigs.py. Do not hand-edit; regenerate via")
w("// tools/gen_verb_info_header.py.")
w("//")
w("// Caveats on argHint/argCount (the dispatch ORDER is exact):")
w("//  - float args parsed by the __fastcall GFCharStringToFloat show as 'str';")
w("//  - some entity `.Verb` handlers delegate arg parsing and show argCount 0.")
w("// Treat argHint as autocomplete guidance, not a hard arg-count contract.")
w("#ifndef FORGE_CUTSCENE_VERB_INFO_HPP")
w("#define FORGE_CUTSCENE_VERB_INFO_HPP")
w("")
w("#include <array>")
w("#include <string_view>")
w("")
w("namespace forge::cutscene {")
w("")
w("struct VerbInfo {")
w("    std::string_view name;")
w("    int dispatchOrder;   // 1-based position in the strncmp chain")
w("    int argCount;        // highest arg slot the handler was seen to read")
w("    std::string_view argHint;  // e.g. \"arg1:int arg2:bool\"")
w("};")
w("")
w(f"inline constexpr std::array<VerbInfo, {len(rows)}> kVerbInfo = {{{{")
for verb, order, ac, sig in rows:
    w(f'    {{"{verb}", {order}, {ac}, "{sig}"}},')
w("}};")
w("")
w("}  // namespace forge::cutscene")
w("")
w("#endif  // FORGE_CUTSCENE_VERB_INFO_HPP")

OUT.write_text("\n".join(lines) + "\n", encoding="utf-8")
print(f"wrote {OUT} with {len(rows)} verbs")
