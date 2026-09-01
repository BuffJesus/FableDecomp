-- Retail helper 0x00E32FE0 (NScript::CQ_AwakeningTheOracleScript::CChunkCollectionIndicator::OnPredicateFail)
-- Ordered interface calls and unnamed native fields are preserved exactly.
return function(invoke, read_i32)
    invoke("UpdateQuestInfoCounter", read_i32(92), read_i32(72), -1)
end
