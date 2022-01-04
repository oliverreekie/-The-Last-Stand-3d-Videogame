#include "pch.h"
#include "shield.h"
#include <engine.h>		

engine::shield::shield(std::vector<glm::vec3> vertices) : m_vertices(vertices)
{
	std::vector<glm::vec3> normals;
	//Face
	// Top front normal
	normals.push_back(glm::cross(vertices.at(0) - vertices.at(12), vertices.at(0) - vertices.at(11)));
	// Top left normal
	normals.push_back(glm::cross(vertices.at(11) - vertices.at(10), vertices.at(11) - vertices.at(0)));

	// Top right normal
	normals.push_back(glm::cross(vertices.at(11) - vertices.at(12), vertices.at(11) - vertices.at(1)));
	// Top back normal
	normals.push_back(glm::cross(vertices.at(1) - vertices.at(2), vertices.at(1) - vertices.at(11)));

	// Bottom front normal
	normals.push_back(glm::cross(vertices.at(11) - vertices.at(2), vertices.at(11) - vertices.at(3)));
	// Bottom left normal
	normals.push_back(glm::cross(vertices.at(11) - vertices.at(3), vertices.at(11) - vertices.at(4)));


	// Top front normal
	normals.push_back(glm::cross(vertices.at(11) - vertices.at(4), vertices.at(11) - vertices.at(5)));
	// Top left normal
	normals.push_back(glm::cross(vertices.at(11) - vertices.at(5), vertices.at(11) - vertices.at(6)));

	// Top right normal
	normals.push_back(glm::cross(vertices.at(6) - vertices.at(7), vertices.at(6) - vertices.at(11)));
	// Top back normal
	normals.push_back(glm::cross(vertices.at(7) - vertices.at(8), vertices.at(7) - vertices.at(11)));

	// Bottom front normal
	normals.push_back(glm::cross(vertices.at(8) - vertices.at(9), vertices.at(8) - vertices.at(11)));
	// Bottom left normal
	normals.push_back(glm::cross(vertices.at(11) - vertices.at(9), vertices.at(11) - vertices.at(10)));

	//Sides

	// Top front normal
	normals.push_back(glm::cross(vertices.at(1) - vertices.at(0), vertices.at(1) - vertices.at(13)));
	// Top left normal
	normals.push_back(glm::cross(vertices.at(13) - vertices.at(14), vertices.at(13) - vertices.at(1)));

	// Top right normal
	normals.push_back(glm::cross(vertices.at(1) - vertices.at(14), vertices.at(1) - vertices.at(15)));
	// Top back normal
	normals.push_back(glm::cross(vertices.at(15) - vertices.at(2), vertices.at(15) - vertices.at(1)));

	// Bottom front normal
	normals.push_back(glm::cross(vertices.at(0) - vertices.at(10), vertices.at(0) - vertices.at(23)));
	// Bottom left normal
	normals.push_back(glm::cross(vertices.at(23) - vertices.at(13), vertices.at(23) - vertices.at(0)));

	// Top front normal
	normals.push_back(glm::cross(vertices.at(2) - vertices.at(15), vertices.at(2) - vertices.at(16)));
	// Top left normal
	normals.push_back(glm::cross(vertices.at(16) - vertices.at(3), vertices.at(16) - vertices.at(2)));

	// Top right normal
	normals.push_back(glm::cross(vertices.at(3) - vertices.at(16), vertices.at(3) - vertices.at(17)));
	// Top back normal
	normals.push_back(glm::cross(vertices.at(17) - vertices.at(4), vertices.at(17) - vertices.at(3)));

	// Bottom front normal
	normals.push_back(glm::cross(vertices.at(4) - vertices.at(17), vertices.at(4) - vertices.at(18)));
	// Bottom left normal
	normals.push_back(glm::cross(vertices.at(18) - vertices.at(5), vertices.at(18) - vertices.at(4)));




	// Top right normal
	normals.push_back(glm::cross(vertices.at(5) - vertices.at(18), vertices.at(5) - vertices.at(19)));
	// Top back normal
	normals.push_back(glm::cross(vertices.at(19) - vertices.at(6), vertices.at(19) - vertices.at(5)));

	// Bottom front normal
	normals.push_back(glm::cross(vertices.at(20) - vertices.at(7), vertices.at(20) - vertices.at(6)));
	// Bottom left normal
	normals.push_back(glm::cross(vertices.at(6) - vertices.at(19), vertices.at(6) - vertices.at(20)));

	// Top front normal
	normals.push_back(glm::cross(vertices.at(7) - vertices.at(20), vertices.at(7) - vertices.at(21)));
	// Top left normal
	normals.push_back(glm::cross(vertices.at(21) - vertices.at(8), vertices.at(21) - vertices.at(7)));

	// Top right normal
	normals.push_back(glm::cross(vertices.at(8) - vertices.at(22), vertices.at(8) - vertices.at(9)));
	// Top back normal
	normals.push_back(glm::cross(vertices.at(22) - vertices.at(8), vertices.at(22) - vertices.at(21)));

	// Bottom front normal
	normals.push_back(glm::cross(vertices.at(9) - vertices.at(23), vertices.at(9) - vertices.at(10)));
	// Bottom left normal
	normals.push_back(glm::cross(vertices.at(9) - vertices.at(22), vertices.at(9) - vertices.at(23)));

	//Back
	// Top right normal
	normals.push_back(glm::cross(vertices.at(14) - vertices.at(25), vertices.at(14) - vertices.at(24)));
	// Top back normal
	normals.push_back(glm::cross(vertices.at(24) - vertices.at(15), vertices.at(24) - vertices.at(14)));

	// Bottom front normal
	normals.push_back(glm::cross(vertices.at(25) - vertices.at(13), vertices.at(25) - vertices.at(24)));
	// Bottom left normal
	normals.push_back(glm::cross(vertices.at(24) - vertices.at(13), vertices.at(24) - vertices.at(23)));

	// Top right normal
	normals.push_back(glm::cross(vertices.at(24) - vertices.at(23), vertices.at(24) - vertices.at(22)));
	// Top back normal
	normals.push_back(glm::cross(vertices.at(22) - vertices.at(21), vertices.at(22) - vertices.at(24)));

	// Bottom front normal
	normals.push_back(glm::cross(vertices.at(24) - vertices.at(21), vertices.at(24) - vertices.at(20)));
	// Bottom left normal
	normals.push_back(glm::cross(vertices.at(20) - vertices.at(19), vertices.at(20) - vertices.at(24)));

	// Top right normal
	normals.push_back(glm::cross(vertices.at(19) - vertices.at(18), vertices.at(19) - vertices.at(24)));
	// Top back normal
	normals.push_back(glm::cross(vertices.at(24) - vertices.at(18), vertices.at(24) - vertices.at(17)));

	// Bottom front normal
	normals.push_back(glm::cross(vertices.at(17) - vertices.at(16), vertices.at(17) - vertices.at(24)));
	// Bottom left normal
	normals.push_back(glm::cross(vertices.at(24) - vertices.at(16), vertices.at(24) - vertices.at(15)));

	std::vector<mesh::vertex> shield_vertices
	{

		//  position			normal			      tex coord
		//Face
		//Top front
		{ vertices.at(0),		normals.at(0),		{ 0.f,  0.f } },
		{ vertices.at(12),		normals.at(0),		{ 1.f,  0.f } },
		{ vertices.at(11),		normals.at(0),		{ 0.5f, 1.f } },

		//Top left
		{ vertices.at(11),		normals.at(1),		{ 0.f,  0.f } },
		{ vertices.at(10),		normals.at(1),		{ 1.f,  0.f } },
		{ vertices.at(0),		normals.at(1),		{ 0.5f, 1.f } },

		//Top right
		{ vertices.at(11),		normals.at(2),		{ 0.f,  0.f } },
		{ vertices.at(12),		normals.at(2),		{ 1.f,  0.f } },
		{ vertices.at(1),		normals.at(2),		{ 0.5f, 1.f } },

		//Top back
		{ vertices.at(1),		normals.at(3),		{ 0.f,  0.f } },
		{ vertices.at(2),		normals.at(3),		{ 1.f,  0.f } },
		{ vertices.at(11),		normals.at(3),		{ 0.5f, 1.f } },

		//Bottom front      
		{ vertices.at(11),		normals.at(4),		{ 0.f,  0.f } },
		{ vertices.at(2),		normals.at(4),		{ 1.f,  0.f } },
		{ vertices.at(3),		normals.at(4),		{ 0.5f, 1.f } },

		//Bottom left
		{ vertices.at(11),		normals.at(5),		{ 0.f,  0.f } },
		{ vertices.at(3),		normals.at(5),		{ 1.f,  0.f } },
		{ vertices.at(4),		normals.at(5),		{ 0.5f, 1.f } },



		//Top front
		{ vertices.at(11),		normals.at(6),		{ 0.f,  0.f } },
		{ vertices.at(4),		normals.at(6),		{ 1.f,  0.f } },
		{ vertices.at(5),		normals.at(6),		{ 0.5f, 1.f } },

		//Top left
		{ vertices.at(11),		normals.at(7),		{ 0.f,  0.f } },
		{ vertices.at(5),		normals.at(7),		{ 1.f,  0.f } },
		{ vertices.at(6),		normals.at(7),		{ 0.5f, 1.f } },

		//Top right
		{ vertices.at(11),		normals.at(8),		{ 0.f,  0.f } },
		{ vertices.at(6),		normals.at(8),		{ 1.f,  0.f } },
		{ vertices.at(7),		normals.at(8),		{ 0.5f, 1.f } },

		//Top back
		{ vertices.at(11),		normals.at(9),		{ 0.f,  0.f } },
		{ vertices.at(7),		normals.at(9),		{ 1.f,  0.f } },
		{ vertices.at(8),		normals.at(9),		{ 0.5f, 1.f } },

		//Bottom front      
		{ vertices.at(11),		normals.at(10),		{ 0.f,  0.f } },
		{ vertices.at(8),		normals.at(10),		{ 1.f,  0.f } },
		{ vertices.at(9),		normals.at(10),		{ 0.5f, 1.f } },

		//Bottom left
		{ vertices.at(11),		normals.at(11),		{ 0.f,  0.f } },
		{ vertices.at(9),		normals.at(11),		{ 1.f,  0.f } },
		{ vertices.at(10),		normals.at(11),		{ 0.5f, 1.f } },

		//Sides

		//Top front
		{ vertices.at(1),		normals.at(12),		{ 0.f,  0.f } },
		{ vertices.at(0),		normals.at(12),		{ 1.f,  0.f } },
		{ vertices.at(13),		normals.at(12),		{ 0.5f, 1.f } },

		//Top left
		{ vertices.at(13),		normals.at(13),		{ 0.f,  0.f } },
		{ vertices.at(14),		normals.at(13),		{ 1.f,  0.f } },
		{ vertices.at(1),		normals.at(13),		{ 0.5f, 1.f } },

		//Top right
		{ vertices.at(1),		normals.at(14),		{ 0.f,  0.f } },
		{ vertices.at(14),		normals.at(14),		{ 1.f,  0.f } },
		{ vertices.at(15),		normals.at(14),		{ 0.5f, 1.f } },

		//Top back
		{ vertices.at(15),		normals.at(15),		{ 0.f,  0.f } },
		{ vertices.at(2),		normals.at(15),		{ 1.f,  0.f } },
		{ vertices.at(1),		normals.at(15),		{ 0.5f, 1.f } },

		//Bottom front      
		{ vertices.at(0),		normals.at(16),		{ 0.f,  0.f } },
		{ vertices.at(10),		normals.at(16),		{ 1.f,  0.f } },
		{ vertices.at(23),		normals.at(16),		{ 0.5f, 1.f } },

		//Bottom left
		{ vertices.at(23),		normals.at(17),		{ 0.f,  0.f } },
		{ vertices.at(13),		normals.at(17),		{ 1.f,  0.f } },
		{ vertices.at(0),		normals.at(17),		{ 0.5f, 1.f } },




		//Top front
		{ vertices.at(2),		normals.at(18),		{ 0.f,  0.f } },
		{ vertices.at(15),		normals.at(18),		{ 1.f,  0.f } },
		{ vertices.at(16),		normals.at(18),		{ 0.5f, 1.f } },

		//Top left
		{ vertices.at(16),		normals.at(19),		{ 0.f,  0.f } },
		{ vertices.at(3),		normals.at(19),		{ 1.f,  0.f } },
		{ vertices.at(2),		normals.at(19),		{ 0.5f, 1.f } },

		//Top right
		{ vertices.at(3),		normals.at(20),		{ 0.f,  0.f } },
		{ vertices.at(16),		normals.at(20),		{ 1.f,  0.f } },
		{ vertices.at(17),		normals.at(20),		{ 0.5f, 1.f } },

		//Top back
		{ vertices.at(17),		normals.at(21),		{ 0.f,  0.f } },
		{ vertices.at(4),		normals.at(21),		{ 1.f,  0.f } },
		{ vertices.at(3),		normals.at(21),		{ 0.5f, 1.f } },

		//Bottom front      
		{ vertices.at(4),		normals.at(22),		{ 0.f,  0.f } },
		{ vertices.at(17),		normals.at(22),		{ 1.f,  0.f } },
		{ vertices.at(18),		normals.at(22),		{ 0.5f, 1.f } },

		//Bottom left
		{ vertices.at(18),		normals.at(23),		{ 0.f,  0.f } },
		{ vertices.at(5),		normals.at(23),		{ 1.f,  0.f } },
		{ vertices.at(4),		normals.at(23),		{ 0.5f, 1.f } },



		//Top right
		{ vertices.at(5),		normals.at(24),		{ 0.f,  0.f } },
		{ vertices.at(18),		normals.at(24),		{ 1.f,  0.f } },
		{ vertices.at(19),		normals.at(24),		{ 0.5f, 1.f } },

		//Top back
		{ vertices.at(19),		normals.at(25),		{ 0.f,  0.f } },
		{ vertices.at(6),		normals.at(25),		{ 1.f,  0.f } },
		{ vertices.at(5),		normals.at(25),		{ 0.5f, 1.f } },

		//Bottom front      
		{ vertices.at(20),		normals.at(26),		{ 0.f,  0.f } },
		{ vertices.at(7),		normals.at(26),		{ 1.f,  0.f } },
		{ vertices.at(6),		normals.at(26),		{ 0.5f, 1.f } },

		//Bottom left
		{ vertices.at(6),		normals.at(27),		{ 0.f,  0.f } },
		{ vertices.at(19),		normals.at(27),		{ 1.f,  0.f } },
		{ vertices.at(20),		normals.at(27),		{ 0.5f, 1.f } },




		//Bottom front      
		{ vertices.at(7),		normals.at(28),		{ 0.f,  0.f } },
		{ vertices.at(20),		normals.at(28),		{ 1.f,  0.f } },
		{ vertices.at(21),		normals.at(28),		{ 0.5f, 1.f } },

		//Bottom left
		{ vertices.at(21),		normals.at(29),		{ 0.f,  0.f } },
		{ vertices.at(8),		normals.at(29),		{ 1.f,  0.f } },
		{ vertices.at(7),		normals.at(29),		{ 0.5f, 1.f } },

		//Top right
		{ vertices.at(22),		normals.at(30),		{ 0.f,  0.f } },
		{ vertices.at(9),		normals.at(30),		{ 1.f,  0.f } },
		{ vertices.at(8),		normals.at(30),		{ 0.5f, 1.f } },

		//Top back
		{ vertices.at(8),		normals.at(31),		{ 0.f,  0.f } },
		{ vertices.at(21),		normals.at(31),		{ 1.f,  0.f } },
		{ vertices.at(22),		normals.at(31),		{ 0.5f, 1.f } },

		//Bottom front      
		{ vertices.at(23),		normals.at(32),		{ 0.f,  0.f } },
		{ vertices.at(10),		normals.at(32),		{ 1.f,  0.f } },
		{ vertices.at(9),		normals.at(32),		{ 0.5f, 1.f } },

		//Bottom left
		{ vertices.at(9),		normals.at(33),		{ 0.f,  0.f } },
		{ vertices.at(22),		normals.at(33),		{ 1.f,  0.f } },
		{ vertices.at(23),		normals.at(33),		{ 0.5f, 1.f } },

		//Back

		//Top right
		{ vertices.at(14),		normals.at(34),		{ 0.f,  0.f } },
		{ vertices.at(25),		normals.at(34),		{ 1.f,  0.f } },
		{ vertices.at(24),		normals.at(34),		{ 0.5f, 1.f } },

		//Top back
		{ vertices.at(24),		normals.at(35),		{ 0.f,  0.f } },
		{ vertices.at(15),		normals.at(35),		{ 1.f,  0.f } },
		{ vertices.at(14),		normals.at(35),		{ 0.5f, 1.f } },

		//Bottom front      
		{ vertices.at(25),		normals.at(36),		{ 0.f,  0.f } },
		{ vertices.at(13),		normals.at(36),		{ 1.f,  0.f } },
		{ vertices.at(24),		normals.at(36),		{ 0.5f, 1.f } },

		//Bottom left
		{ vertices.at(24),		normals.at(37),		{ 0.f,  0.f } },
		{ vertices.at(13),		normals.at(37),		{ 1.f,  0.f } },
		{ vertices.at(23),		normals.at(37),		{ 0.5f, 1.f } },


		//Top right
		{ vertices.at(24),		normals.at(38),		{ 0.f,  0.f } },
		{ vertices.at(23),		normals.at(38),		{ 1.f,  0.f } },
		{ vertices.at(22),		normals.at(38),		{ 0.5f, 1.f } },

		//Top back
		{ vertices.at(22),		normals.at(39),		{ 0.f,  0.f } },
		{ vertices.at(21),		normals.at(39),		{ 1.f,  0.f } },
		{ vertices.at(24),		normals.at(39),		{ 0.5f, 1.f } },

		//Bottom front      
		{ vertices.at(24),		normals.at(40),		{ 0.f,  0.f } },
		{ vertices.at(21),		normals.at(40),		{ 1.f,  0.f } },
		{ vertices.at(20),		normals.at(40),		{ 0.5f, 1.f } },

		//Bottom left
		{ vertices.at(20),		normals.at(41),		{ 0.f,  0.f } },
		{ vertices.at(19),		normals.at(41),		{ 1.f,  0.f } },
		{ vertices.at(24),		normals.at(41),		{ 0.5f, 1.f } },


		//Top right
		{ vertices.at(19),		normals.at(42),		{ 0.f,  0.f } },
		{ vertices.at(18),		normals.at(42),		{ 1.f,  0.f } },
		{ vertices.at(24),		normals.at(42),		{ 0.5f, 1.f } },

		//Top back
		{ vertices.at(24),		normals.at(43),		{ 0.f,  0.f } },
		{ vertices.at(18),		normals.at(43),		{ 1.f,  0.f } },
		{ vertices.at(17),		normals.at(43),		{ 0.5f, 1.f } },

		//Bottom front      
		{ vertices.at(17),		normals.at(44),		{ 0.f,  0.f } },
		{ vertices.at(16),		normals.at(44),		{ 1.f,  0.f } },
		{ vertices.at(24),		normals.at(44),		{ 0.5f, 1.f } },

		//Bottom left
		{ vertices.at(24),		normals.at(45),		{ 0.f,  0.f } },
		{ vertices.at(16),		normals.at(45),		{ 1.f,  0.f } },
		{ vertices.at(15),		normals.at(45),		{ 0.5f, 1.f } },

	};

	const std::vector<uint32_t> shield_indices
	{
		//Face
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

		//Sides
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
		66, 67, 68,
		69, 70, 71,

		72, 73, 74,
		75, 76, 77,
		78, 79, 80,
		81, 82, 83,

		84, 85, 86,
		87, 88, 89,
		90, 91, 92,
		93, 94, 95,
		96, 97, 98,
		99, 100, 101,

		102, 103, 104,
		105, 106, 107,
		108, 109, 110,
		111, 112, 113,

		114, 115, 116,
		117, 118, 119,
		120, 121, 122,
		123, 124, 125,

		126, 127, 128,
		129, 130, 131,
		132, 133, 134,
		135, 136, 137
	};
	//Create the mesh with the verticies and indicies
	m_mesh = engine::mesh::create(shield_vertices, shield_indices);
}

engine::shield::~shield() {}

engine::ref<engine::shield> engine::shield::create(std::vector<glm::vec3> vertices)
{
	return std::make_shared<engine::shield>(vertices);
}


