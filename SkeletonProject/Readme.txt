Assignment II - Intro to 3D & Procedural Geometry

By: Hugues Ross -and- Johnathan Omalia

-------------------------------------------------------------------------------

This exercise was to intended to make various 3D primitives procedurally.
We made the following:
-Cone
-Cylinder
-UV Sphere

In addition, we made some more shapes that weren't required:
-Plane
-Tube
-DoubleCone
-Cuboid (Rectangular pprism with arbitrary face counts on each axis)

For proof of the required shapes, look closely at the 'potted plant' floating
in the sky, as it contains all of them + a tube.

-------------------------------------------------------------------------------

We also needed to improve the camera.
The new camera is much better, and gives the user more freedom!
The controls are:
WASD - Move Forward/Left/Back/Right (Based on orientation)
QE - Move Down/Up (Fixed axis)
Left Click + Drag - Turn the camera
As with the original, F toggles fullscreen.

-------------------------------------------------------------------------------

Lastly, although the demo doesn't make it apparent, we have a fully functioning
Scene Graph with support for parent/child relationships and multiple cameras.
The camera now generates a view matrix from its world transform, but can also
do the reverse when a specific lookat matrix is provided!
