# Trainer Header

This is a small, but useful header containing tools to assist with lightweight trainer creation. The Trainer class contains a dynamic address resolver, a base address resolver, templated read/write functions for use with any type you'd like, and automatic Winapi hooking. The total amount of code for all of this is relatively small, but it's designed in a way that's reusable and applicable to most scenarios where you'd need to create a trainer.

# Instructions
The Trainer class contains a pure virtual method at the absolute top, thus making the class abstract. You should not attempt to make an instance of the trainer class, rather you should subclass it, and override the pure virtual method in the superclass.

Within the subclass, you are free to use the protected methods from the superclass. If you would like to see examples on how to operate the superclass, see the game-hacks folder for a game-specific subclass in use.

# Game Hacks
This repository also comes with a folder containing an example of how to use the trainer. I may make more examples as time goes on, and each example technically works as its own functioning game hack in a header format.