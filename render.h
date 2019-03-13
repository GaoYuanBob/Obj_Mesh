#pragma once

#include <QOpenGLWidget>

#include <QOpenGLFunctions>
#include <QOpenGLFunctions_4_3_Core>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>

#include <camera.h>
#include "meshmodel.h"
#include <objloader.h>

/// 用一个宏定义来进行debug，可以输出出错的文件以及行数，以及错误类型
/// glGetError() 需要 OpenGL上下文，这里还没有，所以用宏
#ifndef NDEBUG
#define PrintGLErrorMsg(file, line)																		\
{																										\
	GLenum errorCode;																					\
	while ((errorCode = glGetError()) != GL_NO_ERROR)													\
	{																									\
		std::string error;																				\
		switch (errorCode)																				\
		{																								\
		case GL_INVALID_ENUM:                  error = "INVALID_ENUM";					break;			\
		case GL_INVALID_VALUE:                 error = "INVALID_VALUE";					break;			\
		case GL_INVALID_OPERATION:             error = "INVALID_OPERATION";				break;			\
		case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW";				break;			\
		case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW";				break;			\
		case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY";					break;			\
		case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;			\
		}																								\
		printf("%0 | %0 (%d)", error.c_str(), file, line);												\
	}																									\
}

#elif 
#define #define PrintGLErrorMsg(file, line)		void(0);
#endif


class Render : public QOpenGLWidget,protected QOpenGLFunctions_4_3_Core
{
	Q_OBJECT

public:
    Render(QWidget * parent = nullptr);

// 三个必须实现的虚函数
    void initializeGL()override;
    void paintGL()override;
    void resizeGL(int w, int h) override;

	void create_mesh_for_OpenGL();


	MeshModel * mm;
	bool show_bbx;
	enum mode{ WIRE_FRAME, FACE_WIRE, ONLY_FACE, POINT } render_mode;

protected:

	void mouseMoveEvent(QMouseEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;

private:

    QOpenGLVertexArrayObject vao;
    QOpenGLBuffer vbo;
    QOpenGLBuffer ebo;

    QOpenGLShaderProgram program;
	QMatrix4x4 projection;

	FocusCamera camera;
	QPoint last_mouse_pos;	// Two dimensions

public slots:

	void set_ONLY_FACE_mode();
	void set_FACE_WIRE_mode();
	void set_WIRE_FRAME_mode();
	void set_POINT_mode();
	void set_BBX_mode();
};