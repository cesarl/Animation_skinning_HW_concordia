#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <vector>
#include <map>
#include "splitstring.h"
#include <memory>

enum InterpolationType
{
	Matrix = 0,
	Euler_angles = 1,
	Quaternion_linear = 2,
	Quaternion_SLERP = 3
};

struct Keyframe
{
	std::array<glm::quat, 17> orientations;
	unsigned int indice;
	Keyframe()
	{
		orientations.fill(glm::quat(1, 0, 0, 0));
	}
};

struct Timeline
{
	std::map <unsigned int, std::shared_ptr<Keyframe>> list;
	unsigned int from = 0;
	unsigned int to = 0;
	std::string path;

	~Timeline()
	{
	}

	std::shared_ptr<Keyframe> getFrame(unsigned int index)
	{
		if (list.find(index) != std::end(list))
			return list[index];
		return nullptr;
	}

	std::shared_ptr<Keyframe> createFrame(unsigned int index)
	{
		if (index < from)
			from = index;
		else if (from > to)
			to = from;
		if (list.find(index) != std::end(list))
			return list[index];
		auto t = std::make_shared<Keyframe>();
		list.insert(std::make_pair(index, t));
		t->indice = index;
		if (list.size() == 1)
			return t;
		auto it = list.find(index);
		t->orientations = it->second->orientations;
		return t;
	}

	bool save()
	{
		if (path.empty())
			assert(false);
		std::ofstream file(path);
		if (!file.is_open())
			return false;
		for (auto &e : list)
		{
			file << std::to_string(e.first) + " ";
			for (auto &f : e.second->orientations)
			{
				file << f.w << " " << f.x << " " << f.y << " " << f.z << " ";
			}
			file << std::endl;
		}
	}

	bool load(const std::string &name)
	{
		list.clear();

		path = "../" + name;
		std::ifstream file(path);
		if (!file.is_open())
			return false;
		std::string str;
		bool existingKey = false;
		while (std::getline(file, str))
		{
			existingKey = true;
			std::vector<std::string> conc;
			splitstring splitStr(str);
			conc = splitStr.split(' ');

			if (conc.size() != 69)
			{
				std::cout << "Bad file format." << std::endl;
				return false;
			}

			auto f = createFrame(std::atof(conc[0].c_str()));

			auto ind = 0;
			for (auto i = 1; i < 69; i += 4)
			{
				f->orientations[ind] = glm::quat(
					std::atof(conc[i].c_str())
					, std::atof(conc[i + 1].c_str())
					, std::atof(conc[i + 2].c_str())
					, std::atof(conc[i + 3].c_str())
					);
				++ind;
			}
		}
		if (!existingKey)
			createFrame(0);
		return true;
	}

	//std::shared_ptr<Keyframe> getFrom(unsigned int index)
	//{
	//	for (auto &e : list)
	//	{
	//		if ()
	//	}
	//}

	glm::mat4 getInterpolatedValue(unsigned int join, unsigned int frame, InterpolationType type)
	{
		if (type == Matrix)
		{
			glm::mat4 res(1);
			auto f = getFrame(frame);
			if (f != nullptr)
			{
				res = glm::mat4_cast(f->orientations[join]);
				return res;
			}
			else
			{
				//auto from = getFrom(index);
				//auto to = getTo(index);
				//res = glm::mix()
				return res;
			}
		}
	}
};
