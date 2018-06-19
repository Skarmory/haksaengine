#include "mdl.h"

MDLFile::MDLFile(void)
{

}

MDLFile::~MDLFile(void)
{
	for (auto mesh : _meshes)
		delete mesh;
}