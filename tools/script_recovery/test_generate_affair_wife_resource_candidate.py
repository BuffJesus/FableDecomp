import tempfile
import unittest
from pathlib import Path

from lupa.lua54 import LuaRuntime
from tools.script_recovery.generate_affair_wife_resource_candidate import DRAFT,generate
from tools.script_recovery import test_generate_book_trader_resource_candidate as book_tests

MOCK=book_tests.MOCK+r'''
function quest:EntitySetDeedReactionsEnabled() end
function quest:RegisterBoundConsciousCondition() record('condition') end
package.preload['NewOakValeIntro.native_quest_helpers']=function()
    return {
        AddGoodDeed=function(q,actor,value) assert(q==quest and actor==me);record('good.deed',value) end,
        AddBadDeed=function(q,actor,value) assert(q==quest and actor==me);record('bad.deed',value) end
    }
end
local stateBool=quest.GetStateBool
function quest:GetStateBool(name)
    if name=='HeroDiscoveredInfidelity' then return options.discovered or false end
    return stateBool(self,name)
end
function quest:GiveHeroYesNoQuestion(prompt,yes,no,help,flag)
    assert(prompt=='TEXT_QST_048_AFFAIR_WIFE_QUESTION' and yes=='TEXT_OBJECT_HERO_ANSWER_YES')
    assert(no=='TEXT_OBJECT_HERO_ANSWER_NO' and help=='' and flag)
    record('question',prompt)
end
function resources:NewThingFromScriptName(name)
    assert(name=='NOVI_AffairMan');local id=create('thing');record('husband',id);return id
end
function resources:ThingPosition(id) require_kind(id,'thing');return {x=30,y=40,z=50} end
local approachChecks=0
function resources:ThingsAreWithinDistance(actor,id,distance)
    assert(actor==me and distance==3);require_kind(id,'thing')
    approachChecks=approachChecks+1;return approachChecks>(options.approach_checks or 0)
end
function me:IsDistanceFromPositionOver(position,distance)
    assert(distance==10);return options.running_line or false
end
local nextFrame=quest.NewScriptFrame
function quest:NewScriptFrame(...)
    if options.trace_frames then record('frame') end
    return nextFrame(self,...)
end
local terminating=quest.IsActiveThreadTerminating
function quest:IsActiveThreadTerminating()
    local result=terminating(self)
    if options.trace_frames then record('term',result) end
    return result
end
function resources:MoveToPosition(id,position,radius,kind,a,b)
    require_kind(id,'resource');assert(position.x==30 and radius==2 and kind==1 and not a and b)
    record('move.husband')
end
function resources:ClearCommands(id) require_kind(id,'resource');record('clear.commands') end
function resources:ClearAllActions(id) require_kind(id,'resource');record('clear.actions') end
function resources:FaceThing(actor,target,snap)
    assert(actor==me);require_kind(target,'thing');record('face.husband',snap)
end
function resources:AddConversationPerson(id,actor)
    require_kind(actor,'thing');record('participant',id,actor)
end
function resources:AddConversationLine(id,text,speaker,listener,flag)
    assert(not flag)
    if speaker==me then require_kind(listener,'thing') else require_kind(speaker,'thing');assert(listener==me) end
    record('line',id,text,speaker==me and 'wife' or 'husband')
    if options.error_at=='reply' then error('injected reply failure') end
end
local activeKey=nil
function resources:WithArgumentKey(number,body)
    assert(activeKey==nil)
    local key={text='TEXT_QST_048_AFFAIR_WIFE_WHATS_THIS_'..tostring(number)}
    activeKey=key;record('key.new',number)
    function key:Exists()
        assert(activeKey==self);record('key.exists',number)
        return number <= (options.text_limit or 40)
    end
    function key:ResetToFirst()
        assert(activeKey==self);self.text='TEXT_QST_048_AFFAIR_WIFE_WHATS_THIS_10';record('key.reset')
    end
    local ok,result=pcall(body,key)
    record('key.destroy',number);activeKey=nil
    if not ok then error(result) end
    assert(type(result)=='boolean');return result
end
function resources:AddArgumentKeyLine(key,id,speaker,listener)
    assert(activeKey==key and speaker==me);require_kind(listener,'thing')
    record('line',id,key.text,'wife')
end
function resources:PlayAnimation(id,name,a,b,c,d,byte,e,f)
    require_kind(id,'resource');assert(not a and not b and not c and d and not e and not f)
    record('animation',name)
end
function quest:IsConversationActive() return false end
function quest:MsgDoesTextExist() return true end
function setGoingForHusband()
    -- Exercise the retained-partner state directly; preserve production locals.
    for i=1,20 do
        local name,value=debug.getupvalue(Init,i)
        if name=='__native_entity_state' then value:SetStateBool('GoingForHusband',true);return end
    end
    error('wife state unavailable')
end
'''


class WifeCandidateTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):cls.source,cls.report=generate()

    def run_case(self,**options):
        lua=LuaRuntime();lua.globals().options=lua.table_from(options)
        lua.execute(MOCK);lua.execute(self.source);lua.execute('Init(quest,me)')
        if options.get('going'):lua.execute('setGoingForHusband()')
        error=None
        try:lua.execute('Main(quest,me)')
        except Exception as exc:error=str(exc)
        events=[tuple(row[i] for i in range(1,len(row)+1)) for row in lua.globals().events.values()]
        return events,error

    def test_entry_cancellation_does_not_construct_resource(self):
        events,error=self.run_case(stop_check=1)
        self.assertIsNone(error);self.assertEqual(events,[('condition',)])

    def test_initial_acquisition_and_idle_cancellation_cleanup(self):
        for scenario in ({},{'fail_acquire':True},{'hit':True},{'hit':True,'busy':True}):
            for check in range(1,35):
                with self.subTest(scenario=scenario,check=check):
                    events,error=self.run_case(**scenario,stop_check=check,stop_frame=4)
                    self.assertIsNone(error)
                    if ('new.resource',1) in events:self.assertEqual(events[-1],('destroy.resource',1))

    def test_speech_uses_resource_and_destroys_health_temporary_first(self):
        events,error=self.run_case(hit=True)
        self.assertIsNone(error)
        speech=('speak',1,'TEXT_QST_048_AFFAIR_WIFE_ON_HIT')
        self.assertIn(speech,events)
        self.assertLess(events.index(('destroy.thing',3)),events.index(speech))
        self.assertLess(events.index(('pause',False)),events.index(('destroy.movie',2)))

    def test_changed_draft_rejects_before_output(self):
        with tempfile.TemporaryDirectory() as directory:
            path=Path(directory)/'wife.lua';path.write_text(DRAFT.read_text().replace('AcquireControl(3)','AcquireControl(4)'))
            with self.assertRaisesRegex(ValueError,'draft changed'):generate(draft=path)

    def test_partner_path_cancellation_releases_husband_before_resource(self):
        for scenario in ({},{'hit':True},{'talk':True},{'hit':True,'health':0},{'talk':True,'health':0}):
            for check in range(1,45):
                with self.subTest(scenario=scenario,check=check):
                    events,error=self.run_case(going=True,busy=True,**scenario,stop_check=check,stop_frame=5)
                    self.assertIsNone(error)
                    if ('husband',2) in events:
                        self.assertEqual(events[-2:],[('destroy.thing',2),('destroy.resource',1)])

    def test_argument_lines_alternate_speakers(self):
        events,error=self.run_case(going=True,busy=True,stop_frame=5)
        self.assertIsNone(error)
        lines=[event for event in events if event[0]=='line']
        self.assertGreaterEqual(len(lines),2)
        self.assertEqual(lines[0][2:],('TEXT_QST_048_AFFAIR_WIFE_WHATS_THIS_10','wife'))
        self.assertEqual(lines[1][2:],('TEXT_QST_048_AFFAIRMAN_IN_TROUBLE','husband'))

    def test_special_ability_exclusion_uses_scoped_hit_result(self):
        for excluded in (False,True):
            events,error=self.run_case(special=True,excluded=excluded)
            self.assertIsNone(error)
            self.assertEqual(('speak',1,'TEXT_QST_048_AFFAIR_WIFE_ON_HIT') in events,not excluded)

    def test_live_text_lookup_accepts_fifty_and_resets_only_missing_key(self):
        for limit in (20,60):
            events,error=self.run_case(going=True,busy=True,stop_frame=12,text_limit=limit)
            self.assertIsNone(error)
            lines=[e[2] for e in events if e[0]=='line' and e[3]=='wife']
            self.assertEqual('TEXT_QST_048_AFFAIR_WIFE_WHATS_THIS_50' in lines,limit==60)
            self.assertIn(('key.reset',),events)

    def test_key_lives_through_reply_and_closes_on_error(self):
        events,error=self.run_case(going=True,busy=True,stop_frame=5,error_at='reply')
        self.assertIn('injected reply failure',error)
        reply=next(i for i,e in enumerate(events) if e[0]=='line' and e[3]=='husband')
        close=events.index(('key.destroy',10))
        self.assertLess(reply,close)
        self.assertLess(close,events.index(('destroy.thing',2)))

    def test_disclosure_question_routes_to_husband_only_for_yes(self):
        for answer in (0,1):
            events,error=self.run_case(talk=True,discovered=True,answer=answer,stop_frame=5)
            self.assertIsNone(error)
            self.assertIn(('question','TEXT_QST_048_AFFAIR_WIFE_QUESTION'),events)
            self.assertEqual(any(e[0]=='husband' for e in events),answer==1)

    def test_disclosure_cancellation_cleans_movie_and_resource(self):
        for waits in (0,2):
            for check in range(1,60):
                with self.subTest(waits=waits,check=check):
                    events,error=self.run_case(talk=True,discovered=True,wait_answers=waits,stop_check=check,stop_frame=6)
                    self.assertIsNone(error)
                    if ('new.resource',1) in events:self.assertEqual(events[-1],('destroy.resource',1))
