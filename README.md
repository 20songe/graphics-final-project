# Chicken Salad - Final Project

## Contributors
Daniel Cho, Rosella Liu, Ria Rajesh, Erica Song

## Summary
Using OBJ files, we rendered a scene with a tree on water, surrounded by rocks, moss, and grass.
The leaves of the tree fall into the water, causing ripples. We also used dudv
mapping to make the water look more realistic.

## How to Run the Project
Make sure the working directory is set to the final project root directory.

Click run. You will see a window with buttons on the left side labelled with the textures
they contain. For best results, click on the buttons in order from top to bottom and select the
OBJ files with the same names as the button label. You can mix and match the textures with different
objects by selecting a different OBJ file than the one named on the button.

The buttons labelled "Upload Scene File 6" and "Upload Scene File 7" do not yet have textures associated
with them but can be used to add more textures if necessary via the realtime.cpp file. For this project, you
can ignore these buttons.

## Features Implemented
- parsing OBJ files - Daniel, Erica, and Ria
- normal mapping - Daniel and Rosella
- du/dv mapping - Rosella
- realtime reflections - Erica and Rosella
- ripples - Daniel, Erica, and Rosella
- realtime textures - Daniel and Ria
- particle generator - Ria

## Disclaimer
This project takes a lot of computational power and
memory because of high-resolution textures, 4K textures to be specific.
Thus, this may not run on all computers, and it took us roughly 4 minutes
to compile on our laptop.
