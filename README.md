#### How to use it

Only compatible with Visual 2013

- Compile in release mode
- Copy the executable at the root of the project (same folder as the README file)
- Launch it `COMP447.exe` or `COMP447.exe fileName.anim`. If you don't provide argument, program will automatically load dance.anim, break-dance.anim and wave.anim

#### Architecture

A mesh contain a `Skeleton` and a `Timeline`.

A `Timeline` contain a list of `KeyFrame`, one for each frame, from 0 to X.

A `KeyFrame` contain an array of 17 quaternions (17 is the number of joints that the skeleton have) expressing the rotation of each join.

During animation time, the `Skeleton` ask to the `Timeline` the interpolated rotation for each bone at time T of the animation. The `Timeline` proceed to an interpolation (Matrix, Euler, LERP, SLERP) with quaternions and return a matrix. This matrix is used to compute the new local position of the joint.

The rest of the skinning computation is the same as the past homework.

I provide a GUI to help user to test the program.

I used GLM (OpenGL Mathematics), for vector, matrix and quaternions - so you can remove me 3 points :).
