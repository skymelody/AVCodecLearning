#include "Utils.h"
#include <ctime>
namespace Utils
{
	double GetRoutineExecuteTimeConsume(const std::function<void()> func)
	{
		const auto start = clock();
		func();
		const auto end = clock();
		return double(end - start) / CLOCKS_PER_SEC;
	}
}
