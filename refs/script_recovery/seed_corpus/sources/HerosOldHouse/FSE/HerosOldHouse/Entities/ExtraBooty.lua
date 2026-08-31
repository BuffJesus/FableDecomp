local MY_SCRIPT_NAME = "ExtraBooty"

function Init(quest, me)
end

function Main(quest, me)
    quest:Log("ExtraBooty: Main() started. Monitoring digging spot...")

    -- Wait until the digging spot is dug up (becomes disabled)
    while quest:IsDiggingSpotEnabled(me) do
        if not quest:NewScriptFrame(me) then
            return
        end
    end

    quest:Log("ExtraBooty: Digging spot dug up!")
    quest:SetStateBool("BootyDugUp", true)

    if not quest:GetStateBool("WifeAttacked") then
        local activeQuestName = quest:GetActiveQuestName()
        quest:SetQuestAsCompleted(activeQuestName, false, false, false)
        quest:Log("ExtraBooty: Quest set as completed!")
    end
end
