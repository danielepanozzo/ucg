3D Scene Editor
===============

In this exercise you will implement a 3D editor that allows to prepare 3D scenes composed of multiple3D objects.

### Using Eigen

In all exercises you will need to do operations with vectors and matrices. To simplify the code, you will use [Eigen](http://eigen.tuxfamily.org/).
Have a look at the [Getting Started](http://eigen.tuxfamily.org/dox/GettingStarted.html) page of Eigen as well as the [Quick Reference](http://eigen.tuxfamily.org/dox/group__QuickRefPage.html}) page for a reference of the basic matrix operations supported.

### OpenGL

In all exercises you will use OpenGL 3.3 with GLSL version 150. [Read here](http://docs.gl) the documentation of OpenGL functions.

### GLFW

To handle input events (keyboard and mouse), we rely on GLFW to abstract the low-level OS-specific details of the process. Please refer to the [documentation](http://www.glfw.org/docs/latest/) of GLFW for more details.

### Preparing the Environment

Follow instructions the [general rules](https://github.com/nyu-cg-fall-17/computer-graphics/blob/master/RULES.md) to setup what you need for the assignment.

### Submission

Please refer to [this section](https://github.com/nyu-cg-fall-17/computer-graphics/blob/master/RULES.md#what-to-hand-in) of the general rules for more details on what to hand in.

For each tasks, add at least one image in the readme demonstrating the results.
The code that you used for all tasks should be provided.

**Note**: For the purpose of exercises 1 to 4, the world space, camera space, and canonical viewing volume will be one and the same. Camera and projection matrices can be optionally implemented as part of exercise 5.


Ex.1: Scene Editor
------------------

Implement an interactive application that allows to add, edit, and delete 3D meshes. New objects can be added to the scene in three ways:

- The key <kbd>1</kbd> will add a unit cube in the origin. The coordinates of the cube should be specified directly in the C++ source code.

- The key <kbd>2</kbd> will import a new copy of the mesh `bumpy_cube.off`, scale it to fit into a unit cube and center it on the origin.

- The key <kbd>3</kbd> will import a new copy the mesh `bunny.off`, scale it to fit into a unit cube and center it on the origin.

Note that you can have multiple copies of the same object in the scene, and each copy can have its own position, scale, and rotation.

For this exercise and the next, all transformations **must** be done in the shader. The VBO containing the vertex positions of each object should be uploaded only once to the GPU. Remember to pass `GL_FALSE` as the 3rd argument to [`glUniformMatrix4fv`](http://docs.gl/gl3/glUniform), as matrices in Eigen are stored column-major by default.


Ex.2: Shading
-------------

In this exercise, you will implement different shading options for the triangles. The scene should always contain at least one light source, set as a shader uniform. Add an option to switch between the following rendering modes:

1. **Wireframe**: only the edges of the triangles are drawn.

2. **Flat Shading**: each triangle is rendered using a unique color (i.e. the normal of all the fragments that compose a triangle is simply the normal of the plane that contains it). On top of the flat shaded triangle, you should draw the wireframe.

3. **Phong Shading**: the normals are specified on the vertices of the mesh and interpolated in the interior. The lighting equation should be evaluated for each fragment.

![image](img/bunny.png)

Both flat shading and Phong shading should use the same code in the shader program. The only difference lies in the attributes that are sent to the vertex shader. You can reuse the code from the ray-tracing assignment for the lighting equation (you should implement both *diffuse* and *specular* shading components in the fragment shader).

Note that to perform flat shading, you will need to send per-face normal attributes to the shader program. However, with the OpenGL pipeline, it is only possible to send *per-vertex* attributes. One simple way to go around this limitation is simply to duplicate the input vertices for every triangle in the mesh. That is, from a pair **(V, F)** describing the mesh coordinates and triangles, build a new pair **(V', F')**, where every vertex from **V'** appears only once in **F'**.

To compute the per-vertex normals you should first compute the per-face normals, and then average them on the neighboring vertices. In other words, the normal of the vertex of a mesh should be the average of the normals of the faces touching it. Remember to normalize the normals after averaging.


Ex.3: Object Selection
----------------------

Implement object selection in the viewer. Clicking on a object will select the object, changing its color.
You should reuse the ray-triangle intersection code from the ray-tracing assignments, and implement this operation using ray-casting.

Note that, because vertex coordinates are transformed by a different object matrix **M** in the vertex shader (from object space to world space), you should compute the ray parameter **(e, d)** in the canonical viewing volume (same as the world space for this exercise), transform the parameters by **M<sup>-1</sup>** (remember to apply only the linear part of **M<sup>-1</sup>** to **d**), compute the intersection in object space, and then transform back the intersection point from the object space to the world space.


Ex.4: Object Control (Optional 2.5 pts)
--------------------

When an object is selected, it should be possible to translate it, rotate it around its barycenter, and rescale it without changing its barycenter. These extra transformation matrices should be combined with the initial rescaling/translation operation performed in Exercise 1. In other words, the final object transformation can be written as **M** = **T×R×S×S0×T0**, where **S0** and **T0** are the initial scaling and translation applied to center the object around its unit box, and **T**, **R** and **S** are the editable transformation matrices that you will modify in this exercise.

All these actions should be associated to keyboard keys (and the choice of keys should be detailed in the readme).
Each object should also have a rendering setting associated with it, which can be either wireframe, flat shading, or phong shading, as implemented in Exercise 2. When an object is selected, it must be possible to switch between the different rendering modes by pressing three keys on the keyboard.


Ex.5: Camera Control (Optional 2.5 pts)
-------------------------------

Add the possibility to translate the position of the camera (similarly to the Assignment 5), but in this exercise the camera should always *point to the origin*. It should be possible to move it around, but the camera should always face the origin.

Implement both a *orthographic camera* (similar to the one that you used for Assignment 5, but in 3D) and a *perspective camera*. The cameras should take into account the size of the window, properly adapting the aspect ration to not distort the image whenever the window is resized. All functionalities should work after resizing the window, including object selection and editing of the scene.

Note that for the shading to be correct, the lighting equation need to be computed in the camera space (sometimes called the *eye space*). This is because in the camera space the position of the camera center is known (it is (0,0,0)), but in the canonical viewing volume normals can be distorted by the perspective projection, so shading will be computed incorrectly.

**Note:** Translating the camera should only affect the *camera matrix* (that transforms the world space into the camera space). The projection matrix should not be updated in this step.
Conversely, when the window is resized by the user, only the *projection matrix* should be updated, since we want to preserve the pixels aspect ratio. Thus, resizing the window will change the field of view on X or Y (you should decide which one is fixed), but should not change the camera matrix.
