#ifndef SKELETON_H
#define SKELETON_H
#include <vector>
#include <string>
#include <fstream>
#include <cstdlib>
#include <glm/glm.hpp>
#include <array>
#include <Keyframe.hpp>

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
	glm::vec3 position; // position of the joint
    glm::vec2 screenCoord; //screen coord
	glm::mat4 local; // local transformation matrice relative to parent
	glm::mat4 global; // global transformation matrice
	glm::mat4 offset; // global offset of the joint in bind pose
	glm::mat4 localOffset; // local offset of the joint in bind pose
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
	std::unique_ptr<Timeline> timeline = nullptr;
    std::vector<Joint> joints;
    bool hasJointSelected;   
	std::size_t selectedJoint;
	std::vector<float> weights;
	GLfloat* vertices;
	GLfloat* verticesCopy;
	GLfloat* normals;
	GLfloat* normalsCopy;
	std::array<glm::mat4, 17> interpolatedMatrix;
	void interpolate(float frame);

	Skeleton(){ hasJointSelected = false; interpolatedMatrix.fill(glm::mat4(1)); };
	~Skeleton();
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

	void update(std::size_t rootJoint, float frame);

	bool loadWeights(const std::string &file);

	void updateSkin(_GLMmodel *model);

	void initSkin(_GLMmodel *model);
};

#endif
