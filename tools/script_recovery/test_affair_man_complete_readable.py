import tempfile
import unittest
from unittest.mock import patch
from pathlib import Path
from lupa.lua54 import LuaRuntime
from tools.script_recovery.build_readable_new_oakvale import build
from tools.script_recovery.affair_man_complete import generate
from tools.script_recovery.test_affair_man_complete import run
from tools.script_recovery import test_affair_man_complete as trace_harness


class AffairManCompleteReadableTests(unittest.TestCase):
    def test_emitted_init_and_main_keep_recovered_adapters(self):
        candidate, _ = generate()
        with tempfile.TemporaryDirectory() as directory:
            report = build(Path(directory))
            source = (Path(directory)/'FSE/NewOakValeIntro/Entities/NOVI_AffairMan.lua').read_text()
            self.assertTrue(report['syntax']['ok'])
            for token in ('TODO(native)', 'goto ', 'ReadAnimationArgument5()', 'resources:PlayAnimation('):
                self.assertNotIn(token, source)
            self.assertEqual(source.count('resources:PlayAffairManAnimation('), 3)
            self.assertEqual(source.count('RegisterBoundConsciousCondition()'), 1)
            row = next(r for r in report['functions'] if r['owner']=='NOVI_AffairMan' and r['function']=='Main')
            self.assertEqual(row['implementationFunction'], '__resource_main')
            lua=LuaRuntime();lua.execute(source)
            lua.execute('''
                local events={}
                local resources={InitializeAffairManActor=function(_,actor)assert(actor==17);events[#events+1]='init' end}
                local q={WithRetailResources=function(_,body) events[#events+1]='scope';body(resources);events[#events+1]='close' end}
                Init(q,17)
                assert(table.concat(events,',')=='scope,init,close')
            ''')
            # The builder adds the separately recovered entry condition to the baseline.
            from tools.script_recovery.native_new_oakvale_conditions import recover
            expected, _ = recover('NOVI_AffairMan', candidate)
            for interaction in ('hit', 'talk', 'idle'):
                for stop in (1,2,4,8,16,30):
                    case=dict(acquire=True,terminateAtFrame=5,terminateAtCheck=stop)
                    case[interaction]=True
                    harness = trace_harness.HARNESS.replace('    local quest = {}', '''    local quest = {}
    function quest:RegisterBoundConsciousCondition(...) assert(select('#',...)==0);rec('condition') end
''')
                    with patch.object(trace_harness, 'HARNESS', harness):
                        self.assertEqual(run(expected,case),run(source,case),case)


if __name__=='__main__':unittest.main()
