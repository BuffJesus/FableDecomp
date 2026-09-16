import itertools,unittest
from unittest.mock import patch
from tools.script_recovery.wife_animation_native import execute
from tools.script_recovery.wife_animation_recovery import lower,prove
from tools.script_recovery.wife_complete_candidate import generate
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery import test_wife_complete_candidate as composition
class WifeAnimationTests(unittest.TestCase):
    def test_original_cstring_raw_byte_call_and_cleanup_sequences(self):
        for kind,raw,populated in itertools.product((0,1),(0,1,2,255),(False,True)):
            key=('ST_ARGUING_POINT_AWAY','ST_ARGUING_POINT_AT')[kind]
            expected=[('key.new',key),('raw',raw)]
            if populated:expected.append(('animation',key,0,0,0,1,raw,0,0))
            expected.append(('key.destroy',key));self.assertEqual(execute(kind,raw,populated),expected)
    def test_native_site_and_source_mutations_rejected(self):
        class Changed(RData):
            def bytes_at(self,address,size):
                raw=super().bytes_at(address,size)
                if address<=self.pc<address+size:raw=bytearray(raw);raw[self.pc-address]^=1;return bytes(raw)
                return raw
        for pc in (0xdb362d,0xdb3636,0xdb3644,0xdb364f,0xdb3672,0xdb367b,0xdb3695,0xdb369e,0x7e73d9):
            d=Changed();d.pc=pc
            with self.assertRaises(ValueError):prove(d)
        source,report=generate()
        self.assertEqual(source.count('resources:PlayWifeArgumentAnimation('),2)
        self.assertNotIn('resources:ReadAnimationArgument5()',source)
        with self.assertRaises(ValueError):lower(source)
        self.assertIn('argumentAnimation',report['completionPass'])
    def test_whole_main_phase_composition_keeps_cancellation_and_errors(self):
        source,_=generate();before=source
        for key in ('ST_ARGUING_POINT_AWAY','ST_ARGUING_POINT_AT'):
            before=before.replace(f'resources:PlayWifeArgumentAnimation(wife_resource, "{key}")',
                f'resources:PlayAnimation(wife_resource, "{key}", false, false, false, true, resources:ReadAnimationArgument5(), false, false)')
        shim='''
function resources:PlayWifeArgumentAnimation(id,key)
    return self:PlayAnimation(id,key,false,false,false,true,self:ReadAnimationArgument5(),false,false)
end
'''
        scenarios=({},{'hit':True},{'talk':True},{'talk':True,'discovered':True},
            {'talk':True,'discovered':True,'answer':0},{'fail_acquire':True},
            {'going':True,'busy':True},{'going':True,'busy':True,'talk':True},
            {'going':True,'busy':True,'hit':True},{'going':True,'approach_checks':3},
            {'going':True,'approach_checks':3,'running_line':True},{'going':True,'busy':True,'text_limit':20})
        # Only the separately native/compiled-tested atomic method is abstracted.
        with patch.object(composition,'ADAPTERS',composition.ADAPTERS+shim):
            for scenario,stop in itertools.product(scenarios,range(1,65)):
                args=dict(scenario,stop_check=stop,stop_frame=8,trace_frames=True)
                self.assertEqual(composition.run(before,**args),composition.run(source,**args),args)
            for scenario,where in (({'hit':True},'health'),({'hit':True},'speak'),({'going':True,'busy':True},'reply')):
                a=composition.run(before,**scenario,error_at=where,stop_frame=8)
                b=composition.run(source,**scenario,error_at=where,stop_frame=8)
                self.assertEqual(a[0],b[0]);self.assertEqual(bool(a[1]),bool(b[1]))
if __name__=='__main__':unittest.main()
