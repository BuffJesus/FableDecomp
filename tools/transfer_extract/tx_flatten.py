"""Flatten base:<Class> markers in refs/transfer_field_orders.json.

Pure JSON transform (no disasm): each {"type": "base:<Class>"} marker entry is
replaced, in place, by that base class's (recursively flattened) field list, so
consumers get a fully-inlined order matching fable-defs' base-field inlining.
Inlined fields are tagged "from_base": "<Class>" for provenance; the base
classes remain as their own top-level entries too.

Then re-runs the fable-defs order-preserving-subsequence check and rewrites each
class's "fable_defs_order_match" flag.

Usage: python tools/transfer_extract/tx_flatten.py [--write]
  (default: dry-run report; --write updates refs/transfer_field_orders.json)
"""
import json, re, glob, os, sys

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
JSON = os.path.join(ROOT, "refs", "transfer_field_orders.json")
DEFDIR = "C:/Users/Cornelio/Documents/EgoCoreInspect/fable-defs/packages/defs/src/def"


def flatten_fields(cls, defs, stack=()):
    """Return cls's field list with base markers recursively inlined."""
    if cls in stack:
        raise RuntimeError("base cycle: " + " -> ".join(stack + (cls,)))
    out = []
    for f in defs[cls]["fields"]:
        t = f.get("type", "")
        if t.startswith("base:"):
            base = t[5:]
            if base not in defs:
                out.append(f)  # unknown base: keep marker
                continue
            for bf in flatten_fields(base, defs, stack + (cls,)):
                nf = dict(bf)
                nf.setdefault("from_base", base)
                out.append(nf)
        else:
            out.append(f)
    return out


def load_fable_defs():
    fd = {}
    for p in glob.glob(DEFDIR + "/*.rs"):
        txt = open(p, encoding="utf-8").read()
        sm = re.search(r"pub struct (\w+Def)\b", txt)
        if not sm:
            continue
        fd["C" + sm.group(1)] = re.findall(r'#\[def\("([^"]+)"', txt)
    return fd


def is_subseq(short, long):
    it = iter(long)
    return all(any(x == y for y in it) for x in short)


def main():
    write = "--write" in sys.argv
    d = json.load(open(JSON, encoding="utf-8"))
    defs = d["defs"]

    n_markers = sum(
        1 for v in defs.values() for f in v["fields"] if f.get("type", "").startswith("base:")
    )

    # Flatten every def (recomputed from originals so nested bases resolve once).
    flattened = {cls: flatten_fields(cls, defs) for cls in defs}
    for cls in defs:
        defs[cls]["fields"] = flattened[cls]

    # Re-score against fable-defs.
    fd = load_fable_defs()
    changed, still_partial = [], []
    for cls in defs:
        # namespaced UI defs carry a fable_defs_class alias (CUIDef -> CUiDef)
        order = fd.get(cls) or fd.get(defs[cls].get("fable_defs_class", ""))
        prev = defs[cls].get("fable_defs_order_match", "?")
        if order is None:
            continue
        ourn = [f["name"] for f in defs[cls]["fields"] if f.get("name")]
        if is_subseq(order, ourn):
            new = "exact"
        else:
            only_fd = [x for x in order if x not in ourn]
            if only_fd:
                new = "partial(fd-only:%s)" % ",".join(only_fd)
            else:
                # every fable-defs name is present in ours, but the subsequence
                # still fails => fable-defs unrolls an array field (repeats the
                # #[def] name per element) while our disasm sees one Transfer in
                # a loop. Report the over-multiplied names.
                from collections import Counter
                fdc, oc = Counter(order), Counter(ourn)
                arr = [f"{n}x{fdc[n]}" for n in dict.fromkeys(order) if fdc[n] > oc[n]]
                new = "partial(array-unrolled:%s)" % ",".join(arr)
            still_partial.append((cls, only_fd))
        if new != prev:
            changed.append((cls, prev, new))
        defs[cls]["fable_defs_order_match"] = new

    d["_meta"]["base_fields_inlined"] = (
        "base:<Class> markers expanded inline; inlined fields tagged from_base"
    )

    exact = sum(1 for v in defs.values() if v.get("fable_defs_order_match") == "exact")
    scored = sum(
        1 for cls in defs if cls in fd or defs[cls].get("fable_defs_class") in fd
    )
    print("base markers inlined:", n_markers)
    print("classes re-scored vs fable-defs:", scored)
    print("exact matches now:", exact, "/", scored)
    print("\nchanged flags:")
    for cls, a, b in changed:
        print("  %-28s %s -> %s" % (cls, a, b))
    if still_partial:
        print("\nstill partial:")
        for cls, only in still_partial:
            print("  %-28s fd-only=%s" % (cls, only))

    if write:
        json.dump(d, open(JSON, "w", encoding="utf-8"), indent=1, ensure_ascii=False)
        print("\nWROTE", JSON)
    else:
        print("\n(dry run; pass --write to update the JSON)")


if __name__ == "__main__":
    main()
