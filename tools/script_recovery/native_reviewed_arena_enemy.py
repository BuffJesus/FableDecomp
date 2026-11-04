"""Arena's ArenaEnemy reads of the quest's round snapshot, creature counters and health bar.

The entity reaches the quest object through `this+0x14`; Ghidra prints the round
vector walks (rounds stride 0x38, waves 0x3c, creature groups 0x38) and the
`TotalCreatures` counter array (quest +0xD0, stride 4) as raw pointer arithmetic
the lifter cannot express. These are the same fields `native_arena_rounds` maps to
the snapshot keys the sidecar's InitialiseArenaRounds stores (wave +0x28
NumWaveCreatures; group +0x28 CreatureType, +0x34 DeathScore).

Reviewed against retail bytes (sha256 gates below):
- Main 0x00F1A990: the creature-group loop compares the entity's def name with each
  group's CreatureType and records the matching index in `this+0x24`; the cursor
  `iVar11` is the group byte offset (0 then +0x38 with each `ctr_50` step). The
  inline CCharString equality (same object / both non-null with equal length and
  Compare==0) is string equality for these non-empty definition names. The health
  bar colour is built byte by byte at F1AC84..F1AC98: B=0, G=R=A=0xFF.
- OnPredicateFail 0x00F1A8F0: decrements TotalCreatures[this+0x24] and, when
  killed by the hero, adds that group's DeathScore to NewCrowdPoints (+0xF4).

Every rewrite must match its exact reviewed count; otherwise the function is left
unchanged.
"""
import hashlib

MAIN = 0x00F1A990
ON_PREDICATE_FAIL = 0x00F1A8F0
NATIVE_SHA256 = {
    MAIN: '3e90bbdcea7944a862f6d7ad5ee62e5db058270d56d98655628917cd652dc9b3',
    ON_PREDICATE_FAIL: '3d52ef8ad46a767a812daeea304651d878e15023efa1bdd8c3409f7539abaec7',
}
QUEST = '*(int *)(this + 0x14)'
ROUND = f'*(int *)({QUEST} + 0xa8)'
WAVE = f'*(int *)({QUEST} + 0xac)'
WAVES = f'*(int *)({ROUND} * 0x38 + 0x2c + *(int *)({QUEST} + 0x98))'
WAVE_KEY = f'"Rounds_" .. {ROUND} .. "_Waves_" .. {WAVE}'
GROUP = '*(int *)(this + 0x24)'


def _state(key, kind='Int'):
    return f'QUESTSTATE_Get{kind}(__key({key}))'


def _rewrite(text, edits):
    for old, new, count in edits:
        if text.count(old) != count:
            return None
        text = text.replace(old, new)
    return text


def _main(text):
    num_wave_creatures = f'*(int *)({WAVES} + 0x28 + {WAVE} * 0x3c)'
    compare = '''      iVar9 = *(int *)(*(int *)(*(int *)(iVar9 + 0xa8) * 0x38 + 0x2c + *(int *)(iVar9 + 0x98)) + 0x2c + *(int *)(iVar9 + 0xac) * 0x3c);
      pCVar8 = CScriptThing::GetDefName((CScriptThing *)(this + 8), &xStack_2c);
      CVar1 = *(CCharString *)(iVar9 + 0x28 + iVar11);
      CVar2 = *pCVar8;
      if (CVar1 == CVar2) {
        c_stk_55 = '\\x01';
      }
      else if ((CVar1 == (CCharString)0x0) || (CVar2 == (CCharString)0x0)) {
        c_stk_55 = '\\0';
      }
      else if (*(int *)((int)CVar1 + 4) == *(int *)((int)CVar2 + 4)) {
        iVar9 = CBasicString<char>::Compare(*(void **)CVar1,*(void **)CVar2);
        c_stk_55 = !(iVar9 != 0);
      }
      else {
        c_stk_55 = '\\0';
      }
'''
    creature_type = _state(WAVE_KEY + ' .. "_Creatures_" .. ctr_50 .. "_CreatureType"', 'String')
    compared = f'''      pCVar8 = CScriptThing::GetDefName((CScriptThing *)(this + 8), &xStack_2c);
      iVar9 = ENGINE_StrCmp(pCVar8, {creature_type});
      c_stk_55 = !(iVar9 != 0);
'''
    # the byte cursor advances in lock step with the group index from zero
    if not ('  iVar11 = 0;\n' in text and text.count('iVar11 = iVar11 + 0x38;') == 1
            and '      ctr_50 = ctr_50 + 1;\n      iVar11 = iVar11 + 0x38;\n' in text
            and text.count('  ctr_50 = 0;\n') == 1):
        return None
    return _rewrite(text, [
        (num_wave_creatures, _state(WAVE_KEY + ' .. "_NumWaveCreatures"'), 2),
        (compare, compared, 1),
        ('      iVar11 = iVar11 + 0x38;\n', '', 1),
        ('  ctr_50 = (CCharString)0xffffff00;\n'
         '  iVar11 = GSI->AddQuestInfoBarHealth(p0,(CRGBColour *)&xStack_50,&xStack_4c,1.0);\n',
         '  iVar11 = GSI->AddQuestInfoBarHealth(p0,ENGINE_Colour(255, 255, 0, 255),&xStack_4c,1.0);\n', 1),
    ])


def _on_predicate_fail(text):
    counter = f'__key("TotalCreatures_" .. {GROUP})'
    death_score = (f'*(int *)(*(int *)({WAVES} + 0x2c + {WAVE} * 0x3c) + 0x34 + {GROUP} * 0x38)')
    return _rewrite(text, [
        (f'  piVar1 = (int *)({QUEST} + 0xd0 + {GROUP} * 4);\n  *piVar1 = *piVar1 + -1;\n',
         f'  QUESTSTATE_SetInt({counter}, QUESTSTATE_GetInt({counter}) - 1);\n', 1),
        (death_score, _state(WAVE_KEY + f' .. "_Creatures_" .. {GROUP} .. "_DeathScore"'), 1),
    ])


def recover_arena_enemy(source, fn, rdata):
    address = int(fn['address'], 16)
    if address not in NATIVE_SHA256:
        return source
    start, end = address, int(fn['bodyEndExclusive'], 16)
    raw = rdata.bytes_at(start, end - start)
    if raw is None or hashlib.sha256(raw).hexdigest() != NATIVE_SHA256[address]:
        return source
    newline = '\r\n' if '\r\n' in source else '\n'
    text = source.replace('\r\n', '\n')
    result = (_main if address == MAIN else _on_predicate_fail)(text)
    if result is None:
        return source
    return result.replace('\n', newline)
