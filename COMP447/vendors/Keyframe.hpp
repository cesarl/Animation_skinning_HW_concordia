#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <vector>
#include <map>
#include "splitstring.h"
#include <memory>

struct Keyframe
{
	std::array<glm::quat, 17> orientations;
	unsigned int indice;
};

struct Timeline
{
	std::map <unsigned int, std::shared_ptr<Keyframe>> list;
	unsigned int from;
	unsigned int to;
	std::string path;

	~Timeline()
	{
		if (!save())
			std::cerr << "Error saving file" << std::endl;
	}

	std::shared_ptr<Keyframe> getFrame(unsigned int index)
	{
		if (list.find(index) != std::end(list))
			return list[index];
		return nullptr;
	}

	std::shared_ptr<Keyframe> createFrame(unsigned int index)
	{
		if (list.find(index) != std::end(list))
			return list[index];
		auto t = std::make_shared<Keyframe>();
		list.insert(std::make_pair(index, t));
		t->indice = index;
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
				file << f.w << " " << f.x << " " << f.y << " " << f.z;
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
		while (std::getline(file, str))
		{
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
	}
};
