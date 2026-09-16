"""Compose separately native-verified phases; check Lua cleanup/error boundaries."""
import re
import unittest
from pathlib import Path
from lupa.lua54 import LuaRuntime


class TheresaAcceptanceTests(unittest.TestCase):
    def test_composition_and_errors_always_close_guard_vector_after_actor_map(self):
        folder = Path(__file__).parent
        body = '\n'.join(re.sub(r'\nreturn \w+\s*$', '\n', (folder/name).read_text()) for name in
                         ('theresa_cutscene_actors.lua','theresa_gift_commit.lua','theresa_accept_chocolates.lua'))
        for fail in ('', 'remove', 'map', 'bind', 'macro', 'update', 'clear', 'map-close', 'guards-close'):
            lua = LuaRuntime()
            phase = lua.execute(body+'\nreturn acceptTheresaChocolates')
            state = lua.execute('''
                local fail = ...
                local events, progress = {}, {givenChocolates=false}
                local function record(name)
                    table.insert(events,name)
                    if name==fail then error(name..' failed') end
                end
                local guards = {
                    RemoveLivingGuards=function() record('remove') end,
                    Close=function() record('guards-close') end
                }
                local resources = {
                    NewTheresaGuardVector=function() record('guards');return guards end,
                    NewActorMap=function() record('map');return 9 end,
                    SetActor=function(_,map,key,resource)
                        assert(map==9)
                        assert((key=='HERO' and resource==344) or (key=='THER' and resource==24))
                        record('bind')
                    end,
                    RunMacro=function(_,name,map,setup,skip)
                        assert(name=='CS_OAKVALE_INTRO_THERESA_MEET_YES' and map==9 and setup==false and skip==true)
                        record('macro')
                    end,
                    TakeTheresaChocolatesAndUpdateObjective=function() record('update') end,
                    ClearTheresaInformation=function(_,actor)
                        assert(actor==17 and progress.givenChocolates)
                        record('clear')
                    end,
                    DestroyActorMap=function(_,map) assert(map==9);record('map-close') end
                }
                local quest = {SetStateBool=function(_,key,value)
                    assert(key=='GivenTheresaChocs' and value==true);record('state')
                end}
                return {resources=resources,quest=quest,progress=progress,events=events}
            ''', fail)
            if fail:
                with self.assertRaisesRegex(Exception, fail+' failed'):
                    phase(state.quest,17,state.resources,344,24,state.progress)
            else: phase(state.quest,17,state.resources,344,24,state.progress)
            events = list(state.events.values())
            self.assertEqual(events[-1], 'guards-close')
            if fail not in ('remove','map'): self.assertEqual(events[-2], 'map-close')
            if not fail:
                self.assertEqual(events, ['guards','remove','map','bind','bind','macro','state','update','clear','map-close','guards-close'])
