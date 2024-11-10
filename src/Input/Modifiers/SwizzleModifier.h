#pragma once
#include "Modifier.h"
#include "../../Maths/Vector.h"

namespace Engine3
{
	class SwizzleModifier : public Modifier
	{
	public:
		void operator()(float &value) override {} // Do nothing, doesn't make sense to swizzle a float.
		void operator()(Vector<2> &value) override
		{
			value = { value.Y(), value.X()};
		}
	};
}