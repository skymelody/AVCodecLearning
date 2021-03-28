#include "CommonUtils.h"
#include <ctime>
namespace CommonUtils
{
	double get_routine_execute_time_consume(const std::function<void()> func)
	{
		const auto start = clock();
		func();
		const auto end = clock();
		return double(end - start) / CLOCKS_PER_SEC;
	}
}
