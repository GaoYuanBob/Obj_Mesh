#pragma once

#include <QMainWindow>

#include "render.h"

class MeshModel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

	Render * renderView;

    MainWindow(QWidget *parent = 0);
    ~MainWindow();

	public slots:	
	
	void open_obj_from_folder();

signals:
	void send_mesh_info(QString _name, uint _vs, uint _fs);		// 信号函数不需要实现，只需要在头文件中声明
};