// This example is heavily based on the tutorial at https://open.gl

////////////////////////////////////////////////////////////////////////////////
// OpenGL Helpers to reduce the clutter
#include "helpers.h"
// GLFW is necessary to handle the OpenGL context
#include <GLFW/glfw3.h>
// Linear Algebra Library
#include <Eigen/Dense>
// Timer
#include <chrono>
////////////////////////////////////////////////////////////////////////////////

// VertexBufferObject wrapper
VertexBufferObject VBO;

// Contains the vertex positions
Eigen::MatrixXf V(2,3);

// Contains the view transformation
Eigen::Matrix4f view(4,4);

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	// Get viewport size (canvas in number of pixels)
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	// Get the size of the window (may be different than the canvas size on retina displays)
	int width_window, height_window;
	glfwGetWindowSize(window, &width_window, &height_window);

	// Get the position of the mouse in the window
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	// Deduce position of the mouse in the viewport
	double highdpi = (double) width / (double) width_window;
	xpos *= highdpi;
	ypos *= highdpi;

	// Convert screen position to world coordinates
	Eigen::Vector4f p_screen(xpos,height-1-ypos,0,1);
    Eigen::Vector4f p_canonical((p_screen[0]/width)*2-1,(p_screen[1]/height)*2-1,0,1);
    Eigen::Vector4f p_world = view.inverse()*p_canonical;


	// Update the position of the first vertex if the left button is pressed
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		V.col(0) << p_world[0], p_world[1];
	}

	// Upload the change to the GPU
	VBO.update(V);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	// Update the position of the first vertex if the keys 1,2, or 3 are pressed
	switch (key) {
		case GLFW_KEY_1:
			V.col(0) << -0.5, 0.5;
			break;
		case GLFW_KEY_2:
			V.col(0) << 0, 0.5;
			break;
		case GLFW_KEY_3:
			V.col(0) << 0.5, 0.5;
			break;
		default:
			break;
	}

	// Upload the change to the GPU
	VBO.update(V);
}

int main(void) {
	// Initialize the GLFW library
	if (!glfwInit()) {
		return -1;
	}

	// Activate supersampling
	glfwWindowHint(GLFW_SAMPLES, 8);

	// Ensure that we get at least a 3.2 context
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);

	// On apple we have to load a core profile with forward compatibility
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create a windowed mode window and its OpenGL context
	GLFWwindow * window = glfwCreateWindow(640, 480, "[Float] Hello World", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}

	// Make the window's context current
	glfwMakeContextCurrent(window);

	// Load OpenGL and its extensions
	if (!gladLoadGL()) {
		printf("Failed to load OpenGL and its extensions");
		return(-1);
	}
	printf("OpenGL Version %d.%d loaded", GLVersion.major, GLVersion.minor);

	int major, minor, rev;
	major = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MAJOR);
	minor = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MINOR);
	rev = glfwGetWindowAttrib(window, GLFW_CONTEXT_REVISION);
	printf("OpenGL version recieved: %d.%d.%d\n", major, minor, rev);
	printf("Supported OpenGL is %s\n", (const char*)glGetString(GL_VERSION));
	printf("Supported GLSL is %s\n", (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));

	// Initialize the VAO
	// A Vertex Array Object (or VAO) is an object that describes how the vertex
	// attributes are stored in a Vertex Buffer Object (or VBO). This means that
	// the VAO is not the actual object storing the vertex data,
	// but the descriptor of the vertex data.
	VertexArrayObject VAO;
	VAO.init();
	VAO.bind();

	// Initialize the VBO with the vertices data
	// A VBO is a data container that lives in the GPU memory
	VBO.init();

	V.resize(2,3);
	V << 0,  0.5, -0.5, 0.5, -0.5, -0.5;
	VBO.update(V);

	// Initialize the OpenGL Program
	// A program controls the OpenGL pipeline and it must contains
	// at least a vertex shader and a fragment shader to be valid
	Program program;
	const GLchar* vertex_shader = R"(
		#version 150 core

		in vec2 position;
		uniform mat4 view;
		void main() {
			gl_Position = view * vec4(position, 0.0, 1.0);
		}
	)";

	const GLchar* fragment_shader = R"(
		#version 150 core

		uniform vec3 triangleColor;
		out vec4 outColor;

		void main() {
		    outColor = vec4(triangleColor, 1.0);
		}
	)";

	// Compile the two shaders and upload the binary to the GPU
	// Note that we have to explicitly specify that the output "slot" called outColor
	// is the one that we want in the fragment buffer (and thus on screen)
	program.init(vertex_shader, fragment_shader, "outColor");
	program.bind();

	// The vertex shader wants the position of the vertices as an input.
	// The following line connects the VBO we defined above with the position "slot"
	// in the vertex shader
	program.bindVertexAttribArray("position", VBO);

	// Save the current time --- it will be used to dynamically change the triangle color
	auto t_start = std::chrono::high_resolution_clock::now();

	// Register the keyboard callback
	glfwSetKeyCallback(window, key_callback);

	// Register the mouse callback
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	// Loop until the user closes the window
	while (!glfwWindowShouldClose(window)) {
		// Set the size of the viewport (canvas) to the size of the application window (framebuffer)
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);

		// Bind your VAO (not necessary if you have only one)
		VAO.bind();

		// Bind your program
		program.bind();

		// Set the uniform value depending on the time difference
		auto t_now = std::chrono::high_resolution_clock::now();
		float time = std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_start).count();
		glUniform3f(program.uniform("triangleColor"), (float)(sin(time * 4.0f) + 1.0f) / 2.0f, 0.0f, 0.0f);

        float aspect_ratio = float(height)/float(width); // corresponds to the necessary width scaling

        view <<
        aspect_ratio,0, 0, 0,
        0,           1, 0, 0,
        0,           0, 1, 0,
        0,           0, 0, 1;

        glUniformMatrix4fv(program.uniform("view"), 1, GL_FALSE, view.data());

		// Clear the framebuffer
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw a triangle
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// Swap front and back buffers
		glfwSwapBuffers(window);

		// Poll for and process events
		glfwPollEvents();
	}

	// Deallocate opengl memory
	program.free();
	VAO.free();
	VBO.free();

	// Deallocate glfw internals
	glfwTerminate();
	return 0;
}
