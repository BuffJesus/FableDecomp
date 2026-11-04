function GetRandomSpeech(quest, me)
    local predicateResult = quest:IsActiveThreadTerminating()
    repeat
        if predicateResult then
            return ""
        end
        ::FLOW_after_lab_00e53d52::
        local randomChoice = quest:RetailRandModulo(10)
        if not ((randomChoice ~= lastRandomSpeechIdx1) and (randomChoice ~= lastRandomSpeechIdx2)) then
            quest:NewScriptFrame(me);
            predicateResult = quest:IsActiveThreadTerminating();
            goto continue_1
        end
        if quest:IsActiveThreadTerminating() then
            do return "" end;
            goto FLOW_after_lab_00e53d52
        end
        lastRandomSpeechIdx2 = lastRandomSpeechIdx1
        lastRandomSpeechIdx1 = randomChoice
        if not quest:IsActiveThreadTerminating() then
            do return (quest:GetStateString("RandomSpeech_" .. randomChoice)) end;
            goto FLOW_after_lab_00e53d52
        end
        do return "" end
        goto FLOW_after_lab_00e53d52
        quest:NewScriptFrame(me)
        predicateResult = quest:IsActiveThreadTerminating()
        ::continue_1::
    until false
end

