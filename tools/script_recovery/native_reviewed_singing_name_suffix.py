"""Recover SingingStones' four-letter personal tune message."""
import hashlib
from tools.script_recovery.native_state_pointer_cleanup import remove_mapped_state_cursor

ARG = '-- TODO(native): native_arg_switch_2 = *puVar7'
APPEND = '-- TODO(native): CCharString::operator+=(&xStack_80,(int)p0);'
ARG_DIAG = 'unlifted: native_arg_switch_2 = *puVar7'
APPEND_DIAG = 'unlifted: CCharString::operator+=(&xStack_80,(int)p0);'
SITES = (0x0122dce0, 0x0122dce4, 0x012d8858, 0x012cd3c0)


def restore(source, todo, calls, fn, row, rdata, fields):
    if row is None:
        return source, todo, calls

    def require(ok, reason):
        if not ok:
            raise ValueError('SingingStones name suffix: ' + reason)

    start, end = int(fn['address'], 16), int(fn['bodyEndExclusive'], 16)
    require(start == 0x00ED2BB0 and
            hashlib.sha256(fn['decompile'].encode()).hexdigest() == row['sourceSha256'],
            'reviewed source changed')
    require(hashlib.sha256(rdata.bytes_at(start, end-start)).hexdigest() == row['nativeSha256'],
            'native body changed')
    native = fn['decompile']
    require('puVar7 = (undefined4 *)((int)this + 0x6c);' in native and
            'switch(*puVar7)' in native and 'puVar7 = puVar7 + 1;' in native and
            'while (iVar5 < 4);' in native and
            'CCharString::CCharString((CCharString *)&CStack_80,"TEXT_QST_060_NAME_",-1)' in native and
            'CCharString::operator+=((CCharString *)&CStack_80,(int)p0);' in native,
            'native tune name changed')
    direct = {call['site'].lower(): call for call in fn['calls']}
    require(direct.get('0x00ed2fbc', {}).get('target', '').lower() == '0x0099f100',
            'native append call changed')
    require(all(rdata.bytes_at(site, 4) == (char.encode() + b'\0\0\0')
                for site, char in zip(SITES, 'ABCD')),
            'native letter table changed')
    require(all(fields.get(hex(0x6c + i*4)) == [f'YourPersonalPlayList_{i}', 'Int']
                for i in range(4)), 'personal playlist layout changed')

    arg = ARG + '\n                    native_arg_switch_2 = nil --[[unresolved native value]]'
    require(source.count(arg) == 1 and todo.count(ARG_DIAG) == 1,
            'lifted switch argument changed')
    source = source.replace(arg, 'native_arg_switch_2 = quest:GetStateInt("YourPersonalPlayList_" .. iVar5)')
    todo.remove(ARG_DIAG)
    require(source.count(APPEND) == 1 and todo.count(APPEND_DIAG) == 1,
            'lifted append changed')
    source = source.replace(APPEND, 'tuneName = tuneName .. p0')
    todo.remove(APPEND_DIAG)
    init = '                iVar5 = 0\n                puVar7 = (this + 0x6c)'
    require(source.count(init) == 1, 'lifted tune loop changed')
    source = source.replace(init, '                local tuneName = "TEXT_QST_060_NAME_"\n' + init)
    line = 'quest:AddLineToConversation(iVar5, "TEXT_QST_060_NAME_", r2, pCVar4, false)'
    require(source.count(line) == 1, 'lifted line changed')
    source = source.replace(line, 'quest:AddLineToConversation(iVar5, tuneName, r2, pCVar4, false)')
    source = remove_mapped_state_cursor(source, 'puVar7', 0x6c)
    return source, todo, [*calls, 'GetStateInt']
