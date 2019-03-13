#include "meshmodel.h"

#define TINYOBJLOADER_IMPLEMENTATION // define this in only *one* .cc
#include "objloader.h"
#include <QString>

#include <limits>
#include <iostream>
using namespace std;


MeshModel::MeshModel(const QString& _file_name)
{
	name = _file_name;

	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn;
	std::string err;

	auto ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, _file_name.toStdString().c_str());

	if (!err.empty())
		std::cerr << err << std::endl;

	if (!ret)
		exit(1);

	if(shapes.size() != 1)
	{
		cerr << "One obj contains two or more models??" << endl;
		exit(-1);
	}

	printf("number of faces = %lld\n", shapes[0].mesh.num_face_vertices.size());
	printf("number of vertices = %lld\n", attrib.vertices.size() / 3);
	printf("number of vert normals = %lld\n\n\n", attrib.normals.size() / 3);

	this->faces.resize(shapes[0].mesh.num_face_vertices.size());
	this->indices.resize(shapes[0].mesh.num_face_vertices.size() * 3);	// indices ��СΪ faces���� * 3
	this->vertices.resize(attrib.vertices.size() / 3);
	this->normals.resize(vertices.size());
	
	// 1������ MeshModel �� faces ���ԣ����������������������( unsigned int )���Լ���ķ�����( QVector3D )
	printf("���벢���� faces ����\n");
	for (size_t f = 0; f < shapes[0].mesh.num_face_vertices.size(); f++)
	{
		// ģ��Ӧ��ֻ����������mesh
		if (shapes[0].mesh.num_face_vertices[f] != 3)
		{
			cerr << "This Obj contains not only triangles!!" << endl;
			exit(-1);
		}

		this->faces[f].v1 = shapes[0].mesh.indices[f * 3].vertex_index;		// ���ϵ�һ���������
		this->faces[f].v2 = shapes[0].mesh.indices[f * 3 + 1].vertex_index;	// ���ϵڶ����������
		this->faces[f].v3 = shapes[0].mesh.indices[f * 3 + 2].vertex_index;	// ���ϵ������������

		// printf("�� %llu �����������������Ϊ: %d, %d, %d\n", f, faces[f].v1, faces[f].v2, faces[f].v3);
	}

	// 2������ indices��Ϊ OpenGL ebo Ϊ׼��
	for (uint i = 0; i < shapes[0].mesh.indices.size(); ++i)
		this->indices[i] = shapes[0].mesh.indices[i].vertex_index;

	// ��� indices ���в鿴
	//printf("\nindices ���£�ÿ����һ�������:\n");
	//for (uint i = 0; i < indices.size(); i = i + 3)
	//	printf("%d %d %d\n", indices[i], indices[i + 1], indices[i + 2]);


	// ��ʼ����Сxyz�Լ����xyz
	auto min_value = numeric_limits<float>::max(), max_value = -min_value;
	// printf("min_value = %f, max_value = %f\n", min_value, max_value);
	v_min = QVector3D{ min_value, min_value, min_value };
	v_max = QVector3D{ max_value, max_value, max_value };

	// 3������ MeshModel �� vertices ����( vector<QVector3D> )������������
	for (unsigned long long i = 0; i < vertices.size(); i++)
	{
		vertices[i] = { attrib.vertices[3 * i], attrib.vertices[3 * i + 1], attrib.vertices[3 * i + 2] };
		v_min[0] = min(vertices[i].x(), v_min[0]);
		v_min[1] = min(vertices[i].y(), v_min[1]);
		v_min[2] = min(vertices[i].z(), v_min[2]);
		v_max[0] = max(vertices[i].x(), v_max[0]);
		v_max[1] = max(vertices[i].y(), v_max[1]);
		v_max[2] = max(vertices[i].z(), v_max[2]);
	}
	// printf("v_min = (%.2f, %.2f, %.2f), v_max = (%.2f, %.2f, %.2f)\n", v_min.x(), v_min.y(), v_min.z(), v_max.x(), v_max.y(), v_max.z());

	// �������������в鿴
	//printf("\n���е����������:\n");
	//for (auto v : vertices)
	//	printf("(%.2f, %.2f, %.2f)\n", v.x(), v.y(), v.z());

	// 4������ MeshModel �� normals ����( vector<QVector3D> )�������㷨����
	for (unsigned long long i = 0; i < normals.size(); i++)
		normals[i] = { attrib.normals[3 * i], attrib.normals[3 * i + 1], attrib.normals[3 * i + 2] };

	// �������������в鿴
	//printf("\n���е�ķ���������:\n");
	//for (auto n : normals)
	//	printf("(%.2f, %.2f, %.2f)\n", n.x(), n.y(), n.z());
}