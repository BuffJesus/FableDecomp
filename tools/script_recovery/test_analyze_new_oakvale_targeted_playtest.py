import unittest
from tools.script_recovery.analyze_new_oakvale_targeted_playtest import analyze_text

class TargetedPlaytestAnalysisTests(unittest.TestCase):
 def test_extracts_target_outcomes(self):
  text="""TEXT_QST_048_BARRELMAN_LETDOWN_BROKEN
GetAllThingsWithScriptName: API returned 2 copies for 'NOVI_Barrel'.
    [LuaEntityHost::OnPredicateFail] Dispatching Lua callback for 'NewOakValeIntro/Entities/NOVI_Barrel'.
===== GetThingWithScriptName START: 'NOVI_Barrel' =====
===== GetThingWithScriptName END (Success) =====
NOVI_PROBE WatchBarrels GOLD_ARM broken=5 total=6 container_present=true
===== AddItemToContainer START: item='OBJECT_GOLD_1' =====
===== AddItemToContainer END =====
NOVI_PROBE WatchBarrels GOLD_INSERT_RETURNED item=OBJECT_GOLD_1 container_present=true
NOVI_PROBE BullyRun native RunTo incomplete; completing authored marker move
NOVI_PROBE BullyRun compatibility end frames=42 bully=(1.000,2.000,3.000)
NOVI_PROBE Guard claimed lecture bad=1 dealt=1
    Key: TEXT_QST_048_GUARD_CAUGHT_YOU_10
    Key: TEXT_QST_048_GUARD_CAUGHT_YOU_20
    Key: TEXT_QST_048_GUARD_CRIME_BARREL_BREAKING
    Key: TEXT_QST_048_GUARD_CAUGHT_YOU_30
    Key: TEXT_QST_048_GUARD_CAUGHT_YOU_40
    Key: TEXT_QST_048_GUARD_CAUGHT_YOU_50
    Key: TEXT_QST_048_GUARD_CAUGHT_YOU_60
    Question: TEXT_QST_048_AFFAIR_WIFE_QUESTION
    Key: TEXT_QST_048_AFFAIR_WIFE_THANKYOU
===== GetThingWithScriptName START: 'NOVI_AffairMan' =====
    Heap Buffer After API: CScriptThing* = 0x1, pVTable = 0x2 (Matches Known VTable), WorldUID = 18446741874686306552, WrapperPos = (10.5,20.25,3), pImp.Data = 0x3
===== GetThingWithScriptName END (Success) =====
NOVI_PROBE AffairWife ROUTE_START husband_def=X husband_data=Y wife=(0,0,0)
NOVI_PROBE AffairWife ROUTE_REACHED husband_def=X husband_data=Y wife=(1,0,0)
"""
  result=analyze_text(text);checks=result['checks'];self.assertTrue(all(value for key,value in checks.items() if key!='barrelContradictoryThanks'));self.assertFalse(checks['barrelContradictoryThanks']);self.assertEqual(result['gold']['broken'],5);self.assertEqual(result['gold']['total'],6);self.assertEqual(result['gold']['predicateCallbacks'],1);self.assertEqual(result['gold']['legacyRemainingBarrelLookup']['breaksBefore'],1);self.assertEqual(result['bullyCompatibilityFrames'],42);self.assertEqual(result['wife']['husbandLookup'],{'worldUid':'18446741874686306552','wrapperPos':[10.5,20.25,3.0],'offset':text.index("===== GetThingWithScriptName START: 'NOVI_AffairMan' =====")})
  self.assertEqual(result['gold']['nativeCallStarts'],1);self.assertEqual(result['gold']['insertReturns'],1)
  self.assertEqual(result['guard']['lectureClaims'],1);self.assertEqual(result['guard']['barrelCrimeLines'],1)
  self.assertEqual(result['guard']['claimPairs'],[{'bad':1,'dealt':1}])
  self.assertTrue(result['checks']['guardClaimsUniquelyAdvance'])
  self.assertTrue(result['checks']['guardFirstLectureExactLineSet'])

if __name__=='__main__':unittest.main()
