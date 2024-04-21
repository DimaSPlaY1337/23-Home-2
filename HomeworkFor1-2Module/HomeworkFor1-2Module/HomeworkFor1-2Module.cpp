#include <iostream>
#include <vector>
#include "MatrixClass.h"

using namespace std;

int main()
{
	int s = 2;
	(MatrixClass<float>::ZeroMatrix(s, s)).ShowMatrix();
	(MatrixClass<float>::ASingleMatrix(s, s)).ShowMatrix();

	cout << "Add elements: " << endl;
	MatrixClass<float> ParamMat(s, s);
	ParamMat.InitializeMatrixFromConsole();
	ParamMat.ShowMatrix();
	ParamMat.ShowMatrix((!ParamMat).matrix, s, s);

	cout << "FirstMatrix:" << endl;
	MatrixClass<float> MatrixFromFile(PlaceForMatrix::File);
	char FirstFilePath[] = "C:\\Users\\PC\\Desktop\\Лабы\\Программ\\Дз\\ReadMe.txt";
	MatrixFromFile.ReadMatrixFromFile(FirstFilePath);
	MatrixFromFile.ShowMatrix();

	cout << "SecondMatrix:" << endl;
	MatrixClass<float> TestMat(PlaceForMatrix::File);
	char SecondFilePath[] = "C:\\Users\\PC\\Desktop\\Лабы\\Программ\\Дз\\ReadMeAgain.txt";
	TestMat.ReadMatrixFromFile(SecondFilePath);
	TestMat.ShowMatrix();

	ParamMat = TestMat;
	ParamMat.ShowMatrix();

	cout << "create new matrix" << endl;
	MatrixClass<float> consolemat(PlaceForMatrix::Console);
	cout << "thirdmatrix:" << endl;
	consolemat.ShowMatrix();

	cout << "firstmatrix + thirdmatrix:" << endl;
	MatrixFromFile + consolemat;

	cout << "firstmatrix - thirdmatrix:" << endl;
	MatrixFromFile - consolemat;

	cout << "firstmatrix * secondmatrix:" << endl;
	MatrixFromFile* TestMat;

	cout << "firstmatrix * 2:" << endl;
	MatrixFromFile * 2;


	cout << "FirstMatrix*2 replace 1 line with 2:" << endl;
	MatrixFromFile.ChangePlaceOfLines(1, 2);
	MatrixFromFile.ShowMatrix();

	cout << "Changed FirstMatrix Line 1 * 2:" << endl;
	MatrixFromFile.MultiplyLine(1, 2);
	MatrixFromFile.ShowMatrix();

	cout << "Changed FirstMatrix Line 1 + line 2 * 3:" << endl;
	MatrixFromFile.LinePlusMultiplyLine(1, 2, 3);
	MatrixFromFile.ShowMatrix();

	cout << "FirstMatrix == 2?:";
	if (MatrixFromFile == 2)
		cout << " Yes" << endl;
	else
		cout << " No" << endl;

	cout << "FirstMatrix == SecondMatrix?:";
	if (MatrixFromFile == TestMat)
		cout << " Yes" << endl;
	else
		cout << " No" << endl;

	cout << "FirstMatrix != SecondMatrix?:";
	if (MatrixFromFile != TestMat)
		cout << " Yes" << endl;
	else
		cout << " No" << endl;

	return 0;

}
