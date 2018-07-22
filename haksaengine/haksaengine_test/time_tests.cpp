#include "stdafx.h"
#include "CppUnitTest.h"

#include <iostream>
#include <string>

#include "game_time.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace haksaengine_test
{
	TEST_CLASS(GameTimeTest)
	{
	public:

		TEST_METHOD(GameTime_delta_test)
		{
			GameTime game_time;

			game_time.tick();

			float delta = game_time.delta();

			Assert::IsTrue(delta > 0.0f);
		}

		TEST_METHOD(GameTime_elapsed_test)
		{
			GameTime game_time;

			game_time.tick();

			float time = game_time.delta();
			float elapsed = game_time.elapsed();

			Assert::AreEqual(time, elapsed);

			game_time.tick();

			time += game_time.delta();
			elapsed = game_time.elapsed();

			Assert::AreEqual(time, elapsed);
		}
	};
}