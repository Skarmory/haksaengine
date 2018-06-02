#include <globals.h>
#include <ecs/entity.h>
#include <ecs/component.h>
#include <ecs/system.h>
#include <event.h>
#include <event_manager.h>

#include <string>
#include <iostream>
#include <memory>

EventManager event_manager;

class TestSystemA : public System
{
public:
	TestSystemA(void)
	{
		event_manager.subscribe("TestEvent", this);
	}

	virtual void update(float delta) override
	{
		return;
	}

	virtual void on_event(Event e) override
	{
		std::cout << "Test System A received event" << std::endl;
	}
};

class TestSystemB : public System
{
public:

	virtual void update(float delta) override
	{
		Variant v;
		v.type = Variant::Type::FLOAT;
		v.as_float = 7.77f;

		Event e;
		e.event_type = "TestEvent";
		e.arguments.push_back(v);

		event_manager.dispatch(e);
		
		return;
	}

	virtual void on_event(Event e) override
	{
		std::cout << "Test System B received event" << std::endl;
	}
};

int main(int argc, char** argv)
{
	TestSystemA A;
	TestSystemB B;

	B.update(0.0f);

	char ch;

	std::cin >> ch;
}