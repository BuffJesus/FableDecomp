#!/usr/bin/env python3
"""Certify complete, single-executable native coverage for New Oakvale."""
from __future__ import annotations
import json
from pathlib import Path

FULL_NAMES={"fullInitMain","fullDispatcher","fullLifecycle","fullQuestLifecycle"}

def audit(root:Path)->dict:
 inventories=sorted((root/'refs/script_recovery/new_oakvale_intro/entities').glob('*.json'))
 rows=[];errors=[];source_hashes=set();total_bytes=total_instructions=0
 for inventory_path in inventories:
  inventory=json.loads(inventory_path.read_text(encoding='utf-8-sig'))
  matches=[x for x in inventory.get('retailByteSnapshots',[]) if x.get('name') in FULL_NAMES]
  if not matches:
   errors.append(f'{inventory_path.stem}: no complete snapshot citation');continue
  snapshot_path=root/matches[0]['file']
  if not snapshot_path.is_file():errors.append(f'{inventory_path.stem}: snapshot missing');continue
  snapshot=json.loads(snapshot_path.read_text(encoding='utf-8-sig'));regions=snapshot.get('regions',[])
  terminals=[x.get('terminal') for x in regions]
  if not regions or any(x not in {'ret','jmp'} for x in terminals):errors.append(f'{inventory_path.stem}: invalid terminal topology')
  source_hashes.add(snapshot.get('sourceSha256'));size=sum(x.get('rangeSize',0) for x in regions);ins=sum(x.get('instructionCount',0) for x in regions)
  total_bytes+=size;total_instructions+=ins;rows.append({'inventory':inventory_path.stem,'snapshot':matches[0]['file'],'regions':len(regions),'bytes':size,'instructions':ins})
 expected='41DC91090AE853715AC06D2E9FC96E5D545381D197ED55D624C642F34509AC10'
 if source_hashes!={expected}:errors.append(f'snapshot source hashes differ: {sorted(str(x) for x in source_hashes)}')
 if len(rows)!=17:errors.append(f'complete inventory count is {len(rows)}, expected 17')
 return {'schema':'new-oakvale-full-native-coverage-audit/0.1','inventories':len(rows),'retailBytes':total_bytes,'instructions':total_instructions,'sourceSha256':expected,'rows':rows,'errors':errors,'ok':not errors}

if __name__=='__main__':print(json.dumps(audit(Path(__file__).resolve().parents[2]),indent=2))
