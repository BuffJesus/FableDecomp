-- Retail helper 0x00E33000 (NScript::CV_ArcheryCompetitionScript::UpdateHighScore)
-- Ordered interface calls and unnamed native fields are preserved exactly.
return function(invoke, read_i32, param_1)
    invoke("UpdateQuestInfoCounter", read_i32(88), param_1, -1)
    invoke("UpdateOnlineScore_Archery", -1)
end
