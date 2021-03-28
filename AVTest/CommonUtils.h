#pragma once
#include <functional>
#include <string>
#include <cstdio>

namespace CommonUtils
{
	double get_routine_execute_time_consume(const std::function<void()> func);

	template <typename T>
	T clip_value(T val, T min_val, T max_val)
	{
		if (val > max_val)
		{
			return max_val;
		}
		if (val < min_val)
		{
			return min_val;
		}
		return val;
	}

	class DataDumper
	{
	public:
		DataDumper(const std::string& file_path) :
			_file_path(file_path), _file(nullptr) {}

		~DataDumper()
		{
			if (_file != nullptr)
			{
				close();
			}
		}

		bool open()
		{
			_file = fopen(_file_path.c_str(), "wb+");
			return _file != nullptr;
		}

		bool dump(uint8_t data) const
		{
			if (_file == nullptr)
			{
				return false;
			}
			return fwrite(&data, 1, 1, _file) == 1;
		}

		bool dump(uint8_t* data, const int size) const
		{
			if (_file == nullptr)
			{
				return false;
			}
			const int nwrite = fwrite(data, 1, size, _file);
			return nwrite == size;
		}

		bool flush() const
		{
			if (_file == nullptr)
			{
				return false;
			}
			return fflush(_file) == 0;
		}

		void close()
		{
			if (_file != nullptr)
			{
				fclose(_file);
			}
			_file = nullptr;
			_file_path = "";
		}
	private:
		std::string _file_path;
		FILE* _file;
	};
}
