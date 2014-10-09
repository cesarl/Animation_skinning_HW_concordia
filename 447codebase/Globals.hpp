#pragma once

#include <glm/fwd.hpp>
#include <vector>
#include <atomic>
#include <array>

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
		static const std::string transitionNames[4] = { "matrix", "euler angles", "quaternion linear", "quaternion SLERP"};
		unsigned int transitionMode = 0;
//		static const std::string animationNames[3] 
	};

