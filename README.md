# Trainer Header
Small header to assist in trainer creation by providing trainer utilities. Allows pointer chain resolution, base address discovery, and more.

To make a new game specific trainer, subclass the Trainer class and make sure to overide the pure virtual method in the Trainer class. In the constructor, set the process title, window title, and then call the setup method. The trainer is now ready, and the tools are now at your disposal.

Make sure to compile on same architecture as the game you're targetting.

See https://github.com/Shayna-x02/SCB_MoneyEditor for an example of the header in use.
