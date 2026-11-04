"""PlayWave's multi-marker nearby spawn: pending pushes cross the GetPos call.

F1F67B/F1F684/F1F687 push false, the ArenaEnemy string, and float 1.0
before GetPos at F1F693. F1F6B2 consumes those after pushing pos, creature
type and the hidden return slot. F1F6BD copies the returned thing into
the local at -0xF4. The export attaches pending pushes to GetPos instead.
"""
import hashlib

PLAY_WAVE = 0x00F1EED0
NATIVE_SHA256 = 'e6c92c85661ca4ea5e0e31459825a85c68ef90a21fb10542cf6ebc7347ef72bc'


def recover_arena_wave(source, fn, rdata):
    if int(fn['address'], 16) != PLAY_WAVE:
        return source
    raw = rdata.bytes_at(PLAY_WAVE, int(fn['bodyEndExclusive'], 16) - PLAY_WAVE)
    if raw is None or hashlib.sha256(raw).hexdigest() != NATIVE_SHA256:
        return source
    newline = '\r\n' if '\r\n' in source else '\n'
    text = source.replace('\r\n', '\n')
    edits = [
        ('uVar9 = CScriptThing::GetPos(LOCALLIST_At(xStack_118, (iVar4) / 0xc), 0x3f800000,xStack_78,0);',
         'uVar9 = CScriptThing::GetPos(LOCALLIST_At(xStack_118, (iVar4) / 0xc));'),
        ('iVar6 = GSI->CreateCreatureNearby(xStack_30,creatureType,uVar9);',
         'iVar6 = GSI->CreateCreatureNearby(xStack_30,creatureType,uVar9,1.0,xStack_78,false);'),
    ]
    if any(text.count(old) != 1 for old, _ in edits):
        return source
    for old, new in edits:
        text = text.replace(old, new)
    return text.replace('\n', newline)
