# Frankenstein

Contents:
* [What is it?](#what-is-it)
* [World Editor](#world-editor)
  * [How to configure World Editor](#how-to-configure-world-Editor)

<details>
  <summary></summary>

</details>

## What is it?

This is a 3D multiplayer shooter with two game modes: deathmatch and team deathmatch. The game is being created from scratch using C++ and OpenGL. Along with the game comes a level editor called World Editor. This is just my portfolio project, so don't expect anything incredible.

Why such a bizarre name is used? I conceived it as a mix of Half-Life, Quake and maybe Call of Duty (if I manage to implement killcam on time). Maps are from Half-Life (apart from its own), boosts are from Quake, weapons are from both. And the name is also a pun and refers to one of the first three-dimensional shooters - Wolfenstein 3D.

## World Editor

### How to configure World Editor

World Editor is being created with Qt 5.15, so you need to install and properly configure it in Visual Studio (see Qt Visual Studio Add-in). You also need to adjust paths for additional include and library directories to reflect your file system structure. 

### World Editor features

All four view windows are plain OpenGL canvases. Three-dimensional view uses perspective projection, all two-dimensional ones use orthographic. The grid lines are created on the fly depending on the camera's position in the world.

In the editor you can:

<details>
  <summary>1. Move around, zoom in and out, increase or decrease scale of two-dimensional grids</summary>
  <img src="https://raw.githubusercontent.com/SkyFlame00/frankenstein/master/WorldEditor/media/grid.gif">

</details>

<details>
  <summary>2. Change windows layout proportions by dragging the horizontal, vertical or central handle</summary>
  <img src="https://raw.githubusercontent.com/SkyFlame00/frankenstein/master/WorldEditor/media/handles_dragging.gif">

</details>

<details>
  <summary>3. Create blocks called brushes that you build the level's geometry with</summary>
  <img src="https://raw.githubusercontent.com/SkyFlame00/frankenstein/master/WorldEditor/media/brush_creating.gif">

</details>

<details>
  <summary>4. Move brushes around and change their size</summary>
  <img src="https://raw.githubusercontent.com/SkyFlame00/frankenstein/master/WorldEditor/media/brush_actions.gif">

</details>

<details>
    <summary>5. Clip brushes. After clipping a brush can also be moved, resized or clipped again</summary>
    <img src="https://raw.githubusercontent.com/SkyFlame00/frankenstein/master/WorldEditor/media/brush_clipping.gif">  
    CGAL was used to make [convex hulls](https://doc.cgal.org/latest/Convex_hull_3/index.html#Chapter_3D_Convex_Hulls) from given sets of vertices.

</details>

<details>
  <summary>6. Switch between a couple of viewing modes in 3D viewer: texture mode, polygon mode and wireframe mode</summary>
  <img src="https://raw.githubusercontent.com/SkyFlame00/frankenstein/master/WorldEditor/media/view_modes.gif">

</details>

<details>
    <summary>7. Display normals of polygons</summary>
    <img src="https://raw.githubusercontent.com/SkyFlame00/frankenstein/master/WorldEditor/media/normals.gif">  
    You can toggle display with `Q` button.

</details>

<details>
    <summary>8. Attach different textures to brushes</summary>
    <img src="https://raw.githubusercontent.com/SkyFlame00/frankenstein/master/WorldEditor/media/textures_changing.gif">  
    You can select multiple textures by holding `Left Ctrl` button while clicking on a polygon. There is a small optimization made to rendering multiple textures: for a particular brush in a single render call the algorithm tries to attach as many textures as your GPU allows (but no more than 32 as OpenGL specifies). If the maximum textures limit is exceeded for a particular brush, it will be rendered with more than one draw call.

</details>

<details>
    <summary>9. Shift, scale and rotate textures</summary>
    <img src="https://raw.githubusercontent.com/SkyFlame00/frankenstein/master/WorldEditor/media/textures_actions.gif">  

</details>

<details>
    <summary>10. Save maps in JSON format and then open them</summary>
    <img src="https://raw.githubusercontent.com/SkyFlame00/frankenstein/master/WorldEditor/media/maps_saving.gif">  

</details>

### Hotkeys & Shortcuts
* `W`, `A`, `S`, `D` are the hotkeys for moving around
* In the 3D view hold left mouse button and move it to rotate the camera
* Press `Ctrl + Z` to undo changes, `Ctrl + Shift + Z` to redo changes (there are also buttons in the top toolbar for that)
* Press `Ctrl + S` to save the map
* When in clipping or creating brush mode, press `Esc` to discard actions
* When clipping, press `R` to change the part of the brush that is being discarded
* Press `Q` to display normals of brushes
* Press `1` to enable textured mode, `2` to enable polygon view, `3` to enable wireframe mode
* Press `[` to increase grid's scale and `]` to decrease

### Notes
* Although some optimizations were introduced during the development, the overall performance is relatively poor so far. Possible further optimizations:
  * sending 2D grids' data to the GPU once beforehand (not on each render as it is now)
  * the same can be applied to the points on the 2D grids and construction blocks (images of brushes that are created when using brush creating tool)
* Creating map outside of the grid is not guaranteed to work correctly
* The default path to textures is `resources/textures`. If there is no textures present in this directory, the texture browser will be empty. You can change the destination in `Tools -> Settings -> Textures -> Path to textures`
* The configuration file is stored as `worldeditor.config.json` in the root folder. It is created once you change the default path to textures

[grid]: WorldEditor/media/grid.gif
[handles dragging]: WorldEditor/media/handles_dragging.gif
[brushes creating]: WorldEditor/media/brushes_creating.gif
[brush actions]: WorldEditor/media/brush_actions.gif
[brush clipping]: WorldEditor/media/brush_clipping.gif
[viewing modes]: WorldEditor/media/view_modes.gif
[normals]: WorldEditor/media/normals.gif
[textures changing]: WorldEditor/media/textures_changing.gif
[textures actions]: WorldEditor/media/textures_actions.gif
[maps saving]: WorldEditor/media/maps_saving.gif
