"""Recognize the exact inline by-value copy of an entity's embedded CScriptThing."""
import re
import struct

_COPY_ENTITY_ARGUMENT = re.compile(
    r'(?P<data>\w+) = \*\(undefined4 \*\)\((?P<owner>this|param_1) \+ 0xc\);\s*'
    r'(?P<info>\w+) = \*\(int \*\*\)\((?P=owner) \+ 0x10\);\s*'
    r'if \((?P=info) != \(int \*\)0x0\) \{\s*'
    r'\*(?P=info) = \*(?P=info) \+ 1;\s*\}\s*'
    r'GSI->(?P<method>SetIsPushableByHero|SetWanderCentrePoint|SetWanderMinDistance|SetWanderMaxDistance|SetScriptingStateGroup)\s*\(\s*&PTR__scalar_deleting_destructor__01238c8c,\s*'
    r'(?P=data),\s*(?P=info)(?:,\s*(?P<flag>0x[0-9a-f]+|[0-9]+))?\);')


def fold_self_wrapper_arguments(source):
    """CScriptThing vptr,+4 data,+8 info at entity+8 become the Lua me operand.

    Reviewed by-value signatures in engine_api.tsv: SetIsPushableByHero at 0x008A6DD0,
    SetWanderCentrePoint at 0x008A23B0, distances at 0x008A2690/0x008A2950, and
    SetScriptingStateGroup at 0x008A36D0. The distance signatures' decorated M operand
    proves float32 despite Ghidra's guessed long type; native argument words retain its bits.
    Native reference-count increment belongs to that copy. Keep unmatched lifetime operations.
    Do not remove native locals that are subsequently read; preceding uses may refer to older values.
    """
    count = 0
    protected = [m.span() for m in re.finditer(r'//[^\n]*|/\*[\s\S]*?\*/|"(?:[^"\\]|\\.)*"', source)]

    def replace(match):
        nonlocal count
        if any(start <= match.start() < end for start, end in protected):
            return match[0]
        tail = source[match.end():]
        if any(re.search(r'\b' + re.escape(match[k]) + r'\b', tail) for k in ('data', 'info')):
            return match[0]
        method, flag = match['method'], match['flag']
        if method == 'SetIsPushableByHero' and flag not in ('0', '1'):
            return match[0]
        if (method == 'SetWanderCentrePoint') != (flag is None):
            return match[0]
        if method in ('SetWanderMinDistance', 'SetWanderMaxDistance'):
            word = int(flag, 0)
            if word > 0xffffffff:
                return match[0]
            value = struct.unpack('<f', word.to_bytes(4, 'little'))[0]
            if not (-float('inf') < value < float('inf')):
                return match[0]
            flag = repr(value)
        count += 1
        suffix = '' if flag is None else ',' + flag
        return f'GSI->{method}((CScriptThing *)({match["owner"]} + 8){suffix});' 

    # Work backwards by repeated passes: removing a later dead copy can prove the
    # previous copy dead too. Any real intervening use still blocks removal.
    while True:
        updated = _COPY_ENTITY_ARGUMENT.sub(replace, source)
        if updated == source:
            return source, count
        source = updated
        protected = [m.span() for m in re.finditer(r'//[^\n]*|/\*[\s\S]*?\*/|"(?:[^"\\]|\\.)*"', source)]
