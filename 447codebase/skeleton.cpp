#include "skeleton.h"
#include "splitstring.h"
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>

/*
 * Load skeleton file
 */
void Skeleton::loadSkeleton(std::string skelFileName)
{
    std::string strBone;
    std::ifstream skelFile(skelFileName.c_str());
    if (skelFile.is_open())
    {
        while ( std::getline(skelFile, strBone)) { //Read a line to build a bone
            std::vector<std::string> boneParams;
            splitstring splitStr(strBone);
            boneParams = splitStr.split(' ');
            Joint temp;
            temp.position.x = std::atof(boneParams[1].c_str());
            temp.position.y = std::atof(boneParams[2].c_str());
            temp.position.z = std::atof(boneParams[3].c_str());
			temp.id = std::atoi(boneParams[0].c_str());
			temp.parent = std::size_t(std::atoi(boneParams[4].c_str()));


            if (std::atoi(boneParams[0].c_str()) != joints.size())
            {
                std::cout<<"[Warning!!!] Bone index not match\n";
            }
            joints.push_back(temp);
        }
    }
	for (auto &e : joints)
	{
		if (e.parent != std::size_t(-1))
		{
			joints[e.parent].children.push_back(e.id);
		}
	}
	initSkeleton(0);
	update(0);

}

/*
 * Load Animation
 */
void Skeleton::loadAnimation(std::string skelFileName)
{
}


/*
 * Draw skeleton with OpenGL
 */
void Skeleton::glDrawSkeleton()
{
    //Rigging skeleton
    glDisable(GL_DEPTH_TEST);
    
    glPushMatrix();
    glTranslatef(-0.9,-0.9,-0.9);
	glScalef(1.8,1.8,1.8);
	glPointSize(6);
	glColor3f(1,0,0);
    updateScreenCoord();
    
    for (unsigned i=0; i<joints.size(); i++)
    {
        if (joints[i].isPicked)
            glColor3f(1.0, 0.0, 0.0);
        else if (joints[i].isHovered)
            glColor3f(0.7, 0.7, 0.7);
        else
            glColor3f(0.3, 0.3, 0.3);

        glTranslated(joints[i].position.x, joints[i].position.y, joints[i].position.z);
        glutSolidSphere(0.01, 15, 15);
        glTranslated(-joints[i].position.x, -joints[i].position.y, -joints[i].position.z);

    }
    glPopMatrix();
    
    glEnable(GL_DEPTH_TEST);
}

void Skeleton::updateScreenCoord()
{
    GLint viewport[4];
    GLdouble modelview[16];
    GLdouble projection[16];
    GLdouble winX, winY, winZ;

    glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
    glGetDoublev( GL_PROJECTION_MATRIX, projection );
    glGetIntegerv( GL_VIEWPORT, viewport );
    for (unsigned i=0; i<joints.size(); i++)
    {
        gluProject((GLdouble)joints[i].position.x, (GLdouble)joints[i].position.y, (GLdouble)joints[i].position.z,
                modelview, projection, viewport,
                &winX, &winY, &winZ );
        joints[i].screenCoord.x = winX;
        joints[i].screenCoord.y = (double)glutGet(GLUT_WINDOW_HEIGHT)-winY;
    }
}
void Skeleton::checkHoveringStatus(int x, int y)
{
    double distance = 0.0f;
    double minDistance = 1000.0f;
    int hoveredJoint = -1;
    for(unsigned i=0; i < joints.size(); i++)
    {
        joints[i].isHovered = false;
        distance = sqrt((x - joints[i].screenCoord.x)*(x - joints[i].screenCoord.x) 
                + (y - joints[i].screenCoord.y)*(y - joints[i].screenCoord.y));
        if (distance > 50) continue;
        if (distance < minDistance)
        {
            hoveredJoint = i;
            minDistance = distance;
        }
    }
    if (hoveredJoint != -1) joints[hoveredJoint].isHovered = true;
}

void Skeleton::release()
{
    hasJointSelected = false;
    for (unsigned i=0; i<joints.size(); i++)
    {
        joints[i].isPicked = false;
    }
}

void Skeleton::selectOrReleaseJoint()
{
    bool hasHovered=false;
    for (unsigned i=0; i<joints.size(); i++)
    {
        joints[i].isPicked = false;
        if (joints[i].isHovered)
        {
            hasHovered = true;
            joints[i].isPicked = true;
            hasJointSelected = true;
			selectedJoint = i;
        }
    }
	if (!hasHovered)    //Release joint
	{
		hasJointSelected = false;
		selectedJoint = std::size_t(-1);
	}
}


// Compute transformation relative to parent
void Skeleton::initSkeleton(std::size_t rootJoint)
{
	if (rootJoint >= joints.size())
		return;
	auto &joint = joints[rootJoint];
	if (rootJoint == 0)
		joint.local = glm::translate(glm::mat4(1), joint.position);
	else
	{
		joint.local = glm::translate(glm::mat4(1), joint.position - joints[joint.parent].position);
	}
	joint.global = glm::translate(glm::mat4(1), joint.position);
	joint.offset = joint.local;
	for (auto &e : joint.children)
	{
		initSkeleton(e);
	}
}


void Skeleton::update(std::size_t rootJoint)
{
	if (rootJoint >= joints.size())
		return;
	auto &joint = joints[rootJoint];
	if (rootJoint != 0)
	{
		auto &parent = joints[joint.parent];
		joint.global = parent.global * joint.local;
		glm::vec4 p = joint.global * glm::vec4(0, 0, 0, 1);//joint.local * glm::vec4(parent.position.x, parent.position.y, parent.position.z, 1);
		joint.position = glm::vec3(p.x, p.y, p.z);
		//glm::mat4 trans(1);
		//trans = glm::translate(trans, joint.position - parent.position);
	}
	else
		joint.global = joint.local;
	for (auto &e : joint.children)
	{
		update(e);
	}
}

