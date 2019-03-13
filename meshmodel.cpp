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
	this->indices.resize(shapes[0].mesh.num_face_vertices.size() * 3);	// indices 大小为 faces数量 * 3
	this->vertices.resize(attrib.vertices.size() / 3);
	this->normals.resize(vertices.size());
	
	// 1、设置 MeshModel 的 faces 属性，包括三角形三个点的索引( unsigned int )，以及面的法向量( QVector3D )
	printf("读入并设置 faces 属性\n");
	for (size_t f = 0; f < shapes[0].mesh.num_face_vertices.size(); f++)
	{
		// 模型应该只包含三角形mesh
		if (shapes[0].mesh.num_face_vertices[f] != 3)
		{
			cerr << "This Obj contains not only triangles!!" << endl;
			exit(-1);
		}

		this->faces[f].v1 = shapes[0].mesh.indices[f * 3].vertex_index;		// 面上第一个点的索引
		this->faces[f].v2 = shapes[0].mesh.indices[f * 3 + 1].vertex_index;	// 面上第二个点的索引
		this->faces[f].v3 = shapes[0].mesh.indices[f * 3 + 2].vertex_index;	// 面上第三个点的索引

		// printf("第 %llu 个面的三个顶点索引为: %d, %d, %d\n", f, faces[f].v1, faces[f].v2, faces[f].v3);
	}

	// 2、设置 indices，为 OpenGL ebo 为准备
	for (uint i = 0; i < shapes[0].mesh.indices.size(); ++i)
		this->indices[i] = shapes[0].mesh.indices[i].vertex_index;

	// 输出 indices 进行查看
	//printf("\nindices 如下（每三个一组输出）:\n");
	//for (uint i = 0; i < indices.size(); i = i + 3)
	//	printf("%d %d %d\n", indices[i], indices[i + 1], indices[i + 2]);


	// 初始化最小xyz以及最大xyz
	auto min_value = numeric_limits<float>::max(), max_value = -min_value;
	// printf("min_value = %f, max_value = %f\n", min_value, max_value);
	v_min = QVector3D{ min_value, min_value, min_value };
	v_max = QVector3D{ max_value, max_value, max_value };

	// 3、设置 MeshModel 的 vertices 属性( vector<QVector3D> )，即顶点坐标
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

	// 输出顶点坐标进行查看
	//printf("\n所有点的坐标如下:\n");
	//for (auto v : vertices)
	//	printf("(%.2f, %.2f, %.2f)\n", v.x(), v.y(), v.z());

	// 4、设置 MeshModel 的 normals 属性( vector<QVector3D> )，即顶点法向量
	for (unsigned long long i = 0; i < normals.size(); i++)
		normals[i] = { attrib.normals[3 * i], attrib.normals[3 * i + 1], attrib.normals[3 * i + 2] };

	// 输出顶点坐标进行查看
	//printf("\n所有点的法向量如下:\n");
	//for (auto n : normals)
	//	printf("(%.2f, %.2f, %.2f)\n", n.x(), n.y(), n.z());
}