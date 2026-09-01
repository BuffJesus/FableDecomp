#!/usr/bin/env python3
"""Execute generated standalone helper Lua against its native-derived truth table."""

from __future__ import annotations

import argparse
import hashlib
import json
from pathlib import Path
from typing import Any

from lupa import LuaRuntime


def validate(manifest_path: Path) -> dict[str, Any]:
    manifest = json.loads(manifest_path.read_text(encoding="utf-8-sig"))
    root = manifest_path.parent
    rows = []
    for entry in manifest["entries"]:
        path = root / entry["luaFile"]
        payload = path.read_bytes()
        actual_hash = hashlib.sha256(payload).hexdigest().upper()
        errors = []
        if actual_hash != entry["luaSha256"]:
            errors.append("sha256 mismatch")
        function = LuaRuntime(unpack_returned_tuples=True).execute(payload.decode("utf-8"))
        pattern = entry["semanticPattern"]
        checks = 0
        if pattern["kind"] == "constant-return-switch":
            for case in pattern["cases"]:
                actual = function(case["input"])
                checks += 1
                if actual != case["return"]:
                    errors.append(f"input {case['input']}: expected {case['return']}, got {actual}")
            used = {case["input"] for case in pattern["cases"]}
            defaults = [value for value in (0, -1, max(used, default=0) + 1) if value not in used]
            for value in defaults:
                actual = function(value)
                checks += 1
                if actual != pattern["defaultReturn"]:
                    errors.append(f"default {value}: expected {pattern['defaultReturn']}, got {actual}")
        elif pattern["kind"] == "native-field-initializer":
            actual_writes = []
            function(lambda offset, value: actual_writes.append((offset, 1, value)),
                     lambda offset, value: actual_writes.append((offset, 4, value)))
            expected_writes = [(int(row["fieldOffset"], 0), row["width"],
                                int(row["valueExpression"], 0)) for row in pattern["writes"]]
            checks = len(expected_writes)
            if actual_writes != expected_writes:
                errors.append(f"writes differ: expected {expected_writes}, got {actual_writes}")
        elif pattern["kind"] == "constant-return":
            checks = 1
            actual = function()
            if actual != pattern["return"]:
                errors.append(f"expected {pattern['return']}, got {actual}")
        elif pattern["kind"] == "native-field-return":
            offset = int(pattern["fieldOffset"], 0)
            if pattern["resultTransform"] == "not-zero":
                probes = [(0, False), (1, True), (-1, True)]
            else:
                probes = [(0x12345678, 0x12345678)]
            checks = len(probes)
            for value, expected in probes:
                actual = function(lambda actual_offset, value=value: value
                                  if actual_offset == offset else None)
                if actual != expected:
                    errors.append(f"value {value}: expected {expected}, got {actual}")
        elif pattern["kind"] == "native-global-return":
            address = int(pattern["globalAddress"], 0)
            checks = 1
            actual = function(lambda actual_address: 0x12345678
                              if actual_address == address else None)
            if actual != 0x12345678:
                errors.append(f"global read returned {actual}")
        elif pattern["kind"] == "quest-interface-sequence":
            trace = []
            parameter_values = {name: 0x2300 + index
                                for index, name in enumerate(pattern["parameters"], 1)}
            field_values = {int(argument["offset"], 0): 0x10000 + int(argument["offset"], 0)
                            for operation in pattern["operations"]
                            for argument in operation["arguments"]
                            if argument["kind"] == "field-i32"}
            function(lambda method, *arguments: trace.append((method, list(arguments))),
                     lambda offset: field_values[offset],
                     *(parameter_values[name] for name in pattern["parameters"]))
            expected = []
            for operation in pattern["operations"]:
                arguments = []
                for argument in operation["arguments"]:
                    if argument["kind"] == "field-i32":
                        arguments.append(field_values[int(argument["offset"], 0)])
                    elif argument["kind"] == "parameter":
                        arguments.append(parameter_values[argument["name"]])
                    else:
                        arguments.append(argument["value"])
                expected.append((operation["method"], arguments))
            checks = len(expected)
            if trace != expected:
                errors.append(f"interface trace differs: expected {expected}, got {trace}")
        elif pattern["kind"] == "conditional-u8-call-clear":
            condition = int(pattern["conditionOffset"], 0)
            argument = int(pattern["argumentOffset"], 0)
            target = int(pattern["callTarget"], 0)
            for condition_value, expected in (
                    (0, []),
                    (1, [("call", target, 0x5A), ("write", condition, 0)])):
                trace = []
                function(lambda offset, value=condition_value: value if offset == condition else 0x5A,
                         lambda offset, value: trace.append(("write", offset, value)),
                         lambda address, value: trace.append(("call", address, value)))
                checks += 1
                if trace != expected:
                    errors.append(f"condition {condition_value}: expected {expected}, got {trace}")
        elif pattern["kind"] == "native-script-initializer":
            trace = []
            function(lambda offset, value: trace.append(("set-string", offset, value)),
                     lambda offset, value: trace.append(("write-u8", offset, value)),
                     lambda offset, value: trace.append(("write-u32", offset, value)),
                     lambda pointer, field, value:
                     trace.append(("write-nested-u8", pointer, field, value)),
                     lambda target, *arguments:
                     trace.append(("invoke-native", target, *arguments)))
            expected = []
            for operation in pattern["operations"]:
                if operation["kind"] == "write-nested-u8":
                    expected.append((operation["kind"], int(operation["pointerOffset"], 0),
                                     int(operation["fieldOffset"], 0), operation["value"]))
                elif operation["kind"] == "invoke-native":
                    expected.append((operation["kind"], int(operation["target"], 0),
                                     *operation["arguments"]))
                else:
                    expected.append((operation["kind"], int(operation["offset"], 0),
                                     operation["value"]))
            checks = len(expected)
            if trace != expected:
                errors.append(f"initializer trace differs: expected {expected}, got {trace}")
        elif pattern["kind"] == "archery-quest-info-setup":
            trace = []
            fields = {}
            next_handle = [1001]
            def invoke(method, *arguments):
                trace.append(("invoke", method, *arguments))
                if method in {"AddQuestInfoCounter", "AddQuestInfoTimer"}:
                    value = next_handle[0]
                    next_handle[0] += 1
                    return value
                return None
            def read_i32(offset):
                trace.append(("read-i32", offset))
                return fields[offset]
            def write_i32(offset, value):
                trace.append(("write-i32", offset, value))
                fields[offset] = value
            def read_nested(pointer, field):
                trace.append(("read-nested-i32", pointer, field))
                return 777
            def field_ref(offset):
                trace.append(("field-ref", offset))
                return f"field@{offset}"
            function(invoke, read_i32, write_i32, read_nested, field_ref)
            first, second = pattern["counterEntries"]
            timer = pattern["timerEntry"]
            expected = [
                ("invoke", "AddQuestInfoCounter", first["text"], 0, 1.0),
                ("write-i32", int(first["handleOffset"], 0), 1001),
                ("invoke", "AddQuestInfoCounter", second["text"], 0, 1.0),
                ("write-i32", int(second["handleOffset"], 0), 1002),
                ("field-ref", int(timer["timerOffset"], 0)),
                ("invoke", "AddQuestInfoTimer", f"field@{int(timer['timerOffset'], 0)}",
                 timer["text"], 1.0),
                ("write-i32", int(timer["handleOffset"], 0), 1003),
                ("read-nested-i32", int(pattern["scorePointerOffset"], 0),
                 int(pattern["scoreFieldOffset"], 0)),
                ("read-i32", int(first["handleOffset"], 0)),
                ("invoke", "UpdateQuestInfoCounter", 1001, 777, -1),
                ("invoke", "UpdateOnlineScore_Archery", 777),
                ("invoke", "DisplayQuestInfo", True),
            ]
            checks = len(expected)
            if trace != expected:
                errors.append(f"archery setup trace differs: expected {expected}, got {trace}")
        else:
            errors.append(f"unsupported semantic pattern {pattern['kind']}")
        rows.append({"targetAddress": entry["targetAddress"], "passed": not errors,
                     "checks": checks, "errors": errors})
    return {"schema": "forgefse-native-helper-lua-validation/0.1",
            "summary": {"helpers": len(rows), "passed": sum(row["passed"] for row in rows),
                        "checks": sum(row["checks"] for row in rows),
                        "complete": all(row["passed"] for row in rows)},
            "helpers": rows}


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--manifest", type=Path, required=True)
    parser.add_argument("--output", type=Path)
    args = parser.parse_args()
    result = validate(args.manifest)
    if args.output:
        args.output.parent.mkdir(parents=True, exist_ok=True)
        args.output.write_text(json.dumps(result, indent=2) + "\n", encoding="utf-8")
    print(json.dumps(result["summary"], sort_keys=True))
    return 0 if result["summary"]["complete"] else 1


if __name__ == "__main__":
    raise SystemExit(main())
