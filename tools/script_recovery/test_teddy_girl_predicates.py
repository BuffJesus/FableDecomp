import itertools,unittest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.teddy_girl_predicates import SOURCE,prove
from tools.script_recovery.teddy_girl_presented_native import execute as presented
from tools.script_recovery.teddy_girl_masks_native import execute as masks
TEDDY='OBJECT_TEDDY_BEAR_UNGIVEABLE'

class TeddyGirlPredicateTests(unittest.TestCase):
    def test_original_two_polls_keep_failed_populated_and_successful_empty_outputs(self):
        prove();choices=[(False,TEDDY),(False,None),(True,''),(True,TEDDY),(True,'OTHER')]
        for first,second in itertools.product(choices,repeat=2):
            with self.subTest(first=first,second=second):
                lua=LuaRuntime();lua.execute(SOURCE);r=lua.table();state={'text':'','polls':0};events=[]
                def poll(_,output):
                    self.assertEqual(output,5);events.append(('poll.input',state['text']));result,text=(first,second)[state['polls']];state['polls']+=1
                    if text is not None:state['text']=text
                    events.append(('poll.output',result,state['text']));return result
                r.PollPresentedItem=poll;r.PresentedItemMatches=lambda _,output,name:state['text']==name
                self.assertEqual((lua.globals().TeddyGirlPresentedKind(r,5),events),presented(first,second))

    def test_native_talk_and_hit_short_circuit_preserve_each_live_string(self):
        for kind,answers,mask in itertools.product(('talk','hit'),itertools.product((False,True),repeat=3),(0,256)):
            with self.subTest(kind=kind,answers=answers,mask=mask):
                result,remaining,events=masks(kind,answers,mask)
                if kind=='talk':
                    count=2 if answers[0] else 1;expected=[('string.new','SCRIPT_NAME_HERO'),('query',0,answers[0])]
                    if answers[0]:expected += [('string.new',TEDDY),('hero',),('query',1,answers[1]),('string.destroy',TEDDY)]
                    expected += [('string.destroy','SCRIPT_NAME_HERO')];predicate=answers[0] and answers[1]
                else:
                    count=1 if answers[0] else (3 if answers[1] else 2);expected=[]
                    for i in range(count):expected += [('string.new','SCRIPT_NAME_HERO'),('query',i,answers[i])]
                    expected += [('string.destroy','SCRIPT_NAME_HERO')]*count;predicate=answers[0] or (answers[1] and not answers[2])
                self.assertEqual(result,predicate);self.assertEqual(remaining,mask);self.assertEqual(events,expected)

if __name__=='__main__':unittest.main()
