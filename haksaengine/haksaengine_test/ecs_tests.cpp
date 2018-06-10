#include "stdafx.h"
#include "CppUnitTest.h"

#include <iostream>

#include "services.h"
#include "ecs/entity_manager.h"
#include "ecs/entity.h"
#include "ecs/component.h"
#include "ecs/system.h"

#include "io/blueprint.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace haksaengine_test
{	
	struct TestComponentA : public Component<TestComponentA>
	{
		void load(NamedVariantPack* data) override {}

		float data;
	};

	struct TestComponentB : public Component<TestComponentB>
	{
		void load(NamedVariantPack* data) override {}
	};

	TEST_CLASS(ECSTest)
	{
	public:

		TEST_METHOD(Entity_add_component_test)
		{
			BaseComponent* c = new TestComponentA;
			BaseComponent* d = new TestComponentB;
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

		class TestSystem_EntityCreated : public System
		{
		public:
			virtual void update(float d) override {}

			virtual void on_event(Event ev) override
			{
				Assert::AreEqual("EntityCreatedEvent", ev.event_type.c_str());
			}
		};

		class TestSystem_EntityDestroyed : public System
		{
		public:
			virtual void update(float d) override {}

			virtual void on_event(Event ev) override
			{
				if(first)
					Assert::AreEqual("EntityCreatedEvent", ev.event_type.c_str());
				else
					Assert::AreEqual("EntityDestroyedEvent", ev.event_type.c_str());

				first = false;
			}

			bool first = true;
		};

		class TestSystem_EntityCreatedFromBlueprint : public System
		{
		public:
			virtual void update(float d) override {}

			virtual void on_event(Event ev) override
			{
				Assert::AreEqual("EntityCreatedEvent", ev.event_type.c_str());

				Entity* e = Services::get().get_entity_manager()->get_entity(ev.arguments[0].as_uint);

				Assert::IsTrue(e->has_component<TestComponentA>());
			}
		};

		Services* services;

		TEST_METHOD_INITIALIZE(setup)
		{
			services = new Services;
			EventManager* event_man = new EventManager;
			EntityManager* entity_man = new EntityManager;

			services->set_event_manager(event_man);
			services->set_entity_manager(entity_man);
		}

		TEST_METHOD_CLEANUP(teardown)
		{
			delete services;
		}

		TEST_METHOD(EntityManager_create_entity_test)
		{
			auto entity_man = Services::get().get_entity_manager();

			auto e1_id = entity_man->create_entity();
			auto e2_id = entity_man->create_entity();

			Assert::AreEqual((unsigned int)0, e1_id);
			Assert::AreEqual((unsigned int)1, e2_id);
		}

		TEST_METHOD(EntityManager_create_entity_with_components_test)
		{
			
			auto entity_man = Services::get().get_entity_manager();

			std::vector<BaseComponent*> components;
			components.push_back(new TestComponentA);
			components.push_back(new TestComponentB);

			auto e1_id = entity_man->create_entity(&components);

			Assert::AreEqual((unsigned int)0, e1_id);

			Entity* e = entity_man->get_entity(e1_id);

			Assert::IsTrue(e->has_component<TestComponentA>());
			Assert::IsTrue(e->has_component<TestComponentB>());
		}

		TEST_METHOD(EntityManager_create_entity_from_blueprint)
		{
			Blueprint* bp = new Blueprint;
			TestComponentA* comp = new TestComponentA;
			comp->data = 7.77f;

			bp->add_component(comp);

			TestSystem_EntityCreatedFromBlueprint sys;

			auto entity_man = Services::get().get_entity_manager();

			entity_man->create_entity(bp);

			delete comp;
			delete bp;
		}

		TEST_METHOD(EntityManager_destroy_entity_test)
		{
			auto entity_man = Services::get().get_entity_manager();

			auto e1_id = entity_man->create_entity();

			Assert::AreEqual((unsigned int)0, e1_id);

			entity_man->destroy_entity(e1_id);

			Assert::IsNull(entity_man->get_entity(e1_id));
		}

		TEST_METHOD(EntityManager_get_entity_test)
		{
			auto entity_man = Services::get().get_entity_manager();

			auto e1_id = entity_man->create_entity();

			Entity* entity = entity_man->get_entity(e1_id);

			Assert::IsNotNull(entity);
			Assert::AreEqual(e1_id, entity->get_id());
		}

		TEST_METHOD(EntityManager_get_entity_null_test)
		{
			auto entity_man = Services::get().get_entity_manager();

			Entity* entity = entity_man->get_entity(7777);

			Assert::IsNull(entity);
		}

		TEST_METHOD(EntityManager_create_entity_event_test)
		{
			auto entity_man = Services::get().get_entity_manager();
			TestSystem_EntityCreated system;

			auto e1_id = entity_man->create_entity();
		}

		TEST_METHOD(EntityManager_destroy_entity_event_test)
		{
			auto entity_man = Services::get().get_entity_manager();
			TestSystem_EntityDestroyed system;

			auto e1_id = entity_man->create_entity();

			entity_man->destroy_entity(e1_id);
		}
	};
}