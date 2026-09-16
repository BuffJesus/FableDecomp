"""Retail EC3D40 Main; scoped region-message capability remains explicit."""
BODY='''    quest:AddEntityBinding("RTFE_Sparrow", "RockTrollFirstEncounter/Entities/RTFE_Sparrow", 0)
    quest:AddEntityBinding("RTFE_RockTroll", "RockTrollFirstEncounter/Entities/RTFE_RockTroll", 0)
    quest:AddEntityBinding("M_RTFERockTrollTrigger", "RockTrollFirstEncounter/Entities/M_RTFERockTrollTrigger", 0)
    quest:FinalizeEntityBindings()
    while not quest:IsRegionLoaded("Witchwood1") do
        quest:NewScriptFrame()
        if quest:IsActiveThreadTerminating() then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    quest:CreateThread("WatchForRegionExit", {region = ""})
    if not quest:GetStateBool("Activated") then
        if quest:IsActiveThreadTerminating() then return end
        quest:ActivateQuest("V_RockTrollFirstEncounter_Activate")
        quest:SetStateBool("Activated", true)
    end
    while not quest:GetStateBool("MissionOver") do
        quest:NewScriptFrame()
        if quest:IsActiveThreadTerminating() then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    quest:RemoveQuestInfoElement(quest:GetStateInt("RockTrollHealthBarID"))
    quest:DisplayQuestInfo(false)
    quest:WithRegionLoadedMessage(function(message)
        while not message:Poll() do
            quest:NewScriptFrame()
            if quest:IsActiveThreadTerminating() then return end
        end
        if quest:IsActiveThreadTerminating() then return end
        quest:SetCreatureGeneratorsEnabled("Witchwood1", true)
        quest:DeactivateQuestLater("V_RockTrollFirstEncounter_Activate", 0)
        quest:DeactivateQuestLater(quest:GetActiveQuestName(), 0)
    end)
'''
