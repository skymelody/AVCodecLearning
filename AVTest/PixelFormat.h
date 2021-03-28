#pragma once
#include <vector>
#include <cstdint>

namespace PixelFormat
{
	struct Rgb24Pixel
	{
		Rgb24Pixel(uint8_t red = 0, uint8_t green = 0, uint8_t blue = 0) :
			_r(red), _g(green), _b(blue) {}
		uint8_t _r;
		uint8_t _g;
		uint8_t _b;
	};

	struct Rgb24Image
	{
		Rgb24Image(const std::vector<Rgb24Pixel>& pixels, int width, int height) :
			_pixels(pixels), _width(width), _height(height) {}
		std::vector<Rgb24Pixel> _pixels;
		int _width;
		int _height;
	};

	Rgb24Image gen_rbg_stride_vertical(int width, int height);
	Rgb24Image gen_rbg_stride_horizontal(int width, int height);
	Rgb24Image gen_rgb_red(int width, int height);
	Rgb24Image gen_rgb_blue(int width, int height);
	Rgb24Image gen_rgb_green(int width, int height);
	Rgb24Image gen_rgb_solid(int width, int height, const Rgb24Pixel& pixel);
	bool write_grb_to_file(const std::string& path, const Rgb24Image image);
	bool write_rgb_as_yuv(const std::string& path, const Rgb24Image image);
}

