import unittest

from tools.script_recovery.catalog_pdb_locals import HEADER, parse_dump, symbol_name


class PdbLocalCatalogTests(unittest.TestCase):
    def test_repeated_names_and_reused_offsets_keep_separate_scopes(self):
        rows = [HEADER, 'FUNCTION\tExample::Main\t100\t200',
                '0\t6\t\t\t0\t20\t0\t1\t0\t0\t110',
                '1\t7\tspeech_movie\tMovie\t0\t16\t1\t3\t30006\t-40\t0',
                '0\t6\t\t\t0\t30\t0\t1\t0\t0\t140',
                '1\t7\tspeech_movie\tMovie\t0\t16\t1\t3\t30006\t-40\t0',
                'MATCHES\t1']
        function, = parse_dump('\n'.join(rows))
        self.assertEqual([s['scope'] for s in function['locals']], [1, 2])
        self.assertEqual([s['parent'] for s in function['scopes']], [None, 0, 0])
        self.assertEqual([s['rva'] for s in function['scopes'][1:]], [110, 140])

    def test_incomplete_or_inconsistent_dump_is_rejected(self):
        prefix = HEADER + '\nFUNCTION\tExample::Main\t100\t200\n'
        for source in (prefix, prefix + 'MATCHES\t2',
                       prefix + 'MATCHES\t1\nMATCHES\t1',
                       prefix + '1\t7\tx\t\t8\t4\t1\t3\t30006\t-4\t0\nMATCHES\t1'):
            with self.subTest(source=source), self.assertRaises(ValueError):
                parse_dump(source)

    def test_correspondence_uses_class_names(self):
        self.assertEqual(symbol_name('NOVI_BookTrader', 'Main'),
                         'NScript::CQ_NewOakValeIntroScript::CNOVI_BookTrader::Main')
        self.assertEqual(symbol_name('Q_NewOakValeIntro', 'destructor'),
                         'NScript::CQ_NewOakValeIntroScript::~CQ_NewOakValeIntroScript')


if __name__ == '__main__':
    unittest.main()
