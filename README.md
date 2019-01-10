# Trainer Header

This is a small, but useful header containing tools to assist with lightweight trainer creation. The Trainer class contains a dynamic address resolver, a base address resolver, templated read/write functions for use with any type you'd like, and automatic Winapi hooking. The total amount of code for all of this is relatively small, but it's designed in a way that's reusable and applicable to most scenarios where you'd need to create a trainer.

# Instructions
The Trainer class contains a pure virtual method at the absolute top, thus making the class abstract. You should not attempt to make an instance of the trainer class, rather you should subclass it, and override the pure virtual method in the superclass.

Within the subclass, you are free to use the protected methods from the superclass. If you would like to see examples on how to operate the superclass, see the game-hacks folder for a game-specific subclass in use.

# Game Hacks
This repository also comes with a folder of some pre-made hacks for some games. The hacks consist of headers, containing trainer subclasses. These are also a good example of the trainer class in use. This collection is currently small, consisting of one or two games at the moment, but will grow as I personally create more in my free time and choose to push them. The game hacks folder also comes with images containing the results of various pointer scans made with Cheat Engine, which is where I get my addresses from. I included the results, as I could not narrow the pointer path down to exactly one, so multiple paths may be valid. In case one path breaks, use another.
