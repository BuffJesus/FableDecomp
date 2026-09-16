-- Generated native draft: NOVI_Barrel. Review coverage report before use.
-- Not copied from the working port; registration remains disabled.

local __native_entity_state = {}
do
    local fields = {}
    for _, kind in ipairs({"Bool", "Int", "Float", "String"}) do
        __native_entity_state["GetState" .. kind] = function(_, name) return fields[name] end
        __native_entity_state["SetState" .. kind] = function(_, name, value) fields[name] = value end
    end
end

function Init(quest, me)
end

function Main(quest, me)
    local isDistanceBetweenThingsUnder, instructionGivenBarrels, isXbox, instructionDismissed
    local instructionDismissed2, predicateResult, hero
    local alive = true
    quest:RegisterBoundConsciousCondition()
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    if alive then
        instructionGivenBarrels = quest:GetStateBool("InstructionGiven_Barrels")
        while not instructionGivenBarrels do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            if not alive then
                return
            end
            hero = quest:GetHero()
            isDistanceBetweenThingsUnder = quest:IsDistanceBetweenThingsUnder(me, hero, 2.0)
            if isDistanceBetweenThingsUnder then
                alive = not quest:IsActiveThreadTerminating()
                if not alive then
                    return
                end
                isXbox = quest:IsXbox()
                if not isXbox then
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then
                        return
                    end

                    quest:DisplayGameInfo("TEXT_QST_048_INSTRUCTION_BREAK_BARRELS_PC")
                    instructionDismissed = quest:MsgIsGameInfoClickedPast()
                    while not instructionDismissed do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then
                            return
                        end
                        instructionDismissed = quest:MsgIsGameInfoClickedPast()
                    end
                else
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then
                        return
                    end

                    quest:DisplayGameInfo("TEXT_QST_048_INSTRUCTION_BREAK_BARRELS")
                    instructionDismissed2 = quest:MsgIsGameInfoClickedPast()
                    while not instructionDismissed2 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then
                            return
                        end
                        instructionDismissed2 = quest:MsgIsGameInfoClickedPast()
                    end
                end
                alive = not quest:IsActiveThreadTerminating()
                if not alive then
                    return
                end
                quest:SetStateBool("InstructionGiven_Barrels", true)
            end
            instructionGivenBarrels = quest:GetStateBool("InstructionGiven_Barrels")
        end
        alive = not quest:IsActiveThreadTerminating()
        predicateResult = not alive
        while not predicateResult do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            predicateResult = not alive
        end
    end
end

function OnPredicateFail(quest, me)
    quest:SetStateBool("BarrelBrokenInstantaneous", true)
    quest:SetStateBool("BarrelBrokenPersistent", true)
    local position = me:GetPos()
    quest:SetStateFloat("BarrelBrokenPos_x", position.x)
    quest:SetStateFloat("BarrelBrokenPos_y", position.y)
    quest:SetStateFloat("BarrelBrokenPos_z", position.z)
end

