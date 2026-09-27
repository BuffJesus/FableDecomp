"""BookReaction's indexed markers and by-value actors must survive generation."""
import json
import subprocess
import sys
import tempfile
from pathlib import Path

import pytest
from lupa.lua54 import LuaRuntime

from tools.script_recovery.build_readable_unit import build
from tools.script_recovery.native_evidence_lowering import fold_by_value_things, lower_global_definition_strings
from tools.script_recovery.quest_unit_evidence import thread_names

ROOT = Path(__file__).resolve().parents[2]
EVIDENCE = ROOT / 'refs/script_recovery/book_collecting'


@pytest.fixture(scope='module')
def generated():
    scratch = tempfile.TemporaryDirectory(prefix='book_reaction_', dir=ROOT / 'work')
    out = Path(scratch.name)
    assert out.resolve().is_relative_to((ROOT / 'work').resolve())
    unit = json.loads((EVIDENCE / 'units/V_BookCollecting.json').read_text())
    # Keep native Unicorn state out of the pytest process (Windows ctypes can
    # report an access violation while mapping the retail image under pytest).
    subprocess.run([sys.executable, '-m', 'tools.script_recovery.convert_quest_unit',
                    '--unit', 'book_collecting', '--out', str(out / 'draft')],
                   cwd=ROOT, check=True, capture_output=True, text=True)
    build('book_collecting', draft=out / 'draft', out=out / 'readable')
    inventory = json.loads((EVIDENCE / 'inventory.json').read_text())
    name = thread_names(inventory['threads'])['0x00e566f0']
    assert unit['quest']['functions'][name]['address'] == '0x00e566f0'
    yield name, [(out / stage / 'FSE/V_BookCollecting/V_BookCollecting.lua').read_text()
                 for stage in ('draft', 'readable')]
    scratch.cleanup()


@pytest.mark.parametrize('lines,reading,terminate', [(0, False, False), (2, False, False),
                                                   (2, True, False), (2, False, True)])
def test_generated_worker_keeps_markers_actor_values_and_exit_order(generated, lines, reading, terminate):
    name, sources = generated
    for source in sources:
        lua = LuaRuntime(unpack_returned_tuples=True)
        lua.execute(source)
        events = []
        def record(kind):
            return lambda _self, *args: events.append((kind, *args))
        resources = lua.table_from({
            'MemberResource': lambda _self, key: key,
            **{method: record(method) for method in ('TryAcquire', 'ClearAllActionsIncludingLoopingAnimations',
                                                    'ClearCommands', 'PrepareResource')},
        })
        def marker(_self, offset, index):
            events.append(('marker', offset, index))
            return f'marker-{offset}-{index}'
        quest = lua.table_from({
            'RetailResources': lambda _: resources,
            'GlobalConversations': lambda _self, offset: lua.table_from({4: lua.table_from({'Lines': lines})}),
            'ReadGlobalGameDataStringAt': marker,
            'GetThingWithScriptName': lambda _self, value: value,
            'EntityTeleportToThing': record('teleport'),
            'SetIsPushableByHero': record('pushable'),
            'GetStateBool': lambda *_: reading,
            'IsActiveThreadTerminating': lambda _: terminate,
            'NewScriptFrame': lambda _: not terminate,
        })
        lua.globals().helper_E569D0 = record('conversation')
        lua.globals().DoConversation = record('conversation')
        lua.globals()[name](quest, 3)
        if not lines:
            assert events == []
            continue
        assert [e for e in events if e[0] == 'marker'] == [('marker', 0x4b0, 3), ('marker', 0x4bc, 3)]
        assert [e for e in events if e[0] == 'teleport'] == [
            ('teleport', 'boy0', f'marker-{0x4b0}-3', False),
            ('teleport', 'girl0', f'marker-{0x4bc}-3', False)]
        expected = [('pushable', 'boy0', False), ('pushable', 'girl0', False)]
        if not terminate:
            expected += [('pushable', 'boy0', True), ('pushable', 'girl0', True)]
        assert [e for e in events if e[0] == 'pushable'] == expected
        assert [e for e in events if e[0] == 'conversation'] == (
            [('conversation', 3, 0), ('conversation', 3, 1)] if not reading and not terminate else [])


def test_sliced_copy_requires_matching_data_info_retain_and_vtable():
    source = ('if ((int *)actor._8_4_ != (int *)0x0) {\n'
              '  *(int *)actor._8_4_ = *(int *)actor._8_4_ + 1;\n}\n'
              'copy._4_4_ = actor._4_4_;\n'
              'copy._0_4_ = &PTR__scalar_deleting_destructor__01238c8c;\n'
              'copy._8_4_ = actor._8_4_;')
    assert fold_by_value_things(source) == 'copy = actor;'
    for bad in (source.replace('copy._8_4_ = actor', 'copy._8_4_ = other'),
                source.replace('+ 1;', '+ 2;'), source.replace('01238c8c', '01238c90')):
        assert fold_by_value_things(bad) == bad


def test_marker_read_requires_string_type_global_base_and_word_stride():
    source = '(CCharString_bv *)(*(int *)(DAT_0143e90c + 0x4b0) + index * 4)'
    assert lower_global_definition_strings(source, {}) == 'ENGINE_GlobalGameDataStringAt(0x4b0, index)'
    for bad in (source.replace('CCharString_bv', 'CScriptThing_bv'),
                source.replace('DAT_0143e90c', 'unknown'), source.replace('* 4', '* 8')):
        assert lower_global_definition_strings(bad, {}) == bad
