"""Recover SingingStone hit flag, played-note checks, and playlist store."""
import hashlib

FIRST = '-- TODO(native): if *(__native_entity_state:GetStateInt("self_0x14") + ctr_4c) ~= __native_entity_state:GetStateInt("MyStoneNumber") then goto LAB_00ed34e2 end'
SEEN = '-- TODO(native): if *(__native_entity_state:GetStateInt("self_0x14") + iVar8) == __native_entity_state:GetStateInt("MyStoneNumber") then'
STORE = '-- TODO(native): *(undefined4 *) (*(int *)(this + 0x14) + 0x6c + *(int *)(*(int *)(this + 0x14) + 0x48) * 4) = *(undefined4 *)(this + 0x1c);'
FALSE_FLAG = "-- TODO(native): xStack_4d = '\\0';"
TRUE_FLAG = "-- TODO(native): xStack_4d = '\\x01';"


def restore(source, todo, calls, fn, row, rdata, quest_fields, entity_fields):
    if row is None:
        return source, todo, calls

    def require(ok, reason):
        if not ok:
            raise ValueError('SingingStone play: ' + reason)

    start, end = int(fn['address'], 16), int(fn['bodyEndExclusive'], 16)
    require(start == 0x00ED3370 and
            hashlib.sha256(fn['decompile'].encode()).hexdigest() == row['sourceSha256'],
            'reviewed source changed')
    require(hashlib.sha256(rdata.bytes_at(start, end-start)).hexdigest() == row['nativeSha256'],
            'native body changed')
    native = fn['decompile']
    require("cStack_4d = '\\x01';" in native and "cStack_4d = '\\0';" in native and
            "if (cStack_4d != '\\0')" in native and
            'if (*(int *)(iVar9 + (int)CStack_4c) != *(int *)((int)this + 0x1c))' in native and
            'if (*(int *)(iVar5 + iVar8) == *(int *)((int)this + 0x1c))' in native and
            '0x6c + *(int *)(*(int *)((int)this + 0x14) + 0x48) * 4' in native,
            'native note path changed')
    require(quest_fields.get('0x48') == ['CurrentPlayListIndex', 'Int'] and
            all(quest_fields.get(hex(0x6c + i*4)) == [f'YourPersonalPlayList_{i}', 'Int']
                for i in range(4)) and
            entity_fields.get('0x1c') == ['MyStoneNumber', 'Int'],
            'field layout changed')

    replacements = (
        (FIRST + '\n    if false then goto LAB_00ed34e2 end',
         'if quest:GetStateInt("YourPersonalPlayList_" .. ctr_40) ~= __native_entity_state:GetStateInt("MyStoneNumber") then goto LAB_00ed34e2 end'),
        (SEEN + '\n                    if false then',
         'if quest:GetStateInt("YourPersonalPlayList_" .. iVar9) == __native_entity_state:GetStateInt("MyStoneNumber") then'),
        (FALSE_FLAG, 'xStack_4d = 0'),
        (TRUE_FLAG, 'xStack_4d = 1'),
        (STORE, 'quest:SetStateInt("YourPersonalPlayList_" .. quest:GetStateInt("CurrentPlayListIndex"), __native_entity_state:GetStateInt("MyStoneNumber"))'),
    )
    for old, new in replacements:
        require(source.count(old) == 1, 'lifted note path changed: ' + old[:40])
        source = source.replace(old, new)
    diagnostics = (
        "unlifted: xStack_4d = '\\0';", "unlifted: xStack_4d = '\\x01';",
        'unlifted: *(undefined4 *) (*(int *)(this + 0x14) + 0x6c',
        'unlifted: if *(__native_entity_state:GetStateInt("self_0x14") + ctr_4c)',
        'unlifted: if *(__native_entity_state:GetStateInt("self_0x14") + iVar8)',
    )
    for marker in diagnostics:
        matches = [value for value in todo if value.startswith(marker)]
        require(len(matches) == 1, 'diagnostic changed: ' + marker[:35])
        todo.remove(matches[0])
    return source, todo, [*calls, 'GetStateInt', 'SetStateInt']
