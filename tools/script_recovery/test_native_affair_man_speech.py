import copy
import unittest
from tools.script_recovery import test_native_affair_man_talk_continuation as fixtures
from tools.script_recovery.native_affair_man_speech import map_affair_man_speech


class AffairManSpeechTests(unittest.TestCase):
    def test_all_eight_sites_require_nonblocking_resource_dispatch(self):
        fn,source,data,manifest=fixtures.AffairManTalkContinuationTests().inputs()
        evidence=map_affair_man_speech(fn,data,manifest)[0]
        self.assertEqual(evidence['status'],'mapped')
        self.assertEqual(len(evidence['issues']),8)
        self.assertEqual([c['key'].removeprefix('TEXT_QST_048_AFFAIRMAN_') for c in evidence['calls']],
            ['ON_HIT','HAD_A_DEAL','SOME_NERVE','HOW_FIND_OUT','INTRO','GOOD_LAD','DONT_TELL','SHOO'])
        for call in evidence['calls']:
            self.assertEqual((call['resourceStackOffset'],call['selection'],call['listen'],call['sound2D'],call['overFade']),
                (16,0,False,True,False))
            self.assertLess(call['heroQuery'],call['address'])

    def test_changed_wrapper_or_host_contract_rejects_mapping(self):
        fn,source,data,manifest=fixtures.AffairManTalkContinuationTests().inputs()
        class Changed:
            def bytes_at(self,address,size):
                raw=data.bytes_at(address,size)
                return b'\x90'*size if address==0x7e7390 else raw
        self.assertEqual(map_affair_man_speech(fn,Changed(),manifest)[0]['status'],'rejected')
        changed=copy.deepcopy(manifest)
        changed['Speak']['blocking']=False
        self.assertEqual(map_affair_man_speech(fn,data,changed)[0]['status'],'rejected')
