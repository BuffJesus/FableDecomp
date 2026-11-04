"""Opt-in comparison repair; retains every producer, value and cleanup lifetime.

Native ordered-positive BL is preserved by a normal callee-saved cleanup.
ABI/type/lifetime and masked-x87 conditions remain explicit, as in the existing
frame and predicate proofs. This pass supplies no resource ownership proof.
"""
import hashlib
import json
import re

from tools.script_recovery.native_health_copy_correspondence import prove as prove_copy
from tools.script_recovery.native_hit_health_entry import prove_frame
from tools.script_recovery.native_register_health_roles import prove_roles
from tools.script_recovery.native_saved_register_x87_predicate import prove as prove_predicate
from tools.script_recovery.native_reviewed_saved_health_predicates import PROFILE
from tools.script_recovery.native_evidence_lowering import bind_st0_results, join_wrapped_statements


def serial(value):
    return json.dumps(value, separators=(',', ':')).encode()


def digest(data):
    return hashlib.sha256(data).hexdigest()


def recover(source, todo, calls, fn, row, unit, owner, image, manifest, before, after):
    if row is None:
        return source, list(todo), list(calls), []

    def require(ok, why):
        if not ok:
            raise ValueError('register health comparison: ' + why)

    start, end = int(fn['address'], 16), int(fn['bodyEndExclusive'], 16)
    raw = image.bytes_at(start, end - start)
    for key, data in [('nativeSha256', raw), ('fnSha256', serial(fn)),
                      ('luaPhaseSha256', source.encode()), ('todoSha256', serial(todo)),
                      ('callsSha256', serial(calls)), ('beforeSha256', serial(before)),
                      ('afterSha256', serial(after))]:
        require(data is not None and digest(data) == row[key], key)
    require(owner == row['owner'] and owner in unit.get('entities', {}), 'owner')
    entity = unit['entities'][owner]
    require(entity['vtable'].lower() == row['entityVtable'].lower()
            and entity['functions']['Main']['address'].lower() == fn['address'].lower(), 'registered Main')
    contract = manifest.get('GetHealth')
    require(contract == row['hostContract'] and contract.get('scope') == 'Quest'
            and contract.get('returnType') == 'float'
            and contract.get('parameters') == [dict(name='pThing', type='CScriptThing*', optional=False)], 'host contract')
    require(row['actorProfile'] == 'local-edi', 'frame profile')
    for address, size, pin in PROFILE:
        data = image.bytes_at(address, size)
        require(data is not None and digest(data) == pin, 'native dependency')
    frame = prove_frame(raw, fn, image, entity_vtable=int(entity['vtable'], 16),
        scope_start=int(row['scopeStart'], 16), scope_end=end, frame_depth=row['frameDepth'],
        mask_offset=0, this_definition=int(row['thisDefinition'], 16),
        actor_definition=int(row['actorDefinition'], 16), actor_profile=row['actorProfile'])
    correspondence = prove_copy(fn, before, after, source,
        allow_wrapped_call_spacing=row['allowWrappedCallSpacing'])
    scopes = row['scopes']
    require(isinstance(scopes, list) and 0 < len(scopes) <= 64, 'scope inventory')
    require(len({s['site'].lower() for s in scopes}) == len(scopes), 'duplicate scope')
    normalized = bind_st0_results(join_wrapped_statements(fn['decompile']))
    edits, reviews = [], []
    for scope in scopes:
        site = int(scope['site'], 16)
        uses = [s for s in correspondence['scopes'] if int(s['native']['call']['site'], 16) == site]
        require(len(uses) == 1, 'inverse use inventory')
        use = uses[0]
        result, threshold = use['result'], use['threshold']
        require(use['condition'] == result + ' <= ' + threshold, 'inverse comparison')
        native = scope['predicate']
        actual = prove_predicate(frame, site, image.bytes_at, 0x4aa840,
            positive_exit=int(native['positiveExit'], 16), false_exit=int(native['falseExit'], 16))
        require(actual == native and actual['threshold'] == 0, 'native ordered predicate')
        roles = prove_roles(frame, site, actual, image, getter_target=0x7e7490,
            this_definition=int(row['thisDefinition'], 16), health_slot=0x420)
        require(int(use['native']['getter']['site'], 16) == int(roles['getter'], 16)
                and int(use['native']['cleanup']['site'], 16) == int(roles['cleanup'], 16), 'native producer correspondence')
        label = 'LAB_' + format(int(native['falseExit'], 16), '08x')
        original = 'if (' + use['condition'] + ') goto ' + label + ';'
        require(normalized.count(original) == 1, 'original source inverse exit')
        indices = [n for n, line in enumerate(before) if line.strip() == original]
        require(len(indices) == 1, 'original AST inverse exit')
        events = [e for e in correspondence['copyTrace'] if e['inputIndex'] == indices[0]]
        require(len(events) == 1 and events[0]['target'] == label
                and events[0]['originalJump'].strip() == original, 'inverse tail-copy correspondence')
        event = events[0]
        at = event['outputStartIndex']
        require(after[at].strip() == 'if (' + use['condition'] + ') {'
                and re.fullmatch(re.escape(label) + r'_c\d+:', after[at + 1].strip()) is not None,
                'cloned AST inverse continuation')
        cloned = after[at + 1].strip()[:-1]
        indent = re.match(r' *', use['prefix'])[0]
        require(source[use['end']:].startswith('\n' + indent + '    -- ' + cloned + ': (native jump target)'),
                'Lua cloned inverse continuation')
        begin = use['end'] - len(' then') - len(use['condition'])
        require(source[begin:begin + len(use['condition'])] == use['condition'], 'comparison slice')
        replacement = 'not (' + result + ' > ' + threshold + ')'
        edits.append((begin, begin + len(use['condition']), replacement))
        reviews.append(dict(site=hex(site), native=actual, roles=roles, originalCondition=use['condition'],
            replacement=replacement, nativeFalseLabel=label, luaFalseLabel=cloned,
            tailCopy=event, intentionalDifference='Unordered health now takes the native false branch.'))
    edits.sort()
    require(all(a[1] <= b[0] for a, b in zip(edits, edits[1:])), 'overlapping comparisons')
    out = source
    for a, b, replacement in reversed(edits):
        out = out[:a] + replacement + out[b:]
    return out, list(todo), list(calls), reviews
