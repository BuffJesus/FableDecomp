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
    for row in csv.DictReader(open(oracle, encoding="utf-8-sig"), delimiter="\t"):
        c = candidate(row)
        if c:
            authored.append(c)
    json.dump({"result": {"authored": authored}}, open(out, "w", encoding="utf-8"), indent=1)
    print(f"auto-authored {len(authored)} tiny candidates -> {out}")


if __name__ == "__main__":
    main()
