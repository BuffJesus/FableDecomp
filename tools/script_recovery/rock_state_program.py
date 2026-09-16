"""Verified retail state writes/transfers; shared keys used by root and trigger."""
FIELDS = ((0x48,'MissionSucceeded','Bool'), (0x49,'MissionFailed','Bool'),
    (0x4a,'MissionOver','Bool'), (0x4d,'Activated','Bool'),
    (0x4e,'RockTrollTriggered','Bool'), (0x54,'PlayedExhumeCutScene','Bool'),
    (0x55,'AddedItemsToRockTroll','Bool'), (0x50,'RockTrollHealthBarID','Int'))
PERSISTED = FIELDS[3:]


def functions():
    lines=['function Init(quest)', '    quest:SetCreatureGeneratorsEnabled("Witchwood1", false)']
    for _,name,kind in FIELDS:
        lines.append(f'    quest:SetState{kind}("{name}", '+('0' if kind=='Int' else 'false')+')')
    lines+=['end','','function OnPersist(quest, context)']
    for _,name,kind in PERSISTED:
        api='PersistTransferIntDefault' if kind=='Int' else 'PersistTransferBool'
        default='0' if kind=='Int' else 'false'
        lines.append(f'    local {name} = quest:GetState{kind}("{name}")')
        lines.append(f'    {name} = quest:{api}(context, "{name}", {name}, {default})')
        lines.append(f'    quest:SetState{kind}("{name}", {name})')
    return '\n'.join(lines+['end',''])
