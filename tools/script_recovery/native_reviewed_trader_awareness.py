"""Restore the inner-loop creature in TraderConflictGood's awareness call."""
import hashlib

OLD = 'quest:IsThingAwareOfOtherThingInAnyWay(pThing1, nil --[[missing]])'
NEW = 'quest:IsThingAwareOfOtherThingInAnyWay(pThing1, pCVar7)'
DIAGNOSTIC = 'IsThingAwareOfOtherThingInAnyWay: missing spThing2 (1 args for 2 params)'


def restore(source, todo, calls, fn, row, rdata):
    if row is None:
        return source, todo, calls

    def require(ok, reason):
        if not ok:
            raise ValueError('Trader Conflict awareness: ' + reason)

    start, end = int(fn['address'], 16), int(fn['bodyEndExclusive'], 16)
    require(start == 0x00DFC630 and
            hashlib.sha256(fn['decompile'].encode()).hexdigest() == row['sourceSha256'],
            'reviewed source changed')
    require(hashlib.sha256(rdata.bytes_at(start, end-start)).hexdigest() == row['nativeSha256'],
            'native body changed')
    native = fn['decompile']
    require('pCVar7 = (CScriptThing_bv *)(*(int *)(this + 0x48) + iVar9);' in native and
            'pThing1 = (**(code **)(*piVar1 + 0x118))(piVar1);' in native and
            'bVar4 = (**(code **)(iVar2 + 0x9f0))(*(void **)(this + 0x40),pThing1,pCVar7);' in native,
            'native awareness operands changed')
    require(source.count(OLD) == 1 and
            source.count('pCVar7 = quest:GetStateListAt("AllCreatures", (iVar9) / 0xc)') == 1 and
            todo.count(DIAGNOSTIC) == 1,
            'lifted awareness operands changed')
    todo.remove(DIAGNOSTIC)
    return source.replace(OLD, NEW), todo, calls
