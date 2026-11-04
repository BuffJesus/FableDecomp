"""Pin BordelloLady's explicit resource acquire and cleanup receivers."""
from copy import deepcopy
import json
from pathlib import Path

import pytest
from lupa.lua54 import LuaRuntime

from tools.script_recovery.convert_quest_unit import UnitConverter
from tools.script_recovery.native_reviewed_bordello_lady_resource_lifetime import restore


ROOT = Path(__file__).resolve().parents[2]
SOURCE = ROOT / 'work/codex_lua_marathon64_20260930/baseline/BordelloLady_pre.lua'


@pytest.fixture(scope='module')
def evidence():
    unit = json.loads((ROOT / 'refs/script_recovery/bordello/units/V_Bordello.json').read_text())
    converter = UnitConverter(ROOT / 'refs/script_recovery/bordello/translation_unit_typed.json')
    return (converter.native('0x00e3eb10'),
            unit['reviewedBordelloLadyResourceLifetime']['0x00e3eb10'], converter.rdata)


def test_all_acquires_and_local_cleanup_use_constructed_resource(evidence):
    fn, row, rdata = evidence
    source = SOURCE.read_text()
    changed = restore(source, fn, row, rdata)
    assert changed.count('resources:TryAcquire(ladyResource, me, 4)') == 6
    assert changed.count('resources:PrepareResource(ladyResource)') == 6
    assert changed.count('resources:ReleaseResource(ladyResource)') == 1
    assert changed.count('resources:DestroyMovie(xStack_190)') >= 1
    assert 'me:AcquireControl(' not in changed
    assert restore(source, fn, None, rdata) == source
    LuaRuntime().compile(changed)


@pytest.mark.parametrize('mutation', [
    lambda row: row.__setitem__('nativeSha256', '0' * 64),
    lambda row: row.__setitem__('sourceSha256', '0' * 64),
    lambda row: row['acquireSites'].__setitem__(0, '0x00E3ED9B'),
    lambda row: row['localResourceCalls'].__setitem__('0x00E402F4', '0x00000000'),
    lambda row: row.__setitem__('movieDestructorSite', '0x00E4007F'),
])
def test_stale_evidence_rejected(evidence, mutation):
    fn, row, rdata = evidence
    changed = deepcopy(row)
    mutation(changed)
    with pytest.raises(ValueError, match='reviewed BordelloLady resource lifetime'):
        restore(SOURCE.read_text(), fn, changed, rdata)


def test_changed_lifted_resource_use_rejected(evidence):
    fn, row, rdata = evidence
    source = SOURCE.read_text().replace('me:AcquireControl(8)', 'me:AcquireControl(4)')
    with pytest.raises(ValueError, match='lifted resource use changed'):
        restore(source, fn, row, rdata)


def test_early_movie_destructor_evidence_rejected(evidence):
    fn, row, rdata = evidence
    changed = deepcopy(fn)
    site = next(c for c in changed['calls'] if c['site'].lower() == '0x00e3f73b')
    site['ecxStack'] += 4
    with pytest.raises(ValueError, match='early movie destructor changed'):
        restore(SOURCE.read_text(), changed, row, rdata)


def test_changed_early_movie_cleanup_rejected(evidence):
    fn, row, rdata = evidence
    source = SOURCE.read_text().replace('DestroyMovie("TEXT_OBJECT_HERO_ANSWER_YES")',
                                      'DestroyMovie("different")')
    with pytest.raises(ValueError, match='lifted early movie cleanup changed'):
        restore(source, fn, row, rdata)


def test_interrupted_conversation_releases_movie_handle(evidence):
    fn, row, rdata = evidence
    changed = restore(SOURCE.read_text(), fn, row, rdata)
    lua = LuaRuntime()
    lua.execute('''
        package.preload["V_Bordello.native_quest_helpers"] = function() return {} end
        local interrupted = false
        local movie, resource = {}, {}
        destroyed, released = 0, 0
        local noop = function() end
        resources = setmetatable({}, {__index = function() return noop end})
        function resources:NewResource() return resource end
        function resources:StartMovie() return movie end
        function resources:TryAcquire(id) assert(id == resource); return true end
        function resources:DestroyMovie(id)
            assert(id == movie, "wrong movie handle " .. tostring(id))
            destroyed = destroyed + 1
        end
        function resources:ReleaseResource(id)
            assert(id == resource); released = released + 1
        end
        quest = setmetatable({}, {__index = function() return noop end})
        function quest:RetailResources() return resources end
        function quest:NewScriptFrame() return true end
        function quest:IsActiveThreadTerminating() return interrupted end
        function quest:GetStateBool() return false end
        me = setmetatable({}, {__index = function() return noop end})
        function me:IsTalkedToByHero() return true end
        function me:ClearCommands() interrupted = true end
    ''')
    lua.execute(changed)
    lua.globals().Main(lua.globals().quest, lua.globals().me)
    assert lua.globals().destroyed == 1
    assert lua.globals().released == 1
