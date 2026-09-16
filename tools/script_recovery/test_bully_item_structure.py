"""Whole generated Main comparisons around structured item ownership/joins."""
import itertools
import unittest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.bully_full_resource_candidate import generate as original
from tools.script_recovery.bully_item_structure import generate,lower,HELPER
from tools.script_recovery.test_bully_full_resource_candidate import HARNESS
from tools.script_recovery.test_bully_readable_integration import run as completion_run
from tools.script_recovery.bully_presented_native import execute as native_classifier


def run(source,mode,health,cancel,answer,delay,fault):
    extra='''
    local query=0
    function quest:IsActiveThreadTerminating() query=query+1;rec('query:'..query);return query>=CANCEL end
    function resources:GiveBullyTeddyQuestion() rec('question') end
    local answers=0
    function quest:MsgIsQuestionAnsweredYesOrNo() answers=answers+1;rec('answer');if answers<=DELAY then return -1 end;return ANSWER end
    local complaints=0
    function quest:GetStateBool(key) complaints=complaints+1;rec('bool:'..key);return complaints<=DELAY end
    local tasks=0
    function resources:IsPerformingScriptTask(id) tasks=tasks+1;rec('task');return tasks<=DELAY end
    local acquisitions=0
    function resources:TryAcquire(id,actor,priority) assert(entries[id].live and priority==4);acquisitions=acquisitions+1;rec('acquire');return acquisitions<=2 or acquisitions>2+DELAY end
    function quest:NewScriptFrame() frames=frames+1;rec('frame');assert(frames<100,'FRAME_GUARD') end
    local target=__FAULT__
    if target~='' then
        local owner=resources
        if target=='GetHero' or target=='ClearThingHasInformation' then owner=quest end
        local previous=owner[target]
        owner[target]=function(self,...) rec('fault:'..target);error('BODY_FAULT') end
    end
'''.replace('CANCEL',str(cancel)).replace('DELAY',str(delay)).replace('ANSWER',str(answer)).replace('__FAULT__',repr(fault))
    harness=HARNESS.replace('    source=source..',extra+'\n    source=source..',1)
    lua=LuaRuntime(unpack_returned_tuples=True)
    lua.execute('package={preload={}}; function require(n) return package.preload[n]() end')
    events,ok,error,polls=lua.execute(harness)(source,mode,health)
    return list(events.values()),ok,error.split(':')[-1].strip(),polls


class BullyItemStructureTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):cls.before,_=original();cls.after,cls.report=generate()

    def test_entire_body_item_branches_queries_and_scoped_errors(self):
        cases=0
        for mode,health,cancel,answer,delay in itertools.product(
                ('teddy','other','question'),(1.0,0.0,float('nan')),range(1,36),(-2,0,1,2),(0,2)):
            args=(mode,health,cancel,answer,delay,'')
            self.assertEqual(run(self.before,*args),run(self.after,*args),args)
            cases+=1
        self.assertEqual(cases,2520)
        for mode,fault in itertools.product(('teddy','other','question'),
                ('Speak','ThingHealth','GetHero','GiveBullyTeddyQuestion','ClearThingHasInformation','PollPresentedItem','PresentedItemMatches')):
            args=(mode,1.0,999,1,2,fault)
            self.assertEqual(run(self.before,*args),run(self.after,*args),args)

    def test_init_runoff_and_macro_errors_unchanged(self):
        for mode,cancel in itertools.product(('normal','macro_error'),(1,5,12,20,999)):
            self.assertEqual(completion_run(self.before,mode,1.0,cancel,True),completion_run(self.after,mode,1.0,cancel,True))

    def test_actual_helper_classifier_against_original_instructions(self):
        teddy='OBJECT_TEDDY_BEAR_UNGIVEABLE'
        choices=[(False,teddy),(False,None),(True,''),(True,teddy),(True,'OTHER')]
        for first,second in itertools.product(choices,repeat=2):
            lua=LuaRuntime(unpack_returned_tuples=True);resources=lua.table();quest=lua.table()
            state={'text':'','polls':0};events=[]
            def poll(_,id):
                self.assertEqual(id,71)
                events.append(('poll.input',state['text']))
                result,text=(first,second)[state['polls']];state['polls']+=1
                if text is not None:state['text']=text
                events.append(('poll.output',result,state['text']))
                return result
            resources.PollPresentedItem=poll
            resources.PresentedItemMatches=lambda _,id,key:state['text']==key
            resources.NewPresentedItemOutput=lambda _,me:71
            resources.BullyTalkedWithTeddy=lambda _,me:False
            quest.IsActiveThreadTerminating=lambda _:False
            # Stop at the separately verified movie/acquisition phase boundaries,
            # corresponding to native classifier stops DBBB73/DBBA52/DBBD83.
            source='''local resources,quest=...;local bully_presented,bully_movie,bully_control
local me={};local route="none"
function resources:StartMovie() route="teddy";error("PHASE_BOUNDARY") end
function resources:PrepareResource() route="other";error("PHASE_BOUNDARY") end
'''+HELPER+'''local ok,err=pcall(handle_bully_item)
if not ok then assert(tostring(err):find("PHASE_BOUNDARY"),err) end
return route'''
            self.assertEqual((lua.execute(source,resources,quest),events),native_classifier(first,second))

    def test_fail_closed_and_real_structuring(self):
        self.assertNotIn('goto ',HELPER)
        self.assertNotIn('pCVar',HELPER)
        self.assertLess(self.after.count('goto '),self.before.count('goto ')-20)
        self.assertFalse(self.report['enabled'])
        self.assertFalse(self.report['gameplayComplete'])
        with self.assertRaises(ValueError):lower(self.before.replace('if iVar8 == 1 then','if iVar8 == 2 then'))


if __name__=='__main__':unittest.main()
