#include <iostream>
#include <vector>
#include "MatrixClass.h"

using namespace std;

int main()
{
	cout << "FirstMatrix:" << endl;
	MatrixClass<float> TestMat1(TheOriginOfTheMatrix::File);
	char FirstFilePath[] = "C:\\Users\PC\\Desktop\\Repos\\23-Home-2\\HomeworkFor1-2Module\\ReadMe.txt";
	TestMat1.ReadMatrixFromFile(FirstFilePath);
	//TestMat1.ShowMatrix();

	cout << "SecondMatrix:" << endl;
	MatrixClass<float> TestMat2(TheOriginOfTheMatrix::File);
	char SecondFilePath[] = "C:\\Users\PC\\Desktop\\Repos\\23-Home-2\\HomeworkFor1-2Module\\ReadMeAgain.txt";
	TestMat2.ReadMatrixFromFile(SecondFilePath);
	//TestMat2.ShowMatrix();

	/*TestMat1 + TestMat1;
	TestMat1 - TestMat1;
	if (TestMat1 == TestMat1)
	{
		cout << "true" << endl;
		cout << endl;
	}

	if (TestMat1 == 1)
	{
		cout << "true" << endl;
		cout << endl;
	}

	TestMat1 * 2;*/

	//MatrixClass<float> Block1(TestMat1, TestMat2, false);
	//MatrixClass<float> Block2(TestMat1, TestMat2, true);
	//Block1.Plus(Block1);
	//Block1.Multiply(Block2);

	return 0;
}
