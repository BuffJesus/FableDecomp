import tempfile
import unittest
from pathlib import Path
from lupa.lua54 import LuaRuntime
from tools.script_recovery.build_readable_new_oakvale import build


class DeadFatherReadableTests(unittest.TestCase):
    def test_emitted_entry_lifecycle_and_predicate_override(self):
        with tempfile.TemporaryDirectory() as directory:
            report=build(Path(directory))
            text=(Path(directory)/'FSE/NewOakValeIntro/Entities/OVI_DeadFather.lua').read_text()
            self.assertTrue(report['syntax']['ok']);self.assertFalse(report['deadFatherCandidate']['enabled'])
            self.assertEqual(text.count('quest:RegisterBoundAliveCondition()'),1)
            for token in ('TODO','LAB_','goto ','pCVar','fVar','RegisterBoundConsciousCondition'):self.assertNotIn(token,text)
            rows={r['function']:r for r in report['functions'] if r['owner']=='OVI_DeadFather'}
            for name in ('Init','Main','OnPredicateFail'):
                self.assertEqual(rows[name]['implementationFunction'],'DeadFather'+name)
            for cancel in range(1,5):
                lua=LuaRuntime();lua.execute(text);lua.globals().cancel=cancel
                lua.execute('''
                    local events,terms,owned={},0,false
                    local function event(name) events[#events+1]=name end
                    local resources={
                        InitializeDeadFatherActor=function(_,me) assert(me==17);event('init') end,
                        NewResource=function() owned=true;event('new');return 16 end,
                        PrepareResource=function(_,id) assert(id==16);event('prepare') end,
                        TryAcquire=function(_,id,me,priority) assert(id==16 and me==17 and priority==4);event('acquire');return true end,
                        PlaceDeadFatherAtMarker=function(_,me) assert(me==17);event('place') end,
                        PlayDeadFatherPose=function(_,id) assert(id==16);event('pose') end,
                        RemoveDeadFatherMarker=function(_,me) assert(me==17);event('remove') end,
                    }
                    local q={
                        WithRetailResources=function(_,fn)
                            fn(resources);if owned then event('close');owned=false end
                        end,
                        RegisterBoundAliveCondition=function(_,...) assert(select('#',...)==0);event('condition') end,
                        NewScriptFrame=function() event('frame') end,
                        IsActiveThreadTerminating=function() terms=terms+1;event('term');return terms==cancel end,
                        GetStateBool=function(_,key) assert(key=='DadFound');event('found');return true end,
                    }
                    Init(q,17);Main(q,17)
                    local expected={
                        'init,condition,frame,term',
                        'init,condition,frame,term,new,prepare,acquire,term,close',
                        'init,condition,frame,term,new,prepare,acquire,term,place,pose,found,term,close',
                        'init,condition,frame,term,new,prepare,acquire,term,place,pose,found,term,remove,frame,term,close',
                    }
                    assert(table.concat(events,',')==expected[cancel] and not owned)
                    local n=#events;OnPredicateFail(q,17);assert(#events==n)
                ''')
