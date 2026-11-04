import copy
import hashlib
import json
from pathlib import Path

import pytest

from tools.script_recovery.convert_quest_unit import UnitConverter
from tools.script_recovery.native_reviewed_named_resources import reviewed_resources


@pytest.fixture(scope='module')
def marker():
    root = Path(__file__).resolve().parents[2]
    converter = UnitConverter(root / 'refs/script_recovery/bandit_camp/translation_unit_typed.json')
    fn = converter.native('0x00d11930')
    unit = json.loads((root / 'refs/script_recovery/bandit_camp/units/Q_BanditCampBossBattle.json').read_text())
    row = copy.deepcopy(unit['reviewedNamedResourceReceivers']['0x00d11930'])
    # Minimal lowered-source fixture; native bytes/call witnesses remain actual retail evidence.
    source = '\n'.join(
        f'GSI->GetThingWithScriptName((CScriptThing *)actor{index}, "actor{index}");\n'
        f'{resource["object"]} = RESOURCE_NewResource();\n'
        f'RESOURCE_TryAcquire({resource["object"]}, (CScriptThing *)actor{index}, 4);\n'
        f'CScriptGameResourceObjectScriptedThingBase::_Speak_CScriptGameResourceObjectScriptedThingBase('
        f'(CScriptGameResourceObjectScriptedThingBase *){resource["object"]}, hero, "LINE", 0, 0, 1, 0);'
        for index, resource in enumerate(row['resources']))
    row['loweredSha256'] = hashlib.sha256(source.encode()).hexdigest()
    return source, fn, row, converter.rdata


@pytest.mark.parametrize('newline', ['\n', '\r\n'])
def test_native_receivers_match_reviewed_stack_objects(marker, newline):
    source, fn, row, rdata = marker
    assert reviewed_resources(source.replace('\n', newline), fn, row, rdata) == {'xStack_a4', 'xStack_84', 'xStack_94'}


def test_absent_review_leaves_dispatch_unchanged(marker):
    source, fn, _, rdata = marker
    assert reviewed_resources(source, fn, None, rdata) == set()


@pytest.mark.parametrize('kind', ['body', 'source', 'lowered', 'frame', 'slot', 'acquire', 'speech', 'task', 'missing', 'helper', 'anchor', 'duplicate'])
def test_evidence_drift_is_rejected(marker, kind):
    source, fn, row, rdata = marker
    bad = copy.deepcopy(row)
    if kind == 'body': bad['nativeSha256'] = '0' * 64
    elif kind == 'source': bad['sourceSha256'] = '0' * 64
    elif kind == 'lowered': bad['loweredSha256'] = '0' * 64
    elif kind == 'frame': bad['resources'][0]['frameBytes'] += 4
    elif kind == 'slot': bad['resources'][0]['entrySlot'] += 4
    elif kind == 'acquire': bad['resources'][0]['acquireSite'] = bad['resources'][1]['acquireSite']
    elif kind == 'speech': bad['resources'][0]['speakSites'] = bad['resources'][1]['speakSites']
    elif kind == 'task': bad['resources'][0]['taskSites'] = bad['resources'][1]['taskSites']
    elif kind == 'missing': bad['resources'][0]['taskSites'].pop()
    elif kind == 'helper': bad['helperBytes']['0x7e7390'] = '00' * 15
    elif kind == 'anchor': bad['nativeInstructions'][next(iter(bad['nativeInstructions']))] = '00'
    else: bad['resources'].append(bad['resources'][0])
    with pytest.raises(ValueError):
        reviewed_resources(source, fn, bad, rdata)
