#include "stdafx.h"

#include "..\Project1_167\Vector3.h"
#include <vector>

using namespace System;
using namespace System::Text;
using namespace System::Collections::Generic;
using namespace Microsoft::VisualStudio::TestTools::UnitTesting;

namespace Testing
{
	[TestClass]
	public ref class TestVector3
	{
	private:
		Vector3 *pClass;
		
	public: 
		
		[TestInitialize()]
		void Startup()
		{
			pClass = new Vector3();
		}

		[TestCleanup()]
		void MyTestCleanup() 
		{
			delete(pClass);
			pClass = nullptr;
		};

		[TestMethod]
		void TestDefaultConstructor()
		{
			//
			// TODO: 在此处添加测试逻辑
			//			
			Assert::AreEqual<float>(0, pClass->get('x'));
			Assert::AreEqual<float>(0, pClass->get('y'));
			Assert::AreEqual<float>(0, pClass->get('z'));

			Assert::AreNotEqual<float>(0.111, pClass->get('x'));
			
		}


		[TestMethod]
		void TestConstructor2()
		{
			delete(pClass);
			pClass = new Vector3(1.1, 2.1, 3.11);
			Assert::AreEqual<float>(1.1, pClass->get('x'));
			Assert::AreEqual<float>(2.1, pClass->get('y'));
			Assert::AreEqual<float>(3.11, pClass->get('z'));
		}

		[TestMethod]
		void TestSet()
		{
			pClass->set(2.222, 4.231, 100.323);
			Assert::AreEqual<float>(2.222, pClass->get('x'));
			Assert::AreEqual<float>(4.231, pClass->get('y'));
			Assert::AreEqual<float>(100.323, pClass->get('z'));

		}

		[TestMethod]
		void TestAdd()
		{
			Vector3 addend(2.2, 3.1, 4.4);
			Vector3 &result = pClass->add(addend);

			Assert::AreEqual<float>(2.2, result.get('x'));
			Assert::AreEqual<float>(3.1, result.get('y'));
			Assert::AreEqual<float>(4.4, result.get('z'));


			//POST CONDITION: original vector does not change
			Assert::AreEqual<float>(0, pClass->get('x'));

			//Add again to result
			Vector3 addend2(1.2, 3.1, 4.5);
			Vector3 &result2 = result.add(addend2);
			Assert::AreEqual<float>(3.4, result2.get('x'));
			Assert::AreEqual<float>(6.2, result2.get('y'));
			Assert::AreEqual<float>(8.9, result2.get('z'));

		}

		[TestMethod]
		void TestStaticAdd()
		{
			Vector3 addend(2.2, 3.1, 4.4);
			Vector3 &result = Vector3::Add(*pClass, addend);

			Assert::AreEqual<float>(2.2, result.get('x'));
			Assert::AreEqual<float>(3.1, result.get('y'));
			Assert::AreEqual<float>(4.4, result.get('z'));


			//POST CONDITION: original vector does not change
			Assert::AreEqual<float>(0, pClass->get('x'));

			//Add again to result
			Vector3 addend2(1.2, 3.1, 4.5);
			Vector3 &result2 = Vector3::Add(result, addend2);
			Assert::AreEqual<float>(3.4, result2.get('x'));
			Assert::AreEqual<float>(6.2, result2.get('y'));
			Assert::AreEqual<float>(8.9, result2.get('z'));
		}

		[TestMethod]
		void TestSetVectorArr()
		{
			Vector3 v(1, 2, 3);
			std::vector<float> testArr;
			testArr.push_back(5.f);
			testArr.push_back(6.f);
			testArr.push_back(7.f);

			v.set(0, testArr);
			Assert::AreEqual<float>(5, v['x']);
			Assert::AreEqual<float>(6, v['y']);
			Assert::AreEqual<float>(7, v['z']);

		}


		//TODO: to be continued


	};
}
