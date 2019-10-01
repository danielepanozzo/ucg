Rasterization
=============

In this exercise you will implement a 2D editor for vector graphics.
The editor will allow to draw simple shapes interactively.

### Using Eigen

In all exercises you will need to do operations with vectors and matrices. To simplify the code, you will use [Eigen](http://eigen.tuxfamily.org/).
Have a look at the [Getting Started](http://eigen.tuxfamily.org/dox/GettingStarted.html) page of Eigen as well as the [Quick Reference](http://eigen.tuxfamily.org/dox/group__QuickRefPage.html}) page for a reference of the basic matrix operations supported.

### OpenGL

In all exercises you will use OpenGL 3.3 with GLSL version 150.

### GLFW

To handle input events (keyboard and mouse), we rely on GLFW to abstract the low-level OS-specific details of the process. Please refer to the [documentation](http://www.glfw.org/docs/latest/) of GLFW for more details.

### Preparing the Environment

Follow instructions the [general rules](https://github.com/nyu-cg-fall-17/computer-graphics/blob/master/RULES.md) to setup what you need for the assignment.

### Submission

Please refer to [this section](https://github.com/nyu-cg-fall-17/computer-graphics/blob/master/RULES.md#what-to-hand-in) of the general rules for more details on what to hand in.

For each tasks, add at least one image in the readme demonstrating the results.
The code that you used for all tasks should be provided.


Ex.1: View Control
------------------

Add to the application the capability of changing the camera. The following actions should be supported:

- <kbd>+</kbd> should increase the zoom by 20% zooming in in the center of the screen.

- <kbd>-</kbd> should decrease the zoom by 20% zooming out in the center of the screen.

- <kbd>w</kbd>, <kbd>a</kbd>, <kbd>s</kbd>, and <kbd>d</kbd> should pan the view by 20% of the visible part of the scene, i.e. translate the entire scene, respectively down, right, up and left by 20% of the window size.

This should NOT be implemented by changing the coordinates of the objects in the scene. You must add a view matrix to the vertex shader (as a uniform) that is transforming the position of the vertices of the triangles before they are rendered. Note that you will also have to transform the screen coordinates using the inverse of the view matrix, to ensure that the user interaction will adapt to the current view. ![image](img/view.png)


Ex.2: Add Keyframing
--------------------

### Linear Interpolation

Add the possibility to keyframe one property of an object (size, position, or rotation) and create an animation using linear interpolation between the keyframes. You can use a timer to make the animation automatic, or you could move to the next frame at the press of a button.

For example, you can press a key <kbd>K</kbd> to create a new frame of your animation. Move your triangles to the desired location, then hit <kbd>K</kbd> again to create a new frame. Once this is done, you can press <kbd>A</kbd> to play the animation. You can also add a shortcut <kbd>C</kbd> to clear the frames of the animation, keeping only the first frame. This is only a suggestion, but the essential to **not** hardcode the frame properties in your C++ code: we should be able to create our own animations on the spot.

### Bézier Curves

Add the option to use a [Bézier curve](https://en.wikipedia.org/wiki/B%C3%A9zier_curve) instead of a linear interpolation to animate the desired property (e.g. position) between each keyframe. For example, pressing <kbd>B</kbd> instead of <kbd>A</kbd> will play/advance the animation using the Bézier curve instead of a linear interpolation.
