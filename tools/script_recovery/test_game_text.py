"""game_text: text keys and quest-card titles from the install's text.big (skipped without the install)."""
import unittest

from tools.script_recovery import game_text


class QuestKeyTests(unittest.TestCase):
    def test_stems(self):
        self.assertEqual(game_text.quest_key('Q_WhiteBalverineKnotholeGlade'), 'WHITE_BALVERINE_KNOTHOLE_GLADE')
        self.assertEqual(game_text.quest_key('QS_GuardianTrophyDealerInfo'), 'GUARDIAN_TROPHY_DEALER_INFO')
        self.assertEqual(game_text.quest_key('V_TrophyDealer'), 'TROPHY_DEALER')


@unittest.skipUnless(game_text.TEXT_BIG.is_file(), 'needs the installed English text.big')
class InstallTextTests(unittest.TestCase):
    def test_card_titles(self):
        # the Bandit Camp title was entered by hand in runner_quests/bandit_camp.json and matched in-game (bc15)
        self.assertEqual(game_text.quest_title('Q_BanditCamp'), 'Find The Bandit Seeress')
        self.assertEqual(game_text.quest_title('Q_WhiteBalverineKnotholeGlade'), 'White Balverine')
        self.assertEqual(game_text.text('TEXT_QUEST_TRADER_PATH_TITLE'), 'Witchwood Trader Escort')

    def test_missing_key_is_none(self):
        self.assertIsNone(game_text.text('TEXT_NO_SUCH_KEY_ANYWHERE'))


if __name__ == '__main__':
    unittest.main()
