#!/usr/bin/env python3
"""spike_s1.py -- build the Phase 0 spike Lua stage (S1 + S3) for Oakvale Reborn.

Copies the New Oakvale readable stage (the Lua that played the childhood clean
on sidecar v4) to work/oakvale_reborn/spike_s1/lua and patches ONE file,
NOVI_LiveFather.lua, so that the Father's intro:

  S1  runs the APPENDED macro CS_OVR_SPIKE (script.bin entry authored by
      forge script cutscene-set) right before the retail CS_OAKVALE_INTRO_FATHER,
      inside the same movie session / actor map;
  S3  after the retail scene hands control back, plays a short Lua-driven
      camera beat (StartMovieSequence + CameraMoveToPosAndLookAtThing +
      CameraUseCameraPoint + EndMovieSequence).

Both are bracketed by quest:Log("OVR_SPIKE_S1/S3 ...") lines, so the in-game
verdict is a grep of NoviCompatibility/FableScriptExtender.log:
  ENTERING BLOCKING ... CS_OVR_SPIKE  -> the engine resolved the appended def
  OVR_SPIKE_S3: done ok=true          -> the camera bindings did not throw
Then: python tools/oakvale_reborn/build_custom_intro.py bundle --lua <stage> --tag spike-s1
"""
from __future__ import annotations

import argparse
import pathlib
import shutil

REPO = pathlib.Path(__file__).resolve().parents[2]
SOURCE = REPO / 'work/oakvale_readable_stage_20260916b'
OUT = REPO / 'work/oakvale_reborn/spike_s1/lua'
TARGET = 'NewOakValeIntro/Entities/NOVI_LiveFather.lua'

S1_ANCHOR = '            resources:RunMacro("CS_OAKVALE_INTRO_FATHER", actors, false, true)\n'
S1_PATCH = (
    '            -- Oakvale Reborn spike S1: the first APPENDED cutscene def.\n'
    '            quest:Log("OVR_SPIKE_S1: running appended macro CS_OVR_SPIKE")\n'
    '            resources:RunMacro("CS_OVR_SPIKE", actors, false, true)\n'
    '            quest:Log("OVR_SPIKE_S1: CS_OVR_SPIKE returned")\n'
) + S1_ANCHOR

S3_ANCHOR = (
    '        quest:CameraResetToViewBehindHero(0.0)\n'
    '        quest:CameraDefault()\n'
    '        local xbox = quest:IsXbox()\n'
)
S3_PATCH = (
    '        quest:CameraResetToViewBehindHero(0.0)\n'
    '        quest:CameraDefault()\n'
    '        -- Oakvale Reborn spike S3: a Lua-driven camera beat (no macro).\n'
    '        quest:Log("OVR_SPIKE_S3: start")\n'
    '        local s3ok, s3err = pcall(function()\n'
    '            local heroThing = quest:GetHero()\n'
    '            local pos = heroThing:GetPos()\n'
    '            quest:StartMovieSequence()\n'
    '            quest:CameraMoveToPosAndLookAtThing({ x = pos.x + 3.0, y = pos.y + 3.0, z = pos.z + 2.0 }, heroThing, 2.0)\n'
    '            quest:Pause(2.5)\n'
    '            quest:CameraUseCameraPoint("CAM_OVIF_SHOT2", heroThing, 1.5, 0, 0)\n'
    '            quest:Pause(2.0)\n'
    '            quest:EndMovieSequence()\n'
    '            quest:CameraResetToViewBehindHero(0.0)\n'
    '            quest:CameraDefault()\n'
    '        end)\n'
    '        quest:Log("OVR_SPIKE_S3: done ok=" .. tostring(s3ok) .. " err=" .. tostring(s3err))\n'
    '        local xbox = quest:IsXbox()\n'
)


def main() -> int:
    a = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    a.add_argument('--source', type=pathlib.Path, default=SOURCE)
    a.add_argument('--out', type=pathlib.Path, default=OUT)
    args = a.parse_args()
    if args.out.exists():
        shutil.rmtree(args.out)
    shutil.copytree(args.source, args.out, ignore=shutil.ignore_patterns('*.log'))
    path = args.out / TARGET
    text = path.read_text(encoding='utf-8')
    for anchor, patch, tag in ((S1_ANCHOR, S1_PATCH, 'S1'), (S3_ANCHOR, S3_PATCH, 'S3')):
        if text.count(anchor) != 1:
            raise SystemExit(f'{tag}: anchor not found exactly once in {TARGET}')
        text = text.replace(anchor, patch)
    path.write_text(text, encoding='utf-8', newline='\n')
    print(f'spike stage: {args.out}')
    print(f'patched: {TARGET} (S1 + S3)')
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
