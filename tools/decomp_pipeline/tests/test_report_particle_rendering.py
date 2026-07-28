import sys
import unittest
from pathlib import Path
from unittest import mock


TOOLS_ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(TOOLS_ROOT))

import report_particle_rendering as report  # noqa: E402


class ParticleRenderingReportTests(unittest.TestCase):
    def test_sprite_flag_decoder_preserves_zero_valued_defaults(self):
        self.assertEqual(
            {
                "facing": "2D_FACE_ME",
                "alignment": ["CENTER"],
                "rotate_around_centre": False,
                "rotate_around_z_axis": False,
                "lit": False,
                "modulate_2x": False,
                "disable_zbuffer": False,
                "unknown_bits": 0,
            },
            report.decode_sprite_flags(0),
        )
        decoded = report.decode_sprite_flags(0x3E1)
        self.assertEqual("3D_FACE_ME", decoded["facing"])
        self.assertTrue(decoded["rotate_around_centre"])
        self.assertTrue(decoded["rotate_around_z_axis"])
        self.assertTrue(decoded["lit"])
        self.assertTrue(decoded["modulate_2x"])
        self.assertTrue(decoded["disable_zbuffer"])

    def test_particle_group_blend_state_contract(self):
        self.assertEqual(
            {
                "src_blend": "ONE",
                "dest_blend": "ONE",
                "blend_op": "ADD",
            },
            report.particle_group_d3d9_state(3, 0),
        )
        self.assertEqual(
            {
                "src_blend": "ONE",
                "dest_blend": "INVSRCCOLOR",
                "blend_op": "REVSUBTRACT",
            },
            report.particle_group_d3d9_state(4, 2),
        )
        self.assertEqual(
            {
                "src_blend": "SRCALPHA",
                "dest_blend": "INVSRCALPHA",
                "blend_op": "SUBTRACT",
            },
            report.particle_group_d3d9_state(2, 1),
        )

    def test_two_crosses_are_quarter_turn_apart(self):
        self.assertEqual([0.125, 0.375], report.crossed_turn_offsets(2))

    def test_single_sprite_crosses_are_opt_in(self):
        system = {
            "Name": "rainbow",
            "Components": [{
                "Class": "CPSCSingleSprite",
                "InstanceID": 4,
                "Enabled": True,
                "SpriteBankIndex": 99,
                "NoCrossedSprites": 4,
                "CrossedSprites": False,
                "BlendMode": 4,
                "BlendOp": 0,
            }],
        }
        result = report._component_report(
            1, "TEST", system, system["Components"][0], {}
        )
        self.assertEqual(0, result["crossed_sprite_count"])
        self.assertEqual([], result["crossed_turn_offsets"])

    def test_component_joins_aspect_uv_and_orientation(self):
        update = {
            "Class": "CPSCUpdateNormal",
            "Enabled": True,
            "SetOrientationFromDirection": True,
            "SetOrientationFromGame": False,
        }
        renderer = {
            "Class": "CPSCRenderSprite",
            "InstanceID": 7,
            "Enabled": True,
            "SpriteBankIndex": 4422,
            "BlendMode": 4,
            "BlendOp": 0,
            "SpriteFlags": 0,
            "NoCrossedSprites": 2,
        }
        texture = {
            "id": 4422,
            "name": "SPRITE_CANDLE_FLAME_01",
            "format": "DXT1",
            "width": 32,
            "height": 64,
            "allocated_width": 128,
            "allocated_height": 128,
            "aspect_height_over_width": 2.0,
            "uv_scale_u": 0.25,
            "uv_scale_v": 0.5,
        }
        result = report._component_report(
            137,
            "CANDLE_FLAME",
            {"Name": "flame", "Components": [update, renderer]},
            renderer,
            {4422: texture},
        )
        self.assertEqual(2.0, result["sprite"]["aspect_height_over_width"])
        self.assertEqual("ADDSMOOTH", result["blend_mode_name"])
        self.assertEqual("ADD", result["blend_op_name"])
        self.assertEqual(
            {
                "src_blend": "ONE",
                "dest_blend": "INVSRCCOLOR",
                "blend_op": "ADD",
            },
            result["normal_pass_d3d9_state"],
        )
        self.assertEqual(
            "2D_FACE_ME", result["decoded_sprite_flags"]["facing"]
        )
        self.assertEqual(
            ["CENTER"], result["decoded_sprite_flags"]["alignment"]
        )
        self.assertEqual([0.125, 0.375], result["crossed_turn_offsets"])
        self.assertTrue(result["orientation_from_direction"])
        self.assertIn(
            "sample only real/allocated UV scale, not the full allocation",
            result["diagnostics"],
        )

    @mock.patch.object(report.parse_texture, "iter_texture_entries")
    def test_texture_descriptor_uses_real_over_allocated(self, entries):
        info = bytearray(34)
        info[0:2] = (256).to_bytes(2, "little")
        info[2:4] = (256).to_bytes(2, "little")
        info[6:8] = (64).to_bytes(2, "little")
        info[8:10] = (64).to_bytes(2, "little")
        info[10:12] = (1).to_bytes(2, "little")
        info[12] = 0x1F
        info[17] = 6
        entries.return_value = iter([(
            b"",
            {"name": "PARTICLE_TEXTURES"},
            {
                "id": 4517,
                "name": "SPRITE_WATER_VAPOUR_01",
                "infosize": 34,
                "info": bytes(info),
            },
        )])
        texture = report.load_texture_descriptors("unused")[4517]
        self.assertEqual(0.25, texture["uv_scale_u"])
        self.assertEqual(0.25, texture["uv_scale_v"])


if __name__ == "__main__":
    unittest.main()
