Developing for OptiLAM (Optimal Laminated Additive Manufacturing)
1. Genetic algorithm creates x (sheet width), y (sheet length) and r (roation) sequences.
2. Sequences are passed to a fitness evaluator to obtain best possible configuration.
3. Scalable vector graphic files (rotator and placer) use OpenCV to create shapes on a canvas for object to be printed.
4. Python g code is loaded to machine to print the object.
