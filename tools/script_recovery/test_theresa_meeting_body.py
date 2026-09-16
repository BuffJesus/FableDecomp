"""Meeting composition; native phase and caller-ownership checks are separate."""
import itertools
import re
import unittest
from pathlib import Path
from lupa.lua54 import LuaRuntime


class TheresaMeetingTests(unittest.TestCase):
    def test_meeting_with_actual_question_and_acceptance_helpers(self):
        folder = Path(__file__).parent
        names = ('theresa_cutscene_actors.lua', 'theresa_chocolate_question.lua', 'theresa_gift_commit.lua',
                 'theresa_accept_chocolates.lua', 'theresa_meeting_body.lua')
        body = '\n'.join(re.sub(r'\nreturn \w+\s*$', '\n', (folder/name).read_text()) for name in names)
        for has, acquired, answer, cancel in itertools.product((False, True), (False, True), (0,1,2), range(1,9)):
            lua = LuaRuntime()
            phase = lua.execute(body+'\nreturn meetTheresa')
            state = lua.execute('''
                local has, acquired, answer, cancel = ...
                local events, live, progress, state = {}, {}, {givenChocolates=false}, {}
                local id, terms, polls = 0, 0, 0
                local function new(kind)
                    id=id+1;live[id]=kind;table.insert(events,'new:'..kind);return id
                end
                local function close(id,kind)
                    assert(live[id]==kind);live[id]=nil;table.insert(events,'close:'..kind)
                end
                local q = {
                    IsActiveThreadTerminating=function()
                        terms=terms+1;return terms==cancel
                    end,
                    FixMovieSequenceCamera=function(_,flag) table.insert(events,flag and 'camera:on' or 'camera:off') end,
                    MsgIsQuestionAnsweredYesOrNo=function() polls=polls+1;return polls==1 and -1 or answer end,
                    NewScriptFrame=function() table.insert(events,'frame') end,
                    SetStateBool=function(_,key,value) assert(key=='GivenTheresaChocs' and value);table.insert(events,'given') end
                }
                state.SetStateBool=function(_,key,value) assert(key=='DoneIntro' and value);state.done=true;table.insert(events,'done') end
                local r = {
                    NewResource=function() return new('hero') end,
                    TryAcquireTheresaHero=function(_,hero,priority)
                        assert(live[hero]=='hero' and priority==4);table.insert(events,'acquire');return acquired
                    end,
                    NewActorMap=function() return new('map') end,
                    SetActor=function(_,map,key,control)
                        assert(live[map]=='map');assert((key=='HERO' and live[control]=='hero') or (key=='THER' and control==24))
                    end,
                    StartMovie=function(_,name) assert(name=='');return new('movie') end,
                    Pause=function(_,flag) table.insert(events,flag and 'pause:on' or 'pause:off') end,
                    RunMacro=function(_,name,map,setup,skip)
                        assert(live[map]=='map' and not setup and skip);table.insert(events,name)
                    end,
                    DoesTheresaHeroHaveChocolates=function() return has end,
                    ShowTheresaChocolateQuestion=function() table.insert(events,'question') end,
                    TakeTheresaChocolatesAndUpdateObjective=function() table.insert(events,'update') end,
                    ClearTheresaInformation=function() assert(progress.givenChocolates);table.insert(events,'clear') end,
                    NewTheresaGuardVector=function()
                        local vector = new('guards')
                        return {RemoveLivingGuards=function() table.insert(events,'remove') end,
                                Close=function() close(vector,'guards') end}
                    end,
                    DestroyActorMap=function(_,id) close(id,'map') end,
                    DestroyMovie=function(_,id) close(id,'movie') end,
                    ReleaseResource=function(_,id) close(id,'hero') end
                }
                return {q=q,r=r,state=state,progress=progress,events=events,live=live}
            ''', has, acquired, answer, cancel)
            result = phase(state.q,17,state.r,24,state.state,state.progress)
            events = list(state.events.values())
            self.assertEqual(list(state.live.values()), [])
            expected_cancel = cancel <= 2 or (has and cancel <= (6 if answer==1 else 5))
            self.assertEqual(result, not expected_cancel)
            self.assertEqual(bool(state.state.done), not expected_cancel)
            self.assertEqual(state.progress.givenChocolates, has and answer==1 and not expected_cancel)
            if cancel > 2:
                self.assertEqual(events.count('acquire'), 1)
                self.assertEqual(events[-4:], ['pause:off','close:movie','close:map','close:hero'])
                self.assertEqual('camera:off' in events, not expected_cancel)
