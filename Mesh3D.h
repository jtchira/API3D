#include "common.h"
#include "Vertex.h"

class Mesh3D {

private:

	int id;
	glm::vec3 colorRGB;
	std::vector<Vertex> meshVertex;
	
public:
	Mesh3D();
	void getId();
	void addVertex(Vertex);
	Vertex getMeshVertex();

};
