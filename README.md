# Terminal Raytracer
This repository is for a horrible renderer written in c that will pivot a camera
mesh around a mesh and render a depth map to stdout (very very slowly) written
from scratch.
# Running
Download the code and run `make` in the root directory, then run
`./build/render path/to/file.obj`. For convenience, the code is currently
configured for the command `./build/render res/suzanne.obj`, since suzanne is
included in the repository. The more rows/cols in the terminal, the longer it
takes to run (which is very long). By default, it will render 60 frames to text
files in the `output` directory before freeing everything, but this can be changed
in `main.c:150`.