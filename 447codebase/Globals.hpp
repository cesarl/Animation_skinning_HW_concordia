#pragma once

#include <glm/fwd.hpp>
#include <vector>
#include <atomic>
#include <array>
#include <Keyframe.hpp>
#include <memory>

	//////////
	/// VERY DIRTY GLOBALS - Shame on me :(

	static unsigned int windowWidth = 1024;
	static unsigned int windowHeight = 768;
	static unsigned int fontTex;
	double mouse_x, mouse_y;
	bool rMouse, lMouse;

	namespace GLOBALS
	{
		bool editing = true;
		static const char* transitionNames[4] = { "matrix", "euler angles", "quaternion linear", "quaternion SLERP"};
		int transitionMode = 0;
        static const char* animationNames[] = { "none", "wave.anim", "break-dance.anim", "dance.anim" };
		int animationNbr = 0;
		int frames = 0;
		static std::unique_ptr<Timeline> timeline = nullptr;
	};

