"""Retire unread Lua placeholders for the known script-interface dispatch slot.

The role/offset contract is the same as native_dispatch_scaffolding: +4 on
entity scripts, +0x40 on quests. Calls carry that interface implicitly through
the Lua receiver. This pass requires a private binding with no surviving use;
it never infers liveness from unfinished native control-flow placeholders.
"""
import re

from tools.script_recovery.readable_lua import tokens
from tools.script_recovery.lua_binding_guard import ambiguous_bindings


PAIR = re.compile(
    r'^(?P<indent>[ \t]*)(?P<comment>-- (?P<annotation>TODO\(native\)|reviewed native interface-pointer staging): (?P<native>\w+) = '
    r'\*\((?:this|param_1) \+ (?P<offset>4|0x4|64|0x40)\);?)[ \t]*\r?\n'
    r'(?P=indent)(?P<local>local )?(?P<lua>\w+) = nil '
    r'(?P<marker>--\[\[(?P<marker_kind>unresolved native value|reviewed native interface-pointer staging)\]\])[ \t]*\r?\n', re.M)
DECL = re.compile(r'^[ \t]*local[ \t]+(?P<names>[A-Za-z_]\w*(?:[ \t]*,[ \t]*[A-Za-z_]\w*)*)[ \t]*$', re.M)


def prune_unused_dispatch_seeds(source, *, entity):
    if type(entity) is not bool:
        return source, []
    matches = list(PAIR.finditer(source))
    if not matches:
        return source, []
    stream = tokens(source)
    starts = {token.start():token for token in stream}
    ambiguous = ambiguous_bindings(source)
    expected = 4 if entity else 0x40
    pairs = []
    for match in matches:
        comment, marker = starts.get(match.start('comment')), starts.get(match.start('marker'))
        paired = ((match['annotation'] == 'TODO(native)' and match['marker_kind'] == 'unresolved native value')
                  or (match['annotation'] == 'reviewed native interface-pointer staging'
                      and match['marker_kind'] == 'reviewed native interface-pointer staging'))
        if (paired and int(match['offset'],0) == expected and comment is not None and marker is not None
                and comment.lastgroup == 'comment' and marker.lastgroup == 'longcomment'):
            pairs.append(match)
    plans = []
    for name in sorted({match['lua'] for match in pairs}):
        if name in ambiguous:
            continue
        owned = [match for match in pairs if match['lua'] == name]
        rest = source
        for match in reversed(owned):
            rest = rest[:match.start()] + re.sub(r'[^\n]', ' ', match[0]) + rest[match.end():]
        declarations = []
        for match in DECL.finditer(rest):
            local_at = match.start() + len(match[0]) - len(match[0].lstrip())
            token = starts.get(local_at)
            if token is not None and token.lastgroup == 'identifier' and token[0] == 'local':
                names = [value.strip() for value in match['names'].split(',')]
                if name in names:
                    declarations.extend([match] * names.count(name))
        if len(declarations) + sum(bool(match['local']) for match in owned) != 1:
            continue  # global, parameter, shadowed, or otherwise ambiguous storage
        for match in reversed(declarations):
            fragment = re.sub(r'\b'+re.escape(name)+r'\b',lambda m:' '*len(m[0]),match[0])
            # Keep offsets stable for the remaining conservative name scan.
            rest = rest[:match.start()]+fragment+rest[match.end():]
        if re.search(r'\b'+re.escape(name)+r'\b',rest):
            continue  # Includes other assignments, closures, comments and opaque literals.
        plans.extend(owned)
    evidence = []
    for match in sorted(plans,key=lambda match:match.start(),reverse=True):
        evidence.append(dict(luaLocal=match['lua'],nativeLocal=match['native'],
                             offset=int(match['offset'],0),nativeStatement=match['comment'],
                             role='script-interface dispatch',
                             proof='One private declaration; only dispatch-placeholder writes; no surviving references.'))
        source = source[:match.start()]+source[match.end():]
    return source,list(reversed(evidence))
