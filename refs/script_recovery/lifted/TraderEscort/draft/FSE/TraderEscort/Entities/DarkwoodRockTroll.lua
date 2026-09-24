-- Generated native draft: DarkwoodRockTroll. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local __native_entity_state = {}
do
    local fields = {}
    for _, kind in ipairs({"Bool", "Int", "Float", "String", "Thing"}) do
        __native_entity_state["GetState" .. kind] = function(_, name) return fields[name] end
        __native_entity_state["SetState" .. kind] = function(_, name, value) fields[name] = value end
    end
end

function Main(quest, me)
    local fret_0, fret_00, pQuestName, pTarget
    local alive = true
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    local bVar3 = not alive
    if not bVar3 then
        pTarget = quest:GetHero()
        quest:GiveThingBestEnemyTarget(me, pTarget)
        fret_0 = quest:GetHealth(me)
        if 0.0 < fret_0 then
            repeat
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then
                    return
                end
                fret_00 = quest:GetHealth(me)
            until not (0.0 < fret_00)
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if not bVar3 then
            pQuestName = quest:GetActiveQuestName()
            quest:SetQuestCardObjective(pQuestName, "TEXT_QUEST_DARKWOOD_TRADER_ESCORT_OBJECTIVE_03", "BarrowFields", "BarrowFields")
            quest:SetStateBool("TradersShouldBeScared", false)
        end
    end
end

function Init(quest, me)
    quest:SetStateBool("TradersShouldBeScared", true)
    local pQuestName = quest:GetActiveQuestName()
    quest:SetQuestCardObjective(pQuestName, "TEXT_QUEST_DARKWOOD_TRADER_ESCORT_OBJECTIVE_04", "BarrowFields", "BarrowFields")
    quest:DisplayQuestInfo(true)
    local iVar1 = quest:AddQuestInfoBarHealth(me, {R = 255, G = 0, B = 0, A = 255}, "HUD_QUEST_ICON_EARTH_TROLL", 1.0)
    __native_entity_state:SetStateInt("HealthBarID", iVar1)
    quest:MiniMapAddMarker(me, "HUD_ORB_QUEST_CORE")
end

function OnPersist(quest, me, context)
end

function OnPredicateFail(quest, me)
    local bVar2, pQuestName
    local cVar3 = me:MsgIsKilledBy("")
    if (not cVar3) or (not quest:GetStateBool("TradersShouldBeScared")) then
        bVar2 = false
    else
        bVar2 = true
    end
    if bVar2 then
        pQuestName = quest:GetActiveQuestName()
        quest:SetQuestCardObjective(pQuestName, "TEXT_QUEST_DARKWOOD_TRADER_ESCORT_OBJECTIVE_03", "BarrowFields", "BarrowFields")
        quest:SetStateBool("TradersShouldBeScared", false)
    end
end

