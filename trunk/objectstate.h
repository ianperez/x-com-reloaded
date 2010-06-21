#pragma once

#include <string>

namespace ufo
{
	class ObjectState
	{
	public:

		virtual void save(string path) { }
		virtual void load(string path) { }
	};
}