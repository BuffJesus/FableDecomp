import itertools,unittest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.wife_complete_candidate import generate,baseline,lower,correct_approach_entry
import re
from tools.script_recovery.wife_conversation_native import execute,prove
from tools.script_recovery.test_generate_affair_wife_resource_candidate import MOCK
from tools.script_recovery.lift_native_lua import RData

ADAPTERS='''
function resources:PlayWifeArgumentAnimation(id,key)
    return self:PlayAnimation(id,key,false,false,false,true,self:ReadAnimationArgument5(),false,false)
end
function resources:InitializeWifeActor(me)
    quest:EntitySetAsDamageable(me,false);quest:EntitySetAsKillable(me,false,false)
    quest:EntitySetAsToAddToComboMultiplierWhenHit(me,false);quest:SetThingHasInformation(me,false,true,false)
    quest:EntitySetAsUseMovementInActions(me,false);quest:SetIsPushableByHero(me,false)
end
function resources:SetWifeDeedReactionsDisabled(me) quest:EntitySetDeedReactionsEnabled(me,false) end
function resources:AddWifeWhereHusbandConversation(me)
    local id=quest:AddNewConversation(me,false,false)
    quest:AddPersonToConversation(id,quest:GetHero())
    quest:AddLineToConversation(id,"TEXT_QST_048_AFFAIR_WIFE_WHERES_HUSBAND",me,quest:GetHero(),false)
end
'''

def run(source,**options):
    lua=LuaRuntime();lua.globals().options=lua.table_from(options);lua.execute(MOCK+ADAPTERS);lua.execute(source);lua.execute('Init(quest,me)')
    if options.get('going'):lua.execute('setGoingForHusband()')
    error=None
    try:lua.execute('Main(quest,me)')
    except Exception as exc:error=str(exc).split('injected')[-1] if 'injected' in str(exc) else str(exc)
    return [tuple(row[i] for i in range(1,len(row)+1)) for row in lua.globals().events.values()],error

class WifeCompleteCandidateTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):cls.before,_=baseline();cls.after,cls.report=generate()

    def test_whole_main_phase_queries_cleanup_and_errors(self):
        # The original native dispatcher proves one intentional timing correction:
        # approach yields/queries before the first running-line/distance check.
        expected=re.sub(r'  -- TODO\(native\):[^\n]*','',self.before)
        expected=correct_approach_entry(expected)
        scenarios=({},{'hit':True},{'talk':True},{'talk':True,'discovered':True},
            {'talk':True,'discovered':True,'answer':0},{'fail_acquire':True},
            {'going':True,'busy':True},{'going':True,'busy':True,'talk':True},
            {'going':True,'busy':True,'hit':True},{'going':True,'approach_checks':3},
            {'going':True,'approach_checks':3,'running_line':True},{'going':True,'busy':True,'text_limit':20})
        for scenario,stop in itertools.product(scenarios,range(1,65)):
            args=dict(scenario,stop_check=stop,stop_frame=8,trace_frames=True)
            self.assertEqual(run(expected,**args),run(self.after,**args),args)
        for scenario,location in (({'hit':True},'health'),({'hit':True},'speak'),({'going':True,'busy':True},'reply')):
            # Normalize changed source line numbers; compare cleanup and primary error text.
            a=run(expected,**scenario,error_at=location,stop_frame=8)
            b=run(self.after,**scenario,error_at=location,stop_frame=8)
            self.assertEqual(a[0],b[0]);self.assertIn(location if location!='speak' else 'speech',a[1]);self.assertIn(location if location!='speak' else 'speech',b[1])

    def test_original_conversation_key_survives_second_hero_lookup(self):
        for id,heroes in itertools.product((-2147483648,-1,0,1,2147483647),itertools.product((False,True),repeat=2)):
            self.assertEqual(execute(id,heroes),[('state','ForceFirstTimeSpeak',False),('new',id,False,False),
                ('hero',0,heroes[0]),('person',id,heroes[0]),('key.new',),('hero',1,heroes[1]),('line',id,heroes[1],False),('key.destroy',)])

    def test_changed_lifetime_operands_and_source_rejected(self):
        class Changed(RData):
            def bytes_at(self,address,size):
                raw=super().bytes_at(address,size)
                if address<=self.pc<address+size:raw=bytearray(raw);raw[self.pc-address]^=1;return bytes(raw)
                return raw
        for pc in (0xdb3338,0xdb3368,0xdb3372,0xdb3385,0xdb338f,0x1260f0c+0x5b8):
            d=Changed();d.pc=pc
            with self.assertRaises(ValueError):prove(d)
        with self.assertRaises(ValueError):lower(self.before+'\nconsume(unknown)')
        self.assertNotIn('TODO(native)',self.after)
        self.assertNotIn('goto ',self.after)
        self.assertEqual(self.after.count('RegisterBoundConsciousCondition()'),1)

if __name__=='__main__':unittest.main()
