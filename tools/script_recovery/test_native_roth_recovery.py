import copy
import json
from pathlib import Path
from unittest.mock import patch

import pytest

from tools.script_recovery.convert_quest_unit import UnitConverter, disambiguate_call_labels
from tools.script_recovery.lift_native_lua import RE_LOCAL_ASSIGN
from tools.script_recovery.native_evidence_lowering import fold_position_reads
from tools.script_recovery.native_reviewed_resource_constructors import restore

ROTH = '0x00f21880'
GUARD2 = '0x00f19bb0'


@pytest.fixture(scope='module')
def captured(tmp_path_factory):
    root = Path(__file__).resolve().parents[2]
    unit = json.loads((root / 'refs/script_recovery/arena/units/Q_Arena.json').read_text())
    converter = UnitConverter(root / 'refs/script_recovery/arena/translation_unit_typed.json')
    seen = {}

    def capture(source, fn, row, rdata):
        if row is not None and fn['address'].lower() in (ROTH, GUARD2):
            seen[fn['address'].lower()] = (source, fn, row, rdata)
        return restore(source, fn, row, rdata)

    with patch('tools.script_recovery.native_reviewed_resource_constructors.restore', capture), \
            patch.object(converter, 'constructor_defaults', return_value={}):
        converter.convert(unit, tmp_path_factory.mktemp('roth'))
    assert set(seen) == {ROTH, GUARD2}
    return seen


def test_roth_resource_storage_release_and_actor(captured):
    source, fn, row, rdata = captured[ROTH]
    result = restore(source, fn, row, rdata)
    assert 'xStack_144 = RESOURCE_NewResource();' in result
    assert 'ACTORMAP_Set(xStack_bc, "Roth", xStack_144);' in result
    assert 'RESOURCE_DestroyMovie(xStack_144)' not in result and 'CPhysicsMeshInfo::~' not in result


@pytest.mark.parametrize('kind', ['actorCall', 'actorSlot', 'actorSource', 'actorHelper'])
def test_roth_actor_map_drift_is_rejected(captured, kind):
    source, original_fn, original_row, rdata = captured[ROTH]
    row, fn = copy.deepcopy(original_row), copy.deepcopy(original_fn)
    site = next(c for c in fn['calls'] if c['site'].lower() == '0x00f21cbd')
    if kind == 'actorCall':
        row['actorMapResourceSites'][0]['call'] = '0xf21cbe'
    elif kind == 'actorSlot':
        site['pushedStack'][0] = -320
    elif kind == 'actorSource':
        row['actorMapResourceSites'][0]['sourceCall'] = 'ACTORMAP_Set(xStack_bc, "Rath", &xStack_144);'
    else:
        row['helperBytes']['0xcd3d2e'] = '00' * 32
    with pytest.raises(ValueError):
        restore(source, fn, row, rdata)


def test_guard2_alias_occurrence_rewrite(captured):
    source, fn, row, rdata = captured[GUARD2]
    part = row['resourceDestructorRewrites'][0]
    assert part['sourceCall'] == 'RESOURCE_ReleaseResource(ppuVar18);' and part['sourceOccurrence'] == 0
    result = restore(source, fn, row, rdata)
    assert result.count('RESOURCE_ReleaseResource(xStack_c8);') >= 1 and result.count('RESOURCE_ReleaseResource(ppuVar18);') == 1
    for change in ({'sourceOccurrence': 2}, {'sourceCallCount': 3}, {'sourceCall': 'RESOURCE_ReleaseResource(ppuVar17);'}):
        drifted = copy.deepcopy(row)
        drifted['resourceDestructorRewrites'][0].update(change)
        with pytest.raises(ValueError):
            restore(source, fn, drifted, rdata)


def test_byte_split_assignment_is_a_local():
    assert RE_LOCAL_ASSIGN.match('        pCVar22_b3 = !(fret_0 <= (float10)0.0);')
    assert RE_LOCAL_ASSIGN.match('  uVar3_b1 = 0;')
    assert not RE_LOCAL_ASSIGN.match('  xStack_10_b3 = 0;')


def test_staged_vector_copy_into_named_local():
    text = ('  C3DVector center;\n  undefined4 uVar14;\n'
            '    uVar14 = pCVar6.x;\n    uVar17 = pCVar6.y;\n    uVar21 = pCVar6.z;\n    gsivt6 = **(int **)(this + 4);\n'
            '    center._4_4_ = uVar17;\n    center._0_4_ = uVar14;\n    center._8_4_ = uVar21;\n'
            '    GSI->SetWanderCentrePoint(pThing,center);\n    uVar14 = pCVar7.x;\n')
    out = fold_position_reads(text)
    assert '    center = ENGINE_VectorCopy(pCVar6);\n    gsivt6 = **(int **)(this + 4);\n' in out
    # a temporary read after the copy, a swapped member or an undeclared object keep the stores
    for bad in (text.replace('    uVar14 = pCVar7.x;\n', '    f(uVar14);\n'),
                text.replace('center._0_4_ = uVar14', 'center._0_4_ = uVar17').replace('center._4_4_ = uVar17', 'center._4_4_ = uVar14'),
                text.replace('  C3DVector center;\n', '')):
        assert 'ENGINE_VectorCopy' not in fold_position_reads(bad)


def test_shared_label_pairs_in_call_order_when_text_unreconciled():
    label = 'CScriptGameResourceObjectMovieBase::~CScriptGameResourceObjectMovieBase'
    text = f'  {label}(a);\n  {label}(b);\n'
    calls = [dict(site='0x10', target='0x007E74D0', currentName=label),
             dict(site='0x20', target='0x006E7B80', currentName=label)]
    fn = dict(calls=calls, callOrder=['0x20', '0x10'])
    with patch('tools.script_recovery.convert_quest_unit._text_order_sites', return_value=None):
        out, renamed = disambiguate_call_labels(text, calls, fn)
        assert out.index('__at6e7b80(a)') < out.index('__at7e74d0(b)')
        # a site missing from callOrder keeps the address order
        out, _ = disambiguate_call_labels(text, calls, dict(fn, callOrder=['0x20']))
        assert out.index('__at7e74d0(a)') < out.index('__at6e7b80(b)')
