Idea:
- Roquelike
 -> PRG dungeons, permadeath, ...
- 2D topdown
- medieval/fantasy setting
- weapons/armor are color-coded: 6 colors RGB + CYM
- weapons are (more) effective if colors are opposite
- armor is (more) effective if colors match

what's working so far:
- build for web and SDL (linux?)
- graphics + motion (no art)
- level generation (walls & stairs only, no multi-layer dungeons)

open points:
- populate level
- starting point for player
- AI
- collision (from level layout)
- items
- subitems/chests/loot
- ART

Used libraries:
- emscripten (to build wasm/HTML5)
- sdl2 (input, sound and window management)
- opengl (graphics)
- C++ stdlib
