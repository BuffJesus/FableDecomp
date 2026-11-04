"""Recover SingingStone's hit-created effect list append."""
import hashlib

BLOCK = '''            -- TODO(native): p0 = *(__native_entity_state:GetStateInt("self_0x14") + 0x80)
            p0 = nil --[[unresolved native value]]
            -- TODO(native): if p0 == *(__native_entity_state:GetStateInt("self_0x14") + 0x84) then
            if false then
                -- TODO(native): std__vector_InsertRange((void *)(*(int *)(this + 0x14) + 0x7c),(int)p0,(int)pCVar3,(int)&xStack_4d,1,1);
            else
                if p0 ~= nil then
                    -- TODO(native): p0[1] = *(undefined4 *)(pCVar3 + 0x4);
                    -- TODO(native): piVar1 = *(pCVar3 + 0x8)
                    piVar1 = nil --[[unresolved native value]]
                    -- TODO(native): p0[2] = piVar1;
                    if piVar1 ~= nil then
                        -- TODO(native): *piVar1 = *piVar1 + 1;
                    end
                end
                -- TODO(native): (quest:GetStateListCount("EffectList") * 0xc) = (quest:GetStateListCount("EffectList") * 0xc) + 0xc;
            end
'''
DIAGNOSTICS = (
    'unlifted: p0 = *(__native_entity_state:GetStateInt("self_0x14") + 0x80)',
    'unlifted: if p0 == *(__native_entity_state:GetStateInt("self_0x14") + 0x84) then',
    'unlifted: std__vector_InsertRange(',
    'unlifted: p0[1] = *(undefined4 *)(pCVar3 + 0x4);',
    'unlifted: piVar1 = *(pCVar3 + 0x8)',
    'unlifted: p0[2] = piVar1;',
    'unlifted: *piVar1 = *piVar1 + 1;',
    'unlifted: (QUESTLIST_Count("EffectList") * 0xc)',
)


def restore(source, todo, calls, fn, row, rdata, quest):
    if row is None:
        return source, todo, calls

    def require(ok, reason):
        if not ok:
            raise ValueError('SingingStone effect push: ' + reason)

    start, end = int(fn['address'], 16), int(fn['bodyEndExclusive'], 16)
    require(start == 0x00ED3370 and
            hashlib.sha256(fn['decompile'].encode()).hexdigest() == row['sourceSha256'],
            'reviewed source changed')
    require(hashlib.sha256(rdata.bytes_at(start, end-start)).hexdigest() == row['nativeSha256'],
            'native body changed')
    native = fn['decompile']
    require('p0 = *(undefined4 **)(iVar9 + 0x80);' in native and
            'if (p0 == *(undefined4 **)(iVar9 + 0x84))' in native and
            'std__vector_InsertRange((void *)(iVar9 + 0x7c),(int)p0,(int)pCVar3' in native and
            'p0[1] = *(undefined4 *)&pCVar3->field_0x4;' in native and
            '*(int *)(iVar9 + 0x80) = *(int *)(iVar9 + 0x80) + 0xc;' in native,
            'native vector append changed')
    direct = {call['site'].lower(): call for call in fn['calls']}
    require(direct.get('0x00ed3840', {}).get('target', '').lower() == '0x008ad5f0' and
            direct.get('0x00ed34c2', {}).get('target', '').lower() == '0x008adf90',
            'native list helper changed')
    require(quest['unmappedFields'] == [
        {'offset': '0x7c', 'pdbOffset': '0x90',
         'type': 'vector<CScriptThing,std::allocator<CScriptThing>_>', 'name': 'EffectList'}],
            'effect list layout changed')
    require(source.count(BLOCK) == 1 and source.count('quest:StateListPush("EffectList", pCVar3)') == 1,
            'lifted effect paths changed')
    for marker in DIAGNOSTICS:
        matches = [value for value in todo if value.startswith(marker)]
        require(len(matches) == 1, 'diagnostic changed: ' + marker[:40])
        todo.remove(matches[0])
    source = source.replace(BLOCK, '            quest:StateListPush("EffectList", pCVar3)\n')
    return source, todo, [*calls, 'StateListPush']
