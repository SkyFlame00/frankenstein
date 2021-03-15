# Frankenstein

## What is it?

This is a 3D multiplayer shooter with two game modes: deathmatch and team deathmatch. The game is being created from scratch using C++ and OpenGL. Along with the game comes a level editor called World Editor. This is just my portfolio project, so don't expect anything incredible.

Why is such a bizarre name used? I conceived it as a mix of Half-Life, Quake and maybe Call of Duty (if I manage to implement killcam on time). Maps are from Half-Life (apart from its own), boosts are from Quake, weapons are from both. And the name is also a pun and refers to one of the first three-dimensional shooters - Wolfenstein 3D.

## How to configure World Editor

World Editor is being created with Qt 5.15, so you need to have it installed and properly configured in Visual Studio. You also need to adjust paths for additional include and library directories to reflect your file system structure. 

## World Editor features

All four view windows are plain OpenGL canvases. Three-dimensional view uses perspective projection, all two-dimensional ones use orthographic. The grid lines are created on the fly depending on the camera's position in the world.

1. You can move around two-dimensional grid with `W`, `A`, `S`, `D` keys, zoom in and out the grid with the mouse scroll button and increase or decrease scale with `[` and `]` keys

![Grid actions example][grid]

2. You can change windows layout proportions by dragging the horizontal, vertical or central handle

![Handles dragging example][handles draggins]

3. You can create blocks called brushes that you build level's geometry with

![Brushes creating example][brushes creating]

4. You can move brushes around and change their size

![Brush actions example][brush actions]

5. You can clip brushes. After clipping a brush can also be moved, resized or clipped again

![Brush clipping example][brush clipping]

6. You can switch between a couple of viewing modes in 3D viewer with `V` and `F` keys: texture mode (not yet ready), color mode, polygon mode and wireframe mode

![Viewing modes example][viewing modes]

[grid]: WorldEditor/media/grid.gif
[handles draggins]: WorldEditor/media/handles_dragging.gif
[brushes creating]: WorldEditor/media/brushes-creating.gif
[brush actions]: WorldEditor/media/brush-actions.gif
[brush clipping]: WorldEditor/media/clipping.gif
[viewing modes]: WorldEditor/media/viewing-modes.gif
