"""Arena Q_Arena.CrowdChecker (0x00F1CCE0): its two local arrays.

- `int aiStack_c[3] = {0, 200, 400}` -- the crowd-level thresholds compared with
  NewCrowdPoints (quest +0xF4). Lifted as a nil table it raised "attempt to index a nil
  value" whenever the crowd level was re-evaluated.
- The CrowdSoundSource handle array at -0x12C: F1D15C `lea ecx,[esp+0x44]` (depth 368)
  with the CrowdSoundSource count pushed, then `call 0xA96100` (Vector_ZeroInit) builds
  a zeroed uint array; each source's previous loop sound is stopped and the new
  PlayCriteriaSoundOnThing handle stored in its slot (F1E2xx loop).

Indices stay zero-based in the C and become `+ 1` in the ENGINE_Array* expansions.
Native body sha256 and the Vector_ZeroInit receiver are gated; every rewrite must match
its reviewed count, else the function is left unchanged.
"""
import hashlib

CROWD_CHECKER = 0x00F1CCE0
NATIVE_SHA256 = 'c095beeeebfe44aa55720c555183efdcb6c1463616eb69feebcb5ce6bfcc4802'
ZERO_INIT = 0x00A96100


def recover_crowd_checker(source, fn, rdata):
    if int(fn['address'], 16) != CROWD_CHECKER:
        return source
    raw = rdata.bytes_at(CROWD_CHECKER, int(fn['bodyEndExclusive'], 16) - CROWD_CHECKER)
    if raw is None or hashlib.sha256(raw).hexdigest() != NATIVE_SHA256:
        return source
    zero = [c for c in fn.get('calls', []) if int(c['target'], 16) == ZERO_INIT]
    if len(zero) != 1 or zero[0].get('ecxStack') != -0x12c:
        return source
    newline = '\r\n' if '\r\n' in source else '\n'
    text = source.replace('\r\n', '\n')
    edits = [
        ('  aiStack_c[0] = 0;\n  aiStack_c[1] = 200;\n  aiStack_c[2] = 400;\n',
         '  aiStack_c = ENGINE_IntArray(0, 200, 400);\n', 1),
        ('(aiStack_c[iVar3] <= ', '(ENGINE_ArrayAt(aiStack_c, iVar3) <= ', 1),
        ('Vector_ZeroInit(LOCALLIST_Count(xStack_150));',
         'xStack_12c = ENGINE_ZeroArray(LOCALLIST_Count(xStack_150));', 1),
        ('GSI->StopSound(*(uint *)((int)xStack_12c + uVar9 * 4));',
         'GSI->StopSound(ENGINE_ArrayAt(xStack_12c, uVar9));', 1),
        # the slot address is taken before the call and written after it; uVar9 is unchanged between
        ('          puStack_10 = (uint *)((int)xStack_12c + uVar9 * 4);\n', '', 1),
        ('          *puStack_10 = uVar5;\n', '          ENGINE_ArraySet(xStack_12c, uVar9, uVar5);\n', 1),
    ]
    for old, new, count in edits:
        if text.count(old) != count:
            return source
        text = text.replace(old, new)
    if 'aiStack_c[' in text or 'puStack_10' in text.split('{', 1)[1].replace('uint *puStack_10;', ''):
        return source
    return text.replace('\n', newline)
