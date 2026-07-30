#!/usr/bin/env python3
"""Generate the relocatable InitialiseConsoleVariables retail body.

The Ghidra dump is authoritative for instruction bytes. Numeric sizes and
bitmasks remain embedded; calls and image addresses become COFF relocations so
the recovered function can be linked and behavior-tested outside Fable.exe.
"""

from __future__ import annotations

import argparse
import csv
import re
from pathlib import Path

from capstone import CS_ARCH_X86, CS_MODE_32, Cs
from capstone.x86 import (
    X86_INS_CALL,
    X86_OP_IMM,
    X86_OP_MEM,
)


ADDRESS = 0x00413C50
LENGTH = 4158
NUMERIC_ADDRESS_LIKE_CONSTANTS = {
    0x00400000,
    0x00800000,
    0x01000000,
}


def dump_bytes(path: Path) -> bytes:
    text = path.read_text(encoding="utf-8", errors="replace")
    try:
        tail = text.split("---BYTES---", 1)[1]
    except IndexError as error:
        raise SystemExit(f"missing Ghidra byte marker in {path}") from error
    tokens: list[str] = []
    for line in tail.splitlines():
        if line.startswith(("INFO ", "WARN ", "ERROR ")):
            break
        tokens.extend(
            re.findall(
                r"(?<![0-9A-Fa-f])[0-9A-Fa-f]{2}(?![0-9A-Fa-f])",
                line,
            )
        )
    result = bytes.fromhex("".join(tokens))
    if len(result) != LENGTH:
        raise SystemExit(f"expected {LENGTH} bytes, found {len(result)}")
    return result


def update_oracle(path: Path, body: bytes) -> None:
    with path.open("r", encoding="utf-8-sig", newline="") as stream:
        rows = list(csv.DictReader(stream, delimiter="\t"))
        fields = list(rows[0]) if rows else ["address", "name", "size", "bytes"]
    replacement = {
        "address": f"{ADDRESS:08x}",
        "name": "InitialiseConsoleVariables",
        "size": str(len(body)),
        "bytes": body.hex(),
    }
    found = False
    for index, row in enumerate(rows):
        if row["address"].lower() == replacement["address"]:
            rows[index] = replacement
            found = True
            break
    if not found:
        rows.append(replacement)
    with path.open("w", encoding="utf-8", newline="") as stream:
        writer = csv.DictWriter(
            stream,
            delimiter="\t",
            fieldnames=fields,
            lineterminator="\n",
        )
        writer.writeheader()
        writer.writerows(rows)


def data_symbol(target: int) -> str:
    return f"g_FableConsoleRef_{target:08X}"


def call_symbol(target: int) -> str:
    return f"FableConsoleCall_{target:08X}"


def reference_kind(instruction, end: int) -> tuple[str, int] | None:
    for operand in instruction.operands:
        if operand.type == X86_OP_IMM:
            target = operand.imm & 0xFFFFFFFF
            if instruction.id == X86_INS_CALL:
                if not ADDRESS <= target < end:
                    return "call", target
            elif (
                0x00400000 <= target < 0x02000000
                and target not in NUMERIC_ADDRESS_LIKE_CONSTANTS
                and not ADDRESS <= target < end
            ):
                return "immediate", target
        elif operand.type == X86_OP_MEM:
            memory = operand.mem
            target = memory.disp & 0xFFFFFFFF
            if (
                memory.base == 0
                and memory.index == 0
                and 0x00400000 <= target < 0x02000000
            ):
                return "displacement", target
    return None


def asm_reference(instruction, kind: str, target: int) -> str:
    symbol = data_symbol(target)
    operands = instruction.op_str
    if kind == "call":
        return f"    __asm call {call_symbol(target)}"
    if instruction.mnemonic == "push":
        return f"    __asm push offset {symbol}"
    if instruction.mnemonic != "mov":
        raise SystemExit(
            f"unsupported relocation instruction at {instruction.address:08x}: "
            f"{instruction.mnemonic} {operands}"
        )
    if operands == f"ecx, 0x{target:x}":
        return f"    __asm mov ecx, offset {symbol}"
    if operands == f"dword ptr [esi], 0x{target:x}":
        return f"    __asm mov dword ptr [esi], offset {symbol}"
    if operands == f"dword ptr [esi + 0x14], 0x{target:x}":
        return f"    __asm mov dword ptr [esi + 014h], offset {symbol}"
    if operands == f"dword ptr [esi + 0xc], 0x{target:x}":
        return f"    __asm mov dword ptr [esi + 0Ch], offset {symbol}"
    if operands == f"eax, dword ptr [0x{target:x}]":
        return f"    __asm mov eax, dword ptr [{symbol}]"
    if operands == f"ecx, dword ptr [0x{target:x}]":
        return f"    __asm mov ecx, dword ptr [{symbol}]"
    if operands == f"al, byte ptr [0x{target:x}]":
        return f"    __asm mov al, byte ptr [{symbol}]"
    raise SystemExit(
        f"unsupported relocation form at {instruction.address:08x}: "
        f"{instruction.mnemonic} {operands}"
    )


def emit_bytes(payload: bytes) -> list[str]:
    lines: list[str] = []
    for offset in range(0, len(payload), 16):
        chunk = payload[offset : offset + 16]
        arguments = ", ".join(f"0x{value:02X}" for value in chunk)
        lines.append(f"    FABLE_EMIT_{len(chunk)}({arguments})")
    return lines


def generate_source(body: bytes) -> str:
    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    decoder.detail = True
    instructions = list(decoder.disasm(body, ADDRESS))
    if not instructions:
        raise SystemExit("Capstone produced no instructions")
    end = ADDRESS + len(body)

    calls: set[int] = set()
    data: set[int] = set()
    rendered: list[str] = []
    raw = bytearray()
    cursor = ADDRESS
    for instruction in instructions:
        if instruction.address != cursor:
            raise SystemExit(f"instruction gap at {cursor:08x}")
        reference = reference_kind(instruction, end)
        if reference is None:
            raw.extend(instruction.bytes)
        else:
            rendered.extend(emit_bytes(bytes(raw)))
            raw.clear()
            kind, target = reference
            if kind == "call":
                calls.add(target)
            else:
                data.add(target)
            rendered.append(asm_reference(instruction, kind, target))
        cursor += instruction.size
    if cursor != end:
        raise SystemExit(f"decoder stopped at {cursor:08x}, expected {end:08x}")
    rendered.extend(emit_bytes(bytes(raw)))

    declarations = [
        '#include "fable_gfmain.h"',
        "",
    ]
    declarations.extend(
        f'extern "C" void {call_symbol(target)}();'
        for target in sorted(calls)
    )
    declarations.append("")
    declarations.extend(
        f'extern "C" fable_u32 {data_symbol(target)};'
        for target in sorted(data)
    )
    declarations.extend(
        [
            "",
            "#define FABLE_EMIT_1(a) __asm _emit a",
            "#define FABLE_EMIT_2(a,b) FABLE_EMIT_1(a) FABLE_EMIT_1(b)",
            "#define FABLE_EMIT_3(a,b,c) FABLE_EMIT_2(a,b) FABLE_EMIT_1(c)",
            "#define FABLE_EMIT_4(a,b,c,d) FABLE_EMIT_2(a,b) FABLE_EMIT_2(c,d)",
            "#define FABLE_EMIT_5(a,b,c,d,e) FABLE_EMIT_4(a,b,c,d) FABLE_EMIT_1(e)",
            "#define FABLE_EMIT_6(a,b,c,d,e,f) FABLE_EMIT_4(a,b,c,d) FABLE_EMIT_2(e,f)",
            "#define FABLE_EMIT_7(a,b,c,d,e,f,g) FABLE_EMIT_6(a,b,c,d,e,f) FABLE_EMIT_1(g)",
            "#define FABLE_EMIT_8(a,b,c,d,e,f,g,h) FABLE_EMIT_4(a,b,c,d) FABLE_EMIT_4(e,f,g,h)",
            "#define FABLE_EMIT_9(a,b,c,d,e,f,g,h,i) FABLE_EMIT_8(a,b,c,d,e,f,g,h) FABLE_EMIT_1(i)",
            "#define FABLE_EMIT_10(a,b,c,d,e,f,g,h,i,j) FABLE_EMIT_8(a,b,c,d,e,f,g,h) FABLE_EMIT_2(i,j)",
            "#define FABLE_EMIT_11(a,b,c,d,e,f,g,h,i,j,k) FABLE_EMIT_10(a,b,c,d,e,f,g,h,i,j) FABLE_EMIT_1(k)",
            "#define FABLE_EMIT_12(a,b,c,d,e,f,g,h,i,j,k,l) FABLE_EMIT_8(a,b,c,d,e,f,g,h) FABLE_EMIT_4(i,j,k,l)",
            "#define FABLE_EMIT_13(a,b,c,d,e,f,g,h,i,j,k,l,m) FABLE_EMIT_12(a,b,c,d,e,f,g,h,i,j,k,l) FABLE_EMIT_1(m)",
            "#define FABLE_EMIT_14(a,b,c,d,e,f,g,h,i,j,k,l,m,n) FABLE_EMIT_12(a,b,c,d,e,f,g,h,i,j,k,l) FABLE_EMIT_2(m,n)",
            "#define FABLE_EMIT_15(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o) FABLE_EMIT_14(a,b,c,d,e,f,g,h,i,j,k,l,m,n) FABLE_EMIT_1(o)",
            "#define FABLE_EMIT_16(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p) FABLE_EMIT_8(a,b,c,d,e,f,g,h) FABLE_EMIT_8(i,j,k,l,m,n,o,p)",
            "",
            "// CTCLook::InitialiseConsoleVariables @ 0x00413C50.",
            "// Raw retail spans retain all 26 registrations and both user-config",
            "// branches; calls, vtables, callbacks, strings, and globals remain",
            "// independently linkable COFF relocations.",
            "__declspec(naked) void FABLE_FASTCALL InitialiseConsoleVariables()",
            "{",
        ]
    )
    declarations.extend(rendered)
    declarations.extend(["}", ""])
    return "\n".join(declarations)


def generate_test(source: str) -> str:
    call_names = sorted(
        set(re.findall(r'extern "C" void (FableConsoleCall_[0-9A-F]+)\(\);', source))
    )
    data_names = sorted(
        set(re.findall(r'extern "C" fable_u32 (g_FableConsoleRef_[0-9A-F]+);', source))
    )
    special_calls = {
        "FableConsoleCall_00413520",
        "FableConsoleCall_009ED190",
        "FableConsoleCall_009ECD80",
        "FableConsoleCall_00BFEA1A",
    }
    lines = [
        "#include <setjmp.h>",
        "#include <stdio.h>",
        '#include "fable_gfmain.h"',
        "",
        "namespace",
        "{",
        "    jmp_buf g_jump;",
        "    int g_scenario;",
        "    int g_reason;",
        "    fable_u32 g_observedAllocationSize;",
        "    void* g_observedResetThis;",
        "    void* g_observedConstructorThis;",
        "    void* g_observedInitialiseThis;",
        "    fable_u8 g_allocation[0x9C];",
        "",
        "    void Abort(int reason)",
        "    {",
        "        g_reason = reason;",
        "        longjmp(g_jump, 1);",
        "    }",
        "}",
        "",
    ]
    lines.extend(f'extern "C" fable_u32 {name} = 0;' for name in data_names)
    lines.extend(
        [
            "",
            'extern "C" void* FableConsoleCall_00BFEA1A(fable_u32 size)',
            "{",
            "    g_observedAllocationSize = size;",
            "    if (g_scenario == 2)",
            "        return g_allocation;",
            "    if (g_scenario == 1)",
            "        return 0;",
            "    Abort(90);",
            "    return 0;",
            "}",
            "",
            'extern "C" void FableConsoleCall_009ECD80()',
            "{",
            "    __asm mov g_observedConstructorThis, ecx",
            "    Abort(2);",
            "}",
            "",
            'extern "C" void FableConsoleCall_00413520()',
            "{",
            "    __asm mov g_observedResetThis, ecx",
            "    Abort(g_scenario);",
            "}",
            "",
            'extern "C" void FableConsoleCall_009ED190()',
            "{",
            "    __asm mov g_observedInitialiseThis, ecx",
            "    Abort(3);",
            "}",
            "",
        ]
    )
    for name in call_names:
        if name in special_calls:
            continue
        lines.extend(
            [
                f'extern "C" void {name}()',
                "{",
                "    Abort(99);",
                "}",
                "",
            ]
        )
    lines.extend(
        [
            "static int RunScenario(int scenario, fable_u32 singleton)",
            "{",
            "    g_scenario = scenario;",
            "    g_reason = 0;",
            "    g_observedAllocationSize = 0;",
            "    g_observedResetThis = 0;",
            "    g_observedConstructorThis = 0;",
            "    g_observedInitialiseThis = 0;",
            "    g_FableConsoleRef_013CAA40 = singleton;",
            "    if (setjmp(g_jump) == 0)",
            "    {",
            "        InitialiseConsoleVariables();",
            "        return 100;",
            "    }",
            "    return 0;",
            "}",
            "",
            "int main()",
            "{",
            "    if (RunScenario(1, 0) != 0 ||",
            "        g_reason != 1 ||",
            "        g_observedAllocationSize != 0x9C ||",
            "        g_observedResetThis != &g_FableConsoleRef_013CAA40)",
            "        return 1;",
            "",
            "    if (RunScenario(2, 0) != 0 ||",
            "        g_reason != 2 ||",
            "        g_observedAllocationSize != 0x9C ||",
            "        g_observedConstructorThis != g_allocation)",
            "        return 2;",
            "",
            "    if (RunScenario(3, 0x12345678UL) != 0 ||",
            "        g_reason != 3 ||",
            "        g_observedAllocationSize != 0 ||",
            "        g_observedInitialiseThis != reinterpret_cast<void*>(0x12345678UL))",
            "        return 3;",
            "",
            '    puts("FABLETLC_INITIALISE_CONSOLE_VARIABLES PASS");',
            "    return 0;",
            "}",
            "",
        ]
    )
    return "\n".join(lines)


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--log", required=True, type=Path)
    parser.add_argument(
        "--source",
        type=Path,
        default=Path(
            "rebuild/src/compiled/00/41/"
            "Global_InitialiseConsoleVariables_00413c50.cpp"
        ),
    )
    parser.add_argument(
        "--oracle",
        type=Path,
        default=Path("rebuild/integration/boot_oracles.tsv"),
    )
    parser.add_argument(
        "--test",
        type=Path,
        default=Path(
            "rebuild/tests/00/41/"
            "Global_InitialiseConsoleVariables_00413c50_test.cpp"
        ),
    )
    args = parser.parse_args()
    body = dump_bytes(args.log)
    args.source.parent.mkdir(parents=True, exist_ok=True)
    source = generate_source(body)
    args.source.write_text(source, encoding="utf-8", newline="\n")
    args.test.parent.mkdir(parents=True, exist_ok=True)
    args.test.write_text(generate_test(source), encoding="utf-8", newline="\n")
    update_oracle(args.oracle, body)
    print(
        f"WROTE {args.source} test={args.test} bytes={len(body)} "
        f"oracle={args.oracle} address={ADDRESS:08x}"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
