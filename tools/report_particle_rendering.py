#!/usr/bin/env python3
"""Report the retail rendering contract for selected Fable TLC particle effects.

This joins authored component data from effects.big with the real/allocated
texture dimensions from textures.big.  The output is intended for renderers:
it highlights the two sprite paths, crossed geometry, frame aspect ratio, and
the UV crop that must be preserved when an image occupies only part of its
allocated texture.

Examples:
  python tools/report_particle_rendering.py EFFECTS.BIG TEXTURES.BIG \
      --name CANDLE_FLAME --name WATERFALL --json work/particle_report.json
  python tools/report_particle_rendering.py EFFECTS.BIG TEXTURES.BIG \
      --id 137
"""

import argparse
import json
import os
import sys

_HERE = os.path.dirname(os.path.abspath(__file__))
if _HERE not in sys.path:
    sys.path.insert(0, _HERE)

import parse_effects
import parse_texture


RENDER_CLASSES = frozenset(("CPSCRenderSprite", "CPSCSingleSprite"))

BLEND_MODES = {
    0: "SOLID",
    1: "BOOLEAN_ALPHA",
    2: "ALPHA",
    3: "ADDITIVE",
    4: "ADDSMOOTH",
    5: "CONST_COLOUR",
}

BLEND_OPS = {
    0: "ADD",
    1: "SUB",
    2: "REVSUB",
    3: "ADDSIGNED",
    4: "REVSUBSIGNED",
}

# Normal colour-pass states selected by
# CEnginePrimitiveManagerParticleSpriteGroup::RenderSubPrimitive. These D3D9
# enum values were resolved through the Ego R PDB-backed decompile.
PARTICLE_GROUP_BLEND_STATES = {
    3: ("ONE", "ONE"),
    4: ("ONE", "INVSRCCOLOR"),
}
PARTICLE_GROUP_DEFAULT_BLEND_STATE = ("SRCALPHA", "INVSRCALPHA")

PARTICLE_GROUP_BLEND_OP_STATES = {
    1: "SUBTRACT",
    2: "REVSUBTRACT",
}
PARTICLE_GROUP_DEFAULT_BLEND_OP_STATE = "ADD"


def decode_sprite_flags(value):
    """Decode the PDB-backed EEngineSpriteFlag bit field."""
    if value is None:
        return None
    alignments = []
    for bit, name in (
        (0x02, "LEFT"),
        (0x04, "RIGHT"),
        (0x08, "TOP"),
        (0x10, "BOTTOM"),
    ):
        if value & bit:
            alignments.append(name)
    return {
        "facing": "3D_FACE_ME" if value & 0x01 else "2D_FACE_ME",
        "alignment": alignments or ["CENTER"],
        "rotate_around_centre": bool(value & 0x20),
        "rotate_around_z_axis": bool(value & 0x40),
        "lit": bool(value & 0x80),
        "modulate_2x": bool(value & 0x100),
        "disable_zbuffer": bool(value & 0x200),
        "unknown_bits": value & ~0x3FF,
    }


def particle_group_d3d9_state(blend_mode, blend_op):
    """Return the retail normal-pass D3D9 state for a batched sprite."""
    src, dest = PARTICLE_GROUP_BLEND_STATES.get(
        blend_mode, PARTICLE_GROUP_DEFAULT_BLEND_STATE
    )
    op = PARTICLE_GROUP_BLEND_OP_STATES.get(
        blend_op, PARTICLE_GROUP_DEFAULT_BLEND_OP_STATE
    )
    return {
        "src_blend": src,
        "dest_blend": dest,
        "blend_op": op,
    }


def crossed_turn_offsets(count, full_turn=False):
    """Return the retail centre-of-sector angle offsets, in turns.

    CPSCRenderSprite::PreparePrimitives adds the particle's authored angle to
    these values.  The ordinary crossed-sprite path covers half a turn because
    a two-sided plane is unchanged by a 180-degree rotation.
    """
    if count <= 0:
        return []
    sweep = 1.0 if full_turn else 0.5
    return [((index + 0.5) * sweep) / count for index in range(count)]


def load_texture_descriptors(path):
    """Map texture entry IDs to renderer-relevant descriptor fields."""
    textures = {}
    for _bank, subbank, entry in parse_texture.iter_texture_entries(path):
        if entry["infosize"] < 34:
            continue
        info = parse_texture.parse_info(entry["info"])
        if info is None:
            continue
        width = info["width"]
        height = info["height"]
        alloc_width = info["alloc_w"]
        alloc_height = info["alloc_h"]
        textures[entry["id"]] = {
            "id": entry["id"],
            "name": entry["name"],
            "subbank": subbank["name"],
            "format": info["fmt_name"],
            "format_id": info["fmt"],
            "width": width,
            "height": height,
            "allocated_width": alloc_width,
            "allocated_height": alloc_height,
            "mip_count": info["mipcount"],
            "aspect_height_over_width": (
                height / width if width else None
            ),
            "uv_scale_u": width / alloc_width if alloc_width else None,
            "uv_scale_v": height / alloc_height if alloc_height else None,
        }
    return textures


def _enabled_component(system, class_name):
    for component in system["Components"]:
        if component["Class"] == class_name and component["Enabled"]:
            return component
    return None


def _component_report(effect_id, effect_name, system, component, textures):
    class_name = component["Class"]
    texture_id = component["SpriteBankIndex"]
    crossed_enabled = (
        class_name == "CPSCRenderSprite"
        or bool(component.get("CrossedSprites"))
    )
    crossed_count = (
        int(component.get("NoCrossedSprites", 0))
        if crossed_enabled else 0
    )
    update = _enabled_component(system, "CPSCUpdateNormal")
    texture = textures.get(texture_id)

    diagnostics = []
    if texture is None:
        diagnostics.append("sprite texture descriptor was not found")
    else:
        if texture["aspect_height_over_width"] != 1.0:
            diagnostics.append(
                "quad height must be size * texture height/width"
            )
        if (
            texture["uv_scale_u"] != 1.0
            or texture["uv_scale_v"] != 1.0
        ):
            diagnostics.append(
                "sample only real/allocated UV scale, not the full allocation"
            )
        if texture["format"] == "DXT1":
            diagnostics.append(
                "DXT1 uses one-bit transparent texels in its alpha mode"
            )
        elif texture["format"] == "DXT3":
            diagnostics.append("DXT3 carries explicit four-bit alpha")
    if crossed_count:
        diagnostics.append(
            "emit distinct crossed planes; these are not animation frames"
        )

    result = {
        "effect_id": effect_id,
        "effect_name": effect_name,
        "system": system["Name"],
        "component_class": class_name,
        "instance_id": component["InstanceID"],
        "render_path": (
            "particle sprite group"
            if class_name == "CPSCRenderSprite"
            else "persistent/spline single sprite"
        ),
        "sprite": texture or {"id": texture_id, "missing": True},
        "blend_mode": component.get("BlendMode"),
        "blend_mode_name": BLEND_MODES.get(component.get("BlendMode")),
        "blend_op": component.get("BlendOp"),
        "blend_op_name": BLEND_OPS.get(component.get("BlendOp")),
        "normal_pass_d3d9_state": (
            particle_group_d3d9_state(
                component.get("BlendMode"), component.get("BlendOp")
            )
            if class_name == "CPSCRenderSprite" else None
        ),
        "sprite_flags": component.get("SpriteFlags"),
        "decoded_sprite_flags": (
            decode_sprite_flags(component.get("SpriteFlags"))
            if class_name == "CPSCRenderSprite" else None
        ),
        "sprite_alignment": component.get("SpriteAlignment"),
        "crossed_sprite_count": crossed_count,
        "crossed_turn_offsets": crossed_turn_offsets(crossed_count),
        "face_me_2d": component.get("FaceMe2D"),
        "face_me_3d": component.get("FaceMe3D"),
        "self_illuminating": component.get("SelfIlluminating"),
        "rotate_around_centre": component.get("RotateAroundCentre"),
        "orientation_from_direction": (
            update.get("SetOrientationFromDirection") if update else None
        ),
        "orientation_from_game": (
            update.get("SetOrientationFromGame") if update else None
        ),
        "start_colour_rgba": component.get("StartColour"),
        "mid_colour_rgba": component.get("MidColour"),
        "end_colour_rgba": component.get("EndColour"),
        "diagnostics": diagnostics,
    }
    return result


def build_report(effect_bank, texture_bank, ids=(), names=()):
    """Build a JSON-serialisable joined report for selected emitter entries."""
    effect_data, _subbanks, entries = parse_effects.load_bank(effect_bank)
    textures = load_texture_descriptors(texture_bank)
    selected_ids = set(ids)
    selected_names = tuple(name.casefold() for name in names)

    effects = []
    for entry in entries:
        if selected_ids or selected_names:
            id_match = entry["id"] in selected_ids
            name = entry["name"].casefold()
            name_match = any(needle in name for needle in selected_names)
            if not id_match and not name_match:
                continue
        payload = effect_data[
            entry["offset"]:entry["offset"] + entry["size"]
        ]
        emitter = parse_effects.parse_emitter(payload)
        renderers = []
        for system in emitter["Systems"]:
            for component in system["Components"]:
                if (
                    component["Enabled"]
                    and component["Class"] in RENDER_CLASSES
                ):
                    renderers.append(
                        _component_report(
                            entry["id"],
                            entry["name"],
                            system,
                            component,
                            textures,
                        )
                    )
        effects.append({
            "id": entry["id"],
            "name": entry["name"],
            "emitter_2d": emitter["Emitter2D"],
            "read_z_buffer": emitter["ReadZBuffer"],
            "z_buffer_writeable": emitter["ZBufferWriteable"],
            "renderers": renderers,
        })

    return {
        "contract": {
            "aspect": "quad height = size * frame height / frame width",
            "uv": "UV scale = real dimensions / allocated dimensions",
            "crossed_angles": (
                "base angle + centre of each sector over a half turn"
            ),
            "sprite_flags": (
                "EEngineSpriteFlag is PDB-backed; zero means 2D_FACE_ME "
                "and centered alignment"
            ),
            "colour_order": (
                "asset bytes are BGRA; parser/report values are RGBA"
            ),
            "blend_enums": (
                "EEnginePrimitiveBlendMode/Op names are recovered from the "
                "Ego R PDB; CPSCRenderSprite normal-pass D3D9 blend states "
                "are recovered from RenderSubPrimitive"
            ),
        },
        "effects": effects,
    }


def _print_text(report):
    for effect in report["effects"]:
        print(
            f'{effect["id"]:4d} {effect["name"]} '
            f'(readZ={effect["read_z_buffer"]}, '
            f'zwrite={effect["z_buffer_writeable"]})'
        )
        for renderer in effect["renderers"]:
            sprite = renderer["sprite"]
            if sprite.get("missing"):
                texture_text = f'texture {sprite["id"]} (missing)'
            else:
                texture_text = (
                    f'{sprite["id"]} {sprite["name"]} {sprite["format"]} '
                    f'{sprite["width"]}x{sprite["height"]} '
                    f'in {sprite["allocated_width"]}x'
                    f'{sprite["allocated_height"]}, '
                    f'UV={sprite["uv_scale_u"]:.6g},'
                    f'{sprite["uv_scale_v"]:.6g}, '
                    f'aspect={sprite["aspect_height_over_width"]:.6g}'
                )
            print(
                f'  {renderer["system"]}: {renderer["component_class"]}; '
                f'{texture_text}; blend={renderer["blend_mode"]}/'
                f'{renderer["blend_op"]} '
                f'({renderer["blend_mode_name"]}/'
                f'{renderer["blend_op_name"]}); '
                f'crosses={renderer["crossed_sprite_count"]}; '
                f'turns={renderer["crossed_turn_offsets"]}'
            )
            state = renderer["normal_pass_d3d9_state"]
            if state:
                print(
                    "    D3D9 normal pass: "
                    f'SRC={state["src_blend"]}, '
                    f'DEST={state["dest_blend"]}, '
                    f'OP={state["blend_op"]}'
                )
            flags = renderer["decoded_sprite_flags"]
            if flags:
                print(
                    "    sprite flags: "
                    f'facing={flags["facing"]}, '
                    f'alignment={"+".join(flags["alignment"])}, '
                    f'lit={flags["lit"]}, '
                    f'disableZ={flags["disable_zbuffer"]}'
                )
            for diagnostic in renderer["diagnostics"]:
                print(f'    - {diagnostic}')


def main():
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument("effects_bank", help="path to effects.big")
    parser.add_argument("texture_bank", help="path to textures.big")
    parser.add_argument(
        "--id", action="append", type=int, default=[],
        help="effect ID to report; may be repeated",
    )
    parser.add_argument(
        "--name", action="append", default=[],
        help="case-insensitive effect-name substring; may be repeated",
    )
    parser.add_argument("--json", metavar="PATH", help="also write JSON")
    args = parser.parse_args()

    if not args.id and not args.name:
        parser.error("select at least one effect with --id or --name")
    report = build_report(
        args.effects_bank,
        args.texture_bank,
        ids=args.id,
        names=args.name,
    )
    _print_text(report)
    if args.json:
        with open(args.json, "w", encoding="utf-8") as output:
            json.dump(report, output, indent=2)
            output.write("\n")
        print(f'wrote {len(report["effects"])} effects -> {args.json}')


if __name__ == "__main__":
    main()
