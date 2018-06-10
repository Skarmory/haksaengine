#include "stdafx.h"
#include "CppUnitTest.h"

#include <Windows.h>
#include <iostream>
#include <string>

#include "io/blueprint_loader.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#define STRINGIFY(x) #x

#define EXPAND(x) STRINGIFY(x)

namespace haksaengine_test
{
	struct TestComponentA : public Component<TestComponentA>
	{
		void load(NamedVariantPack* data) override
		{
			this->data = data->get("data").as_float;
		}

		float data;
	};

	TEST_CLASS(BlueprintLoader_Test)
	{
	public:
		TEST_METHOD(BlueprintLoader_load)
		{
			std::string current_dir = EXPAND(UNITTESTPRJ);
			current_dir.erase(0, 1);
			current_dir.erase(current_dir.size() - 2);

			BlueprintLoader bpl(current_dir + "..\\..\\assets\\blueprints\\");

			bpl.register_component<TestComponentA>("TestComponentA");

			Blueprint* bp = bpl.load("blueprint_unittest");

			delete bp;
		}
		
	};
}