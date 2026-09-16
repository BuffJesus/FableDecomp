-- Generated native draft: Q_GuildTrainingDeparture. Review coverage report before use.
-- Registration remains disabled until the package is verified.

function Main(quest)
    local bVar5, ppVar4
    local alive = true
    quest:AddEntityBinding("TheRealGuildmaster", "GuildTrainingDeparture/Entities/TheRealGuildmaster")
    quest:FinalizeEntityBindings()
    local cVar2 = quest:IsQuestActive("Q_GuildTrainingWoodsDeparture")
    while not cVar2 do
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar5 = not alive
        if bVar5 then
            return
        end
        cVar2 = quest:IsQuestActive("Q_GuildTrainingWoodsDeparture")
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar5 = not alive
    if not bVar5 then
        cVar2 = quest:IsQuestActive("Q_GuildTrainingWoodsDeparture")
        while cVar2 do
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar5 = not alive
            if bVar5 then
                return
            end
            cVar2 = quest:IsQuestActive("Q_GuildTrainingWoodsDeparture")
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar5 = not alive
        if not bVar5 then
            ppVar4 = quest:GetActiveQuestName()
            quest:DeactivateQuestLater(ppVar4, nil --[[missing]])
        end
    end
end

function Init(quest)
    local iVar2 = *piVar1
    local uVar3 = quest:GetThingWithScriptName("HeroDepartureStartMarker")
    local ppVar4 = quest:GetHero()
    quest:EntityTeleportToThing(ppVar4, uVar3)
    if unaff_EBP ~= nil then
        -- TODO(native): *unaff_EBP = *unaff_EBP + -1;
        if *unaff_EBP == 0 then
        end
    end
    quest:FadeScreenIn()
    quest:SetStateBool("Finished", false)
    quest:SetStateInt("MeleeGrade", 0)
    quest:SetStateInt("SkillGrade", 0)
    quest:SetStateInt("WillGrade", 0)
end

