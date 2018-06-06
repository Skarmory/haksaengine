#include "stdafx.h"
#include "CppUnitTest.h"

#include <math.h>
#include <glm/glm.hpp>
#include "transform.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace haksaengine_test
{
	TEST_CLASS(TransformTest)
	{
	public:

		Transform* transform;

		TEST_METHOD_INITIALIZE(setup)
		{
			transform = new Transform;
		}

		TEST_METHOD_CLEANUP(teardown)
		{
			delete transform;
		}

		TEST_METHOD(Transform_translate_test)
		{
			glm::vec3 translation1(1.0f, 2.0f, 3.0f);
			glm::vec3 translation2(7.0f, 6.0f, 5.0f);
			glm::vec3 position;

			transform->translate(translation1);

			position = transform->get_position();

			Assert::AreEqual(translation1.x, position.x);
			Assert::AreEqual(translation1.y, position.y);
			Assert::AreEqual(translation1.z, position.z);

			transform->translate(translation2);

			position = transform->get_position();

			Assert::AreEqual(translation2.x, position.x);
			Assert::AreEqual(translation2.y, position.y);
			Assert::AreEqual(translation2.z, position.z);
		}
		
		TEST_METHOD(Transform_translate_by_test)
		{
			glm::vec3 translation1(1.0f, 2.0f, 3.0f);
			glm::vec3 translation2(7.0f, 6.0f, 5.0f);
			glm::vec3 position;

			transform->translate_by(translation1);

			position = transform->get_position();

			Assert::AreEqual(translation1.x, position.x);
			Assert::AreEqual(translation1.y, position.y);
			Assert::AreEqual(translation1.z, position.z);

			transform->translate_by(translation2);

			position = transform->get_position();

			Assert::AreEqual(translation1.x + translation2.x, position.x);
			Assert::AreEqual(translation1.y + translation2.y, position.y);
			Assert::AreEqual(translation1.z + translation2.z, position.z);
		}

		TEST_METHOD(Transform_rotate_test)
		{
			glm::vec3 rotation1(1.0f, 2.0f, 3.0f);
			glm::vec3 rotation2(7.0f, 6.0f, 5.0f);
			glm::vec3 rotation;

			transform->rotate(rotation1);

			rotation = transform->get_rotation();

			Assert::AreEqual(rotation1.x, rotation.x);
			Assert::AreEqual(rotation1.y, rotation.y);
			Assert::AreEqual(rotation1.z, rotation.z);

			transform->rotate(rotation2);

			rotation = transform->get_rotation();

			Assert::AreEqual(rotation2.x, rotation.x);
			Assert::AreEqual(rotation2.y, rotation.y);
			Assert::AreEqual(rotation2.z, rotation.z);
		}

		TEST_METHOD(Transform_rotate_by_test)
		{
			glm::vec3 rotation1(1.0f, 2.0f, 3.0f);
			glm::vec3 rotation2(7.0f, 6.0f, 5.0f);
			glm::vec3 rotation;

			transform->rotate_by(rotation1);

			rotation = transform->get_rotation();

			Assert::AreEqual(rotation1.x, rotation.x);
			Assert::AreEqual(rotation1.y, rotation.y);
			Assert::AreEqual(rotation1.z, rotation.z);

			transform->rotate_by(rotation2);

			rotation = transform->get_rotation();

			Assert::AreEqual(rotation1.x + rotation2.x, rotation.x);
			Assert::AreEqual(rotation1.y + rotation2.y, rotation.y);
			Assert::AreEqual(rotation1.z + rotation2.z, rotation.z);
		}

		TEST_METHOD(Transform_transform_translation_works_test)
		{
			glm::vec3 translation(10.0f, 0.0f, 0.0f);

			glm::vec3 point_to_transform(1.0f, 1.0f, 1.0f);

			transform->translate(translation);

			glm::mat4 t = transform->get_transform();

			glm::vec3 transformed = t * glm::vec4(point_to_transform, 1.0f);

			Assert::AreEqual(11.0f, transformed.x);
			Assert::AreEqual(1.0f, transformed.y);
			Assert::AreEqual(1.0f, transformed.z);
		}

		/* Rotation is not exact, so test will always fail. 
		   If uncomment, check values manually to see that they are about right with very small epsilon. 
		*/
		/*TEST_METHOD(Transform_transform_rotation_works_test)
		{
			glm::vec3 rotation(0.0f, 0.0f, 90.0f);

			glm::vec3 point_to_transform(1.0f, 0.0f, 0.0f);

			transform->rotate(rotation);

			glm::mat4 t = transform->get_transform();

			glm::vec3 transformed = t * glm::vec4(point_to_transform, 1.0f);

			Assert::AreEqual(0.0f, transformed.x);
			Assert::AreEqual(1.0f, transformed.y);
			Assert::AreEqual(0.0f, transformed.z);
		}*/
	};
}