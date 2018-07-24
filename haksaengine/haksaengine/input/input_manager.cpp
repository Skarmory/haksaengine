#include "input/input_manager.h"

InputManager::InputManager(void)
{
	// Default null key
	_key_map["NoKey"] = Key("NoKey");

	// Alphabetical
	_key_map["A"] = Key("A");
	_key_map["B"] = Key("B");
	_key_map["C"] = Key("C");
	_key_map["D"] = Key("D");
	_key_map["E"] = Key("E");
	_key_map["F"] = Key("F");
	_key_map["G"] = Key("G");
	_key_map["H"] = Key("H");
	_key_map["I"] = Key("I");
	_key_map["J"] = Key("J");
	_key_map["K"] = Key("K");
	_key_map["L"] = Key("L");
	_key_map["M"] = Key("M");
	_key_map["N"] = Key("N");
	_key_map["O"] = Key("O");
	_key_map["P"] = Key("P");
	_key_map["Q"] = Key("Q");
	_key_map["R"] = Key("R");
	_key_map["S"] = Key("S");
	_key_map["T"] = Key("T");
	_key_map["U"] = Key("U");
	_key_map["V"] = Key("V");
	_key_map["W"] = Key("W");
	_key_map["X"] = Key("X");
	_key_map["Y"] = Key("Y");
	_key_map["Z"] = Key("Z");

	// Numbers (not numpad)
	_key_map["0"] = Key("0");
	_key_map["1"] = Key("1");
	_key_map["2"] = Key("2");
	_key_map["3"] = Key("3");
	_key_map["4"] = Key("4");
	_key_map["5"] = Key("5");
	_key_map["6"] = Key("6");
	_key_map["7"] = Key("7");
	_key_map["8"] = Key("8");
	_key_map["9"] = Key("9");

	// Other
	_key_map["Esc"] = Key("Esc");
	_key_map["Ctrl"] = Key("Ctrl");
	_key_map["Shift"] = Key("Shift");
	_key_map["Alt"] = Key("Alt");
}

void InputManager::update(void)
{

}

const Key& InputManager::get_key(const char* name) const
{
	if (_key_map.find(name) == _key_map.end())
		return _key_map.at("NoKey");

	return _key_map.at(name);
}