"""Resolve the installed Guardian reward card through the documented card links.

The generic OBJECT decoder failure remains; this checks the known component
tag/link/back-reference and completely decodes its CQuestCardDef payload.
"""
import argparse,hashlib,json,struct
from pathlib import Path
from tools.parse_frontend import load_all,decode_entry,field_tag
from tools.text_build import TextBank
ROOT=Path(__file__).resolve().parents[2]
GAME=Path('C:/Programs/Steam/steamapps/common/Fable The Lost Chapters')


def audit():
    entries,schema,_=load_all(str(GAME),str(ROOT/'ghidra_out/def_schema.json'),'game.bin')
    obj=next(e for e in entries if e['name']=='OBJECT_QUEST_CARD_FIND_TROPHY_DEALER')
    raw=obj['payload'];assert len(raw)==458
    tag,index,parent=struct.unpack_from('<III',raw,77)
    assert tag==field_tag('CQuestCardDef')==0xb4c3a48a and parent==obj['index']
    card=entries[index];assert card['definition']=='CQuestCardDef'
    decoded,leftover=decode_entry(card,schema);assert leftover==0 and len(decoded)==18
    bank=TextBank(str(GAME/'data/lang/English/text.big'))
    texts={}
    for field in ('QuestName','QuestSummary'):
        key=bank.by_id[decoded[field]]['name'];text=bank.decode(key)
        assert text['type']==0 and text['content'];texts[field]=text
    category=entries[decoded['InventoryCategory']];assert category['definition']=='INVENTORY_CATEGORY'
    assert decoded['RegionName']=='Witchwood2'
    def evidence(e):
        return dict(index=e['index'],name=e['name'],definition=e['definition'],
                    payload_sha256=hashlib.sha256(e['payload']).hexdigest(),payload_hex=e['payload'].hex())
    return dict(scope=__doc__,object=evidence(obj),card=evidence(card),decoded=decoded,
                component_tag_offset=77,component_index_offset=81,parent_index_offset=85,
                texts=texts,category=evidence(category),
                layout_source='docs/formats/QUEST_CARD_BYTE_LAYOUT.md',
                source_sha256={n:hashlib.sha256((GAME/n).read_bytes()).hexdigest() for n in
                    ['data/CompiledDefs/game.bin','data/CompiledDefs/names.bin','data/lang/English/text.big']},
                limits=['Unrelated OBJECT fields are not decoded by this audit.',
                        'This verifies existing definition references, not in-game card display or quest activation.'])


if __name__=='__main__':
    p=argparse.ArgumentParser();p.add_argument('--output',type=Path,required=True);a=p.parse_args()
    out=audit();a.output.write_text(json.dumps(out,indent=2,ensure_ascii=False)+'\n',encoding='utf-8')
    print(json.dumps({'object':out['object']['index'],'card':out['card']['index'],
                      'decoded_fields':len(out['decoded']),'title':out['texts']['QuestName']['content']}))
