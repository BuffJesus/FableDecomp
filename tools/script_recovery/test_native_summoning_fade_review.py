import copy
import json
import sys
from pathlib import Path

import pytest

R = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(R))
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.native_reviewed_fade_in_thing import restore


@pytest.fixture(scope='module')
def evidence():
    functions = json.loads((R / 'refs/script_recovery/summoning_the_ship/translation_unit_typed.json').read_text())['functions']
    rows = json.loads((R / 'refs/script_recovery/summoning_the_ship/units/Q_SummoningTheShip.json').read_text())['reviewedFadeInThing']
    return [(fn, rows[fn['address'].lower()]) for fn in functions if fn['address'].lower() in rows]


def test_two_reviewed_calls_recover_only_after_validation(evidence):
    assert len(evidence) == 2
    for fn, row in evidence:
        source = fn['decompile']
        result = restore(source, fn, row, RData())
        assert source.count('FadeInThing(p0,0x40000000);') == 1
        assert result.count('ReviewedFadeInThing(p0,0x40000000);') == 1
        assert restore(source, fn, None, RData()) == source


def test_generated_fade_call_follows_limbo_release():
    for style in ('draft', 'readable'):
        base = R / 'refs/script_recovery/lifted/SummoningTheShip' / style / 'FSE/SummoningTheShip/Entities'
        for name in ('SummonerAttacker', 'SummonerMinion'):
            source = (base / f'{name}.lua').read_text()
            assert source.count('me:FadeInThing(2.0)') == 1
            assert 'TODO(native): FadeInThing' not in source
            assert source.index('EntitySetInLimbo(me, false, true)') < source.index('me:FadeInThing(2.0)')


@pytest.mark.parametrize('fault', ('source_hash', 'native_hash', 'site', 'window', 'helper', 'source', 'parent', 'target', 'duration', 'duplicate'))
def test_stale_or_ambiguous_review_rejected(evidence, fault):
    fn, original = evidence[0]
    fn = copy.deepcopy(fn)
    row = copy.deepcopy(original)
    source = fn['decompile']
    if fault == 'source_hash':
        row['sourceSha256'] = '0' * 64
    elif fault == 'native_hash':
        row['nativeSha256'] = '0' * 64
    elif fault == 'site':
        row['site'] = '0xdf2471'
    elif fault == 'window':
        row['windowSha256'] = '0' * 64
    elif fault == 'helper':
        row['helperSha256'] = '0' * 64
    elif fault == 'source':
        source = source.replace('FadeInThing(p0,0x40000000);', 'changed();')
    elif fault == 'parent':
        fn['decompile'] = fn['decompile'].replace('p0 = (CScriptThing_bv *)(this + 8);', 'p0 = changed;')
        import hashlib
        row['sourceSha256'] = hashlib.sha256(fn['decompile'].encode()).hexdigest()
        source = fn['decompile']
    elif fault == 'target':
        next(c for c in fn['calls'] if int(c['site'], 16) == int(row['site'], 16))['target'] = '0x00CBE716'
    elif fault == 'duration':
        source = source.replace('0x40000000', '0x3f800000')
    else:
        source += '\nFadeInThing(p0,0x40000000);'
    with pytest.raises(ValueError, match='reviewed FadeInThing'):
        restore(source, fn, row, RData())
