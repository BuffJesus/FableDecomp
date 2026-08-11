"""Merge the 6 namespaced UI/dialogue def Transfer bodies into the oracle.

The original --all pass required @@ immediately after the Def name, so it skipped
classes living in a namespace: NUISystem's CUIDef/CUIIconsDef/CUIMiscThingsDef/
CUIStateDef/CUILocaleGraphicsDef and NSpeechGainManager's CDialogueLayerDef. Their
Transfer bodies are ordinary CPersistContext transfers; tx_extract.extract() reads
them fine. This script extracts exactly those (discovered by symbol, not hardcoded
addr), keys them by the binary class name, records the namespace, and adds a
fable_defs_class alias where the binary casing (CUIDef) differs from fable-defs'
struct (CUiDef). Idempotent: overwrites the 6 entries in place.

Usage: python tools/transfer_extract/tx_ui_merge.py [--write]
"""
import json, re, glob, os, sys, importlib.util

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
os.chdir(ROOT)  # tx_extract reads debug_build/ego_r.exe + ghidra_out/ relative to cwd
JSON = os.path.join(ROOT, "refs", "transfer_field_orders.json")
DEFDIR = "C:/Users/Cornelio/Documents/EgoCoreInspect/fable-defs/packages/defs/src/def"

spec = importlib.util.spec_from_file_location(
    "tx", os.path.join(ROOT, "tools", "transfer_extract", "tx_extract.py")
)
tx = importlib.util.module_from_spec(spec)
spec.loader.exec_module(tx)


def fable_defs_keys():
    keys = set()
    for p in glob.glob(DEFDIR + "/*.rs"):
        m = re.search(r"pub struct (\w+Def)\b", open(p, encoding="utf-8").read())
        if m:
            keys.add("C" + m.group(1))
    return keys


def main():
    write = "--write" in sys.argv
    fdk = fable_defs_keys()
    fdk_lower = {k.lower(): k for k in fdk}

    # Discover the namespaced *Def::Transfer symbols the global-@@ regex missed.
    found = {}
    for a, sym in tx.names.items():
        m = tx._CLS_RE.match(sym)
        if m and m.group(2):  # has a namespace
            found[m.group(1)] = (a, m.group(2))

    d = json.load(open(JSON, encoding="utf-8"))
    defs = d["defs"]
    added = []
    for cls, (addr, ns) in sorted(found.items()):
        entry = {"addr": "%#x" % addr, "namespace": ns, "fields": tx.extract(addr)}
        alias = fdk_lower.get(cls.lower())
        if alias and alias != cls:
            entry["fable_defs_class"] = alias  # e.g. CUIDef -> CUiDef
        defs[cls] = entry
        added.append((cls, ns, alias, len(entry["fields"])))

    print("namespaced *Def::Transfer symbols merged:", len(added))
    for cls, ns, alias, n in added:
        tag = " (fable-defs %s)" % alias if alias and alias != cls else ""
        print("  %-24s %-20s %3d fields%s" % (cls, ns, n, tag))
    print("total defs now:", len(defs))

    if write:
        json.dump(d, open(JSON, "w", encoding="utf-8"), indent=1, ensure_ascii=False)
        print("\nWROTE", JSON, "-- now run tx_flatten.py --write to re-score")
    else:
        print("\n(dry run; pass --write)")


if __name__ == "__main__":
    main()
