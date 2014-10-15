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
	static double mouse_x, mouse_y;
	static bool rMouse, lMouse;

	namespace GLOBALS
	{
		static bool editing = true;
		static const char* transitionNames[4] = { "matrix", "euler angles", "quaternion linear", "quaternion SLERP"};
		static int transitionMode = Matrix;
        static const char* animationNames[] = { "none", "wave.anim", "break-dance.anim", "dance.anim" };
		static int animationNbr = 0;
		static int frames = 0;
	};

