import copy
import hashlib
import json
from pathlib import Path

import pytest

from tools.script_recovery.convert_quest_unit import UnitConverter
from tools.script_recovery.native_reviewed_resource_constructors import restore


@pytest.fixture(scope='module')
def guard():
    root = Path(__file__).resolve().parents[2]
    converter = UnitConverter(root / 'refs/script_recovery/arena/translation_unit_typed.json')
    fn = converter.native('0x00f17c70')
    unit = json.loads((root / 'refs/script_recovery/arena/units/Q_Arena.json').read_text())
    row = copy.deepcopy(unit['reviewedInlinedResourceConstructors']['0x00f17c70'])
    source = ('  ppuVar13 = RESOURCE_NewResource();\n'
              '  RESOURCE_PrepareResource(xStack_238);\n'
              '  bVar3 = RESOURCE_TryAcquire(xStack_238, me, 4);\n'
              '  CPhysicsMeshInfo::~CPhysicsMeshInfo((CPhysicsMeshInfo *)&xStack_238);\n')
    # The minimal source fixture uses the actual retail body, calls and witnesses.
    row['loweredSha256'] = hashlib.sha256(source.encode()).hexdigest()
    return source, fn, row, converter.rdata


@pytest.mark.parametrize('newline', ['\n', '\r\n'])
def test_resource_storage_and_early_destructor_are_recovered(guard, newline):
    source, fn, row, rdata = guard
    result = restore(source.replace('\n', newline), fn, row, rdata)
    assert result == ('  xStack_238 = RESOURCE_NewResource();\n'
                      '  ppuVar13 = xStack_238;\n'
                      '  RESOURCE_PrepareResource(xStack_238);\n'
                      '  bVar3 = RESOURCE_TryAcquire(xStack_238, me, 4);\n'
                      '  RESOURCE_ReleaseResource(xStack_238);\n')


def test_unreviewed_function_is_unchanged(guard):
    source, fn, _, rdata = guard
    assert restore(source, fn, None, rdata) == source


@pytest.mark.parametrize('kind', [
    'body', 'source', 'lowered', 'frame', 'slot', 'alias', 'handle', 'constructor',
    'receiver', 'vtable', 'zeros', 'depth', 'acquire', 'acquire_receiver',
    'destructor', 'destructor_source', 'helper', 'anchor', 'missing',
])
def test_native_or_source_drift_is_rejected(guard, kind):
    source, fn, row, rdata = guard
    bad = copy.deepcopy(row)
    if kind == 'body': bad['nativeSha256'] = '0' * 64
    elif kind == 'source': bad['sourceSha256'] = '0' * 64
    elif kind == 'lowered': bad['loweredSha256'] = '0' * 64
    elif kind == 'frame': bad['frameBytes'] += 4
    elif kind == 'slot': bad['entrySlot'] += 4
    elif kind == 'alias': bad['sourceHandle'] = 'xStack_238'
    elif kind == 'handle': bad['resourceHandle'] = 'xStack_234'
    elif kind == 'constructor': bad['constructor']['call'] = bad['earlyDestructor']['call']
    elif kind == 'receiver': bad['constructor']['lea'] = bad['constructor']['call']
    elif kind == 'vtable': bad['constructor']['vtable'] = bad['earlyDestructor']['vtable']
    elif kind == 'zeros': bad['constructor']['zeros'].reverse()
    elif kind == 'depth': bad['constructor']['vtableDepth'] += 4
    elif kind == 'acquire': bad['acquireSites'].pop()
    elif kind == 'acquire_receiver': bad['acquireReceivers'][0]['lea'] = int(bad['constructor']['lea'], 16)
    elif kind == 'destructor': bad['earlyDestructor']['call'] = bad['constructor']['call']
    elif kind == 'destructor_source': bad['earlyDestructor']['sourceCall'] = 'RESOURCE_PrepareResource(xStack_238);'
    elif kind == 'helper': bad['helperBytes']['0x99a430'] = '00' * 32
    elif kind == 'anchor': bad['nativeInstructions'][bad['constructor']['vtable']] = '00'
    else: del bad['nativeInstructions'][bad['constructor']['lea']]
    with pytest.raises(ValueError):
        restore(source, fn, bad, rdata)


@pytest.mark.parametrize('replacement', [
    '  ppuVar13 = 0;',
    '  xStack_238 = RESOURCE_NewResource();',
    '  ppuVar13 = RESOURCE_NewResource();\n  xStack_238 = 0;',
    '  ppuVar13 = RESOURCE_NewResource();\n  ppuVar13 = RESOURCE_NewResource();',
])
def test_ambiguous_constructor_source_is_rejected(guard, replacement):
    source, fn, row, rdata = guard
    changed = source.replace('  ppuVar13 = RESOURCE_NewResource();', replacement)
    updated = dict(row, loweredSha256=hashlib.sha256(changed.encode()).hexdigest())
    with pytest.raises(ValueError):
        restore(changed, fn, updated, rdata)
