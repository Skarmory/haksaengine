#include <globals.h>
#include <ecs/entity.h>
#include <ecs/component.h>

#include <iostream>

class TestComponent : public Component
{
public:
	int data;
};

int main(int argc, char** argv)
{
	Entity* e = new Entity(5);
	std::cout << "Created entity" << std::endl << std::endl;

	TestComponent* c = new TestComponent();
	c->data = 7;

	std::cout << "Created component" << std::endl << std::endl;

	e->add_component<TestComponent>(c);

	std::cout << e->has_component<TestComponent>() << std::endl << std::endl;

	char ch;

	std::cin >> ch;

	delete c;
	delete e;
}