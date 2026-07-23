# FableForge UI/UX research notes

*Created 2026-07-19. Background web research on modding/editor tools, plus
current FableForge constraints. This is a design brief, not a visual mockup.*

## Sources checked

- Bethesda/Creation Kit workflow references:
  - Creation Kit wiki landing page: https://www.creationkit.com/
  - TES Alliance CK basics tutorial:
    https://tesalliance.org/forums/index.php?/topic/6838-ck-basics-lesson-1/
  - CK tutorial describing Object Window, Cell View, and Render Window:
    https://voidwalkergame.wordpress.com/bethesda-ck-tutorial-2/
- xEdit:
  - xEdit overview: https://tes5edit.github.io/docs/2-overview.html
  - xEdit conflict detection:
    https://tes5edit.github.io/docs/5-conflict-detection-and-resolution.html
- Valve Hammer:
  - Valve Developer Community Hammer page:
    https://developer.valvesoftware.com/wiki/Valve_Hammer_Editor
  - Hammer 3D/2D views:
    https://documentation.help/Valve-Hammer-Editor-3.4/The_3D_and_2D_Views.htm
  - Hammer entity properties:
    https://developer.valvesoftware.com/wiki/Hammer_Object_Properties_Dialog
- Blender:
  - Viewport gizmos:
    https://docs.blender.org/manual/en/latest/editors/3dview/display/gizmo.html
  - Properties editor:
    https://docs.blender.org/manual/en/latest/editors/properties_editor.html
- Unreal:
  - Level Editor:
    https://dev.epicgames.com/documentation/unreal-engine/level-editor-in-unreal-engine
  - World Outliner:
    https://dev.epicgames.com/documentation/unreal-engine/outliner-in-unreal-engine
- Unity:
  - Editor interface tutorial:
    https://learn.unity.com/tutorial/explore-the-editor-interface-1-1
  - Inspector manual:
    https://docs.unity3d.com/2022.3/Documentation/Manual/UsingTheInspector.html
- REDkit:
  - Official REDkit overview:
    https://cdprojektred.atlassian.net/wiki/spaces/W3REDkit/overview
  - World creation/editing:
    https://cdprojektred.atlassian.net/wiki/spaces/W3REDkit/pages/6325171
  - Quest graph tutorial:
    https://cdprojektred.atlassian.net/wiki/spaces/W3REDkit/pages/6327243/HOW-TO+Create+a+new+quest
- OpenIV:
  - Main site: https://openiv.com/
  - Mods-folder safety workflow:
    https://openiv.com/?p=1132
- Neverwinter Nights/Aurora:
  - Toolset overview:
    https://nwn.fandom.com/wiki/Toolset
  - Conversation tutorial:
    https://neverwintervault.org/article/tutorial/conversation-tutorial
- Starfield Creation Kit:
  - Steam page:
    https://store.steampowered.com/app/2722710/Starfield_Creation_Kit/

## Patterns to adopt

### 1. The editor backbone should be spatial

Creation Kit, Hammer, Unreal, REDkit, and Blender all put a viewport near the
center of the workflow. The surrounding panes exist to answer three questions:

- What world/cell/level am I editing?
- What object is selected?
- What properties/scripts/assets are attached to it?

For FableForge, the default workspace should be:

- **World / Level Browser** on the left.
- **3D Render View** in the center.
- **Selection / Details Inspector** on the right.
- **Entity List / Script Refs / Problems** as bottom tabs.

Tables stay important, but they should support spatial editing instead of being
the first thing a new user sees.

### 2. Keep simple and advanced modes in the same place

Good modding tools do not hide power, but they do separate common intent from raw
record detail.

FableForge should show:

- Common controls first: move object, duplicate, chest rewards, silver-key count,
  linked script, dialogue text.
- Advanced record view one click away: raw TNG fields, CTC blocks, game.bin
  payload fields, byte offsets, conflict status.

This avoids the v2 problem: Creation Kit/xEdit density without the landing shock.

### 2b. Provide layout presets instead of one perfect layout

The background research pass found the same pattern in Unity, Unreal, GECK/CK,
Hammer, REDkit, and Aurora-like toolsets: different jobs need different pane
weighting. FableForge should ship layout presets, not only a single docked
layout:

- **World building:** large 3D viewport, outliner, placement palette, transform
  inspector.
- **Quest authoring:** quest graph/timeline, script refs, dialogue panel, small
  world preview.
- **Conflict review:** records/overrides table, diff inspector, validation panel.
- **Packaging:** staged files, manifest, original/modified status, publish
  checklist.
- **Beginner:** Home + common tasks + simplified inspector.

### 3. Outliner/list and viewport selection must be symmetric

Unreal's Outliner and Blender's Outliner both make selection work from either the
tree/list or the viewport. Hammer's entity report is also a fast way to locate
and jump to entities.

FableForge needs the same rule:

- click an entity in the 3D viewport -> highlight it in the entity list;
- click a row in the entity list -> frame it in the viewport;
- click a script ref -> select the owning entity and show attached script data;
- click a problem -> jump to the broken record/entity.

### 4. Mod safety is a first-class UI concern

xEdit is powerful because it makes records, overrides, and conflicts visible.
OpenIV is notable for its "mods folder" style safety workflow. FableForge already
has `forge stage` / `forge unstage`; the GUI should elevate that instead of
burying it.

Required UI:

- visible active mod workspace;
- original vs modified status;
- per-file staged/unstaged state;
- validation panel before apply;
- one-click rollback backed by the staging manifest;
- conflicts colored semantically, not decoratively.

Color should never be the only signal. Conflict/status rows need text labels
such as `modified`, `overridden`, `missing reference`, `staged`, and `base game`
because xEdit-style color semantics are powerful but expensive to learn.

### 5. 3D manipulation should use familiar gizmos

Blender/Unreal-style transform gizmos are more intuitive than numeric-only TNG
editing:

- translate arrows/planes;
- rotate rings;
- snap toggle;
- local/world transform toggle;
- numeric transform fields in the inspector;
- duplicate, delete, frame selection, isolate selection.

For Fable TLC specifically, the first editable 3D objects should be TNG
placements: markers, camera points, chests, doors, creatures, spawn points, and
quest/script anchors. Terrain sculpting can come later because LEV write-back is
riskier than TNG transform edits.

### 6. Script and quest UX should be contextual

REDkit quest graphs and Aurora/NWN conversation tooling both point to the same
principle: story content needs a graph/tree view, but it must stay connected to
the world.

FableForge should make scripts approachable by anchoring them to selected
entities:

- select an NPC, marker, trigger, or camera point;
- see attached `ScriptName` / `ScriptData`;
- show related `script.bin` cutscene/region/script records when known;
- show FSE/native API calls as readable actions;
- open "Edit as graph" for quest/cutscene authoring;
- keep a raw command/list view for expert debugging.

## Patterns to avoid

- A corporate dashboard as the primary screen. FableForge is an editor, not an
  analytics product.
- A raw xEdit tree as the first screen. It is powerful, but hostile to new users.
- Emoji or OS-font icons. Use vector icons only; previous mockups produced tofu
  and mojibake in the artifact renderer.
- White/native scrollbars. Dark themed tool surfaces need custom dark scrollbars
  or native ImGui styling.
- Single-mode UI. Beginners need task workflows; expert modders need raw record
  access. Treat these as two faces of the same selected object.

## FableForge-specific UX target

The product should feel like:

- Creation Kit's spatial level editing;
- xEdit's record truth and conflict honesty;
- Blender/Unreal's viewport/outliner/details loop;
- OpenIV's safe staging/rollback mindset;
- Aurora/NWN's approachable conversation/script authoring.

It should not imitate any one tool. The right FableForge identity is a modern,
dark, purple-accented world editor where the 3D view is approachable, and the raw
record truth is always available when needed.
