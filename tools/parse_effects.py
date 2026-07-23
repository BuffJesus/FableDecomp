#!/usr/bin/env python3
r"""Fable TLC particle-effects bank reader — data\Misc\pc\effects.big.

Container: standard BIGB (see parse_bigb.py / TEXTBIG_FORMAT.md), one sub-bank
PARTICLE_MAIN_PC (1,165 entries, all type 0, uncompressed payloads).
Entry payload = one serialized CParticleEmitter (see docs/EFFECTS_FORMAT.md):

  u32   magic (0x64 = bank version)
  cstr  name
  7x  EBOOL emitter flags (2D / preWater / water / zwrite / continuous /
        screenDisplacement / readZ)
  5x  float distances+fades, i32 priority, 5x EBOOL more flags
  u32   systemCount, then per system:
    cstr name, 2x EBOOL, 3x float scale, u32 componentCount, then per component:
      cstr className, u32 instanceID, EBOOL enabled, <component fields>,
      u8 terminator (0x7B)
    u8 terminator (0x26)

Component field layouts are byte-ports of EgoCore ParticleParser.h
(C:\Users\Cornelio\Documents\EgoCoreInspect\EgoCore-master\EgoCore\Particles\),
which is the community answer key. Quantized fields ("qN,scale[,bias]") are
stored as u32 and mapped to float as  raw/N*scale - bias.

Entry id == EParticleEmitter enum value in
data\Defs\RetailHeaders\pc\particles.h — the id game defs reference
(e.g. OBJECT CSmokeGeneratorDef.ParticleEmitter, CChestDef.OpenParticleEffect).

Usage:
  python parse_effects.py <effects.big> --list
  python parse_effects.py <effects.big> --extract <out.json> [--id N] [--name SUBSTR]
  python parse_effects.py <effects.big> --validate
"""
import argparse
import json
import os
import struct
import sys

_HERE = os.path.dirname(os.path.abspath(__file__))
if _HERE not in sys.path:
    sys.path.insert(0, _HERE)
import parse_bigb


class Stream:
    def __init__(self, buf):
        self.b = buf
        self.p = 0

    def u32(self):
        v = struct.unpack_from('<I', self.b, self.p)[0]; self.p += 4; return v

    def i32(self):
        v = struct.unpack_from('<i', self.b, self.p)[0]; self.p += 4; return v

    def f32(self):
        v = struct.unpack_from('<f', self.b, self.p)[0]; self.p += 4; return v

    def ebool(self):
        v = self.b[self.p] != 0; self.p += 1; return v

    def byte(self):
        v = self.b[self.p]; self.p += 1; return v

    def colour(self):  # stored B,G,R,A
        b_, g, r, a = self.b[self.p:self.p + 4]; self.p += 4
        return [r, g, b_, a]

    def vec3(self):
        return [self.f32(), self.f32(), self.f32()]

    def cstr(self):
        e = self.b.index(b'\0', self.p)
        s = self.b[self.p:e].decode('latin1')
        self.p = e + 1
        return s


def _q(s, maxq, scale, bias=0.0):
    """Quantized float: u32 raw -> raw/maxq*scale - bias."""
    return s.u32() / maxq * scale - bias


# ---------------------------------------------------------------- components

def parse_spline_base(s):
    d = {}
    d['ControlPoints'] = [s.vec3() for _ in range(s.u32())]
    if s.ebool():
        d['OffsetOnlyAtStartup'] = s.ebool()
        d['ResetEachFrame'] = s.ebool()
        d['RandomOffsets'] = [{'Limit': s.vec3(), 'Speed': s.vec3()}
                              for _ in range(s.u32())]
    if s.ebool():
        d['MaintainSplineShape'] = s.ebool()
        d['PosParams'] = [s.u32() for _ in range(s.u32())]
    return d


def parse_render_sprite(s):
    d = {}
    d['SpriteBankIndex'] = s.i32(); d['TrailBankIndex'] = s.i32()
    d['StartColour'] = s.colour(); d['MidColour'] = s.colour(); d['EndColour'] = s.colour()
    for k in ('BlendMode', 'TrailBlendMode', 'BlendOp', 'TrailBlendOp',
              'SpriteFlags', 'FadeInEnd', 'FadeOutBegin', 'TrailLength',
              'FlickerMinAlpha', 'FlickerMinSize', 'NoCrossedSprites'):
        d[k] = s.u32()
    d['StartRenderSize'] = _q(s, 2047, 20)
    d['AlphaFadeMinimum'] = _q(s, 127, 1)
    d['EndRenderSize'] = _q(s, 2047, 20)
    d['FlickerBias'] = _q(s, 255, 2, 1)
    d['AnimationTimeSecs'] = s.u32() / 16383 * 99.9 + 0.1
    d['SizeFadeMinimum'] = _q(s, 127, 1)
    d['TrailWidth'] = _q(s, 1023, 10)
    d['FlickerSpeed'] = _q(s, 4095, 30)
    for k in ('UseStartColour', 'UseMidColour', 'UseEndColour', 'AlphaFadeEnable',
              'SizeFadeEnable', 'FlickerEnable', 'ForceAnimationTime'):
        d[k] = s.ebool()
    return d


def parse_emitter_generic(s):
    d = {}
    for k in ('EmitterPosParam', 'DirectionParamName', 'NoParticlesToStart',
              'NoParticlesToStartRand', 'EmitterType'):
        d[k] = s.u32()
    for k in ('Solid', 'UseEmitterLifeSecs', 'UseEmitterTimelineSecs',
              'OrientationXY', 'OrientationXZ', 'OrientationYZ',
              'UseCustomDirection', 'UseOutwardDirection', 'UseParamDirection',
              'OppositeDirection'):
        d[k] = s.ebool()
    d['AngularPerturbation'] = s.u32()
    cd = [None, None, None]
    cd[0] = (s.u32() & 0xFF) / 255 * 2 - 1
    d['ParticlesPerSecond'] = _q(s, 16383, 100)
    d['EmitterSize'] = _q(s, 1023, 10)
    d['RadialBias'] = _q(s, 1023, 10, 5)
    d['MinSpeed'] = _q(s, 1023, 10)
    d['UseForwardDirection'] = s.ebool()
    d['UseRandom2DDirection'] = s.ebool()
    cd[1] = (s.u32() & 0xFF) / 255 * 2 - 1
    cd[2] = (s.u32() & 0xFF) / 255 * 2 - 1
    d['CustomDirection'] = cd
    d['EmitterTimelineSecs'] = _q(s, 32767, 30)
    d['UseRandom3DDirection'] = s.ebool()
    d['EmitterLifeSecs'] = _q(s, 32767, 300)
    d['EmitterStartTime'] = _q(s, 32767, 300)
    d['MaxSpeed'] = _q(s, 1023, 10)
    d['NonUniformScaling'] = [_q(s, 2047, 20, 10) for _ in range(3)]
    if s.ebool():
        d['SplineTension'] = s.f32()
        d['SplineControlPoints'] = [s.vec3() for _ in range(s.u32())]
    return d


def parse_update_normal(s):
    d = {}
    d['FadeInEnd'] = s.u32(); d['FadeOutBegin'] = s.u32()
    for k in ('UseParticleLifeSecs', 'UseRandomRotationAxis',
              'UseRandomInitialRotation', 'UseAccelerationParam',
              'StayWithEmitter', 'UseSystemLifeSecs', 'SystemAlphaFadeEnable',
              'EmissionFadeEnable', 'ParticleCollideWithGround',
              'ParticleCollideWithAnything', 'ParticleDieOnCollision',
              'UseAllAttractors', 'RandomisePosEnable',
              'RandomisePosDistVaryEnable', 'SetOrientationFromDirection',
              'SetOrientationFromGame', 'ParticleCreateDecal',
              'ParticleCreateDecalEmitter'):
        d[k] = s.ebool()
    d['DecalEmitterName'] = s.cstr()
    for k in ('SystemLifeSecs', 'ParticleLifeSecs', 'WindFactor',
              'GravityFactor', 'AirResistance', 'ParticleAccelerationScale',
              'InitialRotationX', 'InitialRotationY', 'InitialRotationZ',
              'RotationMinAngleSpeed', 'RotationMaxAngleSpeed',
              'ParticleBounce', 'SystemAlphaFadeMinimum',
              'RandomisePosDistVaryScale', 'EmissionFadeMinimum'):
        d[k] = s.f32()
    for k in ('ParticleSystemOffset', 'ParticleAcceleration', 'RotationAxis',
              'RandomisePosSpeed', 'RandomisePosScale'):
        d[k] = s.vec3()
    d['AccelerationParam'] = s.u32(); d['OrientFromGameParam'] = s.u32()
    return d


def parse_decal_renderer(s):
    d = {}
    d['DecalLifeSecs'] = s.f32()
    d['DecalBankIndex'] = s.i32()
    d['StartColour'] = s.colour(); d['MidColour'] = s.colour(); d['EndColour'] = s.colour()
    for k in ('BlendMode', 'BlendOp', 'SpriteAlignment', 'FadeInEnd',
              'FadeOutBegin', 'StartRenderSizeInt', 'AlphaFadeMinimumInt',
              'EndRenderSizeInt', 'AnimationTimeSecsInt', 'SizeFadeMinimumInt'):
        d[k] = s.u32()
    for k in ('UseStartColour', 'UseMidColour', 'UseEndColour',
              'AlphaFadeEnable', 'SizeFadeEnable', 'ForceAnimationTime'):
        d[k] = s.ebool()
    for k in ('FlickerMinAlphaInt', 'FlickerMinSizeInt', 'FlickerBiasInt',
              'FlickerSpeedInt'):
        d[k] = s.u32()
    for k in ('FlickerEnable', 'PoolingEnable', 'PoolIncreaseAlphaEnable',
              'PoolIncreaseSizeEnable', 'PoolIncreaseLifeEnable'):
        d[k] = s.ebool()
    d['MaxPoolSize'] = s.f32(); d['MaxPoolAlpha'] = s.f32(); d['MaxPoolLife'] = s.f32()
    d['PoolIncreaseFrameEnable'] = s.ebool()
    d['PoolFrameIncreaseRate'] = s.f32(); d['StencilCubeWidth'] = s.f32()
    return d


def parse_spline(s):
    d = {}
    d['SplineBounce'] = s.ebool()
    d['ScaleSplineSpeed'] = s.ebool()
    d['SplineAnimSpeed'] = s.f32()
    d.update(parse_spline_base(s))
    return d


def parse_single_sprite(s):
    d = {}
    d['SpriteBankIndex'] = s.i32()
    d['StartColour'] = s.colour(); d['MidColour'] = s.colour(); d['EndColour'] = s.colour()
    d['AnimationTimeSecs'] = s.f32(); d['StartRenderSize'] = s.f32(); d['EndRenderSize'] = s.f32()
    d['SpriteAlignment'] = s.i32(); d['NoCrossedSprites'] = s.i32()
    d['InitialAngle'] = s.f32()
    d['FadeInEnd'] = s.i32(); d['FadeOutBegin'] = s.i32()
    d['AlphaFadeMinimum'] = s.f32(); d['SizeFadeMinimum'] = s.f32()
    d['PositionParam'] = s.u32()
    d['BlendMode'] = s.i32(); d['BlendOp'] = s.i32()
    d['TrailBlendMode'] = s.i32(); d['TrailBlendOp'] = s.i32(); d['TrailBankIndex'] = s.i32()
    d['TrailWidth'] = s.f32()
    d['MaxTrailLength'] = s.i32(); d['StayWithEmitterIntFactor'] = s.i32()
    for k in ('UseStartColour', 'UseMidColour', 'UseEndColour',
              'SelfIlluminating', 'AlphaFadeEnable', 'RotateAroundCentre',
              'FaceMe2D', 'FaceMe3D', 'CrossedSprites', 'SizeFadeEnable',
              'ForceAnimationTime', 'StayWithEmitter', 'UsePosition',
              'UseSplinePoints'):
        d[k] = s.ebool()
    d.update(parse_spline_base(s))
    return d


def parse_orbit(s):
    d = {'CentreParam': s.u32(), 'Orbits': []}
    for _ in range(3):
        o = {}
        o['Type'] = s.i32()
        o['Enabled'] = s.u32() != 0
        o['Radius'] = s.f32(); o['Expand'] = s.f32()
        o['CycleFlag'] = s.u32() != 0
        for k in ('CycleTime', 'SqueezeScale', 'SqueezeAngle', 'RotateSpeed',
                  'RotateStart', 'RotateSpeedRandom', 'RotateStartRandom'):
            o[k] = s.f32()
        d['Orbits'].append(o)
    return d


def parse_attractor(s):
    d = {}
    d['AttractorEnabled'] = s.ebool()
    d['AttractorUseParamPosition'] = s.ebool()
    d['AttractorInfluenceFallOff'] = s.i32()
    d['AttractorPositionParam'] = s.u32()
    d['AttractorPositionParamName'] = s.u32()
    d['AttractorInfluenceRadius'] = s.f32()
    d['AttractorInfluenceForce'] = s.f32()
    d['AttractorUserPoints'] = [s.vec3() for _ in range(s.u32())]
    return d


def parse_light(s):
    d = {}
    d['LightPositionParam'] = s.u32()
    for k in ('LightLifeSecs', 'LightRespawnDelaySecs', 'LightStartTime',
              'LightTimelineSecs', 'LightStartRenderWorldRadius',
              'LightEndRenderWorldRadius'):
        d[k] = s.f32()
    d['LightAttenuationFactor'] = s.i32()
    d['LightFadeInEnd'] = s.i32(); d['LightFadeOutBegin'] = s.i32()
    d['LightWorldRadiusFadeMinimum'] = s.f32()
    d['LightColourFadeMinimum'] = s.colour()
    for k in ('LightWorldRadiusFadeEnable', 'LightUseLifeSecs', 'LightEnabled',
              'LightRespawns', 'LightColourFadeEnable', 'LightUseStartColour',
              'LightUseMidColour', 'LightUseEndColour', 'LightUseFadeColour',
              'LightUseTimelineSecs', 'LightHasInitializedTime'):
        d[k] = s.ebool()
    d['LightStartColour'] = s.colour()
    d['LightMidColour'] = s.colour()
    d['LightEndColour'] = s.colour()
    return d


def parse_render_mesh(s):
    d = {}
    d['BankIndex'] = s.i32(); d['TrailBankIndex'] = s.i32()
    d['TrailStartColour'] = s.colour(); d['TrailMidColour'] = s.colour()
    d['TrailEndColour'] = s.colour()
    d['StartColour'] = s.colour(); d['MidColour'] = s.colour(); d['EndColour'] = s.colour()
    for k in ('BlendMode', 'TrailBlendMode', 'BlendOp', 'TrailBlendOp',
              'FadeInEnd', 'FadeOutBegin', 'TrailLength', 'FlickerMinAlpha',
              'FlickerMinSize'):
        d[k] = s.u32()
    d['StartRenderSize'] = [_q(s, 2047, 20) for _ in range(3)]
    d['CentredOnPos'] = s.ebool(); d['AlphaFadeEnable'] = s.ebool()
    d['EndRenderSize'] = [_q(s, 2047, 20) for _ in range(3)]
    for k in ('SizeFadeEnable', 'FlickerEnable', 'TrailUseStartColour',
              'TrailUseMidColour', 'TrailUseEndColour', 'UseRenderSizeParam'):
        d[k] = s.ebool()
    d['FlickerBias'] = _q(s, 255, 2, 1)
    d['FlickerSpeed'] = _q(s, 4095, 30)
    d['AlphaFadeMinimum'] = _q(s, 127, 1)
    d['UseStartColour'] = s.ebool()
    d['SizeFadeMinimum'] = _q(s, 127, 1)
    d['UseMidColour'] = s.ebool()
    d['TrailWidth'] = _q(s, 1023, 10)
    d['UseEndColour'] = s.ebool()
    d['RenderSizeParam'] = s.u32()
    return d


COMPONENT_PARSERS = {
    'CPSCRenderSprite': parse_render_sprite,
    'CPSCUpdateNormal': parse_update_normal,
    'CPSCEmitterGeneric': parse_emitter_generic,
    'CPSCSpline': parse_spline,
    'CPSCSingleSprite': parse_single_sprite,
    'CPSCRenderMesh': parse_render_mesh,
    'CPSCLight': parse_light,
    'CPSCAttractor': parse_attractor,
    'CPSCOrbit': parse_orbit,
    'CPSCDecalRenderer': parse_decal_renderer,
}


# ------------------------------------------------------------------- emitter

def parse_emitter(buf, strict=True):
    """Parse one effects.big entry payload -> dict."""
    s = Stream(buf)
    e = {}
    e['Magic'] = s.u32()
    e['Name'] = s.cstr()
    for k in ('Emitter2D', 'PreWaterEffect', 'WaterEffect', 'ZBufferWriteable',
              'ContinuousEmitter', 'IsScreenDisplacement', 'ReadZBuffer'):
        e[k] = s.ebool()
    for k in ('MaxSpawnDistance', 'MaxDrawDistance', 'FadeOutStart',
              'FadeInEnd', 'FadeInStart'):
        e[k] = s.f32()
    e['Priority'] = s.i32()
    for k in ('DieIfOffscreen', 'OffscreenUpdate', 'ClipEffectToWeatherMask',
              'EnableDithering', 'CalcBoundingSphereOnceOnly'):
        e[k] = s.ebool()
    e['Systems'] = []
    n_sys = s.u32()
    for _ in range(n_sys):
        sysd = {'Name': s.cstr(), 'Enabled': s.ebool(),
                'ScaleParticles': s.ebool(), 'Scale': s.vec3(),
                'Components': []}
        n_comp = s.u32()
        for _ in range(n_comp):
            cname = s.cstr()
            comp = {'Class': cname, 'InstanceID': s.u32(),
                    'Enabled': s.ebool()}
            fn = COMPONENT_PARSERS.get(cname)
            if fn is None:
                raise ValueError(f'unknown component class {cname!r} '
                                 f'at 0x{s.p:x} in {e["Name"]!r}')
            comp.update(fn(s))
            term = s.byte()  # retail writes 0x7B here
            if strict and term not in (0x7B,):
                raise ValueError(f'bad component terminator 0x{term:02x} '
                                 f'after {cname} at 0x{s.p:x}')
            sysd['Components'].append(comp)
        term = s.byte()  # retail writes 0x26 here
        if strict and term not in (0x26,):
            raise ValueError(f'bad system terminator 0x{term:02x} at 0x{s.p:x}')
        e['Systems'].append(sysd)
    e['_consumed'] = s.p
    return e


# ----------------------------------------------------------------------- CLI

def load_bank(path):
    b = open(path, 'rb').read()
    magic, ver, foff, fsize = parse_bigb.parse_header(b)
    if magic != b'BIGB':
        raise SystemExit(f'{path}: not a BIGB container')
    subs, _ = parse_bigb.parse_footer(b, foff)
    entries = []
    for sub in subs:
        es, stats, end, err = parse_bigb.parse_toc(b, sub, foff)
        if err:
            raise SystemExit(f'TOC error in {sub["name"]}: {err}')
        entries.extend(es)
    return b, subs, entries


def main():
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument('bank', help=r'path to effects.big')
    ap.add_argument('--list', action='store_true', help='list entries')
    ap.add_argument('--extract', metavar='OUT.json',
                    help='decode entries to JSON')
    ap.add_argument('--validate', action='store_true',
                    help='strict-parse every entry, report coverage')
    ap.add_argument('--id', type=int, help='only this entry id')
    ap.add_argument('--name', help='only entries whose name contains this '
                    '(case-insensitive)')
    args = ap.parse_args()

    b, subs, entries = load_bank(args.bank)
    if args.id is not None:
        entries = [e for e in entries if e['id'] == args.id]
    if args.name:
        needle = args.name.lower()
        entries = [e for e in entries if needle in e['name'].lower()]

    if args.list:
        for e in entries:
            print(f"{e['id']:5d}  {e['size']:7d}B  {e['name']}")
        print(f'{len(entries)} entries')
        return

    if args.validate:
        ok = bad = slack = 0
        classes = {}
        for e in entries:
            payload = b[e['offset']:e['offset'] + e['size']]
            try:
                em = parse_emitter(payload)
                if em['_consumed'] != len(payload):
                    slack += 1
                    print(f"SLACK id={e['id']} {e['name']}: "
                          f"consumed {em['_consumed']}/{len(payload)}")
                else:
                    ok += 1
                for sysd in em['Systems']:
                    for c in sysd['Components']:
                        classes[c['Class']] = classes.get(c['Class'], 0) + 1
            except Exception as ex:
                bad += 1
                print(f"FAIL id={e['id']} {e['name']}: {ex}")
        print(f'\n{ok} exact / {slack} slack / {bad} failed of {len(entries)}')
        print('component histogram:')
        for k in sorted(classes, key=classes.get, reverse=True):
            print(f'  {classes[k]:5d}  {k}')
        return

    if args.extract:
        out = []
        for e in entries:
            payload = b[e['offset']:e['offset'] + e['size']]
            em = parse_emitter(payload)
            em.pop('_consumed', None)
            out.append({'Id': e['id'], 'EntryName': e['name'], **em})
        with open(args.extract, 'w', encoding='utf-8') as f:
            json.dump(out, f, indent=1)
        print(f'wrote {len(out)} emitters -> {args.extract}')
        return

    ap.print_help()


if __name__ == '__main__':
    main()
