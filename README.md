# gas_model
So, this gas model with chemical reaction simulation

### chemicl reactions
circle + circle -> rect
circle + rect   -> bigger rect with more mass
rect + rect     -> a lot of circles (number of circles equals summ of rect mass)
### details of reactions, energy
All chemical objects has energy, when object collides with hot wall (walls are also objects and have energy) it takes it's part of energy
First two reactions results' speed computed by the law of conversation of momentum, energy delta added to result's potential energy
In third reaction, kinetic and potential energy of two rects equals to balls' summary kinetic energy
The more rects and walls have potential energy, the more red they are
### other details
Every circle has mass 1, rects has different mass
Circle and rect molecules has circle physics
### plot
Plot counts how much rects and circles are there, red is rects (because they are red :-) ) and green is circles.
If you have strange pixel near to horizontal lines, go to GraphicLib.cpp -> Renderer constructor -> set rendererFlags to SDL_RENDERER_SOFTWARE SDL_RENDERER_ACCELERATED. (Everything won't be slower because dt is proportional to real time delta, but accuracy of collisions may be lower)
### buttons
First button adds energy to walls (goes through list of all objects and adds energy to every). Second button spawns a circle in a random place, with zero speed and energy. Plot button changes coord system size so that all values fit to plot's rect and values cover all rect