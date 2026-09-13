"""Reviewed native witnesses for inlined CScriptThing predicates.

A matching vtable offset is insufficient: the decompiler may expose the wrapper's implementation
pointer, and can reuse its stack name for an unrelated string. Each witness pins the source export
and the native caller/callee bytes that prove the hidden return buffer and receiver relationship.
This is intentionally a small reviewed recovery registry, not general stack analysis.
"""
from __future__ import annotations

import hashlib
import json
import re
from pathlib import Path
from typing import Callable


WITNESSES = Path(__file__).with_name("native_thing_predicate_witnesses.json")
_CONDITION = re.compile(
    r"while\s*\(\(\s*(?P<pointer>\w+)\s*!=\s*\(int \*\)0x0\s*&&\s*"
    r"\((?P<flag>\w+)\s*=\s*\(\*\*\(code \*\*\)\(\*(?P=pointer)\s*\+\s*300\)\)"
    r"\(\),\s*(?P=flag)\s*!=\s*'\\0'\)\)\)\s*\{")
_NEGATIVE_CONDITION = re.compile(
    r"if\s*\(\((?P<pointer>\w+)\s*==\s*\(int \*\)0x0\)\s*\|\|\s*"
    r"\((?P<flag>\w+)\s*=\s*\(\*\*\(code \*\*\)\(\*(?P=pointer)\s*\+\s*300\)\)"
    r"\(\),\s*(?P=flag)\s*==\s*'\\0'\)\)\s*\{")


def recover_reviewed_thing_predicates(
    function: dict, annotated: str, read_bytes: Callable[[int, int], bytes | None],
    witnesses: list[dict] | None = None,
) -> tuple[str, list[dict]]:
    """Return rewritten source and auditable evidence, rejecting stale or unavailable inputs."""
    if witnesses is None:
        witnesses = json.loads(WITNESSES.read_text(encoding="utf-8"))["witnesses"]
    try:
        address = int(str(function.get("address", "")), 16)
    except ValueError:
        return annotated, []
    candidates = [w for w in witnesses if int(w["functionAddress"], 16) == address]
    if not candidates:
        return annotated, []
    # Multiple competing witnesses must be reviewed, rather than applied in arbitrary order.
    if len(candidates) != 1:
        return annotated, [{"status": "rejected", "reason": "ambiguous native witnesses"}]
    witness = candidates[0]
    evidence = {"id": witness["id"], "status": "rejected", "functionAddress": witness["functionAddress"]}

    def reject(reason: str) -> tuple[str, list[dict]]:
        return annotated, [dict(evidence, reason=reason)]

    source = function.get("decompile") or ""
    if hashlib.sha256(source.encode("utf-8")).hexdigest() != witness["decompileSha256"]:
        return reject("source export changed; review receiver association again")
    if hashlib.sha256(annotated.encode("utf-8")).hexdigest() != witness["annotatedSha256"]:
        return reject("annotated source changed; review receiver association again")
    for region in witness["nativeRegions"]:
        data = read_bytes(int(region["address"], 16), region["size"])
        if data is None or len(data) != region["size"] or hashlib.sha256(data).hexdigest() != region["sha256"]:
            return reject(f'native bytes unavailable or changed: {region["name"]}')
    for slot in witness["vtableSlots"]:
        data = read_bytes(int(slot["address"], 16), 4)
        if data is None or len(data) != 4 or int.from_bytes(data, "little") != int(slot["target"], 16):
            return reject(f'vtable target unavailable or changed: {slot["name"]}')

    shape = witness.get("shape", "while_alive")
    if shape not in ("while_alive", "if_not_alive"):
        return reject("unsupported predicate shape")
    method = witness.get("lookupMethod", "GetThingWithScriptName")
    if method not in ("GetThingWithScriptName", "GetNearestWithScriptName"):
        return reject("unsupported lookup method")
    lookup = list(re.finditer(r"(?m)^\s*GSI->" + method + r"\((?P<args>[^;\n]*)\);", annotated))
    pattern = _CONDITION if shape == "while_alive" else _NEGATIVE_CONDITION
    conditions = list(pattern.finditer(annotated))
    if len(lookup) != 1 or len(conditions) != 1 or lookup[0].end() >= conditions[0].start():
        return reject("annotated lookup/predicate shape changed")
    condition = conditions[0]
    # Collapsing a null-guarded assignment is safe only if its temporary is dead elsewhere.
    outside = annotated[:condition.start()] + annotated[condition.end():]
    outside = re.sub(r"\bchar\s+" + re.escape(condition["flag"]) + r"\s*;", "", outside)
    if shape == "while_alive" and re.search(r"\b" + re.escape(condition["flag"]) + r"\b", outside):
        return reject("predicate result has another use")
    name = "__native_thing_predicate_1"
    while re.search(r"\b" + name + r"\b", annotated):
        name += "_"
    # Lua bindings may represent an absent wrapper as nil. Both nil and an invalid wrapper must
    # stop this loop; IsAlive itself handles an existing wrapper with a null implementation.
    replacement = f"while ({name} && CScriptThing::IsAlive({name})) {{"
    if shape == "if_not_alive":
        # Preserve the assignment, including its old value when a null receiver skips the call.
        flag = condition["flag"]
        replacement = f"if ((!{name}) || ({flag} = CScriptThing::IsAlive({name}), {flag} == '\\0')) {{"
    result = annotated[:condition.start()] + replacement + annotated[condition.end():]
    call = lookup[0]
    result = result[:call.start()] + f"\n  {name} = GSI->{method}({call['args']});" + result[call.end():]
    evidence.update(status="recovered", receiver=name, predicate="IsAlive",
                    nativeRegions=witness["nativeRegions"], vtableSlots=witness["vtableSlots"],
                    decompileSha256=witness["decompileSha256"], association=witness["association"])
    evidence["annotatedSha256"] = witness["annotatedSha256"]
    return result, [evidence]
