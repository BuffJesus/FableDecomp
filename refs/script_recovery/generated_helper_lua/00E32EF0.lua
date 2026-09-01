-- Retail helper 0x00E32EF0 (NScript::CV_ArcheryCompetitionScript::RemoveArcheryQuestInfo)
-- Ordered interface calls and unnamed native fields are preserved exactly.
return function(invoke, read_i32)
    invoke("DisplayQuestInfo", false)
    invoke("RemoveQuestInfoElement", read_i32(88))
    invoke("RemoveQuestInfoElement", read_i32(92))
    invoke("RemoveQuestInfoElement", read_i32(96))
end
