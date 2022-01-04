#include "pch.h"
#include "hilt.h"
#include <engine.h>		

engine::hilt::hilt(std::vector<glm::vec3> vertices) : m_vertices(vertices)
{
	std::vector<glm::vec3> normals;

	//Guard
	
	// Front face 1 normal
	normals.push_back(glm::cross(vertices.at(1) - vertices.at(0), vertices.at(1) - vertices.at(2)));
	// Front face 2 normal
	normals.push_back(glm::cross(vertices.at(2) - vertices.at(3), vertices.at(2) - vertices.at(1)));

	// Back face 1 normal
	normals.push_back(glm::cross(vertices.at(5) - vertices.at(6), vertices.at(5) - vertices.at(4)));
	// Back face 2 normal
	normals.push_back(glm::cross(vertices.at(5) - vertices.at(7), vertices.at(5) - vertices.at(6)));

	// Bottom face 1 normal
	normals.push_back(glm::cross(vertices.at(3) - vertices.at(5), vertices.at(3) - vertices.at(1)));
	// Bottom face 2 normal
	normals.push_back(glm::cross(vertices.at(3) - vertices.at(7), vertices.at(3) - vertices.at(5)));

	// Top face 1 normal
	normals.push_back(glm::cross(vertices.at(4) - vertices.at(2), vertices.at(4) - vertices.at(0)));
	// Top face 2 normal
	normals.push_back(glm::cross(vertices.at(6) - vertices.at(2), vertices.at(6) - vertices.at(4)));

	// Left face 1 normal
	normals.push_back(glm::cross(vertices.at(4) - vertices.at(0), vertices.at(4) - vertices.at(1)));
	// Left face 2 normal
	normals.push_back(glm::cross(vertices.at(1) - vertices.at(5), vertices.at(1) - vertices.at(4)));

	// Right face 1 normal
	normals.push_back(glm::cross(vertices.at(3) - vertices.at(2), vertices.at(3) - vertices.at(6)));
	// Right face 2 normal
	normals.push_back(glm::cross(vertices.at(6) - vertices.at(7), vertices.at(6) - vertices.at(3)));

	//Holder

	// Front face 1 normal
	normals.push_back(glm::cross(vertices.at(9) - vertices.at(8), vertices.at(9) - vertices.at(10)));
	// Front face 2 normal
	normals.push_back(glm::cross(vertices.at(9) - vertices.at(10), vertices.at(9) - vertices.at(11)));

	// Back face 1 normal
	normals.push_back(glm::cross(vertices.at(12) - vertices.at(13), vertices.at(12) - vertices.at(14)));
	// Back face 2 normal
	normals.push_back(glm::cross(vertices.at(14) - vertices.at(13), vertices.at(14) - vertices.at(15)));

	// Right face 1 normal
	normals.push_back(glm::cross(vertices.at(9) - vertices.at(13), vertices.at(9) - vertices.at(8)));
	// Right face 2 normal
	normals.push_back(glm::cross(vertices.at(8) - vertices.at(13), vertices.at(8) - vertices.at(12)));

	// Left face 1 normal
	normals.push_back(glm::cross(vertices.at(15) - vertices.at(11), vertices.at(15) - vertices.at(10)));
	// Left face 2 normal
	normals.push_back(glm::cross(vertices.at(14) - vertices.at(15), vertices.at(14) - vertices.at(10)));

	// Bottom face 1 normal
	normals.push_back(glm::cross(vertices.at(9) - vertices.at(11), vertices.at(9) - vertices.at(13)));
	// Bottom face 2 normal
	normals.push_back(glm::cross(vertices.at(11) - vertices.at(15), vertices.at(11) - vertices.at(13)));


	std::vector<mesh::vertex> hilt_vertices
	{

		//  position			normal			      tex coord

		//Guard
		
		//Front face 1
		{ vertices.at(1),		normals.at(0),		{ 0.f,  0.f } },
		{ vertices.at(0),		normals.at(0),		{ 1.f,  0.f } },
		{ vertices.at(2),		normals.at(0),		{ 0.5f, 1.f } },

		//Front face 2
		{ vertices.at(2),		normals.at(1),		{ 0.f,  0.f } },
		{ vertices.at(3),		normals.at(1),		{ 1.f,  0.f } },
		{ vertices.at(1),		normals.at(1),		{ 0.5f, 1.f } },

		//Back face 1
		{ vertices.at(5),		normals.at(2),		{ 0.f,  0.f } },
		{ vertices.at(6),		normals.at(2),		{ 1.f,  0.f } },
		{ vertices.at(4),		normals.at(2),		{ 0.5f, 1.f } },

		//Back face 2
		{ vertices.at(5),		normals.at(3),		{ 0.f,  0.f } },
		{ vertices.at(7),		normals.at(3),		{ 1.f,  0.f } },
		{ vertices.at(6),		normals.at(3),		{ 0.5f, 1.f } },

		//Bottom face 1      
		{ vertices.at(3),		normals.at(4),		{ 0.f,  0.f } },
		{ vertices.at(5),		normals.at(4),		{ 1.f,  0.f } },
		{ vertices.at(1),		normals.at(4),		{ 0.5f, 1.f } },

		//Bottom face 2
		{ vertices.at(3),		normals.at(5),		{ 0.f,  0.f } },
		{ vertices.at(7),		normals.at(5),		{ 1.f,  0.f } },
		{ vertices.at(5),		normals.at(5),		{ 0.5f, 1.f } },

		//Top face 1      
		{ vertices.at(4),		normals.at(6),		{ 0.f,  0.f } },
		{ vertices.at(2),		normals.at(6),		{ 1.f,  0.f } },
		{ vertices.at(0),		normals.at(6),		{ 0.5f, 1.f } },

		//Top face 2
		{ vertices.at(6),		normals.at(7),		{ 0.f,  0.f } },
		{ vertices.at(2),		normals.at(7),		{ 1.f,  0.f } },
		{ vertices.at(4),		normals.at(7),		{ 0.5f, 1.f } },

		//Left face 1      
		{ vertices.at(4),		normals.at(8),		{ 0.f,  0.f } },
		{ vertices.at(0),		normals.at(8),		{ 1.f,  0.f } },
		{ vertices.at(1),		normals.at(8),		{ 0.5f, 1.f } },

		//Left face 2
		{ vertices.at(1),		normals.at(9),		{ 0.f,  0.f } },
		{ vertices.at(5),		normals.at(9),		{ 1.f,  0.f } },
		{ vertices.at(4),		normals.at(9),		{ 0.5f, 1.f } },

		//Right face 1      
		{ vertices.at(3),		normals.at(10),		{ 0.f,  0.f } },
		{ vertices.at(2),		normals.at(10),		{ 1.f,  0.f } },
		{ vertices.at(6),		normals.at(10),		{ 0.5f, 1.f } },

		//Right face 2
		{ vertices.at(6),		normals.at(11),		{ 0.f,  0.f } },
		{ vertices.at(7),		normals.at(11),		{ 1.f,  0.f } },
		{ vertices.at(3),		normals.at(11),		{ 0.5f, 1.f } },

		//Holder

		//Front face 1
		{ vertices.at(9),		normals.at(12),		{ 0.f,  0.f } },
		{ vertices.at(8),		normals.at(12),		{ 1.f,  0.f } },
		{ vertices.at(10),		normals.at(12),		{ 0.5f, 1.f } },

		//Front face 2
		{ vertices.at(10),		normals.at(13),		{ 0.f,  0.f } },
		{ vertices.at(11),		normals.at(13),		{ 1.f,  0.f } },
		{ vertices.at(9),		normals.at(13),		{ 0.5f, 1.f } },

		//Back face 1
		{ vertices.at(12),		normals.at(14),		{ 0.f,  0.f } },
		{ vertices.at(13),		normals.at(14),		{ 1.f,  0.f } },
		{ vertices.at(14),		normals.at(14),		{ 0.5f, 1.f } },

		//Back face 2
		{ vertices.at(14),		normals.at(15),		{ 0.f,  0.f } },
		{ vertices.at(13),		normals.at(15),		{ 1.f,  0.f } },
		{ vertices.at(15),		normals.at(15),		{ 0.5f, 1.f } },

		//Right face 1
		{ vertices.at(9),		normals.at(16),		{ 0.f,  0.f } },
		{ vertices.at(13),		normals.at(16),		{ 1.f,  0.f } },
		{ vertices.at(8),		normals.at(16),		{ 0.5f, 1.f } },

		//Right face 2
		{ vertices.at(8),		normals.at(17),		{ 0.f,  0.f } },
		{ vertices.at(13),		normals.at(17),		{ 1.f,  0.f } },
		{ vertices.at(12),		normals.at(17),		{ 0.5f, 1.f } },

		//Left face 1
		{ vertices.at(10),		normals.at(18),		{ 0.f,  0.f } },
		{ vertices.at(15),		normals.at(18),		{ 1.f,  0.f } },
		{ vertices.at(11),		normals.at(18),		{ 0.5f, 1.f } },

		//Left face 2
		{ vertices.at(14),		normals.at(19),		{ 0.f,  0.f } },
		{ vertices.at(15),		normals.at(19),		{ 1.f,  0.f } },
		{ vertices.at(10),		normals.at(19),		{ 0.5f, 1.f } },

		//Bottom face 1
		{ vertices.at(9),		normals.at(20),		{ 0.f,  0.f } },
		{ vertices.at(11),		normals.at(20),		{ 1.f,  0.f } },
		{ vertices.at(13),		normals.at(20),		{ 0.5f, 1.f } },

		//Bottom face 2
		{ vertices.at(11),		normals.at(21),		{ 0.f,  0.f } },
		{ vertices.at(15),		normals.at(21),		{ 1.f,  0.f } },
		{ vertices.at(13),		normals.at(21),		{ 0.5f, 1.f } },
	};

	const std::vector<uint32_t> hilt_indices
	{
		//Guard
		0,  1,  2,	//Top front
		3,  4,  5,	//Top left
		6,	7,	8,	//Top right
		9,	10,	11,	//Top back
		12, 13, 14, //Bottom front
		15, 16, 17, //Bottom left
		18, 19, 20,
		21, 22, 23,
		24, 25, 26,
		27, 28, 29,
		30, 31, 32,
		33, 34, 35,

		//Holder
		36, 37, 38,
		39, 40, 41,
		42, 43, 44,
		45, 46, 47,
		48, 49, 50,
		51, 52, 53,
		54, 55, 56,
		57, 58, 59,
		60, 61, 62,
		63, 64, 65,
		66, 67, 68
	};
	//Create the mesh with the verticies and indicies
	m_mesh = engine::mesh::create(hilt_vertices, hilt_indices);
}

engine::hilt::~hilt() {}

engine::ref<engine::hilt> engine::hilt::create(std::vector<glm::vec3> vertices)
{
	return std::make_shared<engine::hilt>(vertices);
}


