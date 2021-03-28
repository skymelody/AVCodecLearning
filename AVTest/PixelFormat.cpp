#include "PixelFormat.h"
#include <cstdio>
#include <functional>
#include <cstdlib>
#include <ctime>
#include <algorithm>

#define CHECK_WIDTH_AND_HEIGHT(width, height) \
	if (width <= 0 || height <= 0) { \
		return Rgb24Image({}, 0, 0);\
	}

namespace PixelFormat
{

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

		std::vector<Rgb24Pixel> pixels;
		const int npixels = width * height;
		for (int i = 0; i < npixels; ++i)
		{
			pixels.push_back(pixel);
		}
		return { pixels, width, height };
	}

	bool write_rgb_as_yuv(const std::string& path, const Rgb24Image image)
	{
		FILE* file = fopen(path.c_str(), "wb+");
		if (file == nullptr)
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
					fwrite(&data, 1, 1, file);
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
			return 16 + 0.257 * pixel._r + 0.504 * pixel._g + 0.098 * pixel._b;
		}, nullptr);
		trans_and_save([](const Rgb24Pixel& pixel) 
		{
			return 128 - 0.148 * pixel._r - 0.291 * pixel._g + 0.439 * pixel._b;
		}, u_write_predicate);
		trans_and_save([](const Rgb24Pixel& pixel)
		{
			return 128 + 0.439 * pixel._r - 0.368 * pixel._g - 0.071 * pixel._b;
		}, v_write_predicate);
		
		fflush(file);

		return true;
	}

	bool write_grb_to_file(const std::string& path, const Rgb24Image image)
	{
		FILE* file = fopen(path.c_str(), "wb+");
		if (file == nullptr)
		{
			return false;
		}

		for (const auto& pixel : image._pixels) {
			fwrite(&pixel._r, 1, 1, file);
			fwrite(&pixel._g, 1, 1, file);
			fwrite(&pixel._b, 1, 1, file);
		}
		fflush(file);
		return true;
	}
}
