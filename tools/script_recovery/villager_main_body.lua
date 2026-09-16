function Main(quest, me)
    quest:RegisterBoundConsciousCondition()
    quest:NewScriptFrame(me)
    if quest:IsActiveThreadTerminating() then return end
    quest:WithRetailResources(function(resources)
        local control = resources:NewResource()
        local ambientKey = resources:NewText()
        local function runVillager()
            while not quest:IsActiveThreadTerminating() do
                resources:PrepareResource(control)
                if resources:WasVillagerHit(me) then
                    if quest:IsActiveThreadTerminating() then return end
                    resources:SetVillagerHeroAllies(me)
                    require("NewOakValeIntro.native_quest_helpers").AddBadDeed(quest, me, 2)
                    __native_entity_state:SetStateBool("HeroDidHitMe", true)
                    resources:PrepareResource(control)
                    while not resources:TryAcquire(control, me, 4) do
                        quest:NewScriptFrame(me)
                        if quest:IsActiveThreadTerminating() then return end
                    end
                    if quest:IsActiveThreadTerminating() then return end
                    if not handleVillagerAttackedDialogue(quest, me, resources, control) then return end
                else
                    local talkedTo = resources:WasVillagerTalkedTo(me)
                    if quest:IsActiveThreadTerminating() then return end
                    if talkedTo then
                        if not handleVillagerConversation(quest, me, resources, control, __native_entity_state) then return end
                    elseif resources:ShouldVillagerStartAmbientConversation(me, quest:GetStateInt("TalkIntermittentTimer")) then
                        if quest:IsActiveThreadTerminating() then return end
                        local conversation = resources:StartVillagerAmbientConversation(me, quest:GetStateInt("TalkIntermittentTimer"))
                        if not addVillagerAmbientLine(quest, me, resources, quest:GetVillagerSpeechLists(),
                                ambientKey, conversation, GetVillagerSpeechIndex) then return end
                    end
                end
                quest:NewScriptFrame(me)
            end
        end
        local ok, failure = pcall(runVillager)
        local keyReleased, keyError = pcall(resources.DestroyText, resources, ambientKey)
        local controlReleased, controlError = pcall(resources.ReleaseResource, resources, control)
        if not ok then error(failure, 0) end
        if not keyReleased then error(keyError, 0) end
        if not controlReleased then error(controlError, 0) end
    end)
end
