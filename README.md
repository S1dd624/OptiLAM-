Developing for OptiLAM (Optimal Laminated Additive Manufacturing)
1. SVG_rotator rasterizes input svgs to bitmaps.
2. Normalization to fit canvas, breakdown into triangles
3. BitwiseOR performed, fitness score assigned to each shape on degree of overlaps.
5. The genetic algorithm calls the fitness evaluator to find the best solution.
6. resulting best x,y,r lists given to svg_placer to assemble final canvas
7. .nc file generated from cnc_gen to be fed to OptiLAM as instructions.
