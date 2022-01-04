#pragma once

namespace engine
{
	class mesh;

	//Creates the shield object
	class shield
	{
	public:
		//Constructor
		shield(std::vector<glm::vec3> vertices);

		//Destructor
		~shield();

		//Getter methods
		std::vector<glm::vec3> vertices() const { return m_vertices; }
		ref<engine::mesh> mesh() const { return m_mesh; }

		static ref<shield> create(std::vector<glm::vec3> vertices);

	private:
		//The verticies of the shield
		std::vector<glm::vec3> m_vertices;

		ref<engine::mesh> m_mesh;
	};
}
