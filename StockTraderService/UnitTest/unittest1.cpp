#include "stdafx.h"
#include "CppUnitTest.h"
#include "StockTraderService.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
	TEST_CLASS(UnitTest1)
	{
	public:

		TEST_METHOD(TestMethod1)
		{
			Logger::WriteMessage("Attempting to connect mongodb");
			try {
				connectToMongoDb();
			}
			catch (exception &e)
			{
				Logger::WriteMessage(e.what());
			}
			bool bTrue = checkForCredential("Jitesh", "123");
			Assert::AreEqual(bTrue, true);
		}

	};
}