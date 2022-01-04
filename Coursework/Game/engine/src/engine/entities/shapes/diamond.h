#pragma once

namespace engine
{
	class mesh;

	//Creates the diamond object
	class diamond
	{
	public:
		//Constructor
		diamond(std::vector<glm::vec3> vertices);

		//Destructor
		~diamond();

		//Getter methods
		std::vector<glm::vec3> vertices() const { return m_vertices; }
		ref<engine::mesh> mesh() const { return m_mesh; }

		static ref<diamond> create(std::vector<glm::vec3> vertices);

	private:
		//The verticies of the diamond
		std::vector<glm::vec3> m_vertices;

		ref<engine::mesh> m_mesh;
	};
}
