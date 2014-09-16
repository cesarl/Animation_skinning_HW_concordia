#ifndef SKELETON_H
#define SKELETON_H
#include <vector>
#include <string>
#include <fstream>
#include <cstdlib>
#include <glm/glm.hpp>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#ifdef _WIN32
#include "GL/glut.h"
#else
#include <GL/freeglut.h>
#endif
#endif

#include "simpleMath.h"

struct _GLMmodel;

struct Joint
{
    glm::vec3 position;
    glm::vec2 screenCoord;
	glm::mat4 local;
	glm::mat4 global;
	glm::mat4 offset;
    bool isHovered;
    bool isPicked;
	std::size_t id;
	std::size_t parent;
	std::vector<std::size_t> children;
    
    Joint()
		:
		position(0)
		, screenCoord(0)
		, local(glm::mat4(1))
		, global(glm::mat4(1))
		, offset(1)
		, isHovered(0)
		, isPicked(0)
		, id(std::size_t(-1))
		, parent(std::size_t(-1))
    {
    }
};

class Skeleton
{
private:
    /*Update screen coordinates of joints*/
    void updateScreenCoord();
public:
    std::vector<Joint> joints;
    bool hasJointSelected;   
	std::size_t selectedJoint;
	std::vector<float> weights;
	GLfloat* vertices;
	GLfloat* verticesCopy;

    Skeleton(){hasJointSelected = false;};
    /*
     * Load Skeleton file
     */
    void loadSkeleton(std::string skelFileName);

    /*
     * Load animation file
     */
    void loadAnimation(std::string skelFileName);

    /*
     * Draw skeleton with OpenGL
     */
    void glDrawSkeleton();

    /*
     * Check if any joint is hovered by given mouse coordinate
     */
    void checkHoveringStatus(int x, int y);

    void release();
    
    void selectOrReleaseJoint();

	void initSkeleton(std::size_t rootJoint);

	void update(std::size_t rootJoint);

	bool loadWeights(const std::string &file);

	void updateSkin(_GLMmodel *model);

	void initSkin(_GLMmodel *model);
};

#endif
