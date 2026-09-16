"""Restore quest Init's ordered appends into its owned Villager CString lists."""
import json
from tools.script_recovery.native_speech_vectors import recover_vectors,vector_prelude


def recover(source,data):
    vectors=recover_vectors(data.bytes_at)
    timer='    quest:SetTimer(quest:GetStateInt("TalkIntermittentTimer"), 0)'
    if source.count(timer)!=1:raise ValueError('Quest Init timer reset correspondence changed')
    source=source.replace(timer,'''    quest:WithRetailResources(function(resources)
        resources:SetVillagerAmbientTimer(quest:GetStateInt("TalkIntermittentTimer"), 0)
    end)''')
    prelude=vector_prelude(vectors)
    if source.count(prelude)!=1 or source.count('__native_vectors')!=1:
        raise ValueError('Quest speech initializer correspondence changed')
    names={0x9c:('good',True),0xa8:('bad',True),0xb4:('both',True),0xc0:('none',True),
           0xcc:('good',False),0xd8:('bad',False),0xe4:('both',False),0xf0:('none',False)}
    lines=['local villagerSpeechInitializers = {']
    for offset,keys in vectors.items():
        category,male=names[int(offset,0)]
        lines.append('    { category = '+json.dumps(category)+', male = '+str(male).lower()+', keys = {')
        lines.extend('        '+json.dumps(key)+',' for key in keys)
        lines.append('    } },')
    lines.append('}\n')
    source=source.replace(prelude,'\n'.join(lines))
    anchor='    quest:SetStateBool("WhichBadDeedsPerformed_" .. (4), false)\nend'
    if source.count(anchor)!=1:raise ValueError('Quest Init append position changed')
    append='''    local speechLists = quest:GetVillagerSpeechLists()
    for _, group in ipairs(villagerSpeechInitializers) do
        for _, key in ipairs(group.keys) do
            speechLists:Append(group.category, group.male, key)
        end
    end
end'''
    source=source.replace(anchor,anchor[:-3]+append)
    return source,dict(status='owned-speech-list-init',vectors=8,appends=42,
                       repeatedInit='appends to the existing quest-owned lists',
                       runtime='unapplied Villager resource/quest-owner proposal')
