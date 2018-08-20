#include <vector>
#include <unordered_map>
#include <string>
#include <fstream>
#include <iomanip>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 uv;
	
	std::vector<unsigned int> bone_idx;
	std::vector<float>	      bone_weights;
};

struct Mesh
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
};

static unsigned int next_joint_id = 0;
struct Bone
{
	unsigned int id;
	std::string  name;
	aiMatrix4x4	 inverse_bind;
};

struct BonePose
{
	std::string name;

	// Base transform
	aiMatrix4x4 transform;

	// Animation data
	std::vector<aiVectorKey> positions;
	std::vector<aiQuatKey> rotations;
	std::vector<aiVectorKey> scales;

	// Not all nodes correspond to a joint
	bool is_joint;
	unsigned int joint_idx;

	// Tree data flattened into an array, these correspond to array indices
	int parent;
	int id;
	std::vector<int> children;
};

struct Animation
{
	std::string name;
	float duration;
	float timestep;
	std::vector<BonePose*> poses;
};

// Data containers
std::vector<Mesh> meshes;
std::vector<Bone> joints;
std::unordered_map<std::string, Bone> bone_map;
std::vector<aiNodeAnim*> animation_nodes;
std::vector<BonePose*> pose_nodes;
Animation animation;

// Find animation node by name
aiNodeAnim* find_anim_node(const std::string& node_name)
{
	for (auto node : animation_nodes)
		if (node->mNodeName.C_Str() == node_name)
			return node;

	return nullptr;
}

// Write out node hierarchy
void flatten_node_hierarchy(aiNode* node, BonePose* parent)
{
	// Node data is spread throughout various data structures:
	//	- aiNode	 - Contains transform matrix
	//  - aiNodeAnim - Contains animation data over time
	//  - aiBone	 - Contains inverse bind matrix of a joint

	BonePose* new_pose_node = new BonePose;
	new_pose_node->name = node->mName.C_Str();
	new_pose_node->transform = node->mTransformation;

	// Try to get corresponding node animation
	aiNodeAnim* node_anim = find_anim_node(node->mName.C_Str());
	if (node_anim)
	{
		// Add position animation data
		new_pose_node->positions.reserve(node_anim->mNumPositionKeys);
		for (int i = 0; i < node_anim->mNumPositionKeys; i++)
			new_pose_node->positions.push_back(node_anim->mPositionKeys[i]);

		// Add rotation animation data
		new_pose_node->rotations.reserve(node_anim->mNumRotationKeys);
		for (int i = 0; i < node_anim->mNumRotationKeys; i++)
			new_pose_node->rotations.push_back(node_anim->mRotationKeys[i]);

		// Add scale animation data
		new_pose_node->scales.reserve(node_anim->mNumScalingKeys);
		for (int i = 0; i < node_anim->mNumScalingKeys; i++)
			new_pose_node->scales.push_back(node_anim->mScalingKeys[i]);
	}
	else
	{
		// No animation data for this node, so instead just set each to the identity property of that transform
		aiVectorKey pkey;
		pkey.mTime = 0.0f;
		pkey.mValue = aiVector3D(0.0f, 0.0f, 0.0f);
		new_pose_node->positions.push_back(pkey);

		aiQuatKey rkey;
		rkey.mTime = 0.0f;
		rkey.mValue = aiQuaternion(1.0f, 0.0f, 0.0f, 0.0f);
		new_pose_node->rotations.push_back(rkey);

		aiVectorKey skey;
		skey.mTime = 0.0f;
		skey.mValue = aiVector3D(1.0f, 1.0f, 1.0f);
		new_pose_node->scales.push_back(skey);
	}

	// Try to get corresponding bone
	auto joint = bone_map.find(node->mName.C_Str());
	new_pose_node->is_joint = joint != bone_map.end();
	if (new_pose_node->is_joint)
	{
		new_pose_node->joint_idx = joint->second.id;
	}

	// Set node ID and add into vector
	int index = animation.poses.size();
	new_pose_node->id = index;
	animation.poses.push_back(new_pose_node);

	// Set parent ID
	if (parent)
	{
		new_pose_node->parent = parent->id;
		parent->children.push_back(new_pose_node->id);
	}
	else
	{
		new_pose_node->parent = -1;
	}

	// Deal with child nodes
	new_pose_node->children.reserve(node->mNumChildren);
	for (int i = 0; i < node->mNumChildren; i++)
		flatten_node_hierarchy(node->mChildren[i], new_pose_node);
}

void write_mdl(void)
{
	std::string out_file_name = "tree.mdl";

	std::ofstream stream(out_file_name, std::ofstream::out);

	bool has_bones = joints.size() > 0;

	stream << std::fixed << std::setprecision(6);

	stream << "Geosets " << meshes.size() << " {" << std::endl;
	for (int i = 0; i < meshes.size(); i++)
	{
		Mesh* mesh = &meshes[i];

		// Start geoset
		stream << "\t" << "Geoset " << i << " {" << std::endl;

		// Write vertex positions
		stream << "\t\t" << "Vertices " << mesh->vertices.size() << " {" << std::endl;
		for (int j = 0; j < mesh->vertices.size(); j++)
		{
			Vertex* vertex = &mesh->vertices[j];
			stream << "\t\t\t" << "{ " << vertex->position.x << ", " << vertex->position.y << ", " << vertex->position.z << " }" << std::endl;
		}
		stream << "\t\t" << "}" << std::endl;

		// Write vertex normals
		stream << "\t\t" << "Normals " << mesh->vertices.size() << " {" << std::endl;
		for (int j = 0; j < mesh->vertices.size(); j++)
		{
			Vertex* vertex = &mesh->vertices[j];
			stream << "\t\t\t" << "{ " << vertex->normal.x << ", " << vertex->normal.y << ", " << vertex->normal.z << " }" << std::endl;
		}
		stream << "\t\t" << "}" << std::endl;

		// Write vertex uvs
		stream << "\t\t" << "UVs " << mesh->vertices.size() << " {" << std::endl;
		for (int j = 0; j < mesh->vertices.size(); j++)
		{
			Vertex* vertex = &mesh->vertices[j];
			stream << "\t\t\t" << "{ " << vertex->uv.x << ", " << vertex->uv.y << " }" << std::endl;
		}
		stream << "\t\t" << "}" << std::endl;

		if (has_bones)
		{
			// Write vertex bone indices
			stream << "\t\t" << "BoneIndices " << mesh->vertices.size() << " {" << std::endl;
			for (int j = 0; j < mesh->vertices.size(); j++)
			{
				Vertex* vertex = &mesh->vertices[j];

				stream << "\t\t\t" << "{ ";
				for (int k = 0; k < vertex->bone_idx.size() - 1; k++)
				{
					stream << vertex->bone_idx[k] << ", ";
				}
				stream << vertex->bone_idx[vertex->bone_idx.size() - 1];
				stream << " }" << std::endl;

			}
			stream << "\t\t" << "}" << std::endl;

			// Write vertex bone weights
			stream << "\t\t" << "BoneWeights " << mesh->vertices.size() << " {" << std::endl;
			for (int j = 0; j < mesh->vertices.size(); j++)
			{
				Vertex* vertex = &mesh->vertices[j];

				stream << "\t\t\t" << "{ ";
				for (int k = 0; k < vertex->bone_weights.size() - 1; k++)
				{
					stream << vertex->bone_weights[k] << ", ";
				}
				stream << vertex->bone_weights[vertex->bone_weights.size() - 1];
				stream << " }" << std::endl;

			}
			stream << "\t\t" << "}" << std::endl;
		}

		// Write indices
		stream << "\t\t" << "Indices " << mesh->indices.size() << " {" << std::endl;
		stream << "\t\t\t";
		for (int j = 0; j < mesh->indices.size(); j++)
		{
			stream << mesh->indices[j] << ", ";
		}
		stream << std::endl << "\t\t" << "}" << std::endl;

		// End geoset
		stream << "\t" << "}" << std::endl << std::endl;
	}
	stream << "}" << std::endl << std::endl;

	if (has_bones)
	{
		// Write out joint/bone data
		stream << "Bones " << joints.size() << " {" << std::endl;
		for (int i = 0; i < joints.size(); i++)
		{
			stream << "\t" << "Bone " << joints[i].name << " {" << std::endl;
			stream << "\t\t" << "InverseBindMatrix {" << std::endl;
			stream << "\t\t\t" << joints[i].inverse_bind.a1 << ", " << joints[i].inverse_bind.a2 << ", " << joints[i].inverse_bind.a3 << ", " << joints[i].inverse_bind.a4 << "," << std::endl;
			stream << "\t\t\t" << joints[i].inverse_bind.b1 << ", " << joints[i].inverse_bind.b2 << ", " << joints[i].inverse_bind.b3 << ", " << joints[i].inverse_bind.b4 << "," << std::endl;
			stream << "\t\t\t" << joints[i].inverse_bind.c1 << ", " << joints[i].inverse_bind.c2 << ", " << joints[i].inverse_bind.c3 << ", " << joints[i].inverse_bind.c4 << "," << std::endl;
			stream << "\t\t\t" << joints[i].inverse_bind.d1 << ", " << joints[i].inverse_bind.d2 << ", " << joints[i].inverse_bind.d3 << ", " << joints[i].inverse_bind.d4 << std::endl;
			stream << "\t\t" << "}" << std::endl;
			stream << "\t" << "}" << std::endl;
		}
		stream << "}" << std::endl << std::endl;

		// Write out animation pose data
		stream << "Animations " << "1" << " {" << std::endl;
		stream << "\t" << "Animation " << animation.name << " {" << std::endl;
		stream << "\t\t" << "Duration " << animation.duration * animation.timestep << std::endl;
		for (int i = 0; i < animation.poses.size(); i++)
		{
			BonePose* pose = animation.poses[i];
			stream << "\t\t" << "BonePose " << pose->name << " {" << std::endl;

			stream << "\t\t\t" << "Id " << pose->id << std::endl;
			stream << "\t\t\t" << "Parent " << pose->parent << std::endl;

			if (pose->children.size() > 0)
			{
				stream << "\t\t\t" << "Children { ";
				for (int j = 0; j < pose->children.size() - 1; j++)
					stream << pose->children[j] << ", ";
				stream << pose->children[pose->children.size() - 1] << " }" << std::endl;
			}

			if (!pose->is_joint)
			{
				stream << "\t\t\t" << "Helper" << std::endl;
			}
			else
			{
				stream << "\t\t\t" << "Bone " << pose->joint_idx << std::endl;
			}


			// Pose positions by time
			stream << "\t\t\t" << "Positions " << pose->positions.size() << " {" << std::endl;
			for (int j = 0; j < pose->positions.size(); j++)
			{
				aiVectorKey* pkey = &pose->positions[j];
				stream << "\t\t\t\t" << pkey->mTime * animation.timestep << ": { " << pkey->mValue.x << ", " << pkey->mValue.y << ", " << pkey->mValue.z << " }" << std::endl;
			}
			stream << "\t\t\t" << "}" << std::endl;

			// Pose rotations by time
			stream << "\t\t\t" << "Rotations " << pose->rotations.size() << " {" << std::endl;
			for (int j = 0; j < pose->rotations.size(); j++)
			{
				aiQuatKey* rkey = &pose->rotations[j];
				stream << "\t\t\t\t" << rkey->mTime * animation.timestep << ": { " << rkey->mValue.x << ", " << rkey->mValue.y << ", " << rkey->mValue.z << ", " << rkey->mValue.w << " }" << std::endl;
			}
			stream << "\t\t\t" << "}" << std::endl;

			// Pose scales by time
			stream << "\t\t\t" << "Scales " << pose->scales.size() << " {" << std::endl;
			for (int j = 0; j < pose->scales.size(); j++)
			{
				aiVectorKey* skey = &pose->scales[j];
				stream << "\t\t\t\t" << skey->mTime * animation.timestep << ": { " << skey->mValue.x << ", " << skey->mValue.y << ", " << skey->mValue.z << " }" << std::endl;
			}
			stream << "\t\t\t" << "}" << std::endl;

			stream << "\t\t" << "}" << std::endl;
		}
		stream << "\t" << "}" << std::endl;
		stream << "}" << std::endl << std::endl;
	}
}

int main(int argc, char** argv)
{
	std::string file_name = argv[1];

	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(file_name, aiProcessPreset_TargetRealtime_MaxQuality);

	for (int mcount = 0; mcount < scene->mNumMeshes; mcount++)
	{
		Mesh mesh;
		aiMesh* aiMesh = scene->mMeshes[mcount];
		
		// Get vertices
		mesh.vertices.reserve(aiMesh->mNumVertices);
		for (int vcount = 0; vcount < aiMesh->mNumVertices; vcount++)
		{
			Vertex vertex;

			aiVector3D position = aiMesh->mVertices[vcount];
			vertex.position.x = position.x;
			vertex.position.y = position.y;
			vertex.position.z = position.z;

			aiVector3D normal = aiMesh->mNormals[vcount];
			vertex.normal.x = normal.x;
			vertex.normal.y = normal.y;
			vertex.normal.z = normal.z;

			aiVector3D uv = aiMesh->mTextureCoords[0][vcount];
			vertex.uv.x = uv.x;
			vertex.uv.y = uv.y;

			mesh.vertices.push_back(vertex);
		}

		// Get indices
		int index_total = aiMesh->mNumFaces * aiMesh->mFaces->mNumIndices;
		mesh.indices.reserve(index_total);
		for (int fcount = 0; fcount < aiMesh->mNumFaces; fcount++)
		for (int icount = 0; icount < aiMesh->mFaces[fcount].mNumIndices; icount++)
		{
			mesh.indices.push_back(aiMesh->mFaces[fcount].mIndices[icount]);
		}

		// Get bones
		for (int bcount = 0; bcount < aiMesh->mNumBones; bcount++)
		{
			aiBone* bone = aiMesh->mBones[bcount];

			// Stash em in a nice map
			std::string bone_name = aiMesh->mBones[bcount]->mName.C_Str();
			if (bone_map.find(bone_name) == bone_map.end())
			{
				Bone joint;
				joint.id = next_joint_id++;
				joint.name = bone_name;

				joint.inverse_bind = bone->mOffsetMatrix;
				bone_map[bone_name] = joint;	
			}

			Bone j = bone_map[bone_name];

			// Add bone id and weight to vertex
			for (int bone_vertices = 0; bone_vertices < aiMesh->mBones[bcount]->mNumWeights; bone_vertices++)
			{
				aiVertexWeight weight = aiMesh->mBones[bcount]->mWeights[bone_vertices];

				mesh.vertices[weight.mVertexId].bone_idx.push_back(j.id);
				mesh.vertices[weight.mVertexId].bone_weights.push_back(weight.mWeight);
			}
		}

		meshes.push_back(mesh);
	}

	// Flatten animation nodes into a single vector
	// This is possible because the animations exported into FBX from Blender only appear to carry information about the animation that was loaded onto each armature at export time.
	// Each armature is considered a separate animation by FBX, but in reality they're just sub-animations of the overall animation
	for (int a_count = 0; a_count < scene->mNumAnimations; a_count++)
	for (int c_count = 0; c_count < scene->mAnimations[a_count]->mNumChannels; c_count++)
	{
		animation_nodes.push_back(scene->mAnimations[a_count]->mChannels[c_count]);
	}

	if (scene->mNumAnimations > 0)
	{
		std::string animation_name = scene->mAnimations[0]->mName.C_Str();
		animation_name = animation_name.substr(animation_name.find_first_of('|') + 1, animation_name.size());
		animation.name = animation_name;
		animation.duration = scene->mAnimations[0]->mDuration;
		animation.timestep = 1.0f / scene->mAnimations[0]->mTicksPerSecond;
	}

	flatten_node_hierarchy(scene->mRootNode, nullptr);

	// Put the joints into a vector where index == bone id
	joints.resize(bone_map.size());
	for (auto kpv : bone_map)
	{
		joints[kpv.second.id] = kpv.second;
	}

	write_mdl();

	return 0;
}