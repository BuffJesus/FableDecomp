-- Generated native draft: Q_GuildTrainingDeparture. Review coverage report before use.
-- Registration remains disabled until the package is verified.

function Main(quest)
    local pQuestName
    local alive = true
    quest:AddEntityBinding("TheRealGuildmaster", "GuildTrainingDeparture/Entities/TheRealGuildmaster")
    quest:FinalizeEntityBindings()
    local bVar2 = quest:IsQuestActive("Q_GuildTrainingWoodsDeparture")
    while not bVar2 do
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then
            return
        end
        bVar2 = quest:IsQuestActive("Q_GuildTrainingWoodsDeparture")
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if not bVar2 then
        bVar2 = quest:IsQuestActive("Q_GuildTrainingWoodsDeparture")
        if bVar2 then
            repeat
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then
                    return
                end
                bVar2 = quest:IsQuestActive("Q_GuildTrainingWoodsDeparture")
            until not (bVar2)
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if not bVar2 then
            pQuestName = quest:GetActiveQuestName()
            quest:DeactivateQuestLater(pQuestName, 0)
        end
    end
end

function Init(quest)
    local pTargetThing = quest:GetThingWithScriptName(nil --[[missing]])
    local pThingToMove = quest:GetHero()
    quest:EntityTeleportToThing(pThingToMove, pTargetThing, false)
    pTargetThing = nil
    quest:FadeScreenIn()
    quest:SetStateBool("Finished", false)
    quest:SetStateInt("MeleeGrade", 0)
    quest:SetStateInt("SkillGrade", 0)
    quest:SetStateInt("WillGrade", 0)
end

