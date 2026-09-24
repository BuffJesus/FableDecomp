import unittest

from tools.script_recovery.readable_style import name_script_def_reads
from tools.script_recovery.script_def_offsets import load


class ScriptDefOffsetTests(unittest.TestCase):
    """Retail reads that pin the CScriptDef layout (2026-09-24): the leading block is PDB - 4."""

    def setUp(self):
        self.table = load()

    def test_boast_block_is_where_retail_reads_it(self):
        expected = {
            0x168: 'OFEvilNakedBoastCost',               # live Orchard Evil boast UI: 80
            0x180: 'OFGoodNakedBoastCost',               # Orchard Good Init 0x00DD23A0
            0x1c8: 'TraderConflictEvilNakedBoastCost',
            0x1f8: 'TraderEscortNakedBoastCost',
            0x210: 'WhiteBalvNakedBoastCost',
            0x238: 'TCGKillNoBanditsCost',
        }
        for off, name in expected.items():
            self.assertEqual(self.table[f'{off:#x}']['name'], name)
            self.assertNotIn('verified', self.table[f'{off:#x}'])

    def test_late_anchors(self):
        self.assertEqual(self.table['0xd64']['name'], 'OVI_MoralityChangePerDeed')
        self.assertEqual(self.table['0xf10']['name'], 'GUI_MeleeBeetles')

    def test_unproven_middle_keeps_its_number(self):
        self.assertIs(self.table['0x270'].get('verified'), False)
        src = 'x = quest:ReadGlobalGameData(0x270)\ny = quest:ReadGlobalGameData(0x168)\n'
        out, n = name_script_def_reads(src)
        self.assertIn('quest:ReadGlobalGameData(0x270)', out)
        self.assertIn('quest:ReadGlobalGameData(SCRIPT_DEF.OFEvilNakedBoastCost)', out)
        self.assertEqual(n, 1)


if __name__ == '__main__':
    unittest.main()
