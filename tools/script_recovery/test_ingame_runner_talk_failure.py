"""A true state predicate must not hide a failed required conversation."""
import json
from pathlib import Path
from types import SimpleNamespace
import tempfile
import unittest
from unittest.mock import patch, Mock
from tools.script_recovery import ingame_runner as runner

class TalkFailureTests(unittest.TestCase):
    def test_data_selector_finds_requested_actor_in_shared_script_group(self):
        from lupa.lua54 import LuaRuntime
        lua = LuaRuntime()
        lua.execute('''
            actors = {{id=1,GetDataString=function() return "SOPHIA" end},
                      {id=2,GetDataString=function() return "POLLY" end}}
            quest = {}
            function quest:GetAllThingsWithScriptName(name) assert(name=="BordelloLady"); return actors end
            function quest:GetThingWithScriptName(name) return actors[1] end
        ''')
        self.assertEqual(lua.eval(runner.talk_selector('BordelloLady'))['id'],1)
        self.assertEqual(lua.eval(runner.talk_selector('BordelloLady','POLLY'))['id'],2)
        self.assertIsNone(lua.eval(runner.talk_selector('BordelloLady','MISSING')))

    def replay(self, labels, tries, ack_after=None, blind=False, target_values=None, interaction_delay=0, pending_checks=0):
        checked=[]; tabs=[0]; tick=[0]
        def ui(spec):
            if spec=='key TAB':
                tabs[0]+=1
                tick[0]+=interaction_delay
            return ''
        def clock():
            tick[0]+=1
            return tick[0]
        def send(lines, timeout=10):
            line=lines[0]
            if line.startswith('eval '):
                if 'Runner.status' in line: value='frame=10 map=test region=test pos=1,2,3 route=0 party=ok scene=false hp=1.00'
                elif 'testDone' in line:value='true' if len(checked)>pending_checks else 'false'
                elif 'alreadyTrue' in line:
                    checked.append(line)
                    value='true' if len(checked)>pending_checks else 'false'
                elif 'correctActorTargeted' in line:value=target_values.pop(0)
                elif 'conversationStarted' in line:value='true' if tabs[0]>=ack_after else 'false'
                else:value='true'
                return ['[Autopilot] eval '+line+' = '+value]
            return []
        with tempfile.TemporaryDirectory() as tmp:
            root=Path(tmp)
            step=dict(name='talk',talk='teacher',talkLabel='' if blind else 'Teacher',talkSides=[[0,1]],until='alreadyTrue',tries=tries)
            if ack_after is not None:step.update(talkStarted='conversationStarted',talkSides=[[0,1],[1,0]])
            if target_values is not None:step.update(talkTarget='correctActorTargeted',talkSides=[[0,1],[1,0]])
            with patch.object(runner,'ROOT',root),patch.object(runner,'Channel',return_value=Mock(send=send)),patch.object(runner,'Grid',return_value=SimpleNamespace(maps={})),patch.object(runner,'game_input',side_effect=ui),patch.object(runner,'target_labels',side_effect=labels),patch.object(runner.time,'sleep'),patch.object(runner.time,'time',side_effect=clock):
                result=runner.play(SimpleNamespace(bundle='test',tag='test',minutes=10),dict(host='test',maps='test',legs={},done='testDone',steps=[step]))
            trace=[json.loads(l) for l in (root/'work/runner/test.jsonl').read_text().splitlines()]
        self.last_tabs = tabs[0]
        return result,checked,trace
    def test_failed_target_does_not_accept_existing_true_state(self):
        result,checked,trace=self.replay([[]],1)
        self.assertEqual(result,'failed');self.assertEqual(checked,[])
        self.assertFalse(any(r['kind']=='step_done' for r in trace))
    def test_target_failure_uses_retry_before_testing_state(self):
        result,checked,trace=self.replay([[],['Teacher']],2)
        self.assertEqual(result,'done');self.assertEqual(len(checked),1)
        self.assertEqual([r['attempt'] for r in trace if r['kind']=='step_start'],[1,2])
    def test_repeated_target_failures_exhaust_budget(self):
        result,checked,trace=self.replay([[],[]],2)
        self.assertEqual(result,'failed');self.assertEqual(checked,[])
        self.assertEqual(sum(r['kind']=='step_talk_failed' for r in trace),2)
    def test_acknowledgement_repositions_after_recognized_target_does_not_start(self):
        result,checked,trace=self.replay([['Teacher'],['Teacher']],1,ack_after=2)
        self.assertEqual(result,'done');self.assertEqual(len(checked),1)
        self.assertEqual(sum(r['kind']=='talk_unacknowledged' for r in trace),1)
        self.assertEqual(sum(r['kind']=='step_start' for r in trace),1)
    def test_recognized_targets_without_acknowledgement_cannot_pass(self):
        result,checked,trace=self.replay([['Teacher'],['Teacher']],1,ack_after=99)
        self.assertEqual(result,'failed');self.assertEqual(checked,[])
        self.assertEqual(sum(r['kind']=='talk_unacknowledged' for r in trace),2)
    def test_blind_talk_also_requires_requested_acknowledgement(self):
        result,checked,trace=self.replay([[],[]],1,ack_after=99,blind=True)
        self.assertEqual(result,'failed');self.assertEqual(checked,[])
        self.assertEqual(sum(r['kind']=='talk_blind' for r in trace),2)

    def test_wrong_actor_is_not_talked_to_even_with_matching_label(self):
        result,checked,trace=self.replay([['Teacher'],['Teacher']],1,target_values=['false','false'])
        self.assertEqual(result,'failed');self.assertEqual(checked,[])
        self.assertEqual(self.last_tabs,0)
        self.assertEqual([r['value'] for r in trace if r['kind']=='talk_target_check'],['false','false'])

    def test_target_guard_repositions_before_pressing_talk(self):
        result,checked,trace=self.replay([[],[]],1,ack_after=1,blind=True,target_values=['false','true'])
        self.assertEqual(result,'done');self.assertEqual(len(checked),1)
        self.assertEqual(self.last_tabs,1)
        self.assertFalse(any(r['kind']=='talk_blind' for r in trace))

    def test_missing_target_result_never_falls_back_to_blind_talk(self):
        result,checked,trace=self.replay([[],[]],1,blind=True,target_values=['nil','ERROR'])
        self.assertEqual(result,'failed');self.assertEqual(checked,[])
        self.assertEqual(self.last_tabs,0)

    def test_slow_targeting_preserves_completion_window_without_second_talk(self):
        result,checked,trace=self.replay([['Teacher'],['Teacher']],2,interaction_delay=200,pending_checks=1)
        self.assertEqual(result,'done')
        self.assertEqual(len(checked),2)
        self.assertEqual(self.last_tabs,1)
        self.assertEqual([r['attempt'] for r in trace if r['kind']=='step_start'],[1])
