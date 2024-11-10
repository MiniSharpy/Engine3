#pragma once
#include "Modifier.h"
#include "../../Maths/Vector.h"

namespace Engine3
{
	class DeadZoneModifier : public Modifier
	{
	private:
		float DeadZone;
	public:
		DeadZoneModifier() : DeadZone(0.2f) {}
		DeadZoneModifier(float deadZone) : DeadZone(deadZone) {}

		void operator()(float &value) override
		{
			// Axial, effectively.
			value = std::abs(value) > DeadZone ? value : 0;
		}

		void operator()(Vector<2> &value) override
		{
			// https://web.archive.org/web/20190129113357/http://www.third-helix.com/2013/04/12/doing-thumbstick-dead-zones-right.html
			value.X(std::abs(value.X()) > DeadZone ? value.X() : 0);
			value.Y(std::abs(value.Y()) > DeadZone ? value.Y() : 0);
		}
	};
}