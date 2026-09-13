"""Read Guardian Sister's installed macro, text and Bowerstone placements."""
import argparse,hashlib,json,re
from pathlib import Path
from tools.parse_frontend import load_all,decode_entry
from tools.script_recovery.audit_oakvale_macro_speech import strings
from tools.text_build import TextBank
ROOT=Path(__file__).resolve().parents[2]
GAME=Path('C:/Programs/Steam/steamapps/common/Fable The Lost Chapters')

def audit():
    entries,schema,_=load_all(str(GAME),str(ROOT/'ghidra_out/def_schema.json'),'script.bin')
    entry=next(e for e in entries if e['name']=='CS_GUARDIANSISTER_BOWERSTONE')
    assert entry['definition']=='CCutsceneDef'
    decoded,left=decode_entry(entry,schema);assert not left
    macro={field:strings(decoded[field]) for field in ('Macro','SkipCond')}
    keys={'TEXT_QUEST_GUARDIAN_SISTER_INFO_FIRST_OBJECTIVE_01','TEXT_QST_027_MAZE_CALL_HERO_OVER_10','TEXT_QST_027_MAZE_ON_HIT_10'}
    tokens={'MazeAtTavern','M_MazeExit'}
    for lines in macro.values():
        for line in lines:
            keys.update(re.findall(r'\bTEXT_[A-Z0-9_]+\b',line));tokens.update(re.findall(r'\b[A-Za-z_][A-Za-z0-9_]*\b',line))
    bank=TextBank(str(GAME/'data/lang/English/text.big'));texts={}
    def resolve(key):
        if key in texts:return
        value=bank.decode(key);texts[key]=value
        value['payload_sha256']=hashlib.sha256(bank.payload(bank.by_name[key])).hexdigest()
        if value['type']==1:
            value['member_names']=[bank.by_id[i]['name'] for i in value['members']]
            for name in value['member_names']:resolve(name)
        else:assert value['type']==0
    for key in sorted(keys):resolve(key)
    tng=GAME/'data/Levels/FinalAlbion/BowerstoneSlums_v2.tng';records=[]
    for block in re.findall(r'(?ms)^NewThing\b.*?^EndThing;',tng.read_text(encoding='latin1')):
        name=re.search(r'(?m)^ScriptName\s+([^;]+);',block)
        if name and name.group(1)!='NULL' and name.group(1) in tokens:records.append({'name':name.group(1),'record':block})
    assert sum(r['name']=='MazeAtTavern' for r in records)==1
    assert sum(r['name']=='M_MazeExit' for r in records)==1
    world=GAME/'data/Levels/FinalAlbion.wld'
    region=next(block for block in re.findall(r'(?ms)^NewRegion\b.*?^EndRegion;',world.read_text(encoding='latin1'))
        if re.search(r'(?m)^RegionName "BowerstoneSlums";',block))
    assert 'ContainsMap "FinalAlbion\\BowerstoneSlums_v2.lev";' in region
    required={token for token in tokens if token.startswith(('MK_GSI_','CAM_GSI_'))}|{'MazeAtTavern','M_MazeExit'}
    missing=sorted(name for name in required if not any(r['name']==name for r in records))
    assert all(sum(r['name']==name for r in records)<=1 for name in required)
    paths=[world,tng,GAME/'data/CompiledDefs/script.bin',GAME/'data/CompiledDefs/names.bin',GAME/'data/lang/English/text.big']
    return dict(scope=__doc__,macro_name=entry['name'],macro_payload_sha256=hashlib.sha256(entry['payload']).hexdigest(),
        macro=macro,requested_text_keys=sorted(keys),texts=texts,placements=records,region_definition=region,
        missing_authored_placements=missing,
        sources=[{'path':str(p),'sha256':hashlib.sha256(p.read_bytes()).hexdigest()} for p in paths],
        limits=['Placement inventory covers macro tokens and native root/entity anchors in BowerstoneSlums only.',
                'Presence and decoding do not validate engine playback or audible speech.'])

if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);a=p.parse_args()
    if a.output.exists():raise FileExistsError(a.output)
    report=audit();a.output.write_text(json.dumps(report,indent=2,ensure_ascii=False)+'\n',encoding='utf-8')
    print(json.dumps({'macro_commands':{k:len(v) for k,v in report['macro'].items()},'text_keys':len(report['requested_text_keys']),
        'placements':len(report['placements']),'missing_authored_placements':report['missing_authored_placements']}))
