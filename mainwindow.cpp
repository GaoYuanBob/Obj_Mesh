#include "mainwindow.h"
#include <QLayout>
#include <QDockWidget>
#include <QMenuBar>
#include <QMenu>
#include <slices.h>
#include <mesh_info_widget.h>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
	renderView = new Render(this);
	setCentralWidget(renderView);

	auto * mesh_info = new mesh_info_widget(this);

	connect(this, &MainWindow::send_mesh_info, mesh_info, &mesh_info_widget::get_mesh_info);	// MainWindow 发送的信号，第一个参数就是this

	QWidget * widget_dock2 = new QWidget(this);

    auto * slices = new Slices(this);

	auto menu_bar = menuBar();

	auto file_menu = menu_bar->addMenu("Files");
	auto *openAct = new QAction(QIcon(), tr("&Open..."), this);
	openAct->setShortcuts(QKeySequence::Open);
	openAct->setStatusTip(tr("Open an obj file"));
	file_menu->addAction(openAct);

	connect(openAct, &QAction::triggered, this, &MainWindow::open_obj_from_folder);

	auto render_mode_menu = menu_bar->addMenu("Render Modes");

	// 只绘制面
	auto *only_face = new QAction(QIcon(), tr("Only Face"), this);
	only_face->setStatusTip(tr(R"(render model in "ONLY_FACE" mode)"));
	connect(only_face, &QAction::triggered, renderView, &Render::set_ONLY_FACE_mode);
	render_mode_menu->addAction(only_face);

	// 绘制面和边
	auto *face_wire = new QAction(QIcon(), tr("Face Wire"), this);
	face_wire->setStatusTip(tr(R"(render model in "FACE_WIRE" mode)"));
	connect(face_wire, &QAction::triggered, renderView, &Render::set_FACE_WIRE_mode);
	render_mode_menu->addAction(face_wire);

	// 只绘制线框
	auto *wire_frame = new QAction(QIcon(), tr("Wire Frame"), this);
	wire_frame->setStatusTip(tr(R"(render model in "WIRE_FRAME" mode)"));
	connect(wire_frame, &QAction::triggered, renderView, &Render::set_WIRE_FRAME_mode);
	render_mode_menu->addAction(wire_frame);

	// 只绘制点
	auto *only_point = new QAction(QIcon(), tr("Only Point"), this);
	only_point->setStatusTip(tr(R"(render model in "WIRE_FRAME" mode)"));
	connect(only_point, &QAction::triggered, renderView, &Render::set_POINT_mode);
	render_mode_menu->addAction(only_point);

	// 绘制包围盒
	auto *bbx = new QAction(QIcon(), tr("Bounding Box"), this);
	bbx->setStatusTip(tr(R"(render model in "Bounding_Box" mode)"));
	connect(bbx, &QAction::triggered, renderView, &Render::set_BBX_mode);
	render_mode_menu->addAction(bbx);

	auto dock1 = new QDockWidget("Mesh");
	dock1->setWidget(mesh_info);
	dock1->setFixedWidth(250);
	addDockWidget(Qt::RightDockWidgetArea, dock1);

	auto dock2 = new QDockWidget("CBCT");
	dock2->setWidget(widget_dock2);
	dock2->setFixedWidth(250);
	addDockWidget(Qt::RightDockWidgetArea, dock2);

	auto dock3 = new QDockWidget("Slices");
    dock3->setWidget(slices);
	dock3->setFixedWidth(250);
	addDockWidget(Qt::RightDockWidgetArea, dock3);
}

MainWindow::~MainWindow() = default;

void MainWindow::open_obj_from_folder()
{
	printf("open obj file from folder\n");
	if (renderView->mm != nullptr)
	{
		delete renderView->mm;
		renderView->mm = nullptr;
	}

	const auto file_name = QFileDialog::getOpenFileName(this, tr("Open Obj File"), "/home", tr("*.obj"));
	if (file_name.isEmpty())
		return;

	renderView->mm = new MeshModel(file_name);

	emit send_mesh_info(renderView->mm->name, renderView->mm->vertices.size(), renderView->mm->faces.size());

	renderView->create_mesh_for_OpenGL();
}