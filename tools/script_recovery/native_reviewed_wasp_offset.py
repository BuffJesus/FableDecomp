"""Recover both coordinates of Wasp Boss's world-map marker offset."""
import hashlib

OLD = 'quest:SetQuestWorldMapOffset("Q_WaspBoss", 0xa, nil --[[missing]])'
NEW = 'quest:SetQuestWorldMapOffset("Q_WaspBoss", 10, 10)'
DIAGNOSTIC = 'SetQuestWorldMapOffset: missing y (2 args for 3 params)'


def restore(source, todo, calls, fn, row, rdata):
    if row is None:
        return source, todo, calls

    def require(ok, reason):
        if not ok:
            raise ValueError('Wasp Boss map offset: ' + reason)

    start, end = int(fn['address'], 16), int(fn['bodyEndExclusive'], 16)
    require(start == 0x00E0E820 and
            hashlib.sha256(fn['decompile'].encode()).hexdigest() == row['sourceSha256'],
            'reviewed source changed')
    require(hashlib.sha256(rdata.bytes_at(start, end-start)).hexdigest() == row['nativeSha256'],
            'native body changed')
    native = fn['decompile']
    require('CCharString::CCharString((CCharString *)&local_4,"Q_WaspBoss",-1);' in native and
            'local_c = (CCharString_bv)0xa;' in native and
            'uStack_8 = 10;' in native and
            ' + 0x440))(*(int **)(this + 0x40),&local_4,&local_c);' in native,
            'native map offset call changed')
    require(source.count(OLD) == 1 and todo.count(DIAGNOSTIC) == 1,
            'lifted map offset changed')
    todo.remove(DIAGNOSTIC)
    return source.replace(OLD, NEW), todo, calls
