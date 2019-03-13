#ifndef MESH_INFO_WIDGET_H
#define MESH_INFO_WIDGET_H

#include <QWidget>
#include "meshmodel.h"

class QLabel;

class mesh_info_widget : public QWidget
{
    Q_OBJECT
public:
    explicit mesh_info_widget(QWidget *parent = 0);

private:
    QLabel *name_label, *vert_label, *face_label;

signals:

	public slots :

		void get_mesh_info(const QString& _name, const uint& _vs, const uint& _fs) const;
};

#endif // MESH_INFO_WIDGET_H
