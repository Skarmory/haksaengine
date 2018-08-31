#pragma once

#include "ecs/render_logic_system.h"
#include "gfx/uniform_buffer_object.h"
#include "gfx/uniform_data.h"
#include "io/mdl.h"
#include "ecs/skinned_renderable.h"

// A rendering logic class that processes entities with a SkinnedRenderable component
class SkinnedRenderer : public RenderLogicSystem
{
public:

	SkinnedRenderer(SystemOrdering order);
	~SkinnedRenderer(void);

	virtual void update(float delta) override;
	virtual void on_event(Event ev) override;

private:

	void _draw_instanced(const MDLFile& mdl, const SkinnedRenderable* renderable, unsigned int instances);
	void _draw(const MDLFile& mdl, const SkinnedRenderable* renderable, PerDrawDataSkinned* per_draw);

	// Sorting function that sorts entities by their shader ID first and then by their MDL ID
	static bool _order_for_render(unsigned int& left, unsigned int& right);

private:
	
	bool _need_sort;
	unsigned int _instances_max;
	PerInstanceData* _per_instance_data;
};