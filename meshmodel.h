#ifndef MESHMODEL_H
#define MESHMODEL_H

#include <QVector3D>
#include <vector>
#include <QString>

class MeshModel
{
public:
	 MeshModel(const QString& _file_path);

		struct Face
		{
			unsigned int v1{}, v2{}, v3{};
			QVector3D normal;
		};

	std::vector<QVector3D> vertices, normals;
	std::vector<Face> faces;
	std::vector<uint> indices;

	QVector3D v_min, v_max;

	QString name;
};

#endif // MESHMODEL_H
