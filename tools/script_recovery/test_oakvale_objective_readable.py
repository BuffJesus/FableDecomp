import tempfile
import unittest
from pathlib import Path
from lupa.lua54 import LuaRuntime
from tools.script_recovery.build_readable_new_oakvale import build,RAW
from tools.script_recovery.native_oakvale_objective import lower


class OakvaleObjectiveReadableTests(unittest.TestCase):
    def test_emitted_startup_objective_order_and_cancellation(self):
        with tempfile.TemporaryDirectory() as directory:
            report=build(Path(directory))
            source=(Path(directory)/'FSE/NewOakValeIntro/NewOakValeIntro.lua').read_text()
            self.assertTrue(report['syntax']['ok'])
            self.assertEqual(report['initialObjective']['method'],'SetInitialOakvaleObjective')
            for attack,terminate in ((False,False),(True,False),(True,True)):
                lua=LuaRuntime();lua.execute(source);lua.globals().attack=attack;lua.globals().terminate=terminate
                lua.execute('''
                    local events,bindings={},{}
                    local function event(name)events[#events+1]=name end
                    local resources={SetInitialOakvaleObjective=function()event('objective')end}
                    local q={
                        AddEntityBinding=function(_,name,path)assert(path=='NewOakValeIntro/Entities/'..name);bindings[#bindings+1]=name end,
                        FinalizeEntityBindings=function()assert(#bindings==16 and bindings[16]=='OVI_DeadFather');event('finalize')end,
                        GetStateBool=function(_,name)assert(name=='AttackOver');event('state');return attack end,
                        IsActiveThreadTerminating=function()event('term');return terminate end,
                        DeactivateQuest=function(_,name,arg)assert(name=='Q__OakValeIntro_PostAttack' and arg==0);event('deactivate')end,
                        WithRetailResources=function(_,body)event('scope');body(resources);event('close')end,
                        CreateThread=function(_,name)assert(name=='StartBarrelTimer');event('thread')end,
                    }
                    DoMission=function()event('mission')end
                    Main(q)
                    local expected=attack and (terminate and 'finalize,state,term' or 'finalize,state,term,deactivate,scope,objective,close,thread,mission')
                        or 'finalize,state,scope,objective,close,thread,mission'
                    assert(table.concat(events,',')==expected)
                ''')

    def test_changed_source_rejected(self):
        source=(RAW/'FSE/NewOakValeIntro/NewOakValeIntro.lua').read_text()
        with self.assertRaises(ValueError):lower(source.replace('local ppVar3 = quest:GetActiveQuestName()', 'local ppVar3 = "invented"'))


if __name__=='__main__':unittest.main()
