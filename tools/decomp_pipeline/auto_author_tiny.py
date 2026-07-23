#!/usr/bin/env python3
"""Deterministically author trivial tiny functions from oracle bytes.

This is the zero-token counterpart to the ultracode authoring lane. It emits a
workflow-compatible JSON containing candidates for patterns that are completely
determined by the retail bytes:

  c3                 -> empty void function
  c2 04 00           -> empty __stdcall void function with one popped arg
  33 c0 c3 / 31 c0 c3 -> return 0
  6a xx 58 c3        -> return signed imm8
  b8 xx xx xx xx c3  -> return imm32
  8b c1 c2 04 00     -> return first fastcall arg, pop one stack arg
  8b c1 83 e0 xx c3  -> return first fastcall arg & imm8
  8b c1 83 e0 xx c2 04 00 -> same, pop one stack arg
  c7 01 xx xx xx xx c3 -> *first fastcall arg = imm32

The normal verify_and_land.py gate still recompiles and byte-compares every
candidate, so a bad inference can waste a compile but cannot mis-land.

Usage:
  python tools/decomp_pipeline/auto_author_tiny.py <oracle.tsv> <out.json>
"""
import csv
import json
import re
import sys
from pathlib import Path


def sanitize(s: str) -> str:
    s = re.sub(r"[^A-Za-z0-9_]", "_", s or "function")
    s = re.sub(r"_+", "_", s).strip("_")
    if not s or s[0].isdigit():
        s = "fn_" + s
    return s


def split_name(name: str):
    if "::" in name:
        cls, leaf = name.rsplit("::", 1)
        return sanitize(cls), sanitize(leaf)
    if name.startswith("?"):
        m = re.match(r"\?([^@]+)@([^@]+)@@", name)
        if m:
            return sanitize(m.group(2)), sanitize(m.group(1))
    return "global", sanitize(name)


def const_from_bytes(bs: bytes):
    h = bs.hex()
    if h == "c3":
        return ("void", None)
    if h == "c20400":
        return ("void_stdcall_pop4", None)
    if h in ("33c0c3", "31c0c3"):
        return ("int", 0)
    if len(bs) == 4 and bs[0] == 0x6A and bs[2:] == b"\x58\xc3":
        v = bs[1]
        if v >= 0x80:
            v -= 0x100
        return ("int", v)
    if len(bs) == 6 and bs[0] == 0xB8 and bs[-1] == 0xC3:
        return ("int", int.from_bytes(bs[1:5], "little", signed=False))
    if h == "8bc1c20400":
        return ("return_self_pop4", None)
    if len(bs) == 6 and bs[:4] == b"\x8b\xc1\x83\xe0" and bs[-1] == 0xC3:
        return ("and_self", bs[4])
    if len(bs) == 8 and bs[:4] == b"\x8b\xc1\x83\xe0" and bs[-3:] == b"\xc2\x04\x00":
        return ("and_self_pop4", bs[4])
    if len(bs) == 7 and bs[:2] == b"\xc7\x01" and bs[-1] == 0xC3:
        return ("store_imm32", int.from_bytes(bs[2:6], "little", signed=False))
    return None


def candidate(row):
    addr = row["address"].lower()
    name = row.get("name", f"sub_{addr}")
    bs = bytes.fromhex(row["bytes"])
    k = const_from_bytes(bs)
    if not k:
        return None
    rettype, value = k
    cls, leaf = split_name(name)
    module = row.get("module") or (cls if cls != "global" else "_global")
    fn = f"{cls}_{leaf}" if cls != "global" else leaf
    fn = sanitize(fn)
    pattern = f"AUTO_TINY_{addr}_TEST PASS"
    if rettype == "void":
        source = f"void __fastcall {fn}() {{}}\n"
        test = (
            "#include <cstdio>\n"
            f"void __fastcall {fn}() {{}}\n"
            "int main()\n"
            "{\n"
            f"    {fn}();\n"
            f"    std::printf(\"{pattern}\\n\");\n"
            "    return 0;\n"
            "}\n"
        )
    elif rettype == "void_stdcall_pop4":
        source = f"void __stdcall {fn}(int) {{}}\n"
        test = (
            "#include <cstdio>\n"
            f"void __stdcall {fn}(int) {{}}\n"
            "int main()\n"
            "{\n"
            f"    {fn}(123);\n"
            f"    std::printf(\"{pattern}\\n\");\n"
            "    return 0;\n"
            "}\n"
        )
    elif rettype == "return_self_pop4":
        source = f"void* __fastcall {fn}(void* self, int, int)\n{{\n    return self;\n}}\n"
        test = (
            "#include <cstdio>\n"
            f"void* __fastcall {fn}(void* self, int, int)\n{{\n    return self;\n}}\n"
            "int main()\n"
            "{\n"
            "    int x = 0;\n"
            f"    if ({fn}(&x, 1, 2) == &x) {{ std::printf(\"{pattern}\\n\"); return 0; }}\n"
            f"    std::printf(\"AUTO_TINY_{addr}_TEST FAIL\\n\");\n"
            "    return 1;\n"
            "}\n"
        )
    elif rettype == "and_self":
        source = f"int __fastcall {fn}(int self)\n{{\n    return self & {value};\n}}\n"
        test = (
            "#include <cstdio>\n"
            f"int __fastcall {fn}(int self)\n{{\n    return self & {value};\n}}\n"
            "int main()\n"
            "{\n"
            f"    if ({fn}(0x7f) == (0x7f & {value})) {{ std::printf(\"{pattern}\\n\"); return 0; }}\n"
            f"    std::printf(\"AUTO_TINY_{addr}_TEST FAIL\\n\");\n"
            "    return 1;\n"
            "}\n"
        )
    elif rettype == "and_self_pop4":
        source = f"int __fastcall {fn}(int self, int, int)\n{{\n    return self & {value};\n}}\n"
        test = (
            "#include <cstdio>\n"
            f"int __fastcall {fn}(int self, int, int)\n{{\n    return self & {value};\n}}\n"
            "int main()\n"
            "{\n"
            f"    if ({fn}(0x7f, 1, 2) == (0x7f & {value})) {{ std::printf(\"{pattern}\\n\"); return 0; }}\n"
            f"    std::printf(\"AUTO_TINY_{addr}_TEST FAIL\\n\");\n"
            "    return 1;\n"
            "}\n"
        )
    elif rettype == "store_imm32":
        source = f"void __fastcall {fn}(unsigned int* self)\n{{\n    *self = 0x{value:08x};\n}}\n"
        test = (
            "#include <cstdio>\n"
            f"void __fastcall {fn}(unsigned int* self)\n{{\n    *self = 0x{value:08x};\n}}\n"
            "int main()\n"
            "{\n"
            "    unsigned int x = 0;\n"
            f"    {fn}(&x);\n"
            f"    if (x == 0x{value:08x}) {{ std::printf(\"{pattern}\\n\"); return 0; }}\n"
            f"    std::printf(\"AUTO_TINY_{addr}_TEST FAIL\\n\");\n"
            "    return 1;\n"
            "}\n"
        )
    else:
        source = f"int __fastcall {fn}()\n{{\n    return {value};\n}}\n"
        test = (
            "#include <cstdio>\n"
            f"int __fastcall {fn}()\n{{\n    return {value};\n}}\n"
            "int main()\n"
            "{\n"
            f"    if ({fn}() == {value}) {{ std::printf(\"{pattern}\\n\"); return 0; }}\n"
            f"    std::printf(\"AUTO_TINY_{addr}_TEST FAIL\\n\");\n"
            "    return 1;\n"
            "}\n"
        )
    return {
        "address": addr,
        "name": name,
        "module": module,
        "source_cpp": source,
        "test_cpp": test,
        "pass_pattern": pattern,
        "self_status": "AUTO_TINY",
        "self_behav": "AUTO_TINY",
        "win": True,
    }


def main():
    if len(sys.argv) != 3:
        raise SystemExit(__doc__)
    oracle = Path(sys.argv[1])
    out = Path(sys.argv[2])
    authored = []
    for row in csv.DictReader(open(oracle, encoding="utf-8-sig"), delimiter="\t"):
        c = candidate(row)
        if c:
            authored.append(c)
    json.dump({"result": {"authored": authored}}, open(out, "w", encoding="utf-8"), indent=1)
    print(f"auto-authored {len(authored)} tiny candidates -> {out}")


if __name__ == "__main__":
    main()
