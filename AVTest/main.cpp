#include <string>
#include <iostream>
#include "ImageUtils.h"
#include "CommonUtils.h"

int main()
{
	const std::string saveFilePath =
	 	R"(C:\Users\a_sim\Desktop\out.rgb)";

	const auto func = [&] {
		auto rgb_image = ImageUtils::gen_rbg_stride_horizontal(1920, 1080);
		auto yuv_image = ImageUtils::convert_rgb_to_yuv(rgb_image);
		rgb_image = ImageUtils::convert_yuv_to_rgb(yuv_image);
		ImageUtils::write_grb_to_file(saveFilePath, rgb_image);
	};

	auto time_consume = CommonUtils::get_routine_execute_time_consume(func);
	printf("%.20f\n", time_consume);

	return 0;
}