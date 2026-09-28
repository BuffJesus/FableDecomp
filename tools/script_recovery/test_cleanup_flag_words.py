"""MSVC cleanup-flag words start clear; signed-char tests keep their bit."""
from tools.script_recovery.lift_native_lua import cleanup_flag_words, lower_signed_char_tests

FLAGS = '''    flags = flags | 1
    if (flags & 1) ~= 0 then
        flags = flags & 0xfffffffe
    end
    copy = flags
    flags = copy | 0xc0
    if (flags & 0x80) ~= 0 then
        flags = flags & 0xffffff7f
    end'''.splitlines()


def test_self_updated_tested_word_with_rebuild_copy_is_a_flag_word():
    assert cleanup_flag_words(FLAGS, {'flags', 'copy'}) == ['flags', 'copy']


def test_word_spread_over_copies_with_a_dropped_source_copy():
    # V_SickChild TalkingTrader1: `uVar5 = uStack_b0` was dropped; uVar5 is only ever read
    spread = '''    work = saved | 3
    word = work
    if (work & 4) ~= 0 then
        work = work & 0xfffffffb
        word = work
    end'''.splitlines()
    assert cleanup_flag_words(spread, {'work', 'saved', 'word'}) == ['work', 'saved', 'word']
    assert cleanup_flag_words(spread + ['    quest:SetStateInt("W", word)'], {'work', 'saved', 'word'}) == []


def test_words_with_other_uses_are_not_flag_words():
    assert cleanup_flag_words(FLAGS + ['    quest:SetStateInt("X", flags)'], {'flags', 'copy'}) == []
    assert cleanup_flag_words(FLAGS + ['    flags = quest:GetStateInt("X")'], {'flags', 'copy'}) == []
    assert cleanup_flag_words(FLAGS + ['    other = copy + 1'], {'flags', 'copy', 'other'}) == []
    assert cleanup_flag_words(FLAGS, {'copy'}) == []          # not a hoisted local (a parameter)
    tested_only = ['    if (f & 1) ~= 0 then', '        f = f & 0xfffffffe', '    end']
    assert cleanup_flag_words(tested_only, {'f'}) == []       # never set: nothing proves it is one


def test_signed_char_tests_become_bit_tests():
    assert lower_signed_char_tests("if ((char)uStack_374 < '\\0') {") == 'if (((uStack_374 & 0x80) != 0)) {'
    assert (lower_signed_char_tests("if ((char)(uStack_374 >> 8) < '\\0') {")
            == 'if (((uStack_374 & 0x8000) != 0)) {')
    assert lower_signed_char_tests("if ((char)x < 'a') {") == "if ((char)x < 'a') {"
