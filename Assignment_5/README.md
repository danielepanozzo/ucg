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


Ex.1: Triangle Soup Editor
--------------------------

Implement an interactive application that allows to add, edit, and delete triangles. The following operations should be supported:

- The key <kbd>i</kbd> will enable triangle insertion mode. When this mode is enabled, every triple of subsequent mouse clicks will create a triangle connecting the three locations where the mouse has been pressed. The first click will create the starting point of the segment, which will be immediately visualized. As the mouse is moved, a preview of a segment will appear. After the second mouse click, a preview of the triangle will appear. After the third click, the current preview will transform into the final triangle.

![image](img/i.png)

- The key <kbd>o</kbd> will enable triangle translation mode. Each mouse click will select the triangle below the cursor (which will be highlighted), and every movement of the mouse (while keeping the button pressed) will result in a corresponding translation of the triangle. Note that the triangle should move on screen by the same amount as the cursor.

![image](img/o.png)

- The key <kbd>p</kbd> will enable delete mode. When the mouse is pressed, the triangle below the cursor is deleted.


Ex.2: Rotation/Scale
--------------------

When triangle translation mode is enabled, keep the current primitive selected after the mouse is released. If a primitive is selected and you press the keys <kbd>h</kbd> and <kbd>j</kbd>, the triangle will rotate by 10 degree clockwise or counter-clockwise, respectively. The rotations should be done around its barycenter, i.e. the barycenter of the triangle should not change. When the keys <kbd>k</kbd> or <kbd>l</kbd> are pressed, the primitive should be scaled up or down by 25%. Similarly to before, the barycenter of the triangle should not move due to the scaling. For this task, you can directly edit the position of the vertices of the triangles on the CPU side, and re-upload them to the GPU after every change. If you do it directly in the vertex shader, you can gain additional points (see [Ex. 4](#ex4-shader-translationscalingrotation-optional)).


Ex.3: Colors
------------

Add the possibility to paint the color of each vertex in the scene. Color mode is enabled by the key <kbd>c</kbd>. In this mode, every mouse click will select the vertex closer to the current mouse position. After a vertex is selected, pressing a key from <kbd>1</kbd> to <kbd>9</kbd> will change its color (the colors that you use are not important, you can pick whatever colors you like). The color should be interpolated linearly inside the triangles.


Ex.4: Shader Translation/Scaling/Rotation (Optional)
----------------------------------------------------

Upload every single triangle as a separate primitive in a separate VBO (or in a single VBO using offsets for drawing them one by one). For each primitive, upload to the GPU its model matrix (as a uniform) that transforms the triangle from its canonical position (defined at its creation) to the current position (obtained by combining translations, scaling and rotations). The transformation should be executed in the shader, and the content of the VBO storing the vertex positions never updated.
