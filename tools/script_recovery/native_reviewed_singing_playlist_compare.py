"""Recover SingingStones' two four-note playlist comparisons."""
import hashlib
from tools.script_recovery.native_state_pointer_cleanup import remove_mapped_state_cursor


CHECKS = (
    ('-- TODO(native): if piVar6[-8] ~= *piVar6 then',
     'unlifted: if piVar6[-8] ~= *piVar6 then', 'PlayList'),
    ('-- TODO(native): if piVar6[-4] ~= *piVar6 then',
     'unlifted: if piVar6[-4] ~= *piVar6 then', 'RudePlayList'),
)


def restore(source, todo, calls, fn, row, rdata, fields):
    if row is None:
        return source, todo, calls

    def require(ok, reason):
        if not ok:
            raise ValueError('SingingStones playlist comparison: ' + reason)

    start, end = int(fn['address'], 16), int(fn['bodyEndExclusive'], 16)
    require(start == 0x00ED2BB0 and
            hashlib.sha256(fn['decompile'].encode()).hexdigest() == row['sourceSha256'],
            'reviewed source changed')
    require(hashlib.sha256(rdata.bytes_at(start, end-start)).hexdigest() == row['nativeSha256'],
            'native body changed')
    native = fn['decompile']
    require('piVar6 = (int *)((int)this + 0x6c);' in native and
            'piVar6 = piVar6 + 1;' in native and 'while (iVar5 < 4);' in native and
            'if (piVar6[-8] != *piVar6)' in native and
            'if (piVar6[-4] != *piVar6)' in native,
            'native playlist comparison changed')
    for prefix, base in (('PlayList', 0x4c), ('RudePlayList', 0x5c),
                         ('YourPersonalPlayList', 0x6c)):
        require(all(fields.get(hex(base + i*4)) == [f'{prefix}_{i}', 'Int'] for i in range(4)),
                'field layout changed: ' + prefix)
    for marker, diagnostic, prefix in CHECKS:
        needle = marker + '\n                if false then'
        require(source.count(needle) == 1 and todo.count(diagnostic) == 1,
                'lifted comparison changed: ' + prefix)
        replacement = ('if quest:GetStateInt("' + prefix + '_" .. iVar5) ~= '
                       'quest:GetStateInt("YourPersonalPlayList_" .. iVar5) then')
        source = source.replace(needle, replacement)
        todo.remove(diagnostic)
    source = remove_mapped_state_cursor(source, 'piVar6', 0x6c)
    return source, todo, [*calls, 'GetStateInt']
