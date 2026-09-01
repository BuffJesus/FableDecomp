#!/usr/bin/env python3
"""Emit standalone Lua only for completely recognized native helper semantics."""

from __future__ import annotations

import argparse
import hashlib
import json
from pathlib import Path
from typing import Any


def sha256(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest().upper()


def emit_switch(helper: dict[str, Any], pattern: dict[str, Any]) -> str:
    lines = [
        f"-- Retail helper {helper['targetAddress']} ({helper['currentName']})",
        "-- Generated only from a complete native constant-return switch.",
        "local cases = {",
    ]
    for case in pattern["cases"]:
        lines.append(f"    [{case['input']}] = {case['return']},")
    lines.extend(("}", "", "return function(value)",
                  f"    return cases[value] or {pattern['defaultReturn']}", "end", ""))
    return "\n".join(lines)


def emit_initializer(helper: dict[str, Any], pattern: dict[str, Any]) -> str:
    lines = [
        f"-- Retail helper {helper['targetAddress']} ({helper['currentName']})",
        "-- Field names are unresolved; preserve native offsets and write widths.",
        "return function(write_u8, write_u32)",
    ]
    for write in pattern["writes"]:
        function = "write_u8" if write["width"] == 1 else "write_u32"
        value = int(write["valueExpression"], 0)
        lines.append(f"    {function}({int(write['fieldOffset'], 0)}, {value})")
    lines.extend(("end", ""))
    return "\n".join(lines)


def emit_return(helper: dict[str, Any], pattern: dict[str, Any]) -> str:
    header = [f"-- Retail helper {helper['targetAddress']} ({helper['currentName']})"]
    if pattern["kind"] == "constant-return":
        return "\n".join(header + ["return function()", f"    return {pattern['return']}", "end", ""])
    if pattern["kind"] == "native-field-return":
        accessor = pattern["accessor"]
        expression = f"{accessor}({int(pattern['fieldOffset'], 0)})"
        if pattern["resultTransform"] == "not-zero":
            expression += " ~= 0"
        return "\n".join(header + ["-- Preserve the unnamed native field through a typed reader.",
            f"return function({accessor})", f"    return {expression}", "end", ""])
    if pattern["kind"] == "native-global-return":
        return "\n".join(header + ["-- Preserve the exact retail global address through a reader.",
            "return function(read_global)",
            f"    return read_global({int(pattern['globalAddress'], 0)})", "end", ""])
    raise ValueError(f"unsupported return pattern {pattern['kind']}")


def emit_interface_sequence(helper: dict[str, Any], pattern: dict[str, Any]) -> str:
    parameters = ["invoke", "read_i32", *pattern["parameters"]]
    lines = [f"-- Retail helper {helper['targetAddress']} ({helper['currentName']})",
             "-- Ordered interface calls and unnamed native fields are preserved exactly.",
             f"return function({', '.join(parameters)})"]
    for operation in pattern["operations"]:
        arguments = []
        for argument in operation["arguments"]:
            if argument["kind"] == "field-i32":
                arguments.append(f"read_i32({int(argument['offset'], 0)})")
            elif argument["kind"] == "parameter":
                arguments.append(argument["name"])
            elif argument["kind"] == "literal":
                value = argument["value"]
                arguments.append("true" if value is True else "false" if value is False else str(value))
            else:
                raise ValueError(f"unsupported interface argument {argument['kind']}")
        suffix = (", " + ", ".join(arguments)) if arguments else ""
        lines.append(f'    invoke("{operation["method"]}"{suffix})')
    lines.extend(("end", ""))
    return "\n".join(lines)


def emit_conditional_call_clear(helper: dict[str, Any], pattern: dict[str, Any]) -> str:
    condition = int(pattern["conditionOffset"], 0)
    argument = int(pattern["argumentOffset"], 0)
    target = int(pattern["callTarget"], 0)
    return "\n".join([
        f"-- Retail helper {helper['targetAddress']} ({helper['currentName']})",
        "-- Preserve the conditional byte state, exact native call target, and clearing write.",
        "return function(read_u8, write_u8, invoke_native)",
        f"    if read_u8({condition}) ~= 0 then",
        f"        invoke_native({target}, read_u8({argument}))",
        f"        write_u8({condition}, 0)",
        "    end", "end", "",
    ])


def emit_script_initializer(helper: dict[str, Any], pattern: dict[str, Any]) -> str:
    callbacks = ["set_string", "write_u8", "write_u32", "write_nested_u8"]
    if any(operation["kind"] == "invoke-native" for operation in pattern["operations"]):
        callbacks.append("invoke_native")
    lines = [f"-- Retail helper {helper['targetAddress']} ({helper['currentName']})",
             "-- Preserve initializer order and native offsets without inventing field names.",
             f"return function({', '.join(callbacks)})"]
    for operation in pattern["operations"]:
        if operation["kind"] == "set-string":
            value = operation["value"].replace("\\", "\\\\").replace('"', '\\"')
            lines.append(f'    set_string({int(operation["offset"], 0)}, "{value}")')
        elif operation["kind"] == "write-u8":
            lines.append(f'    write_u8({int(operation["offset"], 0)}, {operation["value"]})')
        elif operation["kind"] == "write-u32":
            lines.append(f'    write_u32({int(operation["offset"], 0)}, {operation["value"]})')
        elif operation["kind"] == "write-nested-u8":
            lines.append(f'    write_nested_u8({int(operation["pointerOffset"], 0)}, '
                         f'{int(operation["fieldOffset"], 0)}, {operation["value"]})')
        elif operation["kind"] == "invoke-native":
            arguments = ", ".join(str(value) for value in operation["arguments"])
            lines.append(f'    invoke_native({int(operation["target"], 0)}, {arguments})')
        else:
            raise ValueError(f"unsupported initializer operation {operation['kind']}")
    lines.extend(("end", ""))
    return "\n".join(lines)


def emit_archery_quest_info_setup(helper: dict[str, Any], pattern: dict[str, Any]) -> str:
    lines = [f"-- Retail helper {helper['targetAddress']} ({helper['currentName']})",
             "-- Arguments are recovered from the exact retail call-site instructions.",
             "return function(invoke, read_i32, write_i32, read_nested_i32, field_ref)"]
    for index, entry in enumerate(pattern["counterEntries"], 1):
        lines.append(f'    local handle_{index} = invoke("AddQuestInfoCounter", '
                     f'"{entry["text"]}", 0, 1.0)')
        lines.append(f'    write_i32({int(entry["handleOffset"], 0)}, handle_{index})')
    timer = pattern["timerEntry"]
    lines.append(f'    local timer_handle = invoke("AddQuestInfoTimer", '
                 f'field_ref({int(timer["timerOffset"], 0)}), "{timer["text"]}", 1.0)')
    lines.append(f'    write_i32({int(timer["handleOffset"], 0)}, timer_handle)')
    lines.append(f'    local score = read_nested_i32({int(pattern["scorePointerOffset"], 0)}, '
                 f'{int(pattern["scoreFieldOffset"], 0)})')
    first_handle = int(pattern["counterEntries"][0]["handleOffset"], 0)
    lines.append(f'    invoke("UpdateQuestInfoCounter", read_i32({first_handle}), score, -1)')
    lines.append('    invoke("UpdateOnlineScore_Archery", score)')
    lines.append('    invoke("DisplayQuestInfo", true)')
    lines.extend(("end", ""))
    return "\n".join(lines)


def generate(helper_ir_path: Path, output_dir: Path) -> dict[str, Any]:
    source_bytes = helper_ir_path.read_bytes()
    document = json.loads(source_bytes.decode("utf-8-sig"))
    output_dir.mkdir(parents=True, exist_ok=True)
    entries = []
    for helper in document["helpers"]:
        if not helper.get("luaEmissionReady"):
            continue
        patterns = [row for row in helper["semanticPatterns"]
                    if row["kind"] in {"constant-return-switch", "native-field-initializer",
                                       "constant-return", "native-field-return",
                                       "native-global-return", "quest-interface-sequence",
                                       "conditional-u8-call-clear"}
                                       | {"native-script-initializer", "archery-quest-info-setup"}
                    and row["complete"]]
        if len(patterns) != 1:
            raise ValueError(f"expected one complete switch for {helper['targetAddress']}")
        pattern = patterns[0]
        if pattern["kind"] == "constant-return-switch":
            text = emit_switch(helper, pattern)
        elif pattern["kind"] == "native-field-initializer":
            text = emit_initializer(helper, pattern)
        elif pattern["kind"] == "quest-interface-sequence":
            text = emit_interface_sequence(helper, pattern)
        elif pattern["kind"] == "conditional-u8-call-clear":
            text = emit_conditional_call_clear(helper, pattern)
        elif pattern["kind"] == "native-script-initializer":
            text = emit_script_initializer(helper, pattern)
        elif pattern["kind"] == "archery-quest-info-setup":
            text = emit_archery_quest_info_setup(helper, pattern)
        else:
            text = emit_return(helper, pattern)
        filename = helper["targetAddress"].removeprefix("0x") + ".lua"
        destination = output_dir / filename
        destination.write_text(text, encoding="utf-8", newline="\n")
        entries.append({
            "targetAddress": helper["targetAddress"], "currentName": helper["currentName"],
            "sourceDecompileSha256": helper["decompileSha256"],
            "luaFile": filename, "luaSha256": sha256(text.encode("utf-8")),
            "semanticPattern": pattern,
            "parentInitializerEvidence": helper.get("parentInitializerEvidence"),
            "deploymentEligible": False,
            "deploymentBlocker": "standalone helper only; parent script state/control flow unresolved",
        })
    manifest = {
        "schema": "forgefse-native-helper-lua/0.1",
        "source": str(helper_ir_path.resolve()),
        "sourceSha256": sha256(source_bytes),
        "summary": {"emitted": len(entries), "deploymentEligible": 0},
        "entries": entries,
    }
    (output_dir / "manifest.json").write_text(
        json.dumps(manifest, indent=2) + "\n", encoding="utf-8", newline="\n")
    return manifest


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--helper-ir", type=Path, required=True)
    parser.add_argument("--output-dir", type=Path, required=True)
    args = parser.parse_args()
    result = generate(args.helper_ir, args.output_dir)
    print(json.dumps(result["summary"], sort_keys=True))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
