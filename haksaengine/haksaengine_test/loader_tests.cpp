#include "stdafx.h"
#include "CppUnitTest.h"

#include <Windows.h>
#include <iostream>
#include <string>
#include <exception>
#include <functional>

#include "engine.h"
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

			Assert::AreEqual(7.77f, this->data);
		}

		float data;
	};

	struct TestComponentB : public Component<TestComponentB>
	{
		void load(NamedVariantPack* data) override
		{
			this->data1 = data->get("data1").as_int;
			this->data2 = data->get("data2").as_float;

			Assert::AreEqual(7, this->data1);
			Assert::AreEqual(5.55f, this->data2);
		}

		int data1;
		float data2;
	};

	struct TestComponentC : public Component<TestComponentC>
	{
		void load(NamedVariantPack* data) override {}
	};

	TEST_CLASS(BlueprintLoader_Test)
	{
	public:
		std::string directory;
		BlueprintLoader* bpl;
		Engine* engine;

		TEST_METHOD_INITIALIZE(setup)
		{
			directory = EXPAND(UNITTESTPRJ);
			directory.erase(0, 1);
			directory.erase(directory.size() - 2);

			engine = new Engine;
			engine->initialise();

			Services::get().get_asset_manager()->set_asset_directory_path((directory + "..\\..\\assets\\").c_str());

			bpl = new BlueprintLoader(directory + "..\\..\\assets\\blueprints\\");
		}

		TEST_METHOD_CLEANUP(teardown)
		{
			delete bpl;
			delete engine;
		}

		TEST_METHOD(BlueprintLoader_load)
		{
			Services::get().get_component_manager()->register_component<TestComponentA>("TestComponentA");
			Services::get().get_component_manager()->register_component<TestComponentB>("TestComponentB");

			Blueprint* bp = bpl->load("blueprint_unittest");

			delete bp;
		}

		TEST_METHOD(BlueprintLoader_load_unregistered_component_exception_test)
		{
			std::function<Blueprint*(void)> func = [this] { return bpl->load("blueprint_unittest"); };
			Assert::ExpectException<std::runtime_error>(func);
		}	
	};
}