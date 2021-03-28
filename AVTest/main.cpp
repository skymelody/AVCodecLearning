#include <string>
#include <iostream>
#include "PixelFormat.h"
#include "Utils.h"

int main()
{
	const std::string saveFilePath =
	 	R"(C:\Users\a_sim\Desktop\out.yuv)";
	const auto func = [&] {
		auto image = PixelFormat::gen_rbg_stride_horizontal(1920, 1080);
		PixelFormat::write_rgb_as_yuv(saveFilePath, image);
	};

	auto time_consume = Utils::GetRoutineExecuteTimeConsume(func);
	printf("%.20f\n", time_consume);

	return 0;
}