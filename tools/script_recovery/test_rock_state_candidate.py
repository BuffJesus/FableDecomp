import unittest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.rock_state_candidate import generate,verify
from tools.script_recovery.rock_state_program import FIELDS,functions
from tools.script_recovery.rock_state_native import execute
from tools.script_recovery.lift_native_lua import ROOT,RData


def lua_run(role,initial=None,mode='save',saved=None):
    lua=LuaRuntime();lua.execute(functions());quest=lua.table()
    state={name:(initial or {}).get(name,123 if kind=='Int' else True) for _,name,kind in FIELDS}
    events=[];storage=dict(saved or {})
    for kind in ('Bool','Int'):
        quest['GetState'+kind]=lambda _,name:state[name]
        quest['SetState'+kind]=lambda _,name,value:state.__setitem__(name,value)
    quest.SetCreatureGeneratorsEnabled=lambda _,*args:events.append(('generators',*args))
    def transfer(_,context,name,current,default,kind):
        self_context='context';assert context==self_context
        events.append(('transfer',name,kind,current,default))
        if mode=='save':storage[name]=current;return current
        return storage.get(name,default) if mode=='load' else default
    quest.PersistTransferBool=lambda *args:transfer(*args,'Bool')
    quest.PersistTransferIntDefault=lambda *args:transfer(*args,'Int')
    lua.globals()[role](quest,'context')
    return state,events,storage


class RockStateTests(unittest.TestCase):
    def test_init_matches_native_and_does_not_invent_alive_awake_fields(self):
        self.assertEqual(lua_run('Init'),execute('Init'))
        self.assertEqual(lua_run('Init')[0]['MissionOver'],False)

    def test_save_load_default_full_width_and_nonpersisted_mission(self):
        for value in (0,0x12345678,-1,-2147483648,2147483647):
            initial={'RockTrollHealthBarID':value,'MissionOver':True,'RockTrollTriggered':False}
            for mode,saved in [('save',{}),('load',{}),('load',{'RockTrollHealthBarID':-123456789,'Activated':False}),('default',{})]:
                with self.subTest(value=value,mode=mode,saved=saved):
                    actual=lua_run('OnPersist',initial,mode,saved)
                    self.assertEqual(actual,execute('OnPersist',initial,mode,saved))
                    self.assertTrue(actual[0]['MissionOver'])
                    self.assertNotIn('MissionOver',actual[2])
                    self.assertEqual(actual[1][-1][-1],0)

    def test_state_round_trip_does_not_coerce_nonzero_id_to_bool(self):
        original={'RockTrollHealthBarID':0x76543210,'RockTrollTriggered':True}
        saved=lua_run('OnPersist',original)[2]
        restored=lua_run('OnPersist',lua_run('Init')[0],'load',saved)[0]
        self.assertEqual(restored['RockTrollHealthBarID'],0x76543210)
        self.assertTrue(restored['RockTrollTriggered']);self.assertFalse(restored['MissionOver'])

    def test_native_width_default_and_key_mutations_fail_closed(self):
        class Changed(RData):
            def bytes_at(self,address,size):
                raw=super().bytes_at(address,size)
                return bytes([raw[0]^1])+raw[1:] if address==self.changed else raw
        for address in (0xec3c50,0xec4970,0x410be0,0x4045c0):
            changed=Changed();changed.changed=address
            with self.subTest(address=address),self.assertRaises(ValueError):verify(changed)

    def test_composed_candidate_has_missing_actor_inventory_and_no_stubs(self):
        report=generate();out=ROOT/'work/rock_trigger_converter/state_candidate'
        self.assertFalse(report['enabled']);self.assertFalse(report['gameplayComplete'])
        self.assertEqual(len(report['files']),4)
        self.assertFalse((out/'RockTrollFirstEncounter/Entities/RTFE_RockTroll.lua').exists())
        self.assertIn('RTFE_RockTroll.Main',report['missingFunctions'])
        self.assertEqual(report['state']['RockTrollHealthBarID']['type'],'Int')
        root=(out/'RockTrollFirstEncounter/RockTrollFirstEncounter.lua').read_text()
        self.assertIn('GetStateInt("RockTrollHealthBarID")',root)
        self.assertNotIn('self_0x4a',root)
        with self.assertRaises(ValueError):generate(ROOT/'refs/forbidden')

    def test_shared_loaded_trigger_does_not_invent_mission_completion(self):
        generate();out=ROOT/'work/rock_trigger_converter/state_candidate'
        lua=LuaRuntime()
        lua.execute((out/'RockTrollFirstEncounter/RockTrollFirstEncounter.lua').read_text())
        lua.execute('RootMain=Main')
        lua.execute((out/'RockTrollFirstEncounter/Entities/M_RTFERockTrollTrigger.lua').read_text())
        lua.execute('''
            state={}; frames=0; inRoot=false
            quest={
                SetStateBool=function(_,k,v) state[k]=v end,
                SetStateInt=function(_,k,v) state[k]=v end,
                GetStateBool=function(_,k) return state[k] end,
                GetStateInt=function(_,k) return state[k] end,
                SetCreatureGeneratorsEnabled=function() end,
                PersistTransferBool=function(_,ctx,k,v,d) if k=='RockTrollTriggered' then return true end return d end,
                PersistTransferIntDefault=function(_,ctx,k,v,d) return 0x12345678 end,
                RegisterBoundAliveCondition=function() end,
                NewScriptFrame=function() frames=frames+1 end,
                IsActiveThreadTerminating=function() return inRoot and frames>=2 end,
                AddEntityBinding=function() end, FinalizeEntityBindings=function() end,
                IsRegionLoaded=function() return true end,
                CreateThread=function(_,name) assert(name=='WatchForRegionExit') end,
                ActivateQuest=function() end
            }
            Init(quest); OnPersist(quest,{})
            Main(quest,{}) -- Loaded triggered state skips lookup/spawn entirely.
            assert(frames==1 and state.RockTrollTriggered and not state.MissionOver)
            inRoot=true; RootMain(quest)
            assert(frames==2 and not state.MissionOver)
            assert(state.RockTrollHealthBarID==0x12345678)
        ''')


if __name__=='__main__':unittest.main()
