# procshape

A Python module for procedural 3D mesh generation with the Panda3D engine.

Currently the plan is to create it as a module to use as in procgen projects, where
the user wants to create geometry of any kind and does not want to deal with the pain of collision
shape creation and all other intricacies of procedural mesh generation.

This implementation will leave out the possibilities for texture mapping and focuses on solid
mesh generation using dual contouring. All meshes will be created w/o vertex normals, the user has
to implement respective shader code to compensate for that.

This is a work in progress and I don't have a clear idea on what the *finished* module would look like. 
It is a byproduct of building up my skills in (game) development and will likely look messy,
although I'm trying my best to write [properly documented and easy to read code](docs/dev_guidelines.md). 

I'm open to suggestions, feature requests (just file an issue) and if you want to contribute, 
I'm happy to review and accept any pull request, as long as it improves or extends the code, 
in a useful manner (which of course is an arbitrary metric, set by me).

In the [demo](demo) folder are some examples of what can be done with it, other examples are 
the projects I'm currently working on or have worked on can be seen @ https://www.tizilogic.com 
(*Disclaimer: Currently an old version of the library is being used, this will hopefully be an 
improved rebuild of what I learned during that first implementation*)

Updates will follow...
