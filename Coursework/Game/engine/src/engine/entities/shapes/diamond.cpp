#include "pch.h"
#include "diamond.h"
#include <engine.h>		

engine::diamond::diamond(std::vector<glm::vec3> vertices) : m_vertices(vertices)
{
	std::vector<glm::vec3> normals;
	// Top front normal
	normals.push_back(glm::cross(vertices.at(0) - vertices.at(2), vertices.at(0) - vertices.at(1)));
	// Top left normal
	normals.push_back(glm::cross(vertices.at(0) - vertices.at(3), vertices.at(0) - vertices.at(2)));
	// Top right normal
	normals.push_back(glm::cross(vertices.at(0) - vertices.at(4), vertices.at(0) - vertices.at(3)));
	// Top back normal
	normals.push_back(glm::cross(vertices.at(0) - vertices.at(1), vertices.at(0) - vertices.at(4)));

	// Bottom front normal
	normals.push_back(glm::cross(vertices.at(5) - vertices.at(4), vertices.at(5) - vertices.at(3)));
	// Bottom left normal
	normals.push_back(glm::cross(vertices.at(5) - vertices.at(3), vertices.at(5) - vertices.at(2)));
	// Bottom right normal
	normals.push_back(glm::cross(vertices.at(5) - vertices.at(2), vertices.at(5) - vertices.at(1)));
	// Bottom back normal
	normals.push_back(glm::cross(vertices.at(5) - vertices.at(1), vertices.at(5) - vertices.at(4)));

	std::vector<mesh::vertex> diamond_vertices
	{
		
		//  position			normal			      tex coord

		//Top front
		{ vertices.at(0),		normals.at(0),		{ 0.f,  0.f } },
		{ vertices.at(2),		normals.at(0),		{ 1.f,  0.f } },
		{ vertices.at(1),		normals.at(0),		{ 0.5f, 1.f } },

		//Top left
		{ vertices.at(0),		normals.at(1),		{ 0.f,  0.f } },
		{ vertices.at(3),		normals.at(1),		{ 1.f,  0.f } },
		{ vertices.at(2),		normals.at(1),		{ 0.5f, 1.f } },

		//Top right
		{ vertices.at(3),		normals.at(2),		{ 0.f,  0.f } },
		{ vertices.at(0),		normals.at(2),		{ 1.f,  0.f } },
		{ vertices.at(4),		normals.at(2),		{ 0.5f, 1.f } },

		//Top back
		{ vertices.at(0),		normals.at(3),		{ 0.f,  0.f } },
		{ vertices.at(1),		normals.at(3),		{ 1.f,  0.f } },
		{ vertices.at(4),		normals.at(3),		{ 0.5f, 1.f } },

		//Bottom front      
		{ vertices.at(5),		normals.at(4),		{ 0.f,  0.f } },
		{ vertices.at(3),		normals.at(4),		{ 1.f,  0.f } },
		{ vertices.at(4),		normals.at(4),		{ 0.5f, 1.f } },

		//Bottom left
		{ vertices.at(5),		normals.at(5),		{ 0.f,  0.f } },
		{ vertices.at(2),		normals.at(5),		{ 1.f,  0.f } },
		{ vertices.at(3),		normals.at(5),		{ 0.5f, 1.f } },

		//Bottom right
		{ vertices.at(5),		normals.at(6),		{ 0.f,  0.f } },
		{ vertices.at(1),		normals.at(6),		{ 1.f,  0.f } },
		{ vertices.at(2),		normals.at(6),		{ 0.5f, 1.f } },

		//Bottom back
		{ vertices.at(5),		normals.at(7),		{ 0.f,  0.f } },
		{ vertices.at(4),		normals.at(7),		{ 1.f,  0.f } },
		{ vertices.at(1),		normals.at(7),		{ 0.5f, 1.f } },

	};

	const std::vector<uint32_t> diamond_indices
	{
		0,  1,  2,	//Top front
		3,  4,  5,	//Top left
		6,	7,	8,	//Top right
		9,	10,	11,	//Top back
		12, 13, 14, //Bottom front
		15, 16, 17, //Bottom left
		18, 19, 20, //Bottom right
		21, 22, 23  //Bottom back
	};
	//Create the mesh with the verticies and indicies
	m_mesh = engine::mesh::create(diamond_vertices, diamond_indices);
}

engine::diamond::~diamond() {}

engine::ref<engine::diamond> engine::diamond::create(std::vector<glm::vec3> vertices)
{
	return std::make_shared<engine::diamond>(vertices);
}


