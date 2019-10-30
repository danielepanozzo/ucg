#pragma once

////////////////////////////////////////////////////////////////////////////////
#include <glad/glad.h>
#include <Eigen/Dense>
#include <vector>
#include <string>
////////////////////////////////////////////////////////////////////////////////

class Program;

// -----------------------------------------------------------------------------

class VertexBufferObject {
public:
	typedef unsigned int GLuint;
	typedef int GLint;

	GLuint id;
	GLuint rows;
	GLuint cols;
	GLenum scalar_type; // scalar type stored in the VBO
	GLenum buffer_type; // buffer type, see http://docs.gl/gl3/glBindBuffer

	VertexBufferObject()
		: id(0), rows(0), cols(0), scalar_type(GL_FLOAT), buffer_type(GL_ARRAY_BUFFER)
	{ }

	// Create a new empty VBO
	void init(GLenum st, GLenum bt);

	// Updates the VBO with a matrix M
	template<typename Derived>
	void update(const Eigen::PlainObjectBase<Derived>& M);

	// Select this VBO for subsequent draw calls
	void bind();

	// Unbind this VBO
	void unbind();

	// Release the id
	void free();
};

// -----------------------------------------------------------------------------

class VertexArrayObject {
public:
	unsigned int id;

	VertexArrayObject() : id(0) { }

	// Create a new VAO
	void init();

	// Select this VAO for subsequent draw calls
	void bind();

	// Unbind this VAO
	void unbind();

	// Release the id
	void free();
};

// -----------------------------------------------------------------------------

// This class wraps an OpenGL program composed of two shaders
class Program {
public:
	typedef unsigned int GLuint;
	typedef int GLint;

	GLuint vertex_shader;
	GLuint fragment_shader;
	GLuint program_shader;

	Program() : vertex_shader(0), fragment_shader(0), program_shader(0) { }

	// Create a new shader from the specified source strings
	bool init(const std::string &vertex_shader_string,
		const std::string &fragment_shader_string,
		const std::string &fragment_data_name);

	// Select this shader for subsequent draw calls
	void bind();

	// Release all OpenGL objects
	void free();

	// Return the OpenGL handle of a named shader attribute (-1 if it does not exist)
	GLint attrib(const std::string &name) const;

	// Return the OpenGL handle of a uniform attribute (-1 if it does not exist)
	GLint uniform(const std::string &name) const;

	// Bind a per-vertex array attribute
	GLint bindVertexAttribArray(const std::string &name, VertexBufferObject& VBO) const;

	GLuint create_shader_helper(GLint type, const std::string &shader_string);
};

////////////////////////////////////////////////////////////////////////////////

// From: https://blog.nobel-joergensen.com/2013/01/29/debugging-opengl-using-glgeterror/
void _check_gl_error(const char *file, int line);

///
/// Usage
/// [... some opengl calls]
/// glCheckError();
///
#define check_gl_error() _check_gl_error(__FILE__,__LINE__)

////////////////////////////////////////////////////////////////////////////////

template<typename Derived>
void VertexBufferObject::update(const Eigen::PlainObjectBase<Derived>& M) {
	assert(id != 0);
	// You can other types described here:
	// https://www.khronos.org/opengl/wiki/OpenGL_Type
	switch(scalar_type) {
		case GL_FLOAT:        assert(sizeof(typename Derived::Scalar) == sizeof(GLfloat)); break;
		case GL_INT:          assert(sizeof(typename Derived::Scalar) == sizeof(GLint));   break;
		case GL_UNSIGNED_INT: assert(sizeof(typename Derived::Scalar) == sizeof(GLuint));  break;
		default:
			throw std::runtime_error("OpenGL type not supported in VertexBufferObject class.");
	}
	int current_vao;
	glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &current_vao);
	glBindVertexArray(0); // Make sure to not affect the current VAO
	glBindBuffer(this->buffer_type, id);
	glBufferData(this->buffer_type, sizeof(typename Derived::Scalar)*M.size(), M.data(), GL_DYNAMIC_DRAW);
	glBindBuffer(this->buffer_type, 0);
	rows = M.rows();
	cols = M.cols();
	check_gl_error();
	glBindVertexArray(current_vao);
}
