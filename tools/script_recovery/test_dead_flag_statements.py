"""Cleanup-flag bookkeeping in a slot shared with another object is dropped (V_ChickenKicking Spectator 0x00E63890)."""
from tools.script_recovery.lift_native_lua import drop_dead_flag_statements

SPECTATOR = '''    line = ("TEXT_QST_B17_SPECTATOR_" .. name)
    line = (line .. "_GREETING")
    speech = line
    me:Speak(hero, speech)
    before = line
    if me:MsgIsHitByHero() then
        goto HIT
    else
        flags = before | 3
        line = flags
        if me:MsgIsHitByAnySpecialAbilityFromHero() then
            flags = before | 7
            line = flags
        end
    end
    ::HIT::
    if (flags & 4) ~= 0 then
        flags = flags & 0xfffffffb
        line = flags
    end
    if (flags & 1) ~= 0 then
        -- TODO(native): line = flags & 0xfffffffe;
    end
    quest:SetStateBool("Hit", true)'''.splitlines()


def test_flag_bookkeeping_over_a_dead_string_slot_is_dropped():
    out = drop_dead_flag_statements(SPECTATOR)
    assert not any('flags' in l and not l.lstrip().startswith('--') for l in out)
    assert 'before = line' not in [l.strip() for l in out]
    # the string's own life and the copy that feeds Speak stay
    assert '    speech = line' in out and '    line = (line .. "_GREETING")' in out
    assert '    quest:SetStateBool("Hit", true)' in out


def test_a_later_read_of_the_slot_keeps_everything():
    lines = SPECTATOR + ['    quest:SetStateString("Last", line)']
    assert drop_dead_flag_statements(lines) == lines


def test_a_fresh_definition_after_the_flags_is_fine():
    lines = SPECTATOR + ['    line = "again"', '    quest:SetStateString("Last", line)']
    out = drop_dead_flag_statements(lines)
    assert '    line = "again"' in out and not any(l.strip() == 'line = flags' for l in out)


def test_words_the_seeding_pass_owns_are_left_alone():
    assert drop_dead_flag_statements(SPECTATOR, keep=('flags',)) == SPECTATOR


def test_high_byte_boolean_masks_are_not_flag_bookkeeping():
    # V_BeggarAndChild BeggarBully Main: the flag register also carries a high-byte boolean elsewhere
    lines = '''    word = CONCAT13(1, p1)
    if dead then
        word = p1 & 0xffffff
    end
    if (word >> 0x18) ~= 0 then
        quest:SetStateBool("Seen", true)
    end
    word = saved | 0x40
    if ((word & 0x80) ~= 0) then
        word = word & 0xffffff7f
    end
    if (word & 0x40) ~= 0 then
        word = word & 0xffffffbf
    end
    saved = word'''.splitlines()
    out = drop_dead_flag_statements(lines)
    assert '        word = p1 & 0xffffff' in out and '    if (word >> 0x18) ~= 0 then' in out
    assert '    word = saved | 0x40' not in out and '    if ((word & 0x80) ~= 0) then' not in out


def test_bits_returned_to_the_caller_are_data():
    # V_NewOakValeIntro AffairMan's mask helper hands the bits back through its parameter and result
    lines = '''    local bits = mask | 1
    if (bits & 1) ~= 0 then
        bits = bits & 0xfffffffe
    end
    mask = bits
    result = 1
    result = mask
    return result'''.splitlines()
    assert drop_dead_flag_statements(lines) == lines
