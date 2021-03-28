#pragma once
#include "PixelFormat.h"

namespace ImageUtils
{
	using namespace PixelFormat;
	Rgb24Image gen_rbg_stride_vertical(int width, int height);
	Rgb24Image gen_rbg_stride_horizontal(int width, int height);
	Rgb24Image gen_rgb_red(int width, int height);
	Rgb24Image gen_rgb_blue(int width, int height);
	Rgb24Image gen_rgb_green(int width, int height);
	Rgb24Image gen_rgb_solid(int width, int height, const Rgb24Pixel& pixel);
	// Rgb24Image convert_yuv_to_rgb(const YuvImage& yuv_image);
	YuvImage convert_rgb_to_yuv(const Rgb24Image& rgb_image);
	bool write_grb_to_file(const std::string& path, const Rgb24Image image);
	bool write_rgb_as_yuv(const std::string& path, const Rgb24Image image);
}
