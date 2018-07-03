#pragma once

namespace core
{
	class Uncopyable
	{
		Uncopyable(const Uncopyable& other);
		Uncopyable& operator=(const Uncopyable& other);

	public:
		Uncopyable(void)	{}
	};
}
