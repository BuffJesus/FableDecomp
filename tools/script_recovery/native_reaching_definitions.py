"""Fixed-point reaching definitions over the conservative native flow IR."""
import re
from collections import deque


_LOCAL = r'(?:[A-Za-z]{1,3}Var\d+(?:_\d+)?|\w*Stack_\w+|local_\w+|native_arg_\w+)'
_TOKEN = re.compile(r'\b' + _LOCAL + r'\b')
_WRITE = re.compile(r'\b(' + _LOCAL + r')(?:\._\d+_\d+_)?\s*(?:<<|>>|[+*/%&|^\-])?=(?!=)')
_DIRECT = re.compile(r'^(' + _LOCAL + r')\s*=(?!=).*;$')


def reaching_definitions(graph, call_effects=None):
    """Return incoming local definition sets; None denotes unknown entry/escape.

    Unreachable statements have no incoming state. A conditional assignment adds
    a possible definition; an unconditional assignment replaces earlier ones.
    Address-escaped locals remain unknown because calls may mutate them indirectly.
    Conservative CFG edges can add uncertainty, never establish false uniqueness.
    """
    names = set().union(*(_TOKEN.findall(node.code) for node in graph.statements))
    escaped = {name for name in names if any(re.search(r'(?<!&)\&\s*\(*\s*' + re.escape(name) + r'\b', node.code)
                                            and not (call_effects is not None and hasattr(call_effects, 'output_address_only')
                                                     and call_effects.output_address_only(node.source, name))
                                            for node in graph.statements)}
    incoming = [None] * len(graph.statements)
    if not incoming:
        return incoming
    incoming[0] = {name: frozenset({None}) for name in names}
    pending, queued = deque([0]), {0}
    while pending:
        index = pending.popleft()
        queued.remove(index)
        node = graph.statements[index]
        outgoing = dict(incoming[index])
        direct = _DIRECT.fullmatch(node.code)
        writes = set(_WRITE.findall(node.code))
        writes.update(re.findall(r'\b(' + _LOCAL + r')\s*\[[^;]*\]\s*=(?!=)', node.code))
        # Until call signatures supply read/write effects, a local mentioned in a
        # call may be an output reference hidden by the decompiler's prototype.
        if (re.search(r'\b(?!if\b|while\b|for\b|switch\b|sizeof\b)\w+(?:::\w+|->\w+)*\s*\(', node.code)
                and not (call_effects is not None and call_effects(node.source))):
            writes.update(_TOKEN.findall(node.code))
        # Prefix/postfix increments also define a new value; conservatively retain
        # the previous definition for expressions whose execution is conditional.
        writes.update(re.findall(r'\b(' + _LOCAL + r')\s*(?:\+\+|--)', node.code))
        writes.update(re.findall(r'(?:\+\+|--)\s*(' + _LOCAL + r')\b', node.code))
        for name in writes:
            outgoing[name] = (frozenset({index}) if direct and direct[1] == name else
                              outgoing[name] | {index})
        if call_effects is not None and hasattr(call_effects, 'outputs'):
            for name in call_effects.outputs(node.source, index):
                outgoing[name] = frozenset({index})
        for name in escaped:
            outgoing[name] = frozenset({None})
        for successor in node.successors:
            previous = incoming[successor]
            merged = outgoing if previous is None else {
                name: previous[name] | outgoing[name] for name in names}
            if previous != merged:
                incoming[successor] = dict(merged)
                if successor not in queued:
                    queued.add(successor)
                    pending.append(successor)
    return incoming
