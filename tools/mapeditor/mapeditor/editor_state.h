#pragma once

class EditorState
{
public:
	virtual void execute(void) = 0;

	static EditorState* get(void)
	{
		return current;
	}

	static void swap_state(EditorState* state)
	{
		if (current)
			delete current;

		current = state;
	}


private:
	static EditorState* current;
};

class TexturePaintState : public EditorState
{
public:
	void execute(void) override;
};

class HeightDeformState : public EditorState
{
public:
	void execute(void) override;
};

class UnitPlaceState : public EditorState
{
public:
	void execute(void) override;
};