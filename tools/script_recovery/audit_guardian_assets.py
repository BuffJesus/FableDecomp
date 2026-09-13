"""Read installed Guardian text, macro and quest-card definitions; never write assets."""
import argparse,hashlib,json,re,struct
from pathlib import Path
from tools.parse_frontend import load_all,decode_entry
from tools.script_recovery.audit_oakvale_macro_speech import strings
from tools.text_build import TextBank
ROOT=Path(__file__).resolve().parents[2]
GAME=Path('C:/Programs/Steam/steamapps/common/Fable The Lost Chapters')


def audit():
    entries,schema,_=load_all(str(GAME),str(ROOT/'ghidra_out/def_schema.json'),'script.bin')
    macros=[];keys={'TEXT_QST_077_MAZE_ON_HIT','TEXT_QST_077_MAZE_REPEAT',
                    'TEXT_QUEST_GUARDIAN_TROPHY_DEALER_INFO_OBJECTIVE_01'}
    names={'CS_MAZE_TROPHY_INFO_SETUP','CS_MAZE_TROPHY_INFO_HITME','CS_MAZE_TROPHY_INFO'}
    for e in entries:
        if e['name'] not in names:continue
        assert e['definition']=='CCutsceneDef'
        decoded,left=decode_entry(e,schema);assert not left
        record={'name':e['name'],'payload_sha256':hashlib.sha256(e['payload']).hexdigest()}
        for field in ('Macro','SkipCond'):
            record[field]=strings(decoded[field])
            for line in record[field]:keys.update(re.findall(r'\bTEXT_[A-Z0-9_]+\b',line))
        macros.append(record)
    assert len(macros)==3
    bank=TextBank(str(GAME/'data/lang/English/text.big'))
    decoded_text={}
    def text_entry(name):
        if name in decoded_text:return
        entry=bank.decode(name);decoded_text[name]=entry
        entry['payload_sha256']=hashlib.sha256(bank.payload(bank.by_name[name])).hexdigest()
        if entry['type']==1:
            assert entry['members']
            entry['member_names']=[bank.by_id[i]['name'] for i in entry['members']]
            for child in entry['member_names']:text_entry(child)
        else:assert entry['type']==0
    for key in sorted(keys):text_entry(key)
    entries,schema,_=load_all(str(GAME),str(ROOT/'ghidra_out/def_schema.json'),'game.bin')
    cards=[e for e in entries if e['name']=='OBJECT_QUEST_CARD_FIND_TROPHY_DEALER'];assert len(cards)==1
    card=cards[0];decode_error=None
    try:decoded,left=decode_entry(card,schema)
    except (struct.error,ValueError,KeyError) as error:
        decoded=None;left=None;decode_error=type(error).__name__+': '+str(error)
    # Preserve parser limits instead of treating partial schema coverage as a pass.
    card_record={k:card[k] for k in ('name','index','definition','index_in_definition')}
    card_record.update(payload_sha256=hashlib.sha256(card['payload']).hexdigest(),
                       payload_hex=card['payload'].hex(),decoded=decoded,leftover=left,decode_error=decode_error)
    files=['data/CompiledDefs/names.bin','data/CompiledDefs/game.bin','data/CompiledDefs/script.bin','data/lang/English/text.big']
    return dict(scope=__doc__,source_sha256={n:hashlib.sha256((GAME/n).read_bytes()).hexdigest() for n in files},
                requested_text_keys=sorted(keys),texts=decoded_text,macros=macros,quest_card=card_record,
                limits=['Definition presence and localized group/member resolution only; no speech waveform or playback verification.',
                        'Quest-card payload archived; downstream component links are not certified by this inventory.'])


if __name__=='__main__':
    p=argparse.ArgumentParser();p.add_argument('--output',type=Path,required=True);a=p.parse_args()
    out=audit();a.output.write_text(json.dumps(out,indent=2,ensure_ascii=False)+'\n',encoding='utf-8')
    print(json.dumps({'requested_keys':len(out['requested_text_keys']),'text_entries':len(out['texts']),
                      'macros':len(out['macros']),'card_definition':out['quest_card']['definition'],
                      'card_leftover':out['quest_card']['leftover'],'card_decode_error':out['quest_card']['decode_error']}))
