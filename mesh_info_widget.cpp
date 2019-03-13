#include "mesh_info_widget.h"
#include <QLayout>
#include <QLabel>

mesh_info_widget::mesh_info_widget(QWidget *parent) : QWidget(parent)
{

    name_label = new QLabel("Name: ");
    vert_label = new QLabel("Num of vertices: ");
    face_label = new QLabel("Num of faces: ");

	// ��˳����±߲���
    auto vLayout = new QVBoxLayout;
    vLayout->addWidget(name_label);
    vLayout->addWidget(vert_label);
    vLayout->addWidget(face_label);

	vLayout->addStretch();	// ���±߲���һ�����ɣ�

    setLayout(vLayout);
}


void mesh_info_widget::get_mesh_info(const QString& _name, const uint& _vs, const uint& _fs) const
{
	printf("seting mesh info\n");
	name_label->setText(_name);
	vert_label->setText(QString("Vertices: ") + QString::number(_vs));
	face_label->setText(QString("faces:    ") + QString::number(_fs));
}