"""Conservative presentation of generated Lua, with reversible local-name maps.

Proven independent assignments may be split into separate locals and unused
literal staging removed, with source maps. Semantic names require consistent
value definitions; ambiguous values retain scratch names. Native diagnostics
remain visible and the emitter's native evidence remains the authority.
"""
from __future__ import annotations

import re
from collections import Counter


GENERATED = re.compile(r'(?:[A-Za-z]{1,3}Var\d+(?:_\d+)*|r\d+(?:_\d+)*|native_arg_\w+|[A-Za-z]*Stack_\w+)\Z')
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
    if not re.match(r'[A-Za-z_]', name) or not re.search(r'[a-z]', value):
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
    # things looked up / created by a name: the name is the role
    named = re.fullmatch(r'quest:(?:GetAllThingsWith(?:ScriptName|DefName)|GetStateListCopy)\("([^"]+)"\)', expression)
    if named:
        return camel(re.sub(r'^(?:CREATURE_|OBJECT_)', '', named[1]))
    named = re.fullmatch(r'quest:(?:Get(?:Nearest|Furthest|Random)WithScriptName|GetRandomThingWithScriptName)\([^,]+, "([^"]+)"\)|quest:CreateCreature\("([^"]+)",.*', expression)
    if named:
        return camel(re.sub(r'^(?:CREATURE_|OBJECT_)', '', named[1] or named[2]))
    element = re.fullmatch(r'quest:GetStateListAt\("([^"]+)", .*\)', expression)
    if element:
        return camel(re.sub(r'^All', '', element[1])) + 'Item'
    if re.fullmatch(r'#\w+', expression):
        return camel(expression[1:]) + 'Count' if not GENERATED.fullmatch(expression[1:]) else 'count'
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
                 'Speak': 'speechResult', 'GetNumItemsOfType': 'itemCount', 'AddQuestInfoCounter': 'infoCounter', 'NewResource': 'resource', 'NewActorMap': 'actorMap', 'StartMovie': 'movie', 'GetName': 'name', 'GetDefName': 'defName',
                 'AddQuestInfoBarHealth': 'healthBar', 'GetAllCreaturesExcludingHero': 'creatures', 'GetFollowingEntityList': 'followers'}
        if name in known:
            return known[name]
        if name.startswith(('Get', 'Is', 'Has', 'Can')):
            return camel(name)
    if expression in ('true', 'false', 'not alive') or re.match(r'^not\s', expression):
        return 'predicateResult'
    return None


# When nothing about a temporary's assignments says what it is, the API that consumes it does: a value passed
# to `DeregisterTimer` is a timer id, one passed to `AddPersonToConversation` a conversation id. A rule's role
# may be a callable over the match, for the calls that name the value themselves (the list it indexes, the
# actor-map key it fills); returning None from one declines and moves on.
def _named_after(group, suffix=''):
    def role(m):
        value = m.group(group)
        if GENERATED.fullmatch(value) or value.startswith('scratchValue'):
            return None
        return camel(value) + suffix
    return role


def _list_position(m):
    """`GetStateListAt("AllCreatures", X)` indexes the list; `..., X / 12)` walks it by byte offset."""
    return camel(m.group('of')) + ('Offset' if m.group('scale') else 'Index')


# (pattern with NAME for the temporary, kind, role). `kind` groups the rules that describe the same value
# from different sides - a resource is acquired, released and destroyed - so only a disagreement *between*
# kinds means the slot really holds two values. Within a kind the first (most specific) rule wins.
USE_ROLES = (
    (r'resources:TryAcquire\(\s*NAME\s*,\s*(?P<of>\w+)\s*,', 'resource', _named_after('of', 'Control')),
    (r'resources:ReleaseResource\(\s*NAME\s*\)', 'resource', 'resource'),
    (r'resources:(?:DestroyMovie|StopMovie)\(\s*NAME\s*\)', 'resource', 'movie'),
    (r'resources:TryAcquire\([^,()]+,\s*NAME\s*,', 'thing', 'thing'),
    (r'quest:EntityAttachToScript\(\s*NAME\s*,', 'thing', 'entity'),
    (r'quest:CreateEffect\(\s*NAME\s*,', 'thing', 'thing'),
    (r'resources:SetString\([^,()]+,\s*"(?P<of>[^"]+)"\s*,\s*NAME\b', 'text', _named_after('of')),
    (r'\w+:Speak\([^,()]+,\s*NAME\b', 'text', 'line'),
    (r'resources:(?:SetActor|SetString)\(\s*NAME\s*,', 'actormap', 'actorMap'),
    (r'resources:(?:DestroyActorMap|RunMacro)\(\s*NAME\s*,?', 'actormap', 'actorMap'),
    (r'quest:(?:DeregisterTimer|SetTimer|GetTimer)\(\s*NAME\b', 'timer', 'timerId'),
    (r'quest:(?:AddPersonToConversation|AddLineToConversation|EndConversation|IsConversationActive)\(\s*NAME\b',
     'conversation', 'conversationId'),
    (r'quest:(?:RemoveQuestInfoElement|SetQuestInfoElementActive)\(\s*NAME\b', 'info', 'infoElement'),
    (r'quest:UpdateQuestInfoTick\([^,()]+,\s*NAME\b', 'info', 'ticked'),
    (r'quest:(?:GetStateListAt|StateListErase|StateListSet)\(\s*"(?P<of>[^"]+)"\s*,\s*NAME\s*(?P<scale>/)?',
     'index', _list_position),
    (r'quest:ReadGlobalGameData\w*At\([^,()]+,\s*NAME\b', 'index', 'index'),
)

# a register the compiler used as a bit field: every assignment is a literal or a bitwise step on itself
FLAG_STEP = re.compile(r'(?:0x[0-9a-f]+|\d+)|NAME\s*[&|~]\s*(?:0x[0-9a-f]+|\d+)|\(?NAME\s*[&|~][^()]*\)?')


def use_role(name, code):
    """The role of a value whose assignments say nothing, taken from the API that consumes it.

    Only when the consumers agree about the *kind* of value: a register passed to both `DeregisterTimer`
    and `ReleaseResource` is two values sharing a slot, and either name would be a lie.
    """
    token = r'\b' + re.escape(name) + r'\b'
    found = []
    for pattern, kind, role in USE_ROLES:
        for match in re.finditer(pattern.replace('NAME', token), code):
            chosen = role(match) if callable(role) else role
            if chosen is not None:
                found.append((kind, chosen))
                break
    if not found or len({kind for kind, _ in found}) > 1:
        return None
    return found[0][1]


def flag_register(name, assignments):
    """`X = 0; X = X | 1; X = X & 0xfffffffe` is a bit field, not a scratch value."""
    step = re.compile(FLAG_STEP.pattern.replace('NAME', r'\b' + re.escape(name) + r'\b'))
    real = [a.strip() for a in assignments if a.strip() not in ('nil', '')]
    if len(real) < 2 or not any(re.search(r'[&|~]', a) for a in real):
        return None
    return 'flags' if all(step.fullmatch(a) for a in real) else None


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
    # the role of a copy (`r1_2 = pCVar9_4`) is the role of what it copies: direct roles first, copies after
    direct = {}
    for name, count in declarations.items():
        if count == 1 and GENERATED.fullmatch(name):
            rhs_list = [rhs.strip() for rhs in re.findall(r'^\s*(?:local\s+)?' + re.escape(name) + r'\s*=\s*([^\n]*)', code, re.M)]
            direct[name] = [assignment_role(r) for r in rhs_list if r != 'nil' and not re.fullmatch(r'[A-Za-z_]\w*', r)]
    for name, count in declarations.items():
        if count != 1 or not GENERATED.fullmatch(name):
            continue
        assignments = re.findall(r'^\s*(?:local\s+)?' + re.escape(name) + r'\s*=\s*([^\n]*)', code, re.M)
        roles = []
        for rhs in assignments:
            rhs = rhs.strip()
            if rhs == 'nil':
                continue                                     # a release says nothing about the role
            if re.fullmatch(r'[A-Za-z_]\w*', rhs) and rhs in direct:
                copied = direct[rhs]
                roles.append(copied[0] if copied and all(r is not None and r == copied[0] for r in copied) else None)
            else:
                roles.append(assignment_role(rhs))
        if name.startswith('native_arg_'):
            base, reason = camel(name.removeprefix('native_arg_')), 'native argument role'
        elif roles and all(role is not None and role == roles[0] for role in roles):
            base, reason = roles[0], 'consistent call/state result'
        else:
            # different names from one method (`CreateCreature("CREATURE_A", ...)` / `("CREATURE_B", ...)`): the method's role
            methods = {m.group(1) for rhs in assignments if rhs.strip() != 'nil' for m in [re.match(r'\w+:(\w+)\(', rhs.strip())] if m}
            if len(methods) == 1 and len(assignments) > 1:
                method = next(iter(methods))
                base, reason = {'CreateCreature': 'creature', 'GetThingWithScriptName': 'thing', 'GetAllThingsWithScriptName': 'things',
                                'GetNearestWithScriptName': 'nearest', 'GetFurthestWithScriptName': 'furthest'}.get(method, camel(method)), 'one method, several names'
            else:
                base, reason = 'scratchValue', 'reused or unresolved native temporary'
        if base == 'scratchValue':
            flags = flag_register(name, assignments)
            if flags is not None:
                base, reason = flags, 'a bit field, not a value'
        if base == 'scratchValue':
            consumed = use_role(name, code)
            if consumed is not None:
                base, reason = consumed, 'named by the API that consumes it'
        if base == 'scratchValue' and re.fullmatch(r'scratchValue\d*', name):
            continue        # a second pass over already-named text: the fallback never renumbers its own names
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
