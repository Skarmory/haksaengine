#include <globals.h>

#include <gfx/mesh_loader.h>

#include <string>
#include <iostream>
#include <memory>

int main(int argc, char** argv)
{
	MeshLoader m;

	m.load_mesh("../../assets/outverts.txt");

	char ch;
	std::cin >> ch;
}