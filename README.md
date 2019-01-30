# Weubbe_Eric_Graphics2

Camera Controls
W -> forward along z
S -> backward along z
A -> Left along x
D -> right along x
T -> Up
G -> Down
Mouse-> look around
Z -> Zoom in
X -> Zoom out
J & M -> adjust far plane clipping
H & N -> adjust near plane clipping

Particles are made with the geometry shader (point to quad) and all 10000 of them are moved with the compute shader.

The shield demonstrates Render to Texture. There are trees being rendered on it.
I think the shield also demonstrates multi-texturing. It uses the reflected skybox along with the trees rendered on it as its final texture.

Both the rock and shield demonstrate specular highlights.