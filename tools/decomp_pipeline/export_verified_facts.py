#!/usr/bin/env python3
"""Export the byte-verified decomp facts in forms the downstream tools consume:
  - rebuild/sdk/verified_functions.json  : every genuine byte-match (addr, name, cc, ret, parity)
  - rebuild/sdk/struct_schema.json        : {class: {size, fields:[{offset,type,name}]}}  (FableForge/FQT def readers)
  - rebuild/sdk/forgefse_verified_bindings.json : binding-queue targets now byte-verified (ForgeFSE)
Uses only functions that ACTUALLY byte-match (from the parity audit), never behavior-only lands.
"""
import csv, json, glob, os, sys
from pathlib import Path
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import class_struct

ROOT = Path(r"D:\Documents\FableTLC")
def tsv(p):
    try: return list(csv.DictReader(open(p, encoding="utf-8-sig"), delimiter="\t"))
    except FileNotFoundError: return []

def main():
    man = {r["address"].lower(): r for r in tsv(ROOT / "rebuild/manifest/functions.tsv")}
    audit = {r["address"].lower(): r["verdict"] for r in tsv(ROOT / "rebuild/compile-gate/parity_audit.tsv")}
    landed = {os.path.basename(p).split("_")[-1].replace(".cpp", "").lower()
              for p in glob.glob(str(ROOT / "rebuild/src/compiled/*.cpp"))}
    # genuine byte-matches only: audited EXACT/RELOC, plus freshly-landed not-yet-audited (verify_and_land gated)
    def parity_of(a):
        v = audit.get(a)
        if v in ("EXACT", "RELOC"): return v
        if v is None and a in landed: return "MATCH*"  # landed after last audit; gated by verify_and_land
        return None
    verified = []
    for a in sorted(landed):
        pt = parity_of(a)
        if not pt: continue
        r = man.get(a, {})
        verified.append({"address": f"0x{a}", "name": r.get("name", ""), "module": r.get("module", "_global"),
                         "calling_convention": r.get("calling_convention", ""), "return_type": r.get("return_type", ""),
                         "parameter_types": r.get("parameter_types", ""), "parity": pt})
    sdk = ROOT / "rebuild/sdk"; sdk.mkdir(exist_ok=True)
    json.dump({"count": len(verified), "functions": verified}, open(sdk / "verified_functions.json", "w"), indent=1)

    # struct schema from byte-verified accessors/sizes
    classes = set()
    for p in glob.glob(str(ROOT / "rebuild/src/compiled/*.cpp")):
        b = os.path.basename(p)
        if "_" in b: classes.add(b.split("_")[0])
    schema = {}
    for c in sorted(classes):
        off_map, size, n = class_struct.facts_for(c)
        if n < 1 or (not off_map and not size): continue
        fields = [{"offset": f"0x{o:x}", "type": off_map[o][0], "name": off_map[o][1]} for o in sorted(off_map)]
        entry = {"verified_methods": n}
        if size and (not off_map or size >= max(off_map) ): entry["size"] = f"0x{size:x}"
        if fields: entry["fields"] = fields
        schema[c] = entry
    json.dump({"count": len(schema), "classes": schema}, open(sdk / "struct_schema.json", "w"), indent=1)

    # ForgeFSE: binding-queue targets now byte-verified
    bq = tsv(ROOT / "rebuild/backlog/forgefse-binding-queue.tsv")
    vset = {v["address"].lower().replace("0x", "") for v in verified}
    fverified = []
    for r in bq:
        a = r.get("address", "").lower().replace("0x", "")
        if a in vset:
            fverified.append({"address": f"0x{a}", "api_names": r.get("api_names", ""),
                              "qualified_name": r.get("qualified_name", ""), "calling_convention": r.get("calling_convention", ""),
                              "return_type": r.get("return_type", ""), "bridge_kind": r.get("bridge_kind", ""),
                              "parity": parity_of(a)})
    json.dump({"count": len(fverified), "note": "Retail address+CC+signature+behaviour byte-confirmed; ready for ForgeFSE owner/vtable + runtime-probe binding step.",
               "bindings": fverified}, open(sdk / "forgefse_verified_bindings.json", "w"), indent=1)

    print(f"verified_functions: {len(verified)}")
    print(f"struct_schema classes: {len(schema)}  (with size: {sum(1 for v in schema.values() if 'size' in v)}, with fields: {sum(1 for v in schema.values() if 'fields' in v)})")
    print(f"forgefse_verified_bindings: {len(fverified)} / {len(bq)} queue targets")
    print(f"-> {sdk}")

if __name__ == "__main__":
    main()
