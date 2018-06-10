#include "variant.h"

void NamedVariantPack::set(const char* name, Variant data)
{
	_data_map[name] = data;
}

Variant NamedVariantPack::get(const char* name)
{
	if (_data_map.find(name) != _data_map.end())
		return _data_map[name];
	
	Variant null_variant;
	null_variant.type = Variant::Type::COUNT;

	return null_variant;
}