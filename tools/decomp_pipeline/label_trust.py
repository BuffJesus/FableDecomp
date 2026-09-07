#!/usr/bin/env python3
"""Which class does a landed function REALLY belong to?

The filename prefix is not trustworthy. Ghidra propagates one name across every
byte-identical body, so `rebuild/manifest/functions.tsv` carries 2,977 rows called
`CLandscapeBackgroundPatch::vector_deleting_destructor` at distinct addresses, 573
`CActionDoCreatureAction::GetActionName`, 378 `CActiveFile::OnReadFinished`, and so on.
Those are destructor/thunk bodies of *many* classes wearing one label. Retyping them to
the labelled class's header would be wrong.

A module label is *trusted* for an address only when:
  * the module is a real class (not `_global`, `_gapscan`, or a template instantiation), and
  * the (module, name) pair occurs at exactly ONE address in the manifest, and
  * the name is not a compiler-generated family member (deleting destructors, `operator`,
    `~Class`, `scalar_deleting_destructor`, `__vecDelDtor`).

`is_genuine(path)` wraps crawl/purity.py so callers get one import.

    python label_trust.py                 # summary counts
    python label_trust.py CGameScriptInterface   # trusted landed files for one class
"""
from __future__ import annotations

import collections
import csv
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
MANIFEST = ROOT / "rebuild" / "manifest" / "functions.tsv"
sys.path.insert(0, str(Path(__file__).resolve().parent))
from crawl import purity  # noqa: E402

GENERATED_NAME_MARKERS = (
    "deleting_destructor", "deleting destructor", "__vecDelDtor", "scalar_deleting",
    "vector_deleting", "`vector deleting", "`scalar deleting",
)
UNTRUSTED_MODULES = {"_global", "_gapscan", "", "Global", "global"}


def _norm_addr(a: str) -> str:
    return a.lower().replace("0x", "").zfill(8)


class Manifest:
    def __init__(self, path: Path = MANIFEST):
        self.rows = list(csv.DictReader(open(path, encoding="utf-8-sig"), delimiter="\t"))
        self.by_addr = {_norm_addr(r["address"]): r for r in self.rows if r.get("address")}
        # Count (module, leaf-name) pairs. Names arrive both bare ("Foo") and qualified
        # ("CClass::Foo") for the same method, so compare on the leaf.
        self.pair_count = collections.Counter(
            (r["module"], self.leaf(r["name"])) for r in self.rows)

    @staticmethod
    def leaf(name: str) -> str:
        return (name or "").rsplit("::", 1)[-1]

    def module_of(self, addr: str) -> str | None:
        r = self.by_addr.get(_norm_addr(addr))
        return r["module"] if r else None

    def is_generated_name(self, name: str) -> bool:
        n = name or ""
        leaf = n.rsplit("::", 1)[-1]
        return (any(m in n for m in GENERATED_NAME_MARKERS)
                or leaf.startswith("operator") or leaf.startswith("~"))

    def trust_reason(self, addr: str) -> str:
        """'' when trusted, else a short reason code."""
        r = self.by_addr.get(_norm_addr(addr))
        if not r:
            return "NOT_IN_MANIFEST"
        m, n = r["module"], r["name"]
        if m in UNTRUSTED_MODULES:
            return "NO_CLASS"
        if "<" in m or "<" in (n or ""):
            return "TEMPLATE"
        if self.is_generated_name(n):
            return "GENERATED_NAME"
        if self.pair_count[(m, self.leaf(n))] != 1:
            return f"MISLABELLED_FAMILY({self.pair_count[(m, self.leaf(n))]})"
        return ""

    def trusted_module(self, addr: str) -> str | None:
        return None if self.trust_reason(addr) else self.by_addr[_norm_addr(addr)]["module"]

    def landed(self):
        for r in self.rows:
            if r.get("compiled_source"):
                yield r

    def trusted_landed_for(self, cls: str):
        for r in self.landed():
            a = _norm_addr(r["address"])
            if r["module"] == cls and not self.trust_reason(a):
                yield r


def is_genuine(path: Path | str) -> bool:
    return purity.is_genuine(Path(path).read_text(encoding="utf-8", errors="ignore"))


def classify(path: Path | str) -> str:
    return purity.classify(Path(path).read_text(encoding="utf-8", errors="ignore"))


def main() -> int:
    mf = Manifest()
    if len(sys.argv) > 1:
        cls = sys.argv[1]
        n = 0
        for r in mf.trusted_landed_for(cls):
            p = Path(r["compiled_source"])
            print(r["address"], r["name"], classify(p) if p.exists() else "MISSING", p.name)
            n += 1
        print(f"{n} trusted landed rows for {cls}")
        return 0
    reasons = collections.Counter()
    per_class = collections.Counter()
    for r in mf.landed():
        why = mf.trust_reason(r["address"])
        reasons[why.split("(")[0] or "TRUSTED"] += 1
        if not why:
            per_class[r["module"]] += 1
    print("landed rows by trust:", dict(reasons))
    print("top trusted classes:")
    for c, n in per_class.most_common(15):
        print(f"  {n:5} {c}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
