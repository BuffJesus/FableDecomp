#!/usr/bin/env python3
"""Export the exact installed English Barrel Man outcome groups."""
from __future__ import annotations
import argparse,hashlib,json,sys
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parents[2]))
from tools.text_build import TextBank

GROUPS=("TEXT_QST_048_BARRELMAN_THANKS","TEXT_QST_048_BARRELMAN_LETDOWN_BROKEN",
        "TEXT_QST_048_BARRELMAN_LETDOWN_NOT_BROKE")

def export(bank_path:Path)->dict:
 bank=TextBank(str(bank_path));groups={}
 for name in GROUPS:
  group=bank.decode(name);members=[]
  for member_id in group['members']:
   entry=bank.decode(bank.by_id[member_id]['name'])
   members.append({'id':member_id,'name':entry['name'],'text':entry['content'],
                   'speaker':entry['speaker'],'speechbank':entry['speechbank']})
  groups[name]={'id':group['id'],'members':members}
 return {'schema':'new-oakvale-barrel-man-text-assets/0.1','source':'installed English text.big',
         'sourceSha256':hashlib.sha256(bank_path.read_bytes()).hexdigest().upper(),'groups':groups}

if __name__=='__main__':
 parser=argparse.ArgumentParser();parser.add_argument('bank',type=Path,nargs='?',default=Path(r'C:\Programs\Steam\steamapps\common\Fable The Lost Chapters\data\lang\English\text.big'));parser.add_argument('--output',type=Path);args=parser.parse_args()
 payload=json.dumps(export(args.bank),indent=2,ensure_ascii=False)+'\n'
 if args.output:args.output.write_text(payload,encoding='utf-8')
 else:print(payload,end='')
