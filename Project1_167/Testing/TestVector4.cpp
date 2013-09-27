#include "stdafx.h"

#include "..\Project1_167\Vector4.h"

using namespace System;
using namespace System::Text;
using namespace System::Collections::Generic;
using namespace Microsoft::VisualStudio::TestTools::UnitTesting;

namespace Testing
{
	[TestClass]
	public ref class TestVector4
	{

	private:
		Vector4 *m_pVec;

	public:
		

		[TestInitialize()]
		void StartUp()
		{
			m_pVec = new Vector4();
		}

		[TestCleanup()]
		void Cleanup()
		{
			delete(m_pVec);
			m_pVec = nullptr;
		}

		[TestMethod]
		void TestConstructor()
		{
			Assert::AreEqual<float>(0.0f, m_pVec->get('x'));
			Assert::AreEqual<float>(0.0f, m_pVec->get('y'));
			Assert::AreEqual<float>(0.0f, m_pVec->get('z'));
			Assert::AreEqual<float>(1.0f, m_pVec->get('w'));
		}

		[TestMethod]
		void TestDeHomogenize()
		{
			m_pVec->set(2.0f, 3.0f, 4.0f, 5.0f);
			m_pVec->dehomogenize();
			Assert::AreEqual<float>(2.0f/5.0f, m_pVec->get('x'));
			Assert::AreEqual<float>(3.0f/5.0f, m_pVec->get('y'));
			Assert::AreEqual<float>(4.0f/5.0f, m_pVec->get('z'));
			Assert::AreEqual<float>(5.0f/5.0f, m_pVec->get('w'));
		}

		[TestMethod]
		void TestSubtractOperator()
		{
			m_pVec->set(5.0f, 4.0f, 3.0f, 2.0f);
			Vector4 refVec(2.0f, 3.0f, 1.0f, 2.0f);
			Vector4 &result = *m_pVec - refVec;
			
			Assert::AreEqual<float>(3.0f, result.get('x'));
			Assert::AreEqual<float>(1.0f, result.get('y'));
			Assert::AreEqual<float>(2.0f, result.get('z'));
			Assert::AreEqual<float>(0.0f, result.get('w'));
		}
	};
	
}