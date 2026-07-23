"""Bulk RTTI vtable-slot name port: FableWin (PDB-named donor) -> retail Fable.exe.

For every MSVC RTTI class (.?AV...@@) present in BOTH binaries:
  - walk TypeDescriptor -> CompleteObjectLocator(s) -> vtable(s) in each binary
  - match donor/retail vtables by COL offset (multiple-inheritance safe)
  - require identical slot counts (donor is an Anniversary-era build; classes
    whose vtables diverged are skipped entirely)
  - port ONLY slots whose donor PDB name belongs to the class itself
    (?Method@ThisClass@@...). Base-class impls are ported when their own class
    is processed; this avoids misattributing release-build ICF-folded stubs.
  - a retail address observed with two different demangle-base names is dropped
    (ICF collision guard).

Outputs ghidra_out/labels_rtti_port.tsv (addr<TAB>decorated-name) plus a
skip/stat report to stdout.

Usage: python tools/rtti_port_all.py [--limit N]
"""
import re
import struct
import sys
from collections import defaultdict

import numpy as np

sys.path.insert(0, r"D:\Documents\FableTLC\tools")
from rtti_map import PE, vtables_for_class, resolve_thunk


class FastPE(PE):
    """PE with a sorted u32 index so pointer searches are O(log n) instead of
    a full binary scan per lookup (the bulk port does ~10k lookups/binary).
    Only finds 4-byte-aligned occurrences — COL fields, TD pointers, and
    vtable slots all live aligned in .rdata/.data."""

    def __init__(self, path):
        super().__init__(path)
        n = len(self.data) // 4
        self._words = np.frombuffer(self.data, dtype="<u4", count=n)
        self._order = np.argsort(self._words, kind="stable")
        self._sorted = self._words[self._order]

    def find_all(self, needle):
        if len(needle) == 4:
            value = struct.unpack("<I", needle)[0]
            lo = int(np.searchsorted(self._sorted, value, "left"))
            hi = int(np.searchsorted(self._sorted, value, "right"))
            return sorted(int(i) * 4 for i in self._order[lo:hi])
        return super().find_all(needle)

DONOR = r"D:\Documents\FableTLC\symbols\Fable_Anniversary-2013-02-25\Fable\FableWin.exe"
RETAIL = r"C:\Programs\Steam\steamapps\common\Fable The Lost Chapters\Fable.exe"
NAMES_TSV = r"D:\Documents\FableTLC\ghidra_out\fablewin_pdb_names.tsv"
OUT_TSV = r"D:\Documents\FableTLC\ghidra_out\labels_rtti_port.tsv"


def class_of(decorated):
    """Owning class of a decorated MSVC name, or None.

    ?Method@Class@@...   -> Class        (also ??0/??1/??_G/??_E special names)
    ?M@N@Outer@@...      -> N (innermost scope) — nested/namespaced names keep
    their full scope chain for matching, so we return the raw scope string
    between the first '@' group and '@@'.
    """
    m = re.match(r"^\?\??[^@]*@((?:[^@]+@)+)@", decorated)
    if not m:
        return None
    scopes = m.group(1).rstrip("@").split("@")
    return scopes[0] if scopes else None


def main():
    donor = FastPE(DONOR)
    retail = FastPE(RETAIL)

    names = {}
    for line in open(NAMES_TSV, encoding="utf-8", errors="replace"):
        parts = line.rstrip("\n").split("\t")
        if len(parts) >= 2:
            names[int(parts[0], 16)] = parts[1]

    # All classes present in retail RTTI.
    retail_classes = sorted(set(
        m.group(1).decode("latin1")
        for m in re.finditer(rb"\.\?AV([^\x00@]{1,120})@@", retail.data)))
    limit = None
    if len(sys.argv) > 2 and sys.argv[1] == "--limit":
        limit = int(sys.argv[2])
        retail_classes = retail_classes[:limit]
    print(f"retail RTTI classes: {len(retail_classes)}")

    ported = {}                 # retail addr -> decorated name
    conflicts = set()           # retail addrs with contradicting names
    stats = defaultdict(int)

    for cls in retail_classes:
        mangled = (".?AV" + cls + "@@").encode("latin1")
        if mangled not in donor.data:
            stats["no_donor_rtti"] += 1
            continue

        dvts = vtables_for_class(donor, mangled)
        rvts = vtables_for_class(retail, mangled)
        if not dvts or not rvts:
            stats["no_vtable_either_side"] += 1
            continue

        # Group by COL offset; require a unique vtable per offset on each side.
        dby = defaultdict(list)
        for col_va, offset, vt_va, slots in dvts:
            dby[offset].append(slots)
        rby = defaultdict(list)
        for col_va, offset, vt_va, slots in rvts:
            rby[offset].append(slots)

        matched_any = False
        for offset, dlists in dby.items():
            if offset not in rby:
                continue
            if len(dlists) != 1 or len(rby[offset]) != 1:
                stats["ambiguous_vtable"] += 1
                continue
            dslots, rslots = dlists[0], rby[offset][0]
            if len(dslots) != len(rslots):
                stats["slot_count_mismatch"] += 1
                continue
            matched_any = True
            for i, (dfn, rfn) in enumerate(zip(dslots, rslots)):
                target = resolve_thunk(donor, dfn)
                name = names.get(target)
                if not name:
                    continue
                if class_of(name) != cls:
                    continue  # base-class impl or unrelated; skip
                previous = ported.get(rfn)
                if previous is not None and previous != name:
                    conflicts.add(rfn)
                else:
                    ported[rfn] = name
        if matched_any:
            stats["classes_ported"] += 1

    for addr in conflicts:
        ported.pop(addr, None)

    with open(OUT_TSV, "w", encoding="utf-8", newline="\n") as out:
        for addr in sorted(ported):
            out.write(f"{addr:08x}\t{ported[addr]}\n")

    print(f"classes with at least one matched vtable: {stats['classes_ported']}")
    print(f"skips: no_donor_rtti={stats['no_donor_rtti']}, "
          f"no_vtable_either_side={stats['no_vtable_either_side']}, "
          f"ambiguous_vtable={stats['ambiguous_vtable']}, "
          f"slot_count_mismatch={stats['slot_count_mismatch']}")
    print(f"conflicting retail addresses dropped: {len(conflicts)}")
    print(f"labels written: {len(ported)} -> {OUT_TSV}")


if __name__ == "__main__":
    main()
