Developing for OptiLAM (Optimal Laminated Additive Manufacturing)
1. SVG_rotator normalizes input svgs to bitmaps.
2. Bitmaps, along with generated chromosomes(x,y,theta) are given to fitness evaluator
3. Fitness evaluator computes the fitness score for each bitmap
4. The genetic algorithm calls the fitness evaluator to find the best solution
5. SVG_placer assembles the output svgs.
6. Python g code is loaded to machine to print the object.
