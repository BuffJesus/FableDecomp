"""Pinned Magicman item comparison recovery and stale-evidence rejection."""
from copy import deepcopy
import json
from pathlib import Path

import pytest

from tools.script_recovery.convert_quest_unit import UnitConverter
from tools.script_recovery.native_reviewed_presented_item import restore

ROOT = Path(__file__).resolve().parents[2]
DIAGNOSTIC = ('unlifted: iVar21 = CBasicString<char>::Compare((void *)*unaff_EBP,'
              '"OBJECT_BEER_TANKARD");')


def block(label):
    return f'''            cVar6 = me:MsgIsPresentedWithItem()
            if cVar6 then
                if unaff_EBP == nil then
                    bVar5 = false
                    if bVar5 then
                        goto {label}
                    end
                else
                    -- TODO(native): iVar21 = CBasicString<char>::Compare((void *)*unaff_EBP,"OBJECT_BEER_TANKARD");
                    if iVar21 == 0 then goto {label} end
                end
                goto FLOW_past_{label.lower()}
                ::{label}::'''


@pytest.fixture(scope='module')
def evidence():
    unit = json.loads((ROOT / 'refs/script_recovery/bordello/units/V_Bordello.json').read_text())
    converter = UnitConverter(ROOT / 'refs/script_recovery/bordello/translation_unit_typed.json')
    fn = converter.native('0x00e40e80')
    row = unit['reviewedPresentedItemNames']['0x00e40e80']
    body = ('\n'.join(block(site['label']) for site in row['sites'])).split('\n')
    return fn, row, body, converter.rdata


def test_reviewed_item_poll(evidence):
    fn, row, body, rdata = evidence
    changed, todo = restore(body, [DIAGNOSTIC, DIAGNOSTIC], fn, row, rdata)
    source = '\n'.join(changed)
    assert source.count('g_PresentedItemName = nil') == 2
    assert source.count('g_PresentedItemName == "OBJECT_BEER_TANKARD"') == 2
    assert 'unaff_EBP' not in source and todo == []
    assert restore(body, [DIAGNOSTIC, DIAGNOSTIC], fn, None, rdata)[0] == body


@pytest.mark.parametrize('mutation', [
    lambda row: row.__setitem__('nativeSha256', '0' * 64),
    lambda row: row.__setitem__('sourceSha256', '0' * 64),
    lambda row: row.__setitem__('sidecarSha256', '0' * 64),
    lambda row: row['nativeInstructions'].__setitem__('0x00E42048', '00'),
    lambda row: row['sites'][1].__setitem__('label', 'LAB_00e42094'),
])
def test_stale_item_evidence_fails(evidence, mutation):
    fn, row, body, rdata = evidence
    changed = deepcopy(row)
    mutation(changed)
    with pytest.raises(ValueError, match='reviewed presented item'):
        restore(body, [DIAGNOSTIC, DIAGNOSTIC], fn, changed, rdata)
