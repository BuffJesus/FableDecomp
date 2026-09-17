"""Conservative presentation of generated Lua, with reversible local-name maps.

Proven independent assignments may be split into separate locals and unused
literal staging removed, with source maps. Semantic names require consistent
value definitions; ambiguous values retain scratch names. Native diagnostics
remain visible and the emitter's native evidence remains the authority.
"""
from __future__ import annotations

import re
from collections import Counter


GENERATED = re.compile(r'(?:[A-Za-z]{1,3}Var\d+(?:_\d+)*|r\d+|native_arg_\w+|[A-Za-z]*Stack_\w+)\Z')
TOKEN = re.compile(
    r'(?P<longcomment>--\[(?P<ceq>=*)\[[\s\S]*?\](?P=ceq)\])'
    r'|(?P<comment>--[^\n]*)'
    r'|(?P<longstring>\[(?P<seq>=*)\[[\s\S]*?\](?P=seq)\])'
    r'''|(?P<string>"(?:[^"\\]|\\[\s\S])*"|'(?:[^'\\]|\\[\s\S])*')'''
    r'|(?P<identifier>[A-Za-z_]\w*)|(?P<space>\s+)'
    r'|(?P<operator>\.\.\.|\.\.|::|==|~=|<=|>=|//|<<|>>)|(?P<other>.)'
)
FUNCTION = re.compile(r'^(?:local )?function ([A-Za-z_]\w*)\([^\n]*\)\n', re.M)


def camel(value):
    words = re.findall(r'[A-Z]+(?=[A-Z][a-z]|$)|[A-Z]?[a-z]+|\d+', value.replace('_', ' '))
    name = words[0].lower() + ''.join(w[:1].upper() + w[1:] for w in words[1:]) if words else ''
    if not re.match(r'[A-Za-z_]', name):
        # all-caps marker names ("MK_GTA_MAZE1" would be "1"): keep every word
        words = re.findall(r'[A-Z]+(?=[A-Z][a-z]|$)|[A-Z]?[a-z]+|[A-Z]+|\d+', value.replace('_', ' '))
        name = words[0].lower() + ''.join(w.capitalize() if w.isupper() else w[:1].upper() + w[1:] for w in words[1:]) if words else 'value'
    return name if re.match(r'[A-Za-z_]', name) else 'v' + name


def tokens(source):
    return list(TOKEN.finditer(source))


def rename_identifiers(source, names):
    """Rename variable tokens, preserving literals, comments and member/key names."""
    stream = tokens(source)
    significant = [i for i, t in enumerate(stream)
                   if t.lastgroup not in ('space', 'comment', 'longcomment')]
    previous = {i: stream[significant[n - 1]][0] if n else '' for n, i in enumerate(significant)}
    following = {i: stream[significant[n + 1]][0] if n + 1 < len(significant) else ''
                 for n, i in enumerate(significant)}
    result, table_depth = [], 0
    for i, token in enumerate(stream):
        value = token[0]
        if token.lastgroup == 'identifier' and value in names:
            member = previous.get(i) in ('.', ':')
            table_key = table_depth > 0 and previous.get(i) in ('{', ',', ';') and following.get(i) == '='
            if not member and not table_key:
                value = names[value]
        result.append(value)
        if token.lastgroup == 'other':
            if value == '{':
                table_depth += 1
            elif value == '}':
                table_depth -= 1
    return ''.join(result)


def assignment_role(expression):
    expression = expression.strip()
    guarded_distance = re.fullmatch(r'\((\w+) ~= nil and \1:IsDistanceFromPositionOver\(.+\)\)', expression)
    if guarded_distance:
        return 'outsideDistance'
    state = re.fullmatch(r'\w+:GetState(?:Bool|Int|Float|String)\("([^"]+)"\)', expression)
    if state:
        return camel(state[1])
    thing = re.fullmatch(r'(?:quest:GetThingWithScriptName|resources:NewThingFromScriptName)\("([^"]+)"\)', expression)
    if thing:
        return camel(re.sub(r'^(?:NOVI_|OVI_|M_)', '', thing[1]))
    method = re.match(r'\w+:(\w+)\(', expression)
    if method:
        name = method[1]
        known = {'GetHero': 'hero', 'RegisterTimer': 'timerId', 'GetTimer': 'timeRemaining',
                 'GetHealth': 'health', 'ThingHealth': 'health', 'GetHomePos': 'homePosition',
                 'GetPos': 'position', 'AcquireControl': 'controlAcquired',
                 'TryAcquire': 'controlAcquired', 'IsPerformingScriptTask': 'taskRunning',
                 'ThingAlive': 'thingAlive', 'ThingIsDistanceFromPositionOver': 'outsideDistance',
                 'ThingsAreWithinDistance': 'thingsWithinDistance',
                 'AddNewConversation': 'conversationId', 'StartConversationWithHero': 'conversationId',
                 'RetailRandModulo': 'randomChoice',
                 'MsgIsQuestionAnsweredYesOrNo': 'questionAnswer',
                 'MsgIsGameInfoClickedPast': 'instructionDismissed', 'IsXbox': 'isXbox',
                 'IsTalkedToByHero': 'talkedToByHero', 'MsgIsHitByHero': 'hitByHero',
                 'MsgIsHitByAnySpecialAbilityFromHero': 'hitByHeroAbility',
                 'Speak': 'speechResult', 'GetNumItemsOfType': 'itemCount'}
        if name in known:
            return known[name]
        if name.startswith(('Get', 'Is', 'Has', 'Can')):
            return camel(name)
    if expression in ('true', 'false', 'not alive') or re.match(r'^not\s', expression):
        return 'predicateResult'
    return None


def rename_labels(source, names):
    """Rename only label definitions/references; preserve all other Lua tokens."""
    stream = tokens(source)
    significant = [i for i, t in enumerate(stream)
                   if t.lastgroup not in ('space', 'comment', 'longcomment')]
    replacements = {}
    definitions = Counter()
    for n, i in enumerate(significant):
        token = stream[i]
        before = stream[significant[n-1]][0] if n else ''
        after = stream[significant[n+1]][0] if n+1 < len(significant) else ''
        is_definition = before == '::' and after == '::'
        if is_definition:
            definitions[token[0]] += 1
        if token[0] in names and (is_definition or before == 'goto'):
            replacements[i] = names[token[0]]
    if len(set(names.values())) != len(names) or any(
            definitions[old] != 1 or definitions[new] or not re.fullmatch(r'[A-Za-z_]\w*', new)
            for old, new in names.items()):
        raise ValueError('label mapping is missing, ambiguous or collides')
    return ''.join(replacements.get(i, token[0]) for i, token in enumerate(stream))


def wrap_local_declarations(source, width=100):
    """Wrap declaration-only lists; there are no initializers to reorder."""
    result, changes = [], []
    for number, line in enumerate(source.splitlines(keepends=True), 1):
        match = re.fullmatch(r'( *)local ([A-Za-z_]\w*(?:, [A-Za-z_]\w*)*)\n', line)
        if match is None or len(line.rstrip()) <= width:
            result.append(line)
            continue
        prefix = match[1] + 'local '
        current, wrapped = prefix, []
        for name in match[2].split(', '):
            if current != prefix and len(current) + len(name) + 2 > width:
                wrapped.append(current + '\n')
                current = prefix
            current += (', ' if current != prefix else '') + name
        wrapped.append(current + '\n')
        result.extend(wrapped)
        changes.append({'inputLine': number, 'original': line, 'outputLines': len(wrapped)})
    return ''.join(result), changes


def readable_function(source):
    # Do not interpret comments or strings as declarations/assignments. Preserve
    # string literals for role inference, while blanking comments with line breaks.
    code = ''.join(re.sub(r'[^\n]', ' ', t[0]) if t.lastgroup in ('comment', 'longcomment')
                   or t.lastgroup in ('string', 'longstring') and '\n' in t[0]
                   else t[0] for t in tokens(source))
    declarations = Counter()
    for params in re.findall(r'\bfunction(?:\s+[\w.:]+)?\s*\(([^)]*)\)', code):
        declarations.update(n.strip() for n in params.split(','))
    for line in code.splitlines():
        match = re.match(r'\s*local\s+(?!function\b)([\w, ]+?)(?:\s*=|\s*$)', line)
        if match:
            declarations.update(n.strip() for n in match[1].split(','))
    occupied = {t[0] for t in tokens(source) if t.lastgroup == 'identifier'}
    names, evidence = {}, {}
    for name, count in declarations.items():
        if count != 1 or not GENERATED.fullmatch(name):
            continue
        assignments = re.findall(r'^\s*(?:local\s+)?' + re.escape(name) + r'\s*=\s*([^\n]*)', code, re.M)
        roles = [assignment_role(rhs) for rhs in assignments]
        if name.startswith('native_arg_'):
            base, reason = camel(name.removeprefix('native_arg_')), 'native argument role'
        elif roles and all(role is not None and role == roles[0] for role in roles):
            base, reason = roles[0], 'consistent call/state result'
        else:
            base, reason = 'scratchValue', 'reused or unresolved native temporary'
        chosen, suffix = base, 2
        while chosen in occupied:
            chosen, suffix = base + str(suffix), suffix + 1
        occupied.add(chosen)
        names[name] = chosen
        evidence[name] = {'name': chosen, 'basis': reason, 'assignmentRoles': roles}
    output = rename_identifiers(source, names)
    if rename_identifiers(output, {v: k for k, v in names.items()}) != source:
        raise ValueError('local rename is not reversible')
    return output, evidence


def readable_source(source, *, split_reused=True, inline_literals=False):
    code_starts = {t.start() for t in tokens(source) if t.lastgroup == 'identifier'}
    matches = [m for m in FUNCTION.finditer(source) if m.start() in code_starts]
    if not matches:
        return source, []
    result, mappings = [source[:matches[0].start()]], []
    for index, match in enumerate(matches):
        end = matches[index + 1].start() if index + 1 < len(matches) else len(source)
        chunk = source[match.start():end]
        splits = {}
        removals = {}
        literals = {}
        post_inline_removals = {}
        if split_reused:
            from tools.script_recovery.lua_local_versions import split_hoisted_locals, prune_unused_literal_locals
            chunk, splits = split_hoisted_locals(chunk)
            chunk, removals = prune_unused_literal_locals(chunk)
        if inline_literals:
            from tools.script_recovery.lua_literal_values import inline_literal_locals
            from tools.script_recovery.lua_local_versions import prune_unused_literal_locals
            chunk, literals = inline_literal_locals(chunk)
            chunk, post_inline_removals = prune_unused_literal_locals(chunk)
        chunk, names = readable_function(chunk)
        result.append(chunk)
        mappings.append({'function': match[1], 'sourceLine': source.count('\n', 0, match.start()) + 1,
                         'locals': names, 'splitLocals': splits, 'removedLiteralLocals': removals,
                         'inlinedLiteralLocals': literals})
        mappings[-1]['removedLiteralLocalsAfterInlining'] = post_inline_removals
    return ''.join(result), mappings
