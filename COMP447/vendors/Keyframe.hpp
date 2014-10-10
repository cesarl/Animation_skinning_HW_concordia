#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <vector>
#include <list>

struct Keyframe
{
	std::vector<glm::quat> orientations;
	unsigned int indice;
};

struct Timeline
{
	std::list <Keyframe> list;
	unsigned int from;
	unsigned int to;

	bool load(const std::string &name)
	{

	}
};
