#include "stdafx.h"
#include "CppUnitTest.h"

#include <iostream>
#include <string>

#include "ecs/entity.h"
#include "ecs/component.h"
#include "ecs/system.h"

#include "event_manager.h"
#include "event.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace haksaengine_test
{
	TEST_CLASS(EventTest)
	{
	public:

		class TestSystemA : public System
		{
			virtual void update(float d) override { return; }

			virtual void on_event(Event e) override
			{
				Assert::AreEqual("TestEvent", e.event_type.c_str());
				Assert::AreEqual((int)Variant::Type::FLOAT, (int)e.arguments[0].type);
				Assert::AreEqual(7.77f, e.arguments[0].as_float);
			}
		};

		TEST_METHOD(Event_received_event_is_correct)
		{
			EventManager event_manager;

			TestSystemA A;
			
			event_manager.subscribe("TestEvent", &A);

			Event test_event;
			test_event.event_type = "TestEvent";

			Variant v;
			v.type = v.FLOAT;
			v.as_float = 7.77f;

			test_event.arguments.push_back(v);
		
			event_manager.dispatch(test_event);
		}
	};
}