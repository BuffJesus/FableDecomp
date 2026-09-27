"""Arena's reviewed definition snapshot and PlayWave's nested vector reads.

Only the retail InitialiseVariables/PlayWave entries opt into this recovery.
PDB field names plus retail 0x38/0x3c strides establish the snapshot schema;
mispropagated Conversation/ClanMember library names are not type evidence.
"""
import re

INITIALISE = 0xf25840
PLAY_WAVE = 0xf1eed0
COPY_ROUNDS = 0xf25980
ROUND = '*(int *)(this + 0xa8)'
WAVE = '*(int *)(this + 0xac)'
ROUNDS = '*(int *)(this + 0x98)'
WAVES = f'*(int *)({ROUND} * 0x38 + 0x2c + {ROUNDS})'
CREATURES = f'*(int *)({WAVES} + 0x2c + {WAVE} * 0x3c)'

# Reviewed container implementation closure, not gameplay helpers. Keep native
# evidence in the translation unit; omit only bodies made unreachable in this
# Lua unit by replacing the one definition-copy edge with the runtime boundary.
CONTAINER_HELPERS = frozenset((
    0xf25980, 0xf261a0, 0xf25ac0, 0xf26b30, 0xf25b00, 0xf26920,
    0xf25bb0, 0xf26ae0, 0xf26aa0, 0xf25f60, 0xf25c60, 0xf25c20,
    0xf25f90, 0xf25d20, 0xf26150, 0xf26110, 0xf25f10, 0xf25dd0, 0xf25d90,
))


def replaced_container_helpers(unit, native):
    from tools.script_recovery.convert_quest_unit import SKIP_ROLES, unwrap_statements
    from tools.script_recovery.native_evidence_lowering import normalise_typed_decompile
    functions = unit['quest']['functions']
    initializer = functions.get('InitialiseVariables', {})
    if initializer.get('address', '').lower() != '0x00f25840':
        return set()
    fn = native(initializer['address'])
    if not fn:
        return set()
    labels = {c['currentName']: int(c['target'], 16) for c in fn.get('calls', []) if c.get('currentName')}
    source = normalise_typed_decompile(unwrap_statements(fn['decompile']))
    if initialise_rounds(source, labels) == source:
        return set()
    all_functions = [(n, f) for n, f in functions.items() if n not in SKIP_ROLES]
    for entity in unit['entities'].values():
        all_functions.extend((n, f) for n, f in {**entity['functions'], **entity.get('helpers', {})}.items()
                             if n not in SKIP_ROLES)
    addresses = {int(f['address'], 16) for _, f in all_functions}
    if not CONTAINER_HELPERS <= addresses:
        return set()
    graph = {}
    for address in addresses:
        evidence = native(f'0x{address:08x}')
        if not evidence:
            return set()
        graph[address] = {int(c['target'], 16) for c in evidence.get('calls', [])
                          if c.get('target') and int(c['target'], 16) in addresses}
    def reachable(roots, omit_copy=False):
        found, pending = set(), list(roots)
        while pending:
            address = pending.pop()
            if address in found:
                continue
            found.add(address)
            edges = graph.get(address, set())
            if omit_copy and address == INITIALISE:
                edges = edges - {COPY_ROUNDS}
            pending.extend(edges - found)
        return found
    if reachable({COPY_ROUNDS}) != CONTAINER_HELPERS:
        return set()
    # Every other entry remains a root, including unmatched gameplay helpers.
    used = reachable(addresses - CONTAINER_HELPERS, omit_copy=True)
    return {f'0x{address:08x}' for address in CONTAINER_HELPERS - used}


def initialise_rounds(text, call_labels):
    for label, target in call_labels.items():
        if target != COPY_ROUNDS:
            continue
        pattern = re.compile(r'(?m)^([ \t]*)' + re.escape(label) +
                             r'\(this \+ 0x98,DAT_0143e90c \+ 0x1044\);$')
        if len(pattern.findall(text)) == 1:
            return pattern.sub(r'\1ENGINE_InitialiseArenaRounds();', text)
    return text


def recover_spawn_thing_destination(text, call_labels):
    """F1F873 copies to ESP+A0, not the numeric loop counter at ESP+9C.

    The export's stack depth drifts by four across the earlier GetPos vcall.
    Retail F1F86C and the alternative F1F915 both LEA ECX,[ESP+A4] after
    pushing the source handle; cleanup F1FA98 uses [ESP+A0]. The misleading
    &iStack_d0 destination otherwise overwrites the later numeric loop counter.
    """
    for label, target in call_labels.items():
        if target != 0x8ab980:
            continue
        wrong = f'{label}((CScriptThing *)&iStack_d0,(int)pCVar5);'
        if text.count(wrong) != 1:
            continue
        # Both alternatives converge on the same owned handle before the loop
        # resumes; a changed join or counter lifetime needs fresh evidence.
        join = text.find('LAB_00f1f934:')
        end = text.find('LAB_00f1faba:', join)
        if join < 0 or end < 0 or text.index(wrong) >= join:
            continue
        # Call-site stack normalization names the correctly aligned handle
        # xStack_c0; the untouched decompile calls that same handle CStack_cc.
        alternatives = re.findall(re.escape(label) +
            r'\(\(CScriptThing \*\)&(CStack_cc|xStack_c0),\(int\)pCVar5\);', text[join:end])
        if len(alternatives) != 1:
            continue
        slot = alternatives[0]
        right = f'{label}((CScriptThing *)&{slot},(int)pCVar5);'
        if f'(*(int **)(this + 0x40),&{slot},1);' not in text[join:end]:
            continue
        if 'iStack_d0 = iStack_d0 + 1;' not in text[end:end+160]:
            continue
        return text.replace(wrong, right)
    return text


def round_key():
    return f'"Rounds_" .. {ROUND}'


def wave_key():
    return round_key() + f' .. "_Waves_" .. {WAVE}'


def field(key, name, kind='Int'):
    return f'QUESTSTATE_Get{kind}(__key({key} .. "_{name}"))'


def creature_field(index, name, kind='Int'):
    return field(wave_key() + f' .. "_Creatures_" .. {index}', name, kind)


def recover_round_reads(text):
    """Recover all reviewed pointer uses together, or leave the input unchanged.

    CVar14 is the saved numeric cursor; CStack_108 also has unrelated string
    lifetimes. Only its two resets, two increments and numeric operands change.
    Indices remain zero based, matching the shared-state snapshot keys.
    """
    original = text
    if re.search(r'\b(?:creatureGroupIndex|savedCreatureGroupIndex|creatureType)\b', text):
        return original
    stack = 'xStack_108' if 'xStack_108' in text else 'CStack_108'
    edits = [
        (f'{stack} = (CCharString)0x0;', 'creatureGroupIndex = 0;', 2),
        (f'CVar14 = {stack};', 'savedCreatureGroupIndex = creatureGroupIndex;', 4),
        (f'{stack} = (CCharString)((int)CVar14 + 0x38);',
         'creatureGroupIndex = savedCreatureGroupIndex + 1;', 2),
        (f'(int){stack} < 0xa8', 'creatureGroupIndex < 3', 1),
    ]
    for old, new, count in edits:
        if text.count(old) != count:
            return original
        text = text.replace(old, new)
    # Each vector-base alias is consumed as one CreatureType address before
    # its next assignment. Reject additional/escaped uses or control splits.
    alias_assignment = f'iVar6 = {CREATURES};'
    starts = [m.start() for m in re.finditer(re.escape(alias_assignment), text)]
    if len(starts) != 5:
        return original
    for start in reversed(starts):
        after = start + len(alias_assignment)
        assignment = re.search(r'(?m)^[ \t]*iVar6 = [^\n]*;', text[after:])
        if not assignment:
            return original
        end = after + assignment.end()
        # Two spawn alternatives join an earlier cleanup block. Both reviewed
        # joins restore iVar6 from iStack_c0 before rejoining the numeric loop.
        jump = re.search(r'(?m)^[ \t]*goto (LAB_00f1fcce|LAB_00f1f934);', text[after:end])
        if jump:
            label = jump[1] + ':'
            join = text.index(label)
            restore = text.find('iVar6 = iStack_c0;', join)
            if restore < 0 or re.search(r'\biVar6\b', text[join:restore]):
                return original
            end = after + jump.end()
        scope = text[after:end]
        expected = ('(CCharString *)(iVar6 + 0x28 + (int)CVar14)',
                    f'iVar6 + 0x28 + (int){stack}')
        count = sum(scope.count(value) for value in expected)
        if count != 1:
            return original
        use = next(scope.index(value) for value in expected if value in scope)
        if re.search(r'[{}]|\bgoto\b|(?m:^LAB_\w+:)', scope[:use]):
            return original
        # iVar6 also carries the spawn-point count before/after this lifetime.
        # Give its string value a separate local so the lifter cannot propagate
        # it into the numeric loop after the reviewed join restores that count.
        check = scope
        for value in expected:
            check = check.replace(value, 'RECOVERED_CREATURE_TYPE')
        # The final assignment kills the alias, including a possible RHS use.
        check = re.sub(r'(?m)^([ \t]*)iVar6 =', r'\1NEXT_VALUE =', check)
        if re.search(r'\biVar6\b', check):
            return original
        for value in expected:
            scope = scope.replace(value, 'creatureType')
        replacement = 'creatureType = ' + creature_field('savedCreatureGroupIndex', 'CreatureType', 'String') + ';'
        text = text[:start] + replacement + scope + text[end:]
    # Reads not routed through a temporary vector base.
    replacements = [
        (f'*(int *)({CREATURES} + 0x2c + (int)CVar14)',
         creature_field('savedCreatureGroupIndex', 'NumCreatures'), 2),
        (f'(CCharString *)({CREATURES} + 0x28 + (int)CVar14)',
         creature_field('savedCreatureGroupIndex', 'CreatureType', 'String'), 1),
        (f'(CCharString *)((int){stack} + 0x30 + {CREATURES})',
         creature_field('creatureGroupIndex', 'HUDType', 'String'), 1),
        (f'(CCharString *)({CREATURES} + 0x30)', creature_field('0', 'HUDType', 'String'), 1),
        (f'*(int *)({WAVES} + 0x28 + {WAVE} * 0x3c)', field(wave_key(), 'NumWaveCreatures'), 2),
        (f"*(char *)({WAVES} + 0x38 + {WAVE} * 0x3c) == '\\0'",
         '!' + field(wave_key(), 'ShortWave', 'Bool'), 1),
        (f'*(int *)({ROUND} * 0x38 + 0x28 + {ROUNDS})', field(round_key(), 'NumWaves'), 1),
    ]
    for old, new, count in replacements:
        if text.count(old) != count:
            return original
        text = text.replace(old, new)
    text = re.sub(r'(?m)^  CCharString CVar14;\n', '', text)
    if re.search(r'\bCVar14\b', text) or ROUNDS in text or re.search(r'\(int\)' + stack, text):
        return original
    cursor_lifetime = text[text.index('creatureGroupIndex = 0;'):text.index('creatureGroupIndex < 3')]
    if re.search(r'\b' + stack + r'\b', cursor_lifetime):
        return original
    return text
