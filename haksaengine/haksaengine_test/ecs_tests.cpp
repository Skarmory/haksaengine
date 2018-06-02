#include "stdafx.h"
#include "CppUnitTest.h"

#include "ecs/entity.h"
#include "ecs/component.h"
#include "ecs/system.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace haksaengine_test
{		
	TEST_CLASS(ECSTest)
	{
	public:

		class TestComponent : public Component
		{
		};
		
		TEST_METHOD(Entity_add_component_test)
		{
			TestComponent c;
			Entity e(0);

			e.add_component<TestComponent>(&c);
			
			Assert::IsTrue(e.has_component<TestComponent>());
		}
	};
}