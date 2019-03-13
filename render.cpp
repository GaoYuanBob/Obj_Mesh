#include "render.h"
#include "meshmodel.h"
#include <QMouseEvent>

Render::Render(QWidget *parent):QOpenGLWidget(parent),vbo(QOpenGLBuffer::VertexBuffer),ebo(QOpenGLBuffer::IndexBuffer),camera({0, 0, 5})
{
    // 不能初始化任何和OpenGL相关的东西，因为没有建立context
	projection.setToIdentity();
	mm = nullptr;	// 必须先初始化，为了paintGL()的判断
	render_mode = FACE_WIRE;
	show_bbx = true;
}

void Render::initializeGL()	// 这个函数就跑一次
{
	initializeOpenGLFunctions();

	glClearColor(0.8, 0.8, 0.8, 1);
	glEnable(GL_DEPTH_TEST);

	program.create();
	program.bind();
	program.addShaderFromSourceFile(QOpenGLShader::Vertex, QStringLiteral(R"(E:\QtOpenGLProject\qtopengl\shadersource\phoneshadingvert.glsl)"));
	program.addShaderFromSourceFile(QOpenGLShader::Fragment, QStringLiteral(R"(E:\QtOpenGLProject\qtopengl\shadersource\phoneshadingfrag.glsl)"));
	program.link();
	program.release(); // release就是解绑

	vao.create();
	ebo.create();
	vbo.create();
}

void Render::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (mm == nullptr)
		return;

	program.bind();

	PrintGLErrorMsg(__FILE__, __LINE__);

	QMatrix4x4 model;
	model.setToIdentity();

	program.setUniformValue("model_matrix", model);
	program.setUniformValue("view_matrix", camera.view());
	program.setUniformValue("projection_matrix", projection);
	program.setUniformValue("light_pos", camera.position());
	program.setUniformValue("light_color", QVector3D{ 1.0f, 1.0f, 1.0f });
	program.setUniformValue("object_color", QVector4D{ 0.7f, 0.7f, 0.7f, 1 });
	program.setUniformValue("view_pos", camera.position());

	QOpenGLVertexArrayObject::Binder binder(&vao);

	PrintGLErrorMsg(__FILE__, __LINE__);

	switch(render_mode)
	{
		case ONLY_FACE:
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				// 如果有ebo，最后一个参数就是0，如果没有，最后一个参数就是所有面的点的索引的数组
				// 第二个参数是 indices 大小，也就是面片数 * 3
				glDrawElements(GL_TRIANGLES, mm->indices.size(), GL_UNSIGNED_INT, 0);
				break;
			}
		case FACE_WIRE:
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				// 如果有ebo，最后一个参数就是0，如果没有，最后一个参数就是所有面的点的索引的数组
				// 第二个参数是 indices 大小，也就是面片数 * 3
				glDrawElements(GL_TRIANGLES, mm->indices.size(), GL_UNSIGNED_INT, 0);

				// 画出线框
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				program.setUniformValue("object_color", QVector4D{ 0.1f, 0.1f, 0.1f, 1 });
				glDrawElements(GL_TRIANGLES, mm->indices.size(), GL_UNSIGNED_INT, 0);
				break;
			}
		case WIRE_FRAME:
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				program.setUniformValue("object_color", QVector4D{ 0.1f, 0.1f, 0.1f, 1 });
				glDrawElements(GL_TRIANGLES, mm->indices.size(), GL_UNSIGNED_INT, 0);	// 不能用GL_INT
				break;
			}
		case POINT:
			{
			glPointSize(4.0);
				program.setUniformValue("object_color", QVector4D{ 0.1f, 0.1f, 0.1f, 1 });
				glDrawArrays(GL_POINTS, 0, mm->vertices.size());
				break;
			}
		default:
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				// 如果有ebo，最后一个参数就是0，如果没有，最后一个参数就是所有面的点的索引的数组
				// 第二个参数是 indices 大小，也就是面片数 * 3
				glDrawElements(GL_TRIANGLES, mm->indices.size(), GL_UNSIGNED_INT, 0);
				break;
			}
	}

	// 绘制包围盒
	if(show_bbx)
	{
		
	}

	program.release();
}

void Render::resizeGL(int w, int h)
{
	projection.setToIdentity();
	projection.perspective(45, 1.0 * w / h, 0.01, 100);

	//update();		// 加入刷新的渲染队列，内部有优化
	//repaint();	// 立即刷新绘制
}

void Render::create_mesh_for_OpenGL()
{
	makeCurrent();		// 在三个必须重载的虚函数外的其他地方修改OpenGL状态机的东西，都需要 makeCurrent() 和 doneCurrent();

	vao.bind();

	vbo.bind();
	const auto vbytes = mm->vertices.size() * sizeof(QVector3D);
	//const auto nbytes = mm->vertices.size() * sizeof(QVector3D);
	const auto nbytes = vbytes;
	const auto bytes = vbytes + nbytes;
	vbo.allocate(bytes);
	vbo.write(0, mm->vertices.data(), vbytes);
	vbo.write(vbytes, mm->normals.data(), nbytes);

	glEnableVertexAttribArray(0);	// 不加这个点坐标都是0？!
	glEnableVertexAttribArray(1);

	// 最后一个参数是相对起始位置的指针类型的偏移
	// 一般都是0顶点，1法向，2纹理
	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(QVector3D), reinterpret_cast<void*>(0));
	glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(QVector3D), reinterpret_cast<void*>(vbytes));

	ebo.bind();
	ebo.allocate(mm->indices.data(), mm->indices.size() * sizeof(uint));

	vao.release();	// vao 会记录 vbo、ebo所有操作，所以需要先release否则vao的release也被记录

	vbo.release();
	ebo.release();

	doneCurrent();

	update();		// 不写这个需要在鼠标点击一下屏幕才会刷新, update了模型会直接显示出来
}

void Render::mouseMoveEvent(QMouseEvent * event)
{
	const auto & p = event->pos();
	// Update Camera
	const float dx = p.x() - last_mouse_pos.x();
	const float dy = last_mouse_pos.y() - p.y();

	if (dx == 0 && dy == 0)
		return;
	if ((event->buttons() & Qt::MouseButton::LeftButton) && (event->buttons() & Qt::MouseButton::RightButton))
	{
		const auto directionEx = camera.up()*dy + dx * camera.right();
		camera.movement(directionEx, 0.001);
	}
	else if (event->buttons() & Qt::MouseButton::LeftButton)
	{
		camera.rotation(dx, dy);
	}
	else if (event->buttons() == Qt::MouseButton::RightButton)
	{
		const auto directionEx = camera.front()*dy;
		camera.movement(directionEx, 0.01);
	}

	last_mouse_pos = p;
	update();
}

void Render::mousePressEvent(QMouseEvent * event)
{
	last_mouse_pos = event->pos();
}

void Render::set_ONLY_FACE_mode()
{
	makeCurrent();
	render_mode = ONLY_FACE;
	doneCurrent();
	update();
}

void Render::set_FACE_WIRE_mode()
{
	makeCurrent();
	render_mode = FACE_WIRE;
	doneCurrent();
	update();
}

void Render::set_WIRE_FRAME_mode()
{
	makeCurrent();
	render_mode = WIRE_FRAME;
	doneCurrent();
	update();
}
void Render::set_POINT_mode()
{
	makeCurrent();
	render_mode = POINT;
	doneCurrent();
	update();
}
void Render::set_BBX_mode()
{
	show_bbx = true;
}