#pragma once

namespace engine
{
	class mesh;

	//Creates the blade object
	class blade
	{
	public:
		//Constructor
		blade(std::vector<glm::vec3> vertices);

		//Destructor
		~blade();

		//Getter methods
		std::vector<glm::vec3> vertices() const { return m_vertices; }
		ref<engine::mesh> mesh() const { return m_mesh; }

		static ref<blade> create(std::vector<glm::vec3> vertices);

	private:
		//The verticies of the blade
		std::vector<glm::vec3> m_vertices;

		ref<engine::mesh> m_mesh;
	};
}
