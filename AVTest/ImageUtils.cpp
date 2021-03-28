#include "ImageUtils.h"
#include <cstdio>
#include <functional>
#include <algorithm>

#include "CommonUtils.h"

#define CHECK_WIDTH_AND_HEIGHT(width, height) \
	if (width <= 0 || height <= 0) { \
		return Rgb24Image({}, 0, 0);\
	}

namespace ImageUtils
{

	static uint8_t rgb_pixel_to_y(const Rgb24Pixel& pixel)
	{
		const int y = 16 + 0.257 * pixel._r + 0.504 * pixel._g + 0.098 * pixel._b;
		const int clip_y = CommonUtils::clip_value(y, 0, 255);
		return static_cast<uint8_t>(clip_y);
	}

	static uint8_t rgb_pixel_to_u(const Rgb24Pixel& pixel)
	{
		const int u = 128 - 0.148 * pixel._r - 0.291 * pixel._g + 0.439 * pixel._b;
		const int clip_u = CommonUtils::clip_value(u, 0, 255);
		return static_cast<uint8_t>(clip_u);
	}

	static uint8_t rgb_pixel_to_v(const Rgb24Pixel& pixel)
	{
		const int v = 128 + 0.439 * pixel._r - 0.368 * pixel._g - 0.071 * pixel._b;
		const int clip_v = CommonUtils::clip_value(v, 0, 255);
		return static_cast<uint8_t>(clip_v);
	}

	static uint8_t yuv_pixel_to_r(const YuvPixel& pixel)
	{
		const int r = 1.164 * (pixel._y - 16) + 1.596 * (pixel._v - 128);
		const int clip_r = CommonUtils::clip_value(r, 0, 255);
		return static_cast<uint8_t>(clip_r);
	}

	static uint8_t yuv_pixel_to_g(const YuvPixel& pixel)
	{
		const int g = 1.164 * (pixel._y - 16) - 0.813 * (pixel._v - 128) - 0.391 * (pixel._u - 128);
		const int clip_g = CommonUtils::clip_value(g, 0, 255);
		return static_cast<uint8_t>(clip_g);
	}

	static uint8_t yuv_pixel_to_b(const YuvPixel& pixel)
	{
		const int b = 1.164 * (pixel._y - 16) + 2.018 * (pixel._u - 128);
		const int clip_b = CommonUtils::clip_value(b, 0, 255);
		return static_cast<uint8_t>(clip_b);
	}

	static std::vector<Rgb24Pixel> get_stride_pixel_map()
	{
		return 
		{
			Rgb24Pixel(255, 0, 0),
			Rgb24Pixel(255, 165, 0),
			Rgb24Pixel(255, 255, 0),
			Rgb24Pixel(0, 255, 0),
			Rgb24Pixel(0, 255, 255),
			Rgb24Pixel(0, 0, 255),
			Rgb24Pixel(139, 0, 255)
		};
	}

	// generate 7 vertical strides
	Rgb24Image gen_rbg_stride_vertical(int width, int height)
	{
		CHECK_WIDTH_AND_HEIGHT(width, height);
		int per_stride_width = width / 7;
		if (per_stride_width == 0)
		{
			per_stride_width = 1;
		}

		std::vector<Rgb24Pixel> pixels(width*height);
		auto fill_pixel = [&](int col_beg, int col_end, const Rgb24Pixel& pixel)
		{
			for (int row = 0; row < height; ++row)
			{
				for (int col = col_beg; col < col_end; ++col)
				{
					int pos = row * width + col;
					pixels[pos] = pixel;
				}
			}
		};

		const auto stride_pixel_map = get_stride_pixel_map();
		for (int i = 0; i < stride_pixel_map.size(); ++i)
		{
			int col_beg = i * per_stride_width;
			int col_end = std::min((i+1)*per_stride_width, width);
			fill_pixel(col_beg, col_end, stride_pixel_map[i]);
		}
		return { pixels, width, height };
	}

	// generate 7 horizontal strides
	Rgb24Image gen_rbg_stride_horizontal(int width, int height)
	{
		CHECK_WIDTH_AND_HEIGHT(width, height);
		int per_stride_height = height / 7;
		if (per_stride_height == 0)
		{
			per_stride_height = 1;
		}

		std::vector<Rgb24Pixel> pixels(width*height);
		auto fill_pixel = [&](int row_beg, int row_end, const Rgb24Pixel& pixel)
		{
			for (int row = row_beg; row < row_end; ++row)
			{
				for (int col = 0; col < width; ++col)
				{
					int pos = row * width + col;
					pixels[pos] = pixel;
				}
			}
		};

		const auto stride_pixel_map = get_stride_pixel_map();
		for (int i = 0; i < stride_pixel_map.size(); ++i)
		{
			int row_beg = i * per_stride_height;
			int row_end = std::min((i + 1)*per_stride_height, height);
			fill_pixel(row_beg, row_end, stride_pixel_map[i]);
		}

		return { pixels, width, height };
	}

	Rgb24Image gen_rgb_red(int width, int height)
	{
		return gen_rgb_solid(width, height, Rgb24Pixel(255, 0, 0));
	}

	Rgb24Image gen_rgb_green(int width, int height)
	{
		return gen_rgb_solid(width, height, Rgb24Pixel(0, 255, 0));
	}

	Rgb24Image gen_rgb_blue(int width, int height)
	{
		return gen_rgb_solid(width, height, Rgb24Pixel(0, 0, 255));
	}

	Rgb24Image gen_rgb_solid(int width, int height, const Rgb24Pixel& pixel)
	{
		CHECK_WIDTH_AND_HEIGHT(width, height);

		std::vector<Rgb24Pixel> pixels(width*height);
		for (int i = 0; i < pixels.size(); ++i)
		{
			pixels[i] = pixel;
		}
		return { pixels, width, height };
	}

	// Rgb24Image convert_yuv_to_rgb(const YuvImage& yuv_image)
	// {
	// 	const auto& yuv_pixels = yuv_image._pixels;
	// 	const int width = yuv_image._width;
	// 	const int height = yuv_image._height;
	// 	std::vector<Rgb24Pixel> rgb_pixels(yuv_pixels.size());

	// 	for (int i = 0; i < rgb_pixels.size(); ++i)
	// 	{
	// 		rgb_pixels[i]._r = yuv_pixel_to_r(yuv_pixels[i]);
	// 		rgb_pixels[i]._g = yuv_pixel_to_g(yuv_pixels[i]);
	// 		rgb_pixels[i]._b = yuv_pixel_to_b(yuv_pixels[i]);
	// 	}

	// 	return { rgb_pixels, width, height };
	// }

	YuvImage convert_rgb_to_yuv(const Rgb24Image& rgb_image)
	{
		const auto& rgb_pixels = rgb_image._pixels;
		const int width = rgb_image._width;
		const int height = rgb_image._height;
		std::vector<YuvPixel> yuv_pixels(rgb_pixels.size());

		for (int i = 0; i < yuv_pixels.size(); ++i)
		{
			yuv_pixels[i]._y = rgb_pixel_to_y(rgb_pixels[i]);
			yuv_pixels[i]._u = rgb_pixel_to_u(rgb_pixels[i]);
			yuv_pixels[i]._v = rgb_pixel_to_v(rgb_pixels[i]);
		}

		return { yuv_pixels, width, height };
	}

	bool write_rgb_as_yuv(const std::string& path, const Rgb24Image image)
	{
		CommonUtils::DataDumper dumper(path);
		if (!dumper.open())
		{
			return false;
		}

		const auto trans_and_save = 
			[&](std::function<uint8_t(const Rgb24Pixel& pixel)> trans_func, 
				  std::function<bool(int row, int col)> write_predicate)
		{
			const auto& pixels = image._pixels;
			for (int i = 0; i < pixels.size(); ++i) 
			{
				uint8_t data = trans_func(pixels[i]);
				int row = i / image._width;
				int col = i - row * image._width;
				if (write_predicate == nullptr || write_predicate(row, col))
				{
					dumper.dump(data);
				}
			}
		};

		const auto u_write_predicate = 
			[](int row, int col)
		{
			return row % 2 == 0 && col % 2 == 0;
		};

		const auto v_write_predicate = 
			[](int row, int col)
		{
			return row % 2 == 1 && col % 2 == 0;
		};

		trans_and_save([](const Rgb24Pixel& pixel)
		{
			return rgb_pixel_to_y(pixel);
		}, nullptr);
		trans_and_save([](const Rgb24Pixel& pixel) 
		{
			return rgb_pixel_to_u(pixel);
		}, u_write_predicate);
		trans_and_save([](const Rgb24Pixel& pixel)
		{
			return rgb_pixel_to_v(pixel);
		}, v_write_predicate);
		
		return true;
	}

	bool write_grb_to_file(const std::string& path, const Rgb24Image image)
	{
		CommonUtils::DataDumper dumper(path);
		if (!dumper.open())
		{
			return false;
		}

		for (const auto& pixel : image._pixels) {
			dumper.dump(pixel._r);
			dumper.dump(pixel._g);
			dumper.dump(pixel._b);
		}
		return true;
	}
}