#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <vector>
#include <map>
#include "splitstring.h"
#include <memory>
#include <glm/gtx/matrix_interpolation.hpp>
#include <glm/gtc/matrix_transform.hpp>

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
	bool hasFrame(float t)
	{
		unsigned int tt = ceil(t);
		return (list.find(tt) != std::end(list));
	}
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
		auto it = list.find(index - 1);
		t->orientations = it->second->orientations;
		return t;
	}

	bool save()
	{
		if (path.empty())
			return true;
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

	glm::vec3 linearInterpolate(glm::vec3 from, glm::vec3 to, float dt)
	{
		if (dt > 1.0f)
			return to;
		else if (dt < 0.0)
			return from;
		return ((1.0f - dt) * from) + (dt * to);
	}

	glm::mat4 getInterpolatedValue(unsigned int join, float frame, InterpolationType type)
	{
		if (frame == round(frame))
		{
			glm::mat4 res(1);
			auto f = getFrame(frame);
			if (f != nullptr)
			{
				res = glm::mat4_cast(f->orientations[join]);
				return res;
			}
			return res;
		}
		else
		{
			glm::mat4 res(1);
			auto from = getFrame(floor(frame));
			auto to = getFrame(ceil(frame));
			float delta = frame - floor(frame);

			if (!from || !to)
				return res;

			if (type == Matrix)
			{
				res = glm::interpolate(glm::mat4_cast(from->orientations[join]), glm::mat4_cast(to->orientations[join]), delta);
				return res;
			}
			else if (type == Euler_angles)
			{
				auto fromVec = glm::eulerAngles(from->orientations[join]);
				auto toVec = glm::eulerAngles(to->orientations[join]);
				auto inter = linearInterpolate(fromVec, toVec, delta);
				res = glm::rotate(res, inter.x, glm::vec3(1, 0, 0));
				res = glm::rotate(res, inter.y, glm::vec3(0, 1, 0));
				res = glm::rotate(res, inter.z, glm::vec3(0, 0, 1));
				return res;
			}
			else if (type == Quaternion_linear)
			{
				res = glm::mat4_cast(glm::lerp(from->orientations[join], to->orientations[join], delta));
				return res;
			}
			else if (type == Quaternion_SLERP)
			{
				res = glm::mat4_cast(glm::slerp(from->orientations[join], to->orientations[join], delta));
				return res;
			}

		}
	}
};
