"""Recover HUD colour structs and the exact question/HUD CString nesting."""
from tools.script_recovery.bully_initial_phases import recover
from tools.script_recovery.lift_native_lua import RData

def lower(source,data=None):
    data=data or RData();recover(data)
    expected={0x12d60f0:'HUD_QUEST_ICON_GRANDSON',0x12d9b3c:'TEXT_QST_048_GIVE_TEDDY_TO_BULLY',
              0x12c2188:'TEXT_OBJECT_HERO_ANSWER_YES',0x12c216c:'TEXT_OBJECT_HERO_ANSWER_NO'}
    if any(data.string_at(p)!=v for p,v in expected.items()):raise ValueError('Bully HUD/question literals changed')
    if data.bytes_at(0x122d70e,1)!=b'\0':raise ValueError('Bully empty string literal changed')
    edits=[('quest:AddQuestInfoBar(__native_entity_state:GetStateInt("InitialHealth"), 0, 0xff, 0x0, "HUD_QUEST_ICON_GRANDSON", "", 0xff)',
            'resources:AddBullyHealthBar(__native_entity_state:GetStateInt("InitialHealth"))'),
           ('quest:GiveHeroYesNoQuestion(pCVar22, "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)',
            'resources:GiveBullyTeddyQuestion()')]
    for old,new in edits:
        if source.count(old)!=1:raise ValueError('Bully HUD/question source correspondence changed')
        source=source.replace(old,new)
    return source,{'question':{'construct':[0xdbb7d6,0xdbb7e9,0xdbb7fc,0xdbb80f],
        'call':0xdbb83b,'destroy':[0xdbb848,0xdbb854,0xdbb860,0xdbb86c],'flag':True},
        'hud':{'construct':[0xdbc3cd,0xdbc3e0],'call':0xdbc446,'destroy':[0xdbc459,0xdbc465],
        'colour1BGRA':[0,255,0,255],'colour2BGRA':[0,0,255,255],'max':0.0,'scale':1.0},
        'limits':'Scoped native literals destroy even empty; runtime checkout remains unchanged.'}
