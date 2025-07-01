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
