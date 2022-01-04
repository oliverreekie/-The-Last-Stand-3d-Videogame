#pragma once

namespace engine
{
	class mesh;

	//Creates the trim object
	class trim
	{
	public:
		//Constructor
		trim(std::vector<glm::vec3> vertices);

		//Destructor
		~trim();

		//Getter methods
		std::vector<glm::vec3> vertices() const { return m_vertices; }
		ref<engine::mesh> mesh() const { return m_mesh; }

		static ref<trim> create(std::vector<glm::vec3> vertices);

	private:
		//The verticies of the trim
		std::vector<glm::vec3> m_vertices;

		ref<engine::mesh> m_mesh;
	};
}
