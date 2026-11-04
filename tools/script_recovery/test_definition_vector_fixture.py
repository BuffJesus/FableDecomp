import copy
import json
from pathlib import Path

import pytest
from lupa.lua54 import LuaRuntime

from tools.script_recovery.definition_vector_fixture import decode_vectors, stock_fixture, to_lua, validate, load_fixture
from tools.script_recovery.script_def_offsets import load
from tools.script_recovery.smoke_run_unit import HARNESS

ROOT = Path(__file__).resolve().parents[2]


@pytest.fixture(scope='module')
def stock():
    return stock_fixture(), load()


def execute(source, stock, enabled=True, entity=False):
    fixture, metadata = stock
    lua = LuaRuntime(unpack_returned_tuples=True)
    runner = lua.execute('return function(SOURCE, NAME, FUNCTIONS, FRAME_BUDGET, INSTRUCTION_LIMIT, PARAM_KINDS, ENTITY_FILE, DEFINITION_FIXTURE) ' + HARNESS + ' end')
    return runner(source, 'fixture_test.lua', lua.table('Main'), 4, 1000, lua.table(), entity,
                  to_lua(lua, fixture, metadata) if enabled else None)['Main']


def test_stock_vectors_and_empty_vector_are_preserved(stock):
    fixture, metadata = stock
    assert len(fixture['vectors']) == 12
    vectors = {row['name']: row for row in fixture['vectors']}
    assert vectors['ArcheryNormalPrizes']['items'] == []
    assert vectors['SickChildsSisterAttentionAnims']['items'] == ['ST_TELLING_YOU_TO_COME', 'STANDARD_WAVE_INFO', 'STANDARD_WAVE']
    assert len(vectors['AssassinScreamSoundIds']['items']) == 4
    validate(fixture, metadata)


@pytest.mark.parametrize('fault', ['short', 'truncated', 'trailing', 'huge', 'invalid_hex', 'unresolved', 'unanchored'])
def test_decoder_rejects_incomplete_evidence(fault):
    field = dict(name='Vector', defType='Vector_VCDefString__', retailTransferCall='0x123', value='0100000001000000')
    names = {1: 'FIRST'}
    if fault == 'short': field['value'] = '0100'
    if fault == 'truncated': field['value'] = '0200000001000000'
    if fault == 'trailing': field['value'] += '00000000'
    if fault == 'huge': field['value'] = 'ffffffff'
    if fault == 'invalid_hex': field['value'] = 'gg'
    if fault == 'unresolved': names = {}
    if fault == 'unanchored': field.pop('retailTransferCall')
    with pytest.raises(ValueError): decode_vectors({'0x100': field}, names)


@pytest.mark.parametrize('fault', ['duplicate', 'wrong_name', 'scalar', 'unaligned', 'bool_offset', 'wrong_item', 'unknown_schema'])
def test_fixture_validation_rejects_ambiguous_shapes(stock, fault):
    original, metadata = stock
    fixture = copy.deepcopy(original)
    row = fixture['vectors'][0]
    if fault == 'duplicate': fixture['vectors'].append(copy.deepcopy(row))
    if fault == 'wrong_name': row['name'] = 'InventedName'
    if fault == 'scalar': row['offset'] = 0x168
    if fault == 'unaligned': row['offset'] += 1
    if fault == 'bool_offset': row['offset'] = True
    if fault == 'wrong_item': row['items'] = [123]
    if fault == 'unknown_schema': fixture['schema'] = 'unknown'
    with pytest.raises(ValueError): validate(fixture, metadata)


def test_readonly_vector_shape_and_zero_based_index(stock):
    result = execute('''function Main(q)
        local first, last, capacity = q:ReadGlobalGameData(0x74c), q:ReadGlobalGameData(0x750), q:ReadGlobalGameData(0x754)
        assert(first > 0 and last-first == 12 and capacity == last)
        assert(q:ReadGlobalGameDataDefStringAt(0x74c, 0) == "ST_TELLING_YOU_TO_COME")
        assert(q:ReadGlobalGameDataDefStringAt(0x74c, 2) == "STANDARD_WAVE")
        assert(q:ReadGlobalGameData(0x168) == 1)
    end''', stock)
    assert result['ok'], result['error']


@pytest.mark.parametrize('index', ['-1', '3', '1.5', '"1"', 'nil', '0/0'])
def test_invalid_indices_remain_errors(stock, index):
    result = execute('function Main(q) q:ReadGlobalGameDataDefStringAt(0x74c, ' + index + ') end', stock)
    assert not result['ok'] and 'index out of bounds' in result['error']


def test_unknown_vector_remains_error(stock):
    result = execute('function Main(q) q:ReadGlobalGameDataDefStringAt(0x750,0) end', stock)
    assert not result['ok'] and 'unknown vector offset' in result['error']


@pytest.mark.parametrize('enabled', [False, True])
def test_empty_vector_is_never_clamped(stock, enabled):
    result = execute('''function Main(q)
        local n = (q:ReadGlobalGameData(0x310)-q:ReadGlobalGameData(0x30c)) >> 2
        q:ReadGlobalGameDataDefStringAt(0x30c, 1 % n)
    end''', stock, enabled)
    assert not result['ok'] and "n%0" in result['error']


@pytest.mark.parametrize('package,relative', [
    ('SickChild', 'V_SickChild/Entities/SickChildsSister.lua'),
    ('TraderEscort', 'TraderEscort/Entities/DarkwoodAssassinSpawn.lua')])
def test_actual_generated_main_progresses_past_vector_access(stock, package, relative):
    source = (ROOT / 'refs/script_recovery/lifted' / package / 'readable/FSE' / relative).read_text()
    before = execute(source, stock, enabled=False, entity=True)
    assert not before['ok'] and 'n%0' in before['error']
    after = execute(source, stock, enabled=True, entity=True)
    assert after['ok'], after['error']
    assert 'quest:ReadGlobalGameDataDefStringAt' in list(after['trace'].values())


def test_source_pin_mismatch_is_rejected(stock, tmp_path):
    fixture, _ = stock
    fixture = copy.deepcopy(fixture)
    name = next(iter(fixture['sourceHashes']))
    fixture['sourceHashes'][name] = '0' * 64
    path = tmp_path / 'fixture.json'
    path.write_text(json.dumps(fixture))
    with pytest.raises(ValueError, match='source changed'): load_fixture(path)
