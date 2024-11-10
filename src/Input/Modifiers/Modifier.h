#pragma once
#include "../../Maths/Vector.h"

namespace Engine3
{
	class Modifier
	{
	public:
		virtual ~Modifier() = default;

		virtual void operator()(float &value) = 0;

		virtual void operator()(Vector<2> &value) = 0;
	};

}