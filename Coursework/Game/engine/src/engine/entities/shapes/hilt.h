#pragma once

namespace engine
{
	class mesh;

	//Creates the hilt object
	class hilt
	{
	public:
		//Constructor
		hilt(std::vector<glm::vec3> vertices);

		//Destructor
		~hilt();

		//Getter methods
		std::vector<glm::vec3> vertices() const { return m_vertices; }
		ref<engine::mesh> mesh() const { return m_mesh; }

		static ref<hilt> create(std::vector<glm::vec3> vertices);

	private:
		//The verticies of the hilt
		std::vector<glm::vec3> m_vertices;

		ref<engine::mesh> m_mesh;
	};
}
