#pragma once
#include "../finmath/simulator.h"

namespace test_utils
{
	class ZeroGenerator : public finmath::RandomGenerator
	{
	public:
		double nextValue() {return 0;};
	};
}