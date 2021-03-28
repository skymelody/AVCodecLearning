#pragma once
#include <vector>
#include <cstdint>

namespace PixelFormat
{
	struct Rgb24Pixel
	{
		Rgb24Pixel(uint8_t r = 0, uint8_t g = 0, uint8_t b = 0) :
			_r(r), _g(g), _b(b) {}
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


	struct YuvPixel
	{
		YuvPixel(uint8_t y = 0, uint8_t u = 0, uint8_t v = 0) :
			_y(y), _u(u), _v(v) {}
		uint8_t _y;
		uint8_t _u;
		uint8_t _v;
	};

	struct YuvImage
	{
		YuvImage(const std::vector<YuvPixel>& pixels, int width, int height) :
			_pixels(pixels), _width(width), _height(height) {}
		std::vector<YuvPixel> _pixels;
		int _width;
		int _height;
	};
}

