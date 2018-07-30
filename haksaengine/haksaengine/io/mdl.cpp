#include "mdl.h"

#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "gfx/uniform_data.h"

MDLFile::MDLFile(void)
{

}

MDLFile::~MDLFile(void)
{
	for (auto mesh : _meshes)
		delete mesh;

	if (_animation_texture)
		delete _animation_texture;
}

void MDLFile::create_animation_texture(void)
{
	std::vector<float> data;
	std::vector<glm::mat4> final_transforms;

	// Unfurl animation data into a bone matrix for every bone at every timestep of every animation
	// Store this into a data vector which will be used to create a GPU texture
	for (auto& animation : _animations)
	{
		for (float time = 0.0f; time <= animation.duration; time += (1.0f / 60.0f))
		{
			final_transforms.clear();
			final_transforms.resize(_bones.size());

			_process_bone_hierarchy(animation.root_pose_node, time, animation.pose_nodes, _bones, final_transforms, glm::mat4(1.0));

			for (auto& mat : final_transforms)
			{
				for (int i = 0; i < mat.length(); i++)
				{
					data.push_back(mat[i].x);
					data.push_back(mat[i].y);
					data.push_back(mat[i].z);
					data.push_back(mat[i].w);
				}
			}
		}
	}

	// Get in number of mat4s
	float root = std::ceilf(std::sqrt(data.size() / 4));
	unsigned int rootint = root;

	// Find closest power of 2
	unsigned int size;

	// Powers of two are divisible by 4, which means no 4x4 matrix will spill over rows of the texture, simplifying calculations
	if (rootint <= 32)
		size = 32;
	else if (rootint <= 64)
		size = 64;
	else if (rootint <= 128)
		size = 128;
	else if (rootint <= 256)
		size = 256;
	else if (rootint <= 512)
		size = 512;

	_animation_texture = new Texture(data.data(), data.size() * sizeof(float), size, size, FilterMode::Nearest, TextureFormat::Float, false);

	_animation_texture->initialise();
}

void MDLFile::_process_bone_hierarchy(const BonePoseNode* node, float time, const std::vector<BonePoseNode>& nodes, const std::vector<Bone>& bones, std::vector<glm::mat4>& final_transforms, glm::mat4 parent_transform)
{
	// Get the correct animation values via interpolation
	glm::vec3 scale = node->interpolate_scale(time);
	glm::quat rotation = node->interpolate_rotation(time);
	glm::vec3 position = node->interpolate_position(time);

	// Compose the values into matrices
	glm::mat4 scalem = glm::scale(glm::mat4(1.0f), scale);
	glm::mat4 rotm = glm::mat4_cast(rotation);
	glm::mat4 transm = glm::translate(glm::mat4(1.0f), position);

	// Local transform matrix for this pose node at this time
	glm::mat4 transform = transm * rotm * scalem;

	// Multiply by parent to get global transform of this pose node at this time
	transform = parent_transform * transform;

	// If this pose node corresponds to an actual bone, set the matrix in the list of final bone transforms
	if (node->is_bone)
	{
		const Bone& bone = bones[node->bone_id];

		final_transforms[node->bone_id] = transform * glm::transpose(bone.inverse_bind);
	}

	// Go through hierarchy by sending this node's transform matrix to its children
	for (auto child : node->child_ids)
	{
		_process_bone_hierarchy(&nodes[child], time, nodes, bones, final_transforms, transform);
	}
}