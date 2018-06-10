#include <GL/glew.h>

#include "engine.h"

#include "io/loader.h"
#include "io/blueprint_loader.h"

#include <string>
#include <iostream>
#include <memory>

class A
{

};

class TestLoader : public Loader<A>
{
public:
	TestLoader(const std::string& s) : Loader<A>(s, "") {}

	A* load(const std::string& name) override
	{
		return new A;
	}
};

int main(int argc, char** argv)
{
	Engine e;
	e.initialise();

	TestLoader t("test");

	e.run();

	return 0;
}