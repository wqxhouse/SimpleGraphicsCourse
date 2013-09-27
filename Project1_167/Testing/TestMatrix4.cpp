#include "stdafx.h"
#include "..\Project1_167\Matrix4.h"

using namespace Microsoft::VisualStudio::TestTools::UnitTesting;

namespace Testing
{
	[TestClass]
	public ref class TestMatrix4
	{
		
	private:
		Matrix4 *mat4;

	public:
		[TestInitialize()]
		void Init()
		{
			mat4 = new Matrix4();
		}

		[TestCleanup()]
		void CleanUp()
		{
			delete(mat4);
			mat4 = nullptr;
		}

		[TestMethod]
		void TestConstructor()
		{
			Assert::AreEqual<float>(1.0f, (*mat4)[0]);
			Assert::AreNotEqual<float>(1.0f, (*mat4)[1]);
			Assert::AreEqual<float>(1.0f, (*mat4)[5]);
			Assert::AreEqual<float>(1.0f, (*mat4)[10]);
			Assert::AreEqual<float>(1.0f, (*mat4)[15]);

		}
	};
}