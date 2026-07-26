#!/usr/bin/env python3
"""Deterministically author trivial tiny functions from oracle bytes.

This is the zero-token counterpart to the ultracode authoring lane. It emits a
workflow-compatible JSON containing candidates for patterns that are completely
determined by the retail bytes:

  c3                 -> empty void function
  c2 nn 00           -> empty __stdcall void function with nn/4 popped args
  33 c0 c3 / 31 c0 c3 -> return 0
  b0 xx c3           -> return bool/byte constant
  6a xx 58 c3        -> return signed imm8
  b8 xx xx xx xx c3  -> return imm32
  8b c1 c3           -> return the fastcall self pointer
  8b c1 c2 04 00     -> return first fastcall arg, pop one stack arg
  8b 41 nn c3        -> return a 32-bit field at self + nn
  8b 81 nn nn nn nn c3 -> return a 32-bit field at self + nn
  dd 41 nn c3        -> return a double field at self + nn
  8b c1 83 e0 xx c3  -> return first fastcall arg & imm8
  8b c1 83 e0 xx c2 04 00 -> same, pop one stack arg
  c7 01 xx xx xx xx c3 -> *first fastcall arg = imm32
  b9 xx xx xx xx e9 yy yy yy yy -> tail-call a method on a global object
  a1 xx xx xx xx 85 c0 74 07 50 e8 yy yy yy yy 59 c3
                     -> call a function only when its global pointer is non-null
  b9 xx xx xx xx e8 yy yy yy yy ff 0d zz zz zz zz c3
                     -> call a global object's method, then decrement a counter
  c7 05 xx xx xx xx ii ii ii ii b9 yy yy yy yy e9 zz zz zz zz
                     -> assign a global object's first field, then tail-call a method
  b9 xx xx xx xx e8 yy yy yy yy a1 zz zz zz zz 85 c0 74 07
     50 e8 aa aa aa aa 59 c3
                     -> call a method, then optionally call with a global pointer
  b9 xx xx xx xx c7 05 yy yy yy yy ii ii ii ii e8 zz zz zz zz
     b9 aa aa aa aa e9 bb bb bb bb
                     -> assign a global field and call two methods
  56 57 be xx xx xx xx bf nn nn nn nn 8d 64 24 00 83 ee ss
     8b ce e8 yy yy yy yy 4f 75 f3 5f 5e c3
                     -> reverse-walk a fixed object array and call each method
  56 8b f1 c7 06 xx xx xx xx e8 yy yy yy yy f6 44 24 08 01
     74 07 56 e8 zz zz zz zz 59 8b c6 5e c2 04 00
                     -> restore a vtable, destruct, and optionally delete
  56 8b f1 e8 xx xx xx xx f6 44 24 08 01 74 07 56
     e8 yy yy yy yy 59 8b c6 5e c2 04 00
                     -> destruct an object and optionally delete its storage
  56 8b f1 8d 4e oo e8 xx xx xx xx 8b ce e8 yy yy yy yy
     f6 44 24 08 01 74 07 56 e8 zz zz zz zz 59 8b c6 5e c2 04 00
                     -> destruct a member and its owner, then optionally delete
  51 56 8b f1 8b 56 04 8b 0e 8d 44 24 07 50 e8 xx xx xx xx
     8b 36 85 f6 74 07 56 e8 yy yy yy yy 59 5e 59 c3
                     -> finish an async read and release its resource
  56 8b f1 83 7e 04 00 74 1e 8b 06 ff 70 04 e8 xx xx xx xx
     8b 06 89 40 08 8b 06 83 60 04 00 8b 06 89 40 0c
     83 66 04 00 5e c3
                     -> free and reset a non-empty intrusive list
  53 56 8b 74 24 0c 85 f6 8b d9 74 1c 57 ff 76 0c 8b cb
     e8 xx xx xx xx 8b 7e 08 56 e8 yy yy yy yy 85 ff 59
     8b f7 75 e6 5f 5e 5b c2 04 00
                     -> recursively consume a linked tree

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


CPP_KEYWORDS = {
    "asm", "auto", "bool", "break", "case", "catch", "char", "class",
    "const", "const_cast", "continue", "default", "delete", "do",
    "double", "dynamic_cast", "else", "enum", "explicit", "export",
    "extern", "false", "float", "for", "friend", "goto", "if", "inline",
    "int", "long", "mutable", "namespace", "new", "operator", "private",
    "protected", "public", "register", "reinterpret_cast", "return",
    "short", "signed", "sizeof", "static", "static_cast", "struct",
    "switch", "template", "this", "throw", "true", "try", "typedef",
    "typeid", "typename", "union", "unsigned", "using", "virtual", "void",
    "volatile", "wchar_t", "while",
}


def sanitize(s: str) -> str:
    s = re.sub(r"[^A-Za-z0-9_]", "_", s or "function")
    s = re.sub(r"_+", "_", s).strip("_")
    if not s or s[0].isdigit():
        s = "fn_" + s
    if s in CPP_KEYWORDS:
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


def rows_with_target_metadata(oracle: Path):
    """Yield oracle rows enriched by the selector's sibling target catalog."""
    target_path = oracle.with_name(
        oracle.name.replace("_oracle.tsv", "_targets.json")
    )
    metadata = {}
    if target_path.exists():
        for target in json.loads(target_path.read_text(encoding="utf-8")):
            address = target.get("address", "").lower().replace("0x", "")
            if address:
                metadata[address] = target

    with open(oracle, encoding="utf-8-sig", newline="") as stream:
        for row in csv.DictReader(stream, delimiter="\t"):
            address = row["address"].lower().replace("0x", "")
            target = metadata.get(address)
            if target:
                for field in ("module", "cc", "ret", "pc"):
                    if target.get(field) and not row.get(field):
                        row[field] = target[field]
            yield row


def const_from_bytes(bs: bytes):
    h = bs.hex()
    if h == "c3":
        return ("void", None)
    if len(bs) == 3 and bs[0] == 0xC2 and bs[2] == 0 and bs[1] % 4 == 0:
        return ("void_stdcall_pop", bs[1])
    if h in ("33c0c3", "31c0c3"):
        return ("int", 0)
    if len(bs) == 3 and bs[0] == 0xB0 and bs[-1] == 0xC3:
        return ("bool", bool(bs[1]))
    if len(bs) == 4 and bs[0] == 0x6A and bs[2:] == b"\x58\xc3":
        v = bs[1]
        if v >= 0x80:
            v -= 0x100
        return ("int", v)
    if len(bs) == 6 and bs[0] == 0xB8 and bs[-1] == 0xC3:
        return ("int", int.from_bytes(bs[1:5], "little", signed=False))
    if h == "8bc1c3":
        return ("return_self", None)
    if h == "8bc1c20400":
        return ("return_self_pop4", None)
    if len(bs) == 4 and bs[:2] == b"\x8b\x41" and bs[-1] == 0xC3:
        return ("load_int_field", bs[2])
    if len(bs) == 7 and bs[:2] == b"\x8b\x81" and bs[-1] == 0xC3:
        return ("load_int_field", int.from_bytes(bs[2:6], "little", signed=True))
    if len(bs) == 4 and bs[:2] == b"\xdd\x41" and bs[-1] == 0xC3:
        return ("load_double_field", bs[2])
    if len(bs) == 6 and bs[:4] == b"\x8b\xc1\x83\xe0" and bs[-1] == 0xC3:
        return ("and_self", bs[4])
    if len(bs) == 8 and bs[:4] == b"\x8b\xc1\x83\xe0" and bs[-3:] == b"\xc2\x04\x00":
        return ("and_self_pop4", bs[4])
    if len(bs) == 7 and bs[:2] == b"\xc7\x01" and bs[-1] == 0xC3:
        return ("store_imm32", int.from_bytes(bs[2:6], "little", signed=False))
    if len(bs) == 10 and bs[0] == 0xB9 and bs[5] == 0xE9:
        return ("global_method_tail_thunk", None)
    if (
        len(bs) == 17
        and bs[0] == 0xA1
        and bs[5:11] == b"\x85\xc0\x74\x07\x50\xe8"
        and bs[-2:] == b"\x59\xc3"
    ):
        return ("optional_global_pointer_call", None)
    if (
        len(bs) == 17
        and bs[0] == 0xB9
        and bs[5] == 0xE8
        and bs[10:12] == b"\xff\x0d"
        and bs[-1] == 0xC3
    ):
        return ("global_method_call_then_decrement", None)
    if (
        len(bs) == 20
        and bs[:2] == b"\xc7\x05"
        and bs[10] == 0xB9
        and bs[15] == 0xE9
    ):
        return (
            "global_field_store_then_method_tail",
            int.from_bytes(bs[6:10], "little", signed=False),
        )
    if (
        len(bs) == 27
        and bs[0] == 0xB9
        and bs[5] == 0xE8
        and bs[10] == 0xA1
        and bs[15:21] == b"\x85\xc0\x74\x07\x50\xe8"
        and bs[-2:] == b"\x59\xc3"
    ):
        return ("global_method_then_optional_pointer_call", None)
    if (
        len(bs) == 30
        and bs[0] == 0xB9
        and bs[5:7] == b"\xc7\x05"
        and bs[15] == 0xE8
        and bs[20] == 0xB9
        and bs[25] == 0xE9
    ):
        return (
            "global_field_store_then_two_methods",
            int.from_bytes(bs[11:15], "little", signed=False),
        )
    if (
        len(bs) == 32
        and bs[:3] == b"\x56\x57\xbe"
        and bs[7] == 0xBF
        and bs[12:18] == b"\x8d\x64\x24\x00\x83\xee"
        and bs[19:22] == b"\x8b\xce\xe8"
        and bs[26:] == b"\x4f\x75\xf3\x5f\x5e\xc3"
    ):
        return (
            "reverse_global_object_method_loop",
            (
                int.from_bytes(bs[8:12], "little", signed=False),
                bs[18],
            ),
        )
    if (
        len(bs) == 34
        and bs[:5] == b"\x56\x8b\xf1\xc7\x06"
        and bs[9] == 0xE8
        and bs[14:22] == b"\xf6\x44\x24\x08\x01\x74\x07\x56"
        and bs[22] == 0xE8
        and bs[27:] == b"\x59\x8b\xc6\x5e\xc2\x04\x00"
    ):
        return ("vector_deleting_destructor_with_vftable", None)
    if (
        len(bs) == 28
        and bs[:4] == b"\x56\x8b\xf1\xe8"
        and bs[8:16] == b"\xf6\x44\x24\x08\x01\x74\x07\x56"
        and bs[16] == 0xE8
        and bs[21:] == b"\x59\x8b\xc6\x5e\xc2\x04\x00"
    ):
        return ("scalar_deleting_destructor", None)
    if (
        len(bs) == 38
        and bs[:5] == b"\x56\x8b\xf1\x8d\x4e"
        and bs[6] == 0xE8
        and bs[11:14] == b"\x8b\xce\xe8"
        and bs[18:26] == b"\xf6\x44\x24\x08\x01\x74\x07\x56"
        and bs[26] == 0xE8
        and bs[31:] == b"\x59\x8b\xc6\x5e\xc2\x04\x00"
    ):
        return ("composite_scalar_deleting_destructor", bs[5])
    if (
        len(bs) == 35
        and bs[:14] == b"\x51\x56\x8b\xf1\x8b\x56\x04\x8b\x0e"
        b"\x8d\x44\x24\x07\x50"
        and bs[14] == 0xE8
        and bs[19:27] == b"\x8b\x36\x85\xf6\x74\x07\x56\xe8"
        and bs[31:] == b"\x59\x5e\x59\xc3"
    ):
        return ("finish_async_read_then_release", None)
    if (
        len(bs) == 41
        and bs[:15] == b"\x56\x8b\xf1\x83\x7e\x04\x00\x74\x1e"
        b"\x8b\x06\xff\x70\x04\xe8"
        and bs[19:] == b"\x8b\x06\x89\x40\x08\x8b\x06\x83\x60\x04\x00"
        b"\x8b\x06\x89\x40\x0c\x83\x66\x04\x00\x5e\xc3"
    ):
        return ("free_and_reset_intrusive_list", None)
    if (
        len(bs) == 45
        and bs[:18] == b"\x53\x56\x8b\x74\x24\x0c\x85\xf6\x8b\xd9\x74\x1c"
        b"\x57\xff\x76\x0c\x8b\xcb"
        and bs[18] == 0xE8
        and bs[23:28] == b"\x8b\x7e\x08\x56\xe8"
        and bs[32:] == b"\x85\xff\x59\x8b\xf7\x75\xe6\x5f\x5e\x5b\xc2\x04\x00"
    ):
        return ("consume_linked_tree", None)
    return None


def candidate(row):
    addr = row["address"].lower()
    name = row.get("name", f"sub_{addr}")
    bs = bytes.fromhex(row["bytes"])
    k = const_from_bytes(bs)
    if not k:
        return None
    rettype, value = k
    if rettype == "global_method_tail_thunk":
        # The manifest names on this late-image thunk island are inherited from
        # one oversized template range and are not trustworthy function names.
        # Keep the honest unresolved role plus the address until PDB/RTTI work
        # can identify each concrete object and method.
        name = f"GlobalMethodTailThunk_{addr}"
        cls, leaf = "global", name
        module = "_global"
    elif rettype in (
        "scalar_deleting_destructor",
        "composite_scalar_deleting_destructor",
    ):
        module = row.get("module") or "_global"
        cls = sanitize(module)
        leaf = (
            "ScalarDeletingDestructor"
            if rettype == "scalar_deleting_destructor"
            else "CompositeScalarDeletingDestructor"
        )
    else:
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
    elif rettype == "void_stdcall_pop":
        argument_count = value // 4
        parameters = ", ".join(f"int arg{index}" for index in range(argument_count))
        arguments = ", ".join(str(index + 1) for index in range(argument_count))
        source = f"void __stdcall {fn}({parameters}) {{}}\n"
        test = (
            "#include <cstdio>\n"
            f"void __stdcall {fn}({parameters}) {{}}\n"
            "int main()\n"
            "{\n"
            f"    {fn}({arguments});\n"
            f"    std::printf(\"{pattern}\\n\");\n"
            "    return 0;\n"
            "}\n"
        )
    elif rettype == "bool":
        literal = "true" if value else "false"
        source = f"bool __fastcall {fn}()\n{{\n    return {literal};\n}}\n"
        test = (
            "#include <cstdio>\n"
            f"bool __fastcall {fn}()\n{{\n    return {literal};\n}}\n"
            "int main()\n"
            "{\n"
            f"    if ({fn}() == {literal}) {{ std::printf(\"{pattern}\\n\"); return 0; }}\n"
            f"    std::printf(\"AUTO_TINY_{addr}_TEST FAIL\\n\");\n"
            "    return 1;\n"
            "}\n"
        )
    elif rettype == "return_self":
        source = f"void* __fastcall {fn}(void* self)\n{{\n    return self;\n}}\n"
        test = (
            "#include <cstdio>\n"
            f"void* __fastcall {fn}(void* self)\n{{\n    return self;\n}}\n"
            "int main()\n"
            "{\n"
            "    int value = 0;\n"
            f"    if ({fn}(&value) == &value) {{ std::printf(\"{pattern}\\n\"); return 0; }}\n"
            f"    std::printf(\"AUTO_TINY_{addr}_TEST FAIL\\n\");\n"
            "    return 1;\n"
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
    elif rettype == "global_method_tail_thunk":
        source = (
            "struct AutoTinyThunkTarget\n"
            "{\n"
            "    void Invoke();\n"
            "};\n"
            "extern AutoTinyThunkTarget g_AutoTinyThunkObject;\n"
            f"void __fastcall {fn}()\n"
            "{\n"
            "    g_AutoTinyThunkObject.Invoke();\n"
            "}\n"
        )
        test = (
            "#include <cstdio>\n"
            "static int g_AutoTinyThunkCalls = 0;\n"
            "struct AutoTinyThunkTarget\n"
            "{\n"
            "    void Invoke();\n"
            "};\n"
            "AutoTinyThunkTarget g_AutoTinyThunkObject;\n"
            "void AutoTinyThunkTarget::Invoke()\n"
            "{\n"
            "    ++g_AutoTinyThunkCalls;\n"
            "}\n"
            f"void __fastcall {fn}()\n"
            "{\n"
            "    g_AutoTinyThunkObject.Invoke();\n"
            "}\n"
            "int main()\n"
            "{\n"
            f"    {fn}();\n"
            "    if (g_AutoTinyThunkCalls == 1) "
            f"{{ std::printf(\"{pattern}\\n\"); return 0; }}\n"
            f"    std::printf(\"AUTO_TINY_{addr}_TEST FAIL\\n\");\n"
            "    return 1;\n"
            "}\n"
        )
    elif rettype == "optional_global_pointer_call":
        source = (
            "extern void* g_AutoTinyOptionalObject;\n"
            "extern void __cdecl AutoTinyOptionalTarget(void* object);\n"
            f"void __fastcall {fn}()\n"
            "{\n"
            "    if (g_AutoTinyOptionalObject != 0)\n"
            "        AutoTinyOptionalTarget(g_AutoTinyOptionalObject);\n"
            "}\n"
        )
        test = (
            "#include <cstdio>\n"
            "static int g_AutoTinyOptionalValue = 0;\n"
            "static void* g_AutoTinyObservedObject = 0;\n"
            "void* g_AutoTinyOptionalObject = &g_AutoTinyOptionalValue;\n"
            "void __cdecl AutoTinyOptionalTarget(void* object)\n"
            "{\n"
            "    g_AutoTinyObservedObject = object;\n"
            "}\n"
            f"void __fastcall {fn}()\n"
            "{\n"
            "    if (g_AutoTinyOptionalObject != 0)\n"
            "        AutoTinyOptionalTarget(g_AutoTinyOptionalObject);\n"
            "}\n"
            "int main()\n"
            "{\n"
            f"    {fn}();\n"
            "    if (g_AutoTinyObservedObject == g_AutoTinyOptionalObject) "
            f"{{ std::printf(\"{pattern}\\n\"); return 0; }}\n"
            f"    std::printf(\"AUTO_TINY_{addr}_TEST FAIL\\n\");\n"
            "    return 1;\n"
            "}\n"
        )
    elif rettype == "global_method_call_then_decrement":
        source = (
            "struct AutoTinyMethodTarget\n"
            "{\n"
            "    void Invoke();\n"
            "};\n"
            "extern AutoTinyMethodTarget g_AutoTinyMethodObject;\n"
            "extern unsigned int g_AutoTinyMethodCounter;\n"
            f"void __fastcall {fn}()\n"
            "{\n"
            "    g_AutoTinyMethodObject.Invoke();\n"
            "    --g_AutoTinyMethodCounter;\n"
            "}\n"
        )
        test = (
            "#include <cstdio>\n"
            "static int g_AutoTinyMethodCalls = 0;\n"
            "struct AutoTinyMethodTarget\n"
            "{\n"
            "    void Invoke();\n"
            "};\n"
            "AutoTinyMethodTarget g_AutoTinyMethodObject;\n"
            "unsigned int g_AutoTinyMethodCounter = 2;\n"
            "void AutoTinyMethodTarget::Invoke()\n"
            "{\n"
            "    ++g_AutoTinyMethodCalls;\n"
            "}\n"
            f"void __fastcall {fn}()\n"
            "{\n"
            "    g_AutoTinyMethodObject.Invoke();\n"
            "    --g_AutoTinyMethodCounter;\n"
            "}\n"
            "int main()\n"
            "{\n"
            f"    {fn}();\n"
            "    if (g_AutoTinyMethodCalls == 1 && "
            "g_AutoTinyMethodCounter == 1) "
            f"{{ std::printf(\"{pattern}\\n\"); return 0; }}\n"
            f"    std::printf(\"AUTO_TINY_{addr}_TEST FAIL\\n\");\n"
            "    return 1;\n"
            "}\n"
        )
    elif rettype == "global_field_store_then_method_tail":
        source = (
            "struct AutoTinyFieldTarget\n"
            "{\n"
            "    unsigned int value;\n"
            "    void Invoke();\n"
            "};\n"
            "extern AutoTinyFieldTarget g_AutoTinyFieldObject;\n"
            f"void __fastcall {fn}()\n"
            "{\n"
            f"    g_AutoTinyFieldObject.value = 0x{value:08x};\n"
            "    g_AutoTinyFieldObject.Invoke();\n"
            "}\n"
        )
        test = (
            "#include <cstdio>\n"
            "static int g_AutoTinyFieldCalls = 0;\n"
            "struct AutoTinyFieldTarget\n"
            "{\n"
            "    unsigned int value;\n"
            "    void Invoke();\n"
            "};\n"
            "AutoTinyFieldTarget g_AutoTinyFieldObject = {0};\n"
            "void AutoTinyFieldTarget::Invoke()\n"
            "{\n"
            "    ++g_AutoTinyFieldCalls;\n"
            "}\n"
            f"void __fastcall {fn}()\n"
            "{\n"
            f"    g_AutoTinyFieldObject.value = 0x{value:08x};\n"
            "    g_AutoTinyFieldObject.Invoke();\n"
            "}\n"
            "int main()\n"
            "{\n"
            f"    {fn}();\n"
            f"    if (g_AutoTinyFieldObject.value == 0x{value:08x} && "
            "g_AutoTinyFieldCalls == 1) "
            f"{{ std::printf(\"{pattern}\\n\"); return 0; }}\n"
            f"    std::printf(\"AUTO_TINY_{addr}_TEST FAIL\\n\");\n"
            "    return 1;\n"
            "}\n"
        )
    elif rettype == "global_method_then_optional_pointer_call":
        source = (
            "struct AutoTinyMethodOptionalTarget\n"
            "{\n"
            "    void Invoke();\n"
            "};\n"
            "extern AutoTinyMethodOptionalTarget "
            "g_AutoTinyMethodOptionalObject;\n"
            "extern void* g_AutoTinyMethodOptionalPointer;\n"
            "extern void __cdecl AutoTinyMethodOptionalCall(void* object);\n"
            f"void __fastcall {fn}()\n"
            "{\n"
            "    g_AutoTinyMethodOptionalObject.Invoke();\n"
            "    if (g_AutoTinyMethodOptionalPointer != 0)\n"
            "        AutoTinyMethodOptionalCall("
            "g_AutoTinyMethodOptionalPointer);\n"
            "}\n"
        )
        test = (
            "#include <cstdio>\n"
            "static int g_AutoTinyMethodOptionalCalls = 0;\n"
            "static int g_AutoTinyMethodOptionalValue = 0;\n"
            "static void* g_AutoTinyMethodOptionalObserved = 0;\n"
            "struct AutoTinyMethodOptionalTarget\n"
            "{\n"
            "    void Invoke();\n"
            "};\n"
            "AutoTinyMethodOptionalTarget g_AutoTinyMethodOptionalObject;\n"
            "void* g_AutoTinyMethodOptionalPointer = "
            "&g_AutoTinyMethodOptionalValue;\n"
            "void AutoTinyMethodOptionalTarget::Invoke()\n"
            "{\n"
            "    ++g_AutoTinyMethodOptionalCalls;\n"
            "}\n"
            "void __cdecl AutoTinyMethodOptionalCall(void* object)\n"
            "{\n"
            "    g_AutoTinyMethodOptionalObserved = object;\n"
            "}\n"
            f"void __fastcall {fn}()\n"
            "{\n"
            "    g_AutoTinyMethodOptionalObject.Invoke();\n"
            "    if (g_AutoTinyMethodOptionalPointer != 0)\n"
            "        AutoTinyMethodOptionalCall("
            "g_AutoTinyMethodOptionalPointer);\n"
            "}\n"
            "int main()\n"
            "{\n"
            f"    {fn}();\n"
            "    if (g_AutoTinyMethodOptionalCalls == 1 && "
            "g_AutoTinyMethodOptionalObserved == "
            "g_AutoTinyMethodOptionalPointer) "
            f"{{ std::printf(\"{pattern}\\n\"); return 0; }}\n"
            f"    std::printf(\"AUTO_TINY_{addr}_TEST FAIL\\n\");\n"
            "    return 1;\n"
            "}\n"
        )
    elif rettype == "global_field_store_then_two_methods":
        source = (
            "struct AutoTinyFieldMethodsTarget\n"
            "{\n"
            "    unsigned int value;\n"
            "    void First();\n"
            "    void Second();\n"
            "};\n"
            "extern AutoTinyFieldMethodsTarget "
            "g_AutoTinyFieldMethodsObject;\n"
            f"void __fastcall {fn}()\n"
            "{\n"
            f"    g_AutoTinyFieldMethodsObject.value = 0x{value:08x};\n"
            "    g_AutoTinyFieldMethodsObject.First();\n"
            "    g_AutoTinyFieldMethodsObject.Second();\n"
            "}\n"
        )
        test = (
            "#include <cstdio>\n"
            "static int g_AutoTinyFieldFirstCalls = 0;\n"
            "static int g_AutoTinyFieldSecondCalls = 0;\n"
            "struct AutoTinyFieldMethodsTarget\n"
            "{\n"
            "    unsigned int value;\n"
            "    void First();\n"
            "    void Second();\n"
            "};\n"
            "AutoTinyFieldMethodsTarget g_AutoTinyFieldMethodsObject = {0};\n"
            "void AutoTinyFieldMethodsTarget::First()\n"
            "{\n"
            "    ++g_AutoTinyFieldFirstCalls;\n"
            "}\n"
            "void AutoTinyFieldMethodsTarget::Second()\n"
            "{\n"
            "    ++g_AutoTinyFieldSecondCalls;\n"
            "}\n"
            f"void __fastcall {fn}()\n"
            "{\n"
            f"    g_AutoTinyFieldMethodsObject.value = 0x{value:08x};\n"
            "    g_AutoTinyFieldMethodsObject.First();\n"
            "    g_AutoTinyFieldMethodsObject.Second();\n"
            "}\n"
            "int main()\n"
            "{\n"
            f"    {fn}();\n"
            f"    if (g_AutoTinyFieldMethodsObject.value == 0x{value:08x} && "
            "g_AutoTinyFieldFirstCalls == 1 && "
            "g_AutoTinyFieldSecondCalls == 1) "
            f"{{ std::printf(\"{pattern}\\n\"); return 0; }}\n"
            f"    std::printf(\"AUTO_TINY_{addr}_TEST FAIL\\n\");\n"
            "    return 1;\n"
            "}\n"
        )
    elif rettype == "reverse_global_object_method_loop":
        count, stride = value
        source = (
            "struct AutoTinyReverseLoopTarget\n"
            "{\n"
            f"    unsigned char storage[{stride}];\n"
            "    void Invoke();\n"
            "};\n"
            "extern AutoTinyReverseLoopTarget "
            f"g_AutoTinyReverseLoopObjects[{count}];\n"
            f"void __fastcall {fn}()\n"
            "{\n"
            "    AutoTinyReverseLoopTarget* cursor =\n"
            f"        g_AutoTinyReverseLoopObjects + {count};\n"
            f"    for (int remaining = {count}; remaining != 0; --remaining)\n"
            "    {\n"
            "        --cursor;\n"
            "        cursor->Invoke();\n"
            "    }\n"
            "}\n"
        )
        test = (
            "#include <cstdio>\n"
            "static int g_AutoTinyReverseLoopCalls = 0;\n"
            "struct AutoTinyReverseLoopTarget\n"
            "{\n"
            f"    unsigned char storage[{stride}];\n"
            "    void Invoke();\n"
            "};\n"
            "AutoTinyReverseLoopTarget "
            f"g_AutoTinyReverseLoopObjects[{count}] = {{0}};\n"
            "void AutoTinyReverseLoopTarget::Invoke()\n"
            "{\n"
            "    ++g_AutoTinyReverseLoopCalls;\n"
            "}\n"
            f"void __fastcall {fn}()\n"
            "{\n"
            "    AutoTinyReverseLoopTarget* cursor =\n"
            f"        g_AutoTinyReverseLoopObjects + {count};\n"
            f"    for (int remaining = {count}; remaining != 0; --remaining)\n"
            "    {\n"
            "        --cursor;\n"
            "        cursor->Invoke();\n"
            "    }\n"
            "}\n"
            "int main()\n"
            "{\n"
            f"    {fn}();\n"
            f"    if (g_AutoTinyReverseLoopCalls == {count}) "
            f"{{ std::printf(\"{pattern}\\n\"); return 0; }}\n"
            f"    std::printf(\"AUTO_TINY_{addr}_TEST FAIL\\n\");\n"
            "    return 1;\n"
            "}\n"
        )
    elif rettype == "vector_deleting_destructor_with_vftable":
        source = (
            '#pragma optimize("s", on)\n'
            "struct AutoTinyVectorObject\n"
            "{\n"
            "    void* vftable;\n"
            "};\n"
            "extern void* const g_AutoTinyVectorVftable;\n"
            "extern void __fastcall AutoTinyVectorDestructor(void* self);\n"
            "extern void __cdecl AutoTinyVectorDelete(void* object);\n"
            f"void* __fastcall {fn}("
            "AutoTinyVectorObject* self, int, unsigned int flags)\n"
            "{\n"
            "    self->vftable = (void*)&g_AutoTinyVectorVftable;\n"
            "    AutoTinyVectorDestructor(self);\n"
            "    if (flags & 1)\n"
            "        AutoTinyVectorDelete(self);\n"
            "    return self;\n"
            "}\n"
        )
        test = (
            "#include <cstdio>\n"
            "static int g_AutoTinyVectorDestructorCalls = 0;\n"
            "static int g_AutoTinyVectorDeleteCalls = 0;\n"
            "struct AutoTinyVectorObject\n"
            "{\n"
            "    void* vftable;\n"
            "};\n"
            "void* const g_AutoTinyVectorVftable = "
            "(void*)0x12345678;\n"
            "void __fastcall AutoTinyVectorDestructor(void*)\n"
            "{\n"
            "    ++g_AutoTinyVectorDestructorCalls;\n"
            "}\n"
            "void __cdecl AutoTinyVectorDelete(void*)\n"
            "{\n"
            "    ++g_AutoTinyVectorDeleteCalls;\n"
            "}\n"
            f"void* __fastcall {fn}("
            "AutoTinyVectorObject* self, int, unsigned int flags)\n"
            "{\n"
            "    self->vftable = (void*)&g_AutoTinyVectorVftable;\n"
            "    AutoTinyVectorDestructor(self);\n"
            "    if (flags & 1)\n"
            "        AutoTinyVectorDelete(self);\n"
            "    return self;\n"
            "}\n"
            "int main()\n"
            "{\n"
            "    AutoTinyVectorObject object = {0};\n"
            f"    if ({fn}(&object, 0, 0) != &object ||\n"
            "        object.vftable != &g_AutoTinyVectorVftable ||\n"
            "        g_AutoTinyVectorDestructorCalls != 1 ||\n"
            "        g_AutoTinyVectorDeleteCalls != 0)\n"
            "        return 1;\n"
            f"    {fn}(&object, 0, 1);\n"
            "    if (g_AutoTinyVectorDestructorCalls != 2 ||\n"
            "        g_AutoTinyVectorDeleteCalls != 1)\n"
            "        return 1;\n"
            f"    std::printf(\"{pattern}\\n\");\n"
            "    return 0;\n"
            "}\n"
        )
    elif rettype == "scalar_deleting_destructor":
        source = (
            '#pragma optimize("s", on)\n'
            "extern void __fastcall AutoTinyScalarDestructor(void* self);\n"
            "extern void __cdecl AutoTinyScalarDelete(void* object);\n"
            f"void* __fastcall {fn}("
            "void* self, int, unsigned int flags)\n"
            "{\n"
            "    AutoTinyScalarDestructor(self);\n"
            "    if (flags & 1)\n"
            "        AutoTinyScalarDelete(self);\n"
            "    return self;\n"
            "}\n"
        )
        test = (
            "#include <cstdio>\n"
            "static int g_AutoTinyScalarDestructorCalls = 0;\n"
            "static int g_AutoTinyScalarDeleteCalls = 0;\n"
            "void __fastcall AutoTinyScalarDestructor(void*)\n"
            "{\n"
            "    ++g_AutoTinyScalarDestructorCalls;\n"
            "}\n"
            "void __cdecl AutoTinyScalarDelete(void*)\n"
            "{\n"
            "    ++g_AutoTinyScalarDeleteCalls;\n"
            "}\n"
            f"void* __fastcall {fn}("
            "void* self, int, unsigned int flags)\n"
            "{\n"
            "    AutoTinyScalarDestructor(self);\n"
            "    if (flags & 1)\n"
            "        AutoTinyScalarDelete(self);\n"
            "    return self;\n"
            "}\n"
            "int main()\n"
            "{\n"
            "    int object = 0;\n"
            f"    if ({fn}(&object, 0, 0) != &object ||\n"
            "        g_AutoTinyScalarDestructorCalls != 1 ||\n"
            "        g_AutoTinyScalarDeleteCalls != 0)\n"
            "        return 1;\n"
            f"    {fn}(&object, 0, 1);\n"
            "    if (g_AutoTinyScalarDestructorCalls != 2 ||\n"
            "        g_AutoTinyScalarDeleteCalls != 1)\n"
            "        return 1;\n"
            f"    std::printf(\"{pattern}\\n\");\n"
            "    return 0;\n"
            "}\n"
        )
    elif rettype == "composite_scalar_deleting_destructor":
        source = (
            '#pragma optimize("s", on)\n'
            "extern void __fastcall AutoTinyMemberDestructor(void* member);\n"
            "extern void __fastcall AutoTinyCompositeDestructor(void* self);\n"
            "extern void __cdecl AutoTinyCompositeDelete(void* object);\n"
            f"void* __fastcall {fn}("
            "void* self, int, unsigned int flags)\n"
            "{\n"
            "    AutoTinyMemberDestructor("
            f"static_cast<unsigned char*>(self) + {value});\n"
            "    AutoTinyCompositeDestructor(self);\n"
            "    if (flags & 1)\n"
            "        AutoTinyCompositeDelete(self);\n"
            "    return self;\n"
            "}\n"
        )
        test = (
            "#include <cstdio>\n"
            "static int g_AutoTinyMemberDestructorCalls = 0;\n"
            "static int g_AutoTinyCompositeDestructorCalls = 0;\n"
            "static int g_AutoTinyCompositeDeleteCalls = 0;\n"
            "static void* g_AutoTinyObservedMember = 0;\n"
            "void __fastcall AutoTinyMemberDestructor(void* member)\n"
            "{\n"
            "    ++g_AutoTinyMemberDestructorCalls;\n"
            "    g_AutoTinyObservedMember = member;\n"
            "}\n"
            "void __fastcall AutoTinyCompositeDestructor(void*)\n"
            "{\n"
            "    ++g_AutoTinyCompositeDestructorCalls;\n"
            "}\n"
            "void __cdecl AutoTinyCompositeDelete(void*)\n"
            "{\n"
            "    ++g_AutoTinyCompositeDeleteCalls;\n"
            "}\n"
            f"void* __fastcall {fn}("
            "void* self, int, unsigned int flags)\n"
            "{\n"
            "    AutoTinyMemberDestructor("
            f"static_cast<unsigned char*>(self) + {value});\n"
            "    AutoTinyCompositeDestructor(self);\n"
            "    if (flags & 1)\n"
            "        AutoTinyCompositeDelete(self);\n"
            "    return self;\n"
            "}\n"
            "int main()\n"
            "{\n"
            "    unsigned char object[256] = {0};\n"
            f"    if ({fn}(object, 0, 0) != object ||\n"
            f"        g_AutoTinyObservedMember != object + {value} ||\n"
            "        g_AutoTinyMemberDestructorCalls != 1 ||\n"
            "        g_AutoTinyCompositeDestructorCalls != 1 ||\n"
            "        g_AutoTinyCompositeDeleteCalls != 0)\n"
            "        return 1;\n"
            f"    {fn}(object, 0, 1);\n"
            "    if (g_AutoTinyMemberDestructorCalls != 2 ||\n"
            "        g_AutoTinyCompositeDestructorCalls != 2 ||\n"
            "        g_AutoTinyCompositeDeleteCalls != 1)\n"
            "        return 1;\n"
            f"    std::printf(\"{pattern}\\n\");\n"
            "    return 0;\n"
            "}\n"
        )
    elif rettype == "finish_async_read_then_release":
        source = (
            '#pragma optimize("s", on)\n'
            "struct AutoTinyActiveFile\n"
            "{\n"
            "    void* resource;\n"
            "    void* reader;\n"
            "};\n"
            "extern void __fastcall AutoTinyFinishRead("
            "void* resource, void* reader, char* status);\n"
            "extern void __cdecl AutoTinyReleaseResource(void* resource);\n"
            f"void __fastcall {fn}(AutoTinyActiveFile* self)\n"
            "{\n"
            "    char status;\n"
            "    AutoTinyFinishRead(self->resource, self->reader, &status);\n"
            "    if (self->resource != 0)\n"
            "        AutoTinyReleaseResource(self->resource);\n"
            "}\n"
        )
        test = (
            "#include <cstdio>\n"
            "struct AutoTinyActiveFile\n"
            "{\n"
            "    void* resource;\n"
            "    void* reader;\n"
            "};\n"
            "static int g_AutoTinyFinishReadCalls = 0;\n"
            "static int g_AutoTinyReleaseResourceCalls = 0;\n"
            "static void* g_AutoTinyFinishedResource = 0;\n"
            "static void* g_AutoTinyFinishedReader = 0;\n"
            "void __fastcall AutoTinyFinishRead("
            "void* resource, void* reader, char* status)\n"
            "{\n"
            "    ++g_AutoTinyFinishReadCalls;\n"
            "    g_AutoTinyFinishedResource = resource;\n"
            "    g_AutoTinyFinishedReader = reader;\n"
            "    *status = 1;\n"
            "}\n"
            "void __cdecl AutoTinyReleaseResource(void*)\n"
            "{\n"
            "    ++g_AutoTinyReleaseResourceCalls;\n"
            "}\n"
            f"void __fastcall {fn}(AutoTinyActiveFile* self)\n"
            "{\n"
            "    char status;\n"
            "    AutoTinyFinishRead(self->resource, self->reader, &status);\n"
            "    if (self->resource != 0)\n"
            "        AutoTinyReleaseResource(self->resource);\n"
            "}\n"
            "int main()\n"
            "{\n"
            "    int resource = 0;\n"
            "    int reader = 0;\n"
            "    AutoTinyActiveFile present = {&resource, &reader};\n"
            f"    {fn}(&present);\n"
            "    if (g_AutoTinyFinishReadCalls != 1 ||\n"
            "        g_AutoTinyReleaseResourceCalls != 1 ||\n"
            "        g_AutoTinyFinishedResource != &resource ||\n"
            "        g_AutoTinyFinishedReader != &reader)\n"
            "        return 1;\n"
            "    AutoTinyActiveFile absent = {0, &reader};\n"
            f"    {fn}(&absent);\n"
            "    if (g_AutoTinyFinishReadCalls != 2 ||\n"
            "        g_AutoTinyReleaseResourceCalls != 1)\n"
            "        return 1;\n"
            f"    std::printf(\"{pattern}\\n\");\n"
            "    return 0;\n"
            "}\n"
        )
    elif rettype == "free_and_reset_intrusive_list":
        source = (
            '#pragma optimize("s", on)\n'
            "struct AutoTinyListNode\n"
            "{\n"
            "    void* reserved;\n"
            "    void* allocation;\n"
            "    AutoTinyListNode* next;\n"
            "    AutoTinyListNode* previous;\n"
            "};\n"
            "struct AutoTinyOwnedList\n"
            "{\n"
            "    AutoTinyListNode* head;\n"
            "    long count;\n"
            "};\n"
            "extern void __stdcall AutoTinyFreeListAllocation(void* allocation);\n"
            f"void __fastcall {fn}(AutoTinyOwnedList* self)\n"
            "{\n"
            "    if (self->count != 0)\n"
            "    {\n"
            "        AutoTinyFreeListAllocation(self->head->allocation);\n"
            "        self->head->next = self->head;\n"
            "        self->head->allocation = 0;\n"
            "        self->head->previous = self->head;\n"
            "        self->count = 0;\n"
            "    }\n"
            "}\n"
        )
        test = (
            "#include <cstdio>\n"
            "struct AutoTinyListNode\n"
            "{\n"
            "    void* reserved;\n"
            "    void* allocation;\n"
            "    AutoTinyListNode* next;\n"
            "    AutoTinyListNode* previous;\n"
            "};\n"
            "struct AutoTinyOwnedList\n"
            "{\n"
            "    AutoTinyListNode* head;\n"
            "    long count;\n"
            "};\n"
            "static int g_AutoTinyFreeListCalls = 0;\n"
            "static void* g_AutoTinyFreedAllocation = 0;\n"
            "void __stdcall AutoTinyFreeListAllocation(void* allocation)\n"
            "{\n"
            "    ++g_AutoTinyFreeListCalls;\n"
            "    g_AutoTinyFreedAllocation = allocation;\n"
            "}\n"
            f"void __fastcall {fn}(AutoTinyOwnedList* self)\n"
            "{\n"
            "    if (self->count != 0)\n"
            "    {\n"
            "        AutoTinyFreeListAllocation(self->head->allocation);\n"
            "        self->head->next = self->head;\n"
            "        self->head->allocation = 0;\n"
            "        self->head->previous = self->head;\n"
            "        self->count = 0;\n"
            "    }\n"
            "}\n"
            "int main()\n"
            "{\n"
            "    int allocation = 0;\n"
            "    AutoTinyListNode head = {0, &allocation, 0, 0};\n"
            "    AutoTinyOwnedList list = {&head, 1};\n"
            f"    {fn}(&list);\n"
            "    if (g_AutoTinyFreeListCalls != 1 ||\n"
            "        g_AutoTinyFreedAllocation != &allocation ||\n"
            "        head.next != &head || head.previous != &head ||\n"
            "        head.allocation != 0 || list.count != 0)\n"
            "        return 1;\n"
            f"    {fn}(&list);\n"
            "    if (g_AutoTinyFreeListCalls != 1)\n"
            "        return 1;\n"
            f"    std::printf(\"{pattern}\\n\");\n"
            "    return 0;\n"
            "}\n"
        )
    elif rettype == "consume_linked_tree":
        source = (
            '#pragma optimize("s", on)\n'
            "struct AutoTinyTreeNode\n"
            "{\n"
            "    char reserved[8];\n"
            "    AutoTinyTreeNode* next;\n"
            "    AutoTinyTreeNode* child;\n"
            "};\n"
            "struct AutoTinyTreeWalker { char unused; };\n"
            "struct AutoTinyTreeNodeArgument { AutoTinyTreeNode* node; };\n"
            "extern \"C\" void __cdecl AutoTinyFreeTreeNode("
            "AutoTinyTreeNode* node);\n"
            f"void __fastcall {fn}("
            "AutoTinyTreeWalker* self, AutoTinyTreeNodeArgument argument)\n"
            "{\n"
            "    AutoTinyTreeNode* node = argument.node;\n"
            "    while (node != 0)\n"
            "    {\n"
            "        AutoTinyTreeNodeArgument child = {node->child};\n"
            f"        {fn}(self, child);\n"
            "        AutoTinyTreeNode* next = node->next;\n"
            "        AutoTinyFreeTreeNode(node);\n"
            "        node = next;\n"
            "    }\n"
            "}\n"
        )
        test = (
            "#include <cstdio>\n"
            "struct AutoTinyTreeNode\n"
            "{\n"
            "    char reserved[8];\n"
            "    AutoTinyTreeNode* next;\n"
            "    AutoTinyTreeNode* child;\n"
            "};\n"
            "struct AutoTinyTreeWalker { char unused; };\n"
            "struct AutoTinyTreeNodeArgument { AutoTinyTreeNode* node; };\n"
            "static int g_AutoTinyFreedTreeNodes = 0;\n"
            "extern \"C\" void __cdecl AutoTinyFreeTreeNode("
            "AutoTinyTreeNode*)\n"
            "{\n"
            "    ++g_AutoTinyFreedTreeNodes;\n"
            "}\n"
            f"void __fastcall {fn}("
            "AutoTinyTreeWalker* self, AutoTinyTreeNodeArgument argument)\n"
            "{\n"
            "    AutoTinyTreeNode* node = argument.node;\n"
            "    while (node != 0)\n"
            "    {\n"
            "        AutoTinyTreeNodeArgument child = {node->child};\n"
            f"        {fn}(self, child);\n"
            "        AutoTinyTreeNode* next = node->next;\n"
            "        AutoTinyFreeTreeNode(node);\n"
            "        node = next;\n"
            "    }\n"
            "}\n"
            "int main()\n"
            "{\n"
            "    AutoTinyTreeNode child = {{0}, 0, 0};\n"
            "    AutoTinyTreeNode sibling = {{0}, 0, 0};\n"
            "    AutoTinyTreeNode root = {{0}, &sibling, &child};\n"
            "    AutoTinyTreeWalker walker = {0};\n"
            "    AutoTinyTreeNodeArgument argument = {&root};\n"
            f"    {fn}(&walker, argument);\n"
            "    if (g_AutoTinyFreedTreeNodes != 3)\n"
            "        return 1;\n"
            f"    std::printf(\"{pattern}\\n\");\n"
            "    return 0;\n"
            "}\n"
        )
    elif rettype == "load_int_field":
        source = (
            f"int __fastcall {fn}(const unsigned char* self)\n"
            "{\n"
            f"    return *reinterpret_cast<const int*>(self + {value});\n"
            "}\n"
        )
        test = (
            "#include <cstdio>\n"
            f"int __fastcall {fn}(const unsigned char* self)\n"
            "{\n"
            f"    return *reinterpret_cast<const int*>(self + {value});\n"
            "}\n"
            "int main()\n"
            "{\n"
            f"    unsigned char object[{value + 4}] = {{0}};\n"
            f"    *reinterpret_cast<int*>(object + {value}) = 0x12345678;\n"
            f"    if ({fn}(object) == 0x12345678) {{ std::printf(\"{pattern}\\n\"); return 0; }}\n"
            f"    std::printf(\"AUTO_TINY_{addr}_TEST FAIL\\n\");\n"
            "    return 1;\n"
            "}\n"
        )
    elif rettype == "load_double_field":
        source = (
            f"double __fastcall {fn}(const unsigned char* self)\n"
            "{\n"
            f"    return *reinterpret_cast<const double*>(self + {value});\n"
            "}\n"
        )
        test = (
            "#include <cstdio>\n"
            f"double __fastcall {fn}(const unsigned char* self)\n"
            "{\n"
            f"    return *reinterpret_cast<const double*>(self + {value});\n"
            "}\n"
            "int main()\n"
            "{\n"
            f"    unsigned char object[{value + 8}] = {{0}};\n"
            f"    *reinterpret_cast<double*>(object + {value}) = 123.5;\n"
            f"    if ({fn}(object) == 123.5) {{ std::printf(\"{pattern}\\n\"); return 0; }}\n"
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
    for row in rows_with_target_metadata(oracle):
        c = candidate(row)
        if c:
            authored.append(c)
    json.dump({"result": {"authored": authored}}, open(out, "w", encoding="utf-8"), indent=1)
    print(f"auto-authored {len(authored)} tiny candidates -> {out}")


if __name__ == "__main__":
    main()
