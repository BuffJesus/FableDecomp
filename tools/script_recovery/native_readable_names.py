"""Name single-producer locals only when every native use has that provenance."""
import re

from tools.script_recovery.native_call_effects import ScalarCallEffects, ProducedValue
from tools.script_recovery.native_flow_ir import build_flow_graph
from tools.script_recovery.native_value_ir import NativeValues


def readable_names(statements, manifest, reserved=(), native_calls=()):
    producer = re.compile(r'([A-Za-z]{1,3}Var\d+) = (?:\(CScriptThing \*\))?GSI->(RegisterTimer|GetHero)\(\);')
    if not any(producer.fullmatch(line.strip()) for line in statements):
        return {}
    graph = build_flow_graph(statements)
    if graph is None:
        return {}
    facts = NativeValues(graph, ScalarCallEffects(manifest, native_calls))
    result = {}
    occupied = set(re.findall(r'\b\w+\b', '\n'.join(statements)))
    occupied.update(reserved)
    for node in graph.statements:
        match = producer.fullmatch(node.source.strip())
        if not match or facts.may_repeat(node.index):
            continue
        name, callee = match.groups()
        expected = ProducedValue('thing' if callee == 'GetHero' else 'integer', node.index, callee)
        uses = [n for n in graph.statements if n.index != node.index and re.search(r'\b' + re.escape(name) + r'\b', n.code)]
        if not uses or any(facts.at(n.index, name) != expected for n in uses):
            continue
        # Reject reassignments even if their incoming value was still the producer.
        if any(re.search(r'\b' + re.escape(name) + r'\s*=(?!=)', n.code) for n in uses):
            continue
        base = 'hero' if callee == 'GetHero' else 'timerId'
        chosen, suffix = base, 2
        while chosen in occupied:
            chosen, suffix = base + str(suffix), suffix + 1
        occupied.add(chosen)
        result[name] = chosen
    return result


def rename_lua_locals(lines, names):
    if not names:
        return lines
    # Keep strings and generated comments unchanged, including native evidence text.
    protected = r'--\[\[[\s\S]*?\]\]|--[^\n]*|"(?:[^"\\]|\\.)*"|\'(?:[^\'\\]|\\.)*\''
    pattern = re.compile(protected + r'|\b(?:' + '|'.join(re.escape(n) for n in names) + r')\b')
    return pattern.sub(lambda m: names.get(m[0], m[0]), '\n'.join(lines)).split('\n')
