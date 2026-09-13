#!/usr/bin/env python3
"""Summarize targeted New Oakvale runtime evidence from one ForgeFSE log."""
from __future__ import annotations
import argparse,hashlib,json,re
from pathlib import Path

def analyze_text(text:str)->dict:
 def present(token:str)->bool:return token in text
 def count(token:str)->int:return text.count(token)
 wife_start=re.search(r'NOVI_PROBE AffairWife ROUTE_START[^\r\n]*',text)
 wife_reached=re.search(r'NOVI_PROBE AffairWife ROUTE_REACHED[^\r\n]*',text)
 gold_arm=re.search(r'NOVI_PROBE WatchBarrels GOLD_ARM broken=(\d+) total=(\d+) container_present=(\w+)',text)
 gold_return=re.search(r'NOVI_PROBE WatchBarrels GOLD_INSERT_RETURNED[^\r\n]*container_present=(\w+)',text)
 total_barrels_match=re.search(r"GetAllThingsWithScriptName: API returned (\d+) copies for 'NOVI_Barrel'",text)
 total_barrels=int(total_barrels_match.group(1)) if total_barrels_match else None
 predicate_token="Dispatching Lua callback for 'NewOakValeIntro/Entities/NOVI_Barrel'."
 predicate_offsets=[match.start() for match in re.finditer(re.escape(predicate_token),text)]
 remaining_lookups=[]
 for match in re.finditer(r"===== GetThingWithScriptName START: 'NOVI_Barrel' =====(?P<body>.*?)(?:===== GetThingWithScriptName END \(Success\) =====|===== GetThingWithScriptName START:)",text,re.DOTALL):
  remaining_lookups.append({'offset':match.start(),'success':'END (Success)' in match.group(0),'breaksBefore':sum(offset < match.start() for offset in predicate_offsets)})
 legacy_lookup=next((item for item in remaining_lookups if item['success'] and total_barrels is not None and item['breaksBefore']==total_barrels-1),None)
 bully_end=re.search(r'NOVI_PROBE BullyRun compatibility end frames=(\d+)[^\r\n]*',text)
 guard_first_keys=[f'TEXT_QST_048_GUARD_CAUGHT_YOU_{number}' for number in (10,20,30,40,50,60)]
 guard_key_counts={key:count(f'Key: {key}') for key in guard_first_keys}
 guard_key_counts['TEXT_QST_048_GUARD_CAUGHT_YOU_AGAIN']=count('Key: TEXT_QST_048_GUARD_CAUGHT_YOU_AGAIN')
 guard_claim_pairs=[(int(match.group(1)),int(match.group(2))) for match in re.finditer(r'NOVI_PROBE Guard claimed lecture bad=(\d+) dealt=(\d+)',text)]
 guard_claims=len(guard_claim_pairs)
 gold_call_starts=count("===== AddItemToContainer START: item='OBJECT_GOLD_1' =====")
 gold_insert_returns=count('NOVI_PROBE WatchBarrels GOLD_INSERT_RETURNED item=OBJECT_GOLD_1')
 husband_lookups=[]
 for match in re.finditer(r"===== GetThingWithScriptName START: 'NOVI_AffairMan' =====(?P<body>.*?)(?:===== GetThingWithScriptName END \(Success\) =====|===== GetThingWithScriptName START:)",text,re.DOTALL):
  identity=re.search(r'WorldUID = (\d+), WrapperPos = \(([-+0-9.eE]+),([-+0-9.eE]+),([-+0-9.eE]+)\)',match.group('body'))
  if identity:
   husband_lookups.append({'worldUid':identity.group(1),'wrapperPos':[float(identity.group(i)) for i in range(2,5)],'offset':match.start()})
 route_offset=wife_start.start() if wife_start else len(text)
 route_lookup=next((item for item in reversed(husband_lookups) if item['offset'] < route_offset),None)
 checks={
  'barrelBrokenRebuke':present('TEXT_QST_048_BARRELMAN_LETDOWN_BROKEN'),
  'barrelContradictoryThanks':present('TEXT_QST_048_BARRELMAN_THANKS'),
  'goldBranchArmed':gold_arm is not None,
  'goldContainerPresent':bool(gold_arm and gold_arm.group(3).lower()=='true'),
  'goldNativeCallStarted':present("===== AddItemToContainer START: item='OBJECT_GOLD_1' ====="),
  'goldNativeCallReturned':present('===== AddItemToContainer END =====') and gold_return is not None,
  'goldSingleScriptInsertion':gold_call_starts==1 and gold_insert_returns==1,
  'legacyGoldBranchEvidence':legacy_lookup is not None,
  'bullyCompatibilityNeeded':present('NOVI_PROBE BullyRun native RunTo incomplete'),
  'bullyCompatibilityCompleted':bully_end is not None,
  'guardClaimsUniquelyAdvance':bool(guard_claim_pairs) and all(bad==dealt for bad,dealt in guard_claim_pairs) and len({bad for bad,_ in guard_claim_pairs})==guard_claims,
  'guardFirstLectureExactLineSet':all(guard_key_counts[key]==1 for key in guard_first_keys),
  'wifeSingleDisclosureQuestion':count('Question: TEXT_QST_048_AFFAIR_WIFE_QUESTION')==1,
  'wifeDisclosureAccepted':count('Key: TEXT_QST_048_AFFAIR_WIFE_THANKYOU')==1,
  'wifeRouteStarted':wife_start is not None,
  'wifeRouteReached':wife_reached is not None,
  'wifeHusbandWorldIdentityCaptured':route_lookup is not None,
 }
 return {'schema':'new-oakvale-targeted-playtest-analysis/0.5','checks':checks,'gold':{'broken':int(gold_arm.group(1)) if gold_arm else None,'total':int(gold_arm.group(2)) if gold_arm else total_barrels,'predicateCallbacks':len(predicate_offsets),'nativeCallStarts':gold_call_starts,'insertReturns':gold_insert_returns,'legacyRemainingBarrelLookup':legacy_lookup},'guard':{'lectureClaims':guard_claims,'claimPairs':[{'bad':bad,'dealt':dealt} for bad,dealt in guard_claim_pairs],'speechKeyCounts':guard_key_counts,'barrelCrimeLines':count('Key: TEXT_QST_048_GUARD_CRIME_BARREL_BREAKING')},'bullyCompatibilityFrames':int(bully_end.group(1)) if bully_end else None,'wife':{'questionCount':count('Question: TEXT_QST_048_AFFAIR_WIFE_QUESTION'),'acceptedCount':count('Key: TEXT_QST_048_AFFAIR_WIFE_THANKYOU'),'start':wife_start.group(0) if wife_start else None,'reached':wife_reached.group(0) if wife_reached else None,'husbandLookup':route_lookup}}

def analyze(path:Path)->dict:
 raw=path.read_bytes();result=analyze_text(raw.decode('utf-8',errors='replace'));result['log']=str(path);result['logSha256']=hashlib.sha256(raw).hexdigest().upper();return result

def main()->int:
 ap=argparse.ArgumentParser();ap.add_argument('log',type=Path);ap.add_argument('--output',type=Path);a=ap.parse_args();result=analyze(a.log);payload=json.dumps(result,indent=2)+'\n'
 if a.output:a.output.write_text(payload,encoding='utf-8')
 else:print(payload,end='')
 return 0
if __name__=='__main__':raise SystemExit(main())
