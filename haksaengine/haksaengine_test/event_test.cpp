#include "stdafx.h"
#include "CppUnitTest.h"

#include <iostream>
#include <string>

#include "services.h"

#include "ecs/entity.h"
#include "ecs/component.h"
#include "ecs/system.h"

#include "event/event_manager.h"
#include "event/event.h"
#include "variant.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace haksaengine_test
{
	TEST_CLASS(EventTest)
	{
	public:

		class TestSystemA : public System
		{
		public:
			TestSystemA(SystemOrdering order) : System(order)
			{
				Services::get().get_event_manager()->subscribe("TestEvent", this);
			}

			virtual void update(float d) override { return; }

			virtual void on_event(Event e) override
			{
				Assert::AreEqual("TestEvent", e.event_type.c_str());
				Assert::AreEqual((int)Variant::Type::FLOAT, (int)e.arguments[0].type);
				Assert::AreEqual(7.77f, e.arguments[0].as_float);
			}
		};

		Services* services;
		SystemOrdering order = SystemOrdering(UpdatePriority::GAMEPLAY, 0);

		TEST_METHOD_INITIALIZE(setup)
		{
			services = new Services;
			EventManager* event_manager = new EventManager;
			EntityManager* entity_manager = new EntityManager;

			services->set_event_manager(event_manager);
			services->set_entity_manager(entity_manager);
		}

		TEST_METHOD_CLEANUP(teardown)
		{
			delete services;
		}

		TEST_METHOD(Event_received_event_is_correct)
		{
			TestSystemA A(order);

			Event test_event;
			test_event.event_type = "TestEvent";

			Variant v;
			v.type = v.FLOAT;
			v.as_float = 7.77f;

			test_event.arguments.push_back(v);
		
			Services::get().get_event_manager()->dispatch(test_event);
		}
	};
}