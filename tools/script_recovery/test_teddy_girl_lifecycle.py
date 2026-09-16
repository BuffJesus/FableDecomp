import itertools,unittest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.teddy_girl_lifecycle import SOURCE,prove
from tools.script_recovery.teddy_girl_lifecycle_native import execute
from tools.script_recovery.lift_native_lua import RData

class TeddyGirlLifecycleTests(unittest.TestCase):
    def test_native_init_and_given_order_with_every_initial_boolean_combination(self):
        for kind,initial in itertools.product(('Init','GivenTeddy'),itertools.product((False,True),repeat=4)):
            with self.subTest(kind=kind,initial=initial):
                lua=LuaRuntime();lua.execute(SOURCE);quest=lua.table();resources=lua.table();state=lua.table();events=[]
                names=('DoneIntro','FoundTeddy','SpokeAboutFindingTeddy','HeroHitMe');values=dict(zip(names,initial))
                def store(_,key,value):values[key]=value;events.append(('set',key,value))
                def initialize(_,me):
                    self.assertEqual(me,41);events.extend([('damage',False),('kill',False,False),('combo',False),('information',False,True,False)])
                def take(_):events.extend([('text.new','OBJECT_TEDDY_BEAR_UNGIVEABLE'),('take','OBJECT_TEDDY_BEAR_UNGIVEABLE'),('text.destroy','OBJECT_TEDDY_BEAR_UNGIVEABLE')])
                state.SetStateBool=store;resources.InitializeTeddyGirlActor=initialize;resources.TakeTeddyFromHero=take
                resources.ClearRawInformation=lambda _,me:events.append(('clear',))
                quest.SetMasterGameState=lambda _,key,value:events.append(('master',key,value))
                if kind=='Init':lua.globals().TeddyGirlInitialize(quest,resources,41,state)
                else:lua.globals().TeddyGirlGiven(quest,resources,41,state,lambda:events.append(('goodDeed',)))
                self.assertEqual((events,tuple(values[n] for n in names)),execute(kind,initial))

    def test_changed_state_store_api_flags_and_string_scope_rejected(self):
        original=RData()
        class Changed:
            def __init__(self,at):self.at=at
            def bytes_at(self,at,size):
                raw=bytearray(original.bytes_at(at,size))
                if at<=self.at<at+size:raw[self.at-at]^=1
                return bytes(raw)
            def string_at(self,at):return original.string_at(at)
        for at in (0xdaf011,0xdaf044,0xdaf048,0xdb0628,0xdb0630,0xdb0653):
            with self.subTest(at=at),self.assertRaises(ValueError):prove(Changed(at))

if __name__=='__main__':unittest.main()
