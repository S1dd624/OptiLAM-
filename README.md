OptiLAM process to be made seamless.

Slicing thickness, Time Delay, Bed Height } 3 paramters.

Plan to input these 3 paramters via GUI dialog. All smooth sailing after.

Automation 8_fix handling slices, Nest_new.cpp handling .nc

IceSL: A slicing software, prerequisite. But conflicts with current PC specs. Roadblock.

1/07/25:
- CREATED Tkinter shell GUI that contains all 3 dialogs.
- ONLY LASER DELAY IS DYNAMIC AS OF NOW
- A config file with desired values is communicated to .cpp via aut_8_fix to generate G code, Z (bed height) and time delay are hardcoded to increment progressively.
- Ditch form1.cs, as icesl is incompatible.

2/07/25:
- Around ~4 30 pm, first succesful assembled svg, with utilization of genetic algorithm, was loaded onto EzCad2.
- Loaded SVG was in *optimized SVG* format had one triangle inside another, laser marked only the region between their boundaries. Not ideally envisioned.
- Must group/ungroup, perhaps edit using xml editor, stroke-to-path, all Inkscape.
- 'lets try this.svg', a 2nd attempt was loaded onto EzCad2 with additional edits from Inkscape. Better than final_JUL2.
- Still not all triangles are 'normal', some still are telescoping triangles.
