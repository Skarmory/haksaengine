#include "stdafx.h"
#include "CppUnitTest.h"

#include <iostream>

#include "ecs/entity_manager.h"
#include "ecs/entity.h"
#include "ecs/component.h"
#include "ecs/system.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace haksaengine_test
{		
	TEST_CLASS(ECSTest)
	{
	public:

		class TestComponentA : public Component
		{
		public:
			float data;
		};
		
		class TestComponentB : public Component
		{
		};

		TEST_METHOD(Entity_add_component_test)
		{
			Component* c = new TestComponentA;
			Component* d = new TestComponentB;
			Entity e(0);

			e.add_component(c);
			e.add_component(d);

			Assert::IsTrue(e.has_component<TestComponentA>());
			Assert::IsTrue(e.has_component<TestComponentB>());
		}

		TEST_METHOD(Entity_get_component_test)
		{
			TestComponentA* c = new TestComponentA;
			c->data = 7.77f;

			Entity e(0);

			e.add_component(c);

			Assert::IsTrue(e.has_component<TestComponentA>());

			TestComponentA* a = e.get_component<TestComponentA>();

			Assert::IsNotNull(a);
			Assert::AreEqual(7.77f, a->data);

			delete c;
		}

		TEST_METHOD(Entity_remove_component_test)
		{
			TestComponentA* c = new TestComponentA;

			Entity e(0);

			e.add_component(c);

			Assert::IsTrue(e.has_component<TestComponentA>());

			e.remove_component<TestComponentA>();

			Assert::IsFalse(e.has_component<TestComponentA>());

			TestComponentA* a = e.get_component<TestComponentA>();

			Assert::IsNull(a);
		}

		TEST_METHOD(Entity_get_id_test)
		{
			unsigned int id = 7777;
			Entity e(id);

			Assert::AreEqual(id, e.get_id());
		}
	};

	TEST_CLASS(EntityManagerTest)
	{
	public:
		TEST_METHOD(EntityManager_create_entity_test)
		{
			EventManager event_man;
			EntityManager entity_man(&event_man);

			auto e1_id = entity_man.create_entity();
			auto e2_id = entity_man.create_entity();

			Assert::AreEqual((unsigned int)0, e1_id);
			Assert::AreEqual((unsigned int)1, e2_id);
		}
	};
}