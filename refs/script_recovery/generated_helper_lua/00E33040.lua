-- Retail helper 0x00E33040 (NScript::CV_ArcheryCompetitionScript::AddArcheryQuestInfo)
-- Arguments are recovered from the exact retail call-site instructions.
return function(invoke, read_i32, write_i32, read_nested_i32, field_ref)
    local handle_1 = invoke("AddQuestInfoCounter", "HUD_ICON_MULTI_ARROW", 0, 1.0)
    write_i32(88, handle_1)
    local handle_2 = invoke("AddQuestInfoCounter", "HUD_ICON_ARROW", 0, 1.0)
    write_i32(92, handle_2)
    local timer_handle = invoke("AddQuestInfoTimer", field_ref(108), "HUD_CLOCK_ICON", 1.0)
    write_i32(96, timer_handle)
    local score = read_nested_i32(68, 68)
    invoke("UpdateQuestInfoCounter", read_i32(88), score, -1)
    invoke("UpdateOnlineScore_Archery", score)
    invoke("DisplayQuestInfo", true)
end
