"""Opt-in health-inclusive hit scopes; normal native child ABI remains conditional."""
import hashlib
import json
import re
from capstone import Cs, CS_ARCH_X86, CS_MODE_32, x86 as x
from tools.script_recovery.native_reviewed_hit_query_lifetimes import PROFILE, METHODS, masked, significant
from tools.script_recovery.native_hit_health_query_roles import prove as prove_scope
from tools.script_recovery.native_hit_health_entry import prove as prove_entry

HEALTH_PROFILE = (0x890400, 46, '79c07c4b10801c8f2a80873426ac09fa2590be1d9331bc3bde0b5b7a86c5cef4')


def recover(source, todo, calls, fn, row, unit, owner, image, liveness):
    if row is None:
        return source, list(todo), list(calls), []

    def require(ok, why):
        if not ok:
            raise ValueError('reviewed hit-health lifetimes ' + fn['address'] + ': ' + why)

    digest = lambda data: hashlib.sha256(data).hexdigest()
    require(owner == row['owner'] and owner in unit.get('entities', {}), 'owner changed')
    entity = unit['entities'][owner]
    require(entity['vtable'].lower() == row['entityVtable'].lower(), 'registered vtable changed')
    require(entity['functions']['Main']['address'].lower() == fn['address'].lower(), 'registered function changed')
    start, end = int(fn['address'], 16), int(fn['bodyEndExclusive'], 16)
    raw = image.bytes_at(start, end - start)
    require(raw is not None and digest(raw) == row['nativeExtentSha256'], 'native body changed')
    require(digest(fn['decompile'].encode()) == row['sourceSha256'], 'exported source changed')
    require(digest(source.encode()) == row['luaPhaseSha256'], 'lifted phase changed')
    require(digest(json.dumps(calls, separators=(',', ':')).encode()) == row['callsSha256'], 'call accounting changed')
    require(row['method'] == 'IsHitByHeroExceptAbilityAndHealthAbove' and row['ability'] == 14, 'unsupported helper')
    require(row['minimumHealth'] == 0 and type(row['minimumHealth']) in (int, float), 'unsupported threshold')
    literal = int(row['heroLiteral'], 16)
    require(literal == 0x125d1c8 and image.string_at(literal) == 'SCRIPT_NAME_HERO', 'hero literal changed')
    require(len(row['helperBodies']) == 3 and {b['name'] for b in row['helperBodies']} == set(PROFILE), 'helper identities changed')
    for body in row['helperBodies']:
        address, size, sha = PROFILE[body['name']]
        data = image.bytes_at(address, size)
        require((int(body['address'], 16), body['size'], body['sha256']) == (address, size, sha), 'CString profile changed')
        require(data is not None and digest(data) == sha and data.hex() == body['bytes'], 'CString body changed')
    health = row['healthApi']
    require(health['slot'] == 0x420 and (int(health['target'], 16), health['size'], health['sha256']) == HEALTH_PROFILE, 'unsupported health ABI')
    require(image.bytes_at(int(health['vtable'], 16) + health['slot'], 4) == int(health['target'], 16).to_bytes(4, 'little'), 'health slot target changed')
    health_data = image.bytes_at(int(health['target'], 16), health['size'])
    require(health_data is not None and digest(health_data) == health['sha256'], 'health ABI body changed')
    threshold = int(row['thresholdAddress'], 16)
    require(image.bytes_at(threshold, 4) == b'\0' * 4, 'health threshold changed')
    scope = row['scope']
    a, b = int(scope['start'], 16), int(scope['endExclusive'], 16)
    require(start < a < b <= end and digest(raw[a-start:b-start]) == scope['nativeSha256'], 'native scope changed')
    actor_profile = row.get('actorProfile', 'entry-ebp')
    require(actor_profile in ('entry-ebp', 'local-edi'), 'unsupported actor profile')
    entry_fn = fn
    depth_evidence = row.get('entryCallDepthEvidence')
    if depth_evidence is not None:
        # Reviewed corrected-export anchors affect only the entry/frame proof.
        # Source/call/lifted phase checks below continue to use the original fn.
        original_calls = [fn['calls'], fn['indirectCalls']]
        require(digest(json.dumps(original_calls, separators=(',', ':')).encode()) == depth_evidence['originalCallsSha256'], 'stale original entry call evidence')
        depths = depth_evidence['depths']
        originals = fn['calls'] + fn['indirectCalls']
        require(set(depths) == {c['site'].lower() for c in originals} and len(depths) == len(originals), 'entry depth sites changed')
        require(all(type(d) is int and 0 <= d <= 4096 and d % 4 == 0 for d in depths.values()), 'invalid reviewed entry depth')
        require(digest(json.dumps(depths, sort_keys=True, separators=(',', ':')).encode()) == depth_evidence['depthsSha256'], 'reviewed entry depth profile changed')
        require(all(depths[c['site'].lower()] == c['depth'] for c in originals if a <= int(c['site'], 16) < b), 'scope call anchors changed')
        entry_fn = dict(fn, **{key: [dict(c, depth=depths[c['site'].lower()]) for c in fn[key]] for key in ('calls', 'indirectCalls')})
    try:
        entry = prove_entry(raw, entry_fn, image, entity_vtable=int(entity['vtable'], 16), scope_start=a, scope_end=b,
                            frame_depth=row['frameDepth'], mask_offset=scope['maskOffset'],
                            this_definition=int(row['thisDefinition'], 16), actor_definition=int(row['actorDefinition'], 16), actor_profile=actor_profile)
        native = prove_scope(raw[a-start:b-start], a, constructor=PROFILE['constructor'][0], destructor=PROFILE['destructor'][0],
                             hero_literal=literal, flag_offset=scope['flagOffset'], mask_offset=scope['maskOffset'],
                             object_offsets=tuple(scope['objectOffsets']), temporary_mask=scope['temporaryMask'],
                             threshold_address=threshold, false_exit=int(scope['falseExit'], 16), actor_profile=actor_profile)
    except (AssertionError, KeyError, ValueError) as exc:
        raise ValueError('reviewed hit-health native entry/roles refused: ' + str(exc)) from exc
    from tools.script_recovery.convert_quest_unit import _text_order_sites
    ordered = _text_order_sites(fn['decompile'], fn)
    require(ordered is not None, 'call/source alignment unavailable')
    actual = [dict(site=site['site'].lower(), arguments=args, metadata=site)
              for _, _, args, site, _, _ in ordered if a <= int(site['site'], 16) < b]
    require(len(actual) == 10 and actual == scope['sourceCalls'], 'native call/source correspondence changed')
    constructors = [c['metadata'] for c in actual if int(c['metadata'].get('target', '0'), 16) == PROFILE['constructor'][0]]
    destructors = [c['metadata'] for c in actual if int(c['metadata'].get('target', '0'), 16) == PROFILE['destructor'][0]]
    queries = [c['metadata'] for c in actual if c['metadata'].get('slot') in ('0x54', '0xa8', '0xa4')]
    health_calls = [c['metadata'] for c in actual if c['metadata'].get('slot') == '0x420']
    depth, objects = row['frameDepth'], scope['objectOffsets']
    require(len(constructors) == len(destructors) == len(queries) == 3 and len(health_calls) == 1, 'unsupported local call set')
    require([c['ecxStack'] for c in constructors] == [offset-depth for offset in objects] and all(c['depth'] == depth+8 for c in constructors), 'constructor coordinates changed')
    require(all(c['pushedStack'] == [None, None] for c in constructors), 'constructor pending argument snapshot changed')
    require([c['ecxStack'] for c in destructors] == [offset-depth for offset in reversed(objects)] and all(c['depth'] == depth for c in destructors), 'destructor coordinates changed')
    for index, c in enumerate(queries):
        require(int(c['slot'], 16) == (0x54, 0xa8, 0xa4)[index] and c['depth'] == depth+(8 if index == 2 else 4), 'query ABI changed')
        require(c['pushedStack'] == [objects[index]-depth]+([None] if index == 2 else []), 'query name coordinate changed')
    require(health_calls[0]['depth'] == depth+4 and health_calls[0]['pushedStack'] == [None], 'health argument coordinates changed')
    clean = masked(source)
    require(len(re.findall(r'\bfunction\b', clean)) == 1, 'nested function/closure')
    locals_ = []
    for names in re.findall(r'(?m)^[ \t]*local ([^=\n]+)', clean):
        locals_.extend(n.strip() for n in names.split(','))
    require(len(locals_) == len(set(locals_)), 'duplicate local declaration')
    require(re.fullmatch(r'function [A-Za-z_]\w*\(quest, me\)', clean.splitlines()[0]) is not None, 'function receiver parameters changed')
    require(len(re.findall(r'(?m)^[ \t]*local resources = quest:RetailResources\(\)$', clean)) == 1, 'resource receiver binding changed')
    require(not any(n in locals_ for n in ('quest', 'me')), 'parameter shadowing')
    require(not re.search(r'(?m)^[ \t]*(?:quest|me|resources)\s*=(?!=)', clean), 'receiver binding reassigned')
    flag, scratch, health_alias = scope['flagAlias'], scope['queryAlias'], scope['healthAlias']
    secondary = scope.get('secondaryQueryAlias', scratch)
    require(re.fullmatch(r'[A-Za-z_]\w*', secondary) is not None and secondary in locals_ and secondary not in (flag, health_alias), 'invalid secondary query alias')
    require(len({flag, scratch, health_alias}) == 3 and all(re.fullmatch(r'[A-Za-z_]\w*', n) and n in locals_ for n in (flag, scratch, health_alias)), 'invalid/escaping local roles')
    prefix = scope['oldLuaPrefix']
    require(prefix and source.count(prefix) == 1 and digest(prefix.encode()) == scope['luaPrefixSha256'], 'ambiguous Lua prefix')
    pos = source.index(prefix)
    labels = scope['labels']
    require(len(labels) == 4 and len(set(labels)) == 4 and all(re.fullmatch(r'[A-Za-z_]\w*', n) for n in labels), 'invalid labels')
    clear, past_clear, test, past_test = labels
    expected = f'''{scratch} = me:MsgIsHitByHero()
if not {scratch} then
{secondary} = me:MsgIsHitByAnySpecialAbilityFromHero()
if {secondary} then
{secondary} = me:MsgIsHitByHeroSpecialAbility(0xe)
if not {secondary} then goto {test} end
end
goto {clear}
else
goto {test}
end
goto {past_clear}
::{clear}::
{flag} = 0
::{past_clear}::
goto {past_test}
::{test}::
{health_alias} = quest:GetHealth(me)
{flag} = 1
if {health_alias} <= 0.0 then goto {clear} end
::{past_test}::
'''
    require(significant(prefix) == significant(expected), 'unsupported Lua control/statement shape')
    outside = masked(source[:pos] + source[pos+len(prefix):])
    require(all(not re.search(r'\bgoto\s+'+re.escape(label)+r'\b|::'+re.escape(label)+r'::', outside) for label in labels), 'external Lua label dependency')
    require(scope['diagnostics'] == [], 'unexpected diagnostics')
    inline = re.compile(r'(?m)^(?P<i>[ \t]*)if (?P<c>(?:not )?[A-Za-z_]\w*) then break end$')
    require(len(list(inline.finditer(source))) == row['analysisInlineBreaks'], 'analysis control shape changed')
    analysis = inline.sub(lambda m: m['i']+'if '+m['c']+' then\n'+m['i']+'    break\n'+m['i']+'end', source)
    at = analysis.index(prefix) + len(prefix)
    indent = re.match(r'[ \t]*', prefix)[0]
    proofs = []
    for alias in dict.fromkeys((scratch, secondary, health_alias)):
        marker = indent + alias + ' = nil\n'
        hypothetical = analysis[:at] + marker + analysis[at:]
        proofs.append(dict(alias=alias, analysisOnlyAssignment=True, proof=liveness(hypothetical, at+len(indent), alias)))
    methods = list(METHODS) + ['GetHealth']
    indices = [n for n in range(len(calls)-3) if list(calls[n:n+4]) == methods]
    require(len(indices) == 1 and indices[0] == scope['callListIndex'], 'query/health call accounting ambiguous')
    call_index = indices[0]
    replacement = indent + flag + ' = resources:IsHitByHeroExceptAbilityAndHealthAbove(me, 14, 0.0) and 1 or 0\n'
    out = source[:pos] + replacement + source[pos+len(prefix):]
    updated_calls = list(calls)
    updated_calls[call_index:call_index+4] = ['RESOURCE_IsHitByHeroExceptAbilityAndHealthAbove']
    review = dict(nativeStart=scope['start'], nativeEndExclusive=scope['endExclusive'], nativeRoles=native,
                  registeredEntryFrame=entry, sourceCalls=actual, deadScratch=proofs, numericFlagRetained=True,
                  orderedPositiveHealth=True, requiredCapability=row['method'],
                  scope='Normal query/health/CString lifetime topology with checked source and local roles under explicit native ABI/object contracts. No active heap, sol dispatch, exceptions/reentry or gameplay proof.')
    if depth_evidence is not None:review['reviewedEntryCallDepthEvidence'] = depth_evidence
    return out, list(todo), updated_calls, [review]
