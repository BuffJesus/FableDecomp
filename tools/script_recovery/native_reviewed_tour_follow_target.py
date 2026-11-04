"""Restore TourGuideFollower's retained guide target on its second follow."""
import hashlib

OLD = 'me:FollowThing(nil --[[missing]], quest:ReadGlobalGameData(0x8d0), true)'
NEW = 'me:FollowThing(r1, quest:ReadGlobalGameData(0x8d0), true)'
DIAGNOSTIC = 'FollowThing: missing spTargetThing (2 args for 3 params)'


def restore(source, todo, calls, fn, row, rdata):
    if row is None:
        return source, todo, calls

    def require(ok, reason):
        if not ok:
            raise ValueError('TourGuide follower target: ' + reason)

    start, end = int(fn['address'], 16), int(fn['bodyEndExclusive'], 16)
    require(start == 0x00EE4DD0 and
            hashlib.sha256(fn['decompile'].encode()).hexdigest() == row['sourceSha256'],
            'reviewed source changed')
    require(hashlib.sha256(rdata.bytes_at(start, end-start)).hexdigest() == row['nativeSha256'],
            'native body changed')
    native = fn['decompile']
    require(native.count('_FollowThing_CScriptGameResourceObjectScriptedThingBase__UAEXABVCScriptThing__M_N_Z') == 2 and
            native.count('(int)auStack_34,') == 2 and
            '"TourGuideGuide"' in native and
            '(*(int **)((int)this + 4),(CScriptThing_bv *)auStack_34,&CStack_48);' in native,
            'native guide target changed')
    require(source.count(OLD) == 1 and source.count(NEW) == 1 and
            todo.count(DIAGNOSTIC) == 1,
            'lifted follow calls changed')
    todo.remove(DIAGNOSTIC)
    return source.replace(OLD, NEW), todo, calls
