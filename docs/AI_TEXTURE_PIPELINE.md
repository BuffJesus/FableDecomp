# AI texture pipeline — generate game textures from existing ones

`tools/ai_texture.py` is the generative middle step between the two halves of
`tools/texture_build.py` (which already `decode`s existing textures to PNG and `build`s/
`replace`s PNGs back into the game's DXT format). Built + verified 2026-08-16.

## Security
The OpenAI key is read from `OPENAI_API_KEY` **only** — never stored, logged, or committed.
**Rotate any key shared in plaintext.** Set it per-session:
- PowerShell: `$env:OPENAI_API_KEY = "sk-..."`
- bash: `export OPENAI_API_KEY=sk-...`

## Three-step pipeline (quest-card orb example)
The card orbs are `HUD_ORB_QUEST_CORE/OPTIONAL/VIGNETTE` in
`data/graphics/pc/textures.big` (sub-bank GBANK_MAIN_PC), **DXT3 32×32** (texture ids
5892/5894/5896 — the `States[0].GraphicIndex` of the game.bin CUIDefs; see
docs/QUEST_CARD_TEXTURE_BINDING.md).

```
# 1. extract the existing texture to PNG
python tools/texture_build.py decode "<install>/data/graphics/pc/textures.big" \
    HUD_ORB_QUEST_CORE work/orb_ref.png

# 2. re-imagine it with generative AI (edit mode keeps shape/theme; --transparent for sprites)
python tools/ai_texture.py \
    "a glowing emerald quest orb, ornate gold rim, dark radiant center, game UI sprite, centered" \
    --ref work/orb_ref.png --out work/orb_new.png --final-size 32 --transparent

# 3a. encode to game format (standalone .tex/.info prefix)
python tools/texture_build.py build work/orb_new.png work/orb_new --format dxt3
# 3b. OR splice directly into a copy of the bank, replacing the entry
python tools/texture_build.py replace "<install>/.../textures.big" out/textures.big \
    HUD_ORB_QUEST_CORE work/orb_new.png --format dxt3
```

`ai_texture.py` modes:
- `--ref IMG...` → image **edit** (re-imagine existing texture(s); prompt required). Multiple
  refs allowed.
- no `--ref` → pure **generation** from the prompt.
- `--final-size N` downscales each result to N×N (use the texture's real size, power of 2).
- `--transparent` requests an alpha background (orbs/icons/sprites).
- `--n K` produces K variations (`out_1.png`, `out_2.png`, …).
- `--model` defaults to `gpt-image-1` (supports transparency + multi-ref edits).

## Status / requirements
- Tool built and verified: authenticates and reaches the API; the pipeline (decode → AI →
  encode) is wired and the orb reference extracts clean (DXT3 32×32).
- **Needs OpenAI credits.** A live run currently returns HTTP 429 `insufficient_quota`
  ("no credits remaining") — add credits at platform.openai.com billing, then the commands
  above produce textures. (The key also authenticated, confirming it is live — rotate it.)
- For UI orbs, 32×32 is tiny; the model renders at 1024 and downscales — expect a stylized,
  not pixel-faithful, result. Larger card art (e.g. a 256² card background) has more headroom.
