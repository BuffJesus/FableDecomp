"""Small native-backed bound actor and HUD inputs, separate from remaining dialogs."""
from tools.script_recovery.bully_initial_phases import recover

ALLIANCE='''function BullySetHeroAlliance(quest, resources, me)
    local hero = quest:GetHero()
    resources:SetThingAsAlly(me, hero)
    hero = quest:GetHero()
    resources:SetThingAsAlly(hero, me)
end
'''

def lower(source,data=None):
    recover(data)
    edits=[
        ('quest:ClearThingHasInformation(pCVar6)','quest:ClearThingHasInformation(me)'),
        ('quest:ClearThingHasInformation(nil --[[missing]])','quest:ClearThingHasInformation(me)'),
        ('''            r11 = quest:GetHero()
            quest:EntitySetThingAsAllyOfThing(r11, nil --[[missing]])
            r12 = quest:GetHero()
            quest:EntitySetThingAsAllyOfThing(r12, nil --[[missing]])''',
         '            BullySetHeroAlliance(quest, resources, me)'),
        ('quest:RemoveQuestInfoElement(0)','quest:RemoveQuestInfoElement(quest:GetStateInt("GUIBullyHealthCounter"))'),
        ('''            -- TODO(native): iStack_94 = *(int *)(param_1 + 0x1c) - *(int *)(param_1 + 0x20);
            quest:UpdateQuestInfoBar(quest:GetStateInt("GUIBullyHealthCounter"), iStack_94, 0xbf800000, 0xbf800000)''',
         '''            quest:UpdateQuestInfoBar(quest:GetStateInt("GUIBullyHealthCounter"),
                __native_entity_state:GetStateInt("InitialHealth") - __native_entity_state:GetStateInt("HitsTaken"), -1.0, -1.0)''')]
    for old,new in edits:
        if source.count(old)!=1:raise ValueError('Bully bound actor/HUD correspondence changed')
        source=source.replace(old,new)
    return ALLIANCE+'\n'+source,{'clearSelf':[0xdbb9cb,0xdbbcd9],
        'allianceCalls':[0xdbc47a,0xdbc490],'alliance':'bound self->fresh Hero then fresh Hero->bound self; no null filtering',
        'removeInfo':0xdbc88b,'updateInfo':0xdbc582,
        'hud':'parent+64 actual id; integer InitialHealth-HitsTaken converted to float; trailing floats -1,-1',
        'limits':'Existing quest HUD conversion/colour adapter and remaining conversations not certified.'}
