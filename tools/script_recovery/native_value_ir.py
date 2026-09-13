"""Typed literal/copy facts derived from all reaching definitions of a local."""
from dataclasses import dataclass
import json
import re

from tools.script_recovery.native_reaching_definitions import _LOCAL, reaching_definitions


@dataclass(frozen=True)
class ScalarValue:
    kind: str
    value: object


def literal_value(source):
    source = source.strip()
    if source in ('true', 'false'):
        return ScalarValue('bool', source == 'true')
    if re.fullmatch(r'-?(?:0x[0-9a-fA-F]+|0|[1-9]\d*)', source):
        return ScalarValue('integer', int(source, 16 if '0x' in source else 10))
    if re.fullmatch(r'"[^"\\]*"', source):
        return ScalarValue('string', json.loads(source))
    return None


class NativeValues:
    def __init__(self, graph, call_effects=None):
        self.graph = graph
        self.incoming = reaching_definitions(graph, call_effects)
        self.cache = {}
        self.call_effects = call_effects
        self.repeated = {}

    def may_repeat(self, statement):
        if statement not in self.repeated:
            pending = list(self.graph.statements[statement].successors)
            visited = set()
            while pending:
                current = pending.pop()
                if current == statement:
                    self.repeated[statement] = True
                    return True
                if current not in visited:
                    visited.add(current)
                    pending.extend(self.graph.statements[current].successors)
            self.repeated[statement] = False
        return self.repeated[statement]

    def at(self, statement, name, visiting=frozenset()):
        """A value is known only when every incoming definition proves it.

        Copies resolve at the defining statement, never at the later use. Cycles,
        unknown writes, escaped locals and disagreement return unknown (None).
        """
        key = (statement, name)
        if key in visiting:
            return None
        if key in self.cache:
            return self.cache[key]
        incoming = self.incoming[statement]
        definitions = incoming.get(name, {None}) if incoming is not None else {None}
        if None in definitions:
            return None
        results = set()
        for definition in definitions:
            node = self.graph.statements[definition]
            # A source label is metadata and is absent from node.code. Preserve
            # literal contents by stripping only that same label from source.
            source = re.sub(r'^\s*LAB_[0-9a-f]+:\s*', '', node.source.strip())
            assignment = re.fullmatch(re.escape(name) + r'\s*=(?!=)\s*(.*);', source)
            if assignment is None:
                outputs = (self.call_effects.outputs(source, definition)
                           if self.call_effects is not None and hasattr(self.call_effects, 'outputs')
                           and not self.may_repeat(definition) else {})
                if name not in outputs:
                    return None
                results.add(outputs[name])
                continue
            expression = assignment[1].strip()
            value = literal_value(expression)
            if value is None and re.fullmatch(_LOCAL, expression):
                value = self.at(definition, expression, visiting | {key})
            if (value is None and self.call_effects is not None
                    and hasattr(self.call_effects, 'result') and not self.may_repeat(definition)):
                value = self.call_effects.result(expression, definition)
            if value is None:
                return None
            results.add(value)
        value = next(iter(results)) if len(results) == 1 else None
        self.cache[key] = value
        return value
