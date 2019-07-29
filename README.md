**THIS IS NOT A COMPLETE GAME ENGINE - FAR FROM IT. IT IS NOT INTENDED TO BE USED BY ANYONE.**
IT IS VERY VERY VERY UNFINISHED AND WILL LIKELY NEVER BE UPDATED AGAIN.

# Haksaengine 

Haksaengine (portmanteau of the Korean word "학생" and "engine") is an open source game engine written from scratch using C++. It is being developed as the final project for my MSc.

The engine is being designed with RTS games in mind for the most part, however it is still being kept as modular as I can make it.

A large part of this project is rooted in learning via implementation and the researching about various subsystems in a game engine. 
For this reason, as well as due to severe time constraints, there are many implementation and design tradeoffs that have been made.
In future, after my MSc is finished, I hope to revisit this project and work on it to make it into a more realistic albeit small game engine.

# Dependencies

The project uses the following 3rd party libraries:
- GLM
- GLFW 3
- OpenGL 4.5
- GLEW
- Assimp (fbx2mdl only)

These aren't included in this repository and will need to be sourced manually.

# Usage

The actual engine code is found in the "haksaengine" project. It builds to a DLL, which needs to be linked against by a project that defines the actual game code.

Create an instance of the "Engine" class, and call the "initialise" and then "run" method to start the engine main loop.

The Engine creates some default component types, loaders, and systems to get you started if you choose to use them.

The default asset type is the custom ".mdl" file type (an example can be found in the "assets" directory). It is a pretty simple file that has all the information for a given game model.

However, the engine is designed to be modular so you can create custom loaders and component types and register them with the engine to use them.

# Future

Project currently on hold pending the completion of a proper renderer as a standalone project (which will replace the very rushed   terrible renderer this engine currently has)
