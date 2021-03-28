#pragma once
#include <functional>

namespace Utils
{
	double GetRoutineExecuteTimeConsume(const std::function<void()> func);
}
