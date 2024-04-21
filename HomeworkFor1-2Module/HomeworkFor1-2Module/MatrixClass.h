#include <iostream>
#include <fstream>
#include <thread>
#include "Types.h"
#include <vector>
#include <string> 
using namespace std;

template <typename T>
class MatrixClass
{
public:
	~MatrixClass()
	{
		for (int i = 0; i < lines; i++)
		{
			delete[] matrix[i];
		}
		delete[] matrix;
		//cout << "Matrix deleted" << endl;
	}

	static MatrixClass ZeroMatrix(int lines, int columns)
	{
		MatrixClass<T> ZeroMat(columns, lines);
		ZeroMat.ZeroInitializeMatrix();
		return ZeroMat;
	}

	static MatrixClass ASingleMatrix(int lines, int columns)
	{
		MatrixClass<T> SingleMat(columns, lines);
		SingleMat.ZeroInitializeMatrix();
		for (int i = 0; i < columns; i++)
			for (int j = 0; j < lines; j++)
				if (i == j)
				{
					SingleMat.matrix[i][j] = 1;
					break;
				}

		return SingleMat;
	}

	MatrixClass(PlaceForMatrix Place)
	{
		switch (Place)
		{
		case PlaceForMatrix::None:
			break;
		case PlaceForMatrix::Console:
		{
			Place = PlaceForMatrix::Console;

			cout << "Write count of lines:" << endl;
			cin >> lines;

			cout << "Write count of columns:" << endl;
			cin >> columns;
			ReadMatrixFromConsole();
			break;
		}
		case PlaceForMatrix::File:
		{
			Place = PlaceForMatrix::File;
			break;
		}

		default:
			break;
		}
	}

	MatrixClass(MatrixClass& mat)
	{
		lines = mat.lines;
		columns = mat.columns;
		matrix = new T* [mat.lines];
		for (int i = 0; i < (mat.lines >= mat.columns ? mat.lines : mat.columns); i++)
			matrix[i] = new T[mat.columns];
	}

	MatrixClass(int columns, int lines)
	{
		this->lines = lines;
		this->columns = columns;
		matrix = new T* [lines];
		for (int i = 0; i < lines; i++)
			matrix[i] = new T[columns];
	}

	void ReadMatrixFromFile(char* FilePath)
	{
		CalculateParametrsOfMatrix(FilePath);
		CreateMatrix();
		ZeroInitializeMatrix();
		InitializeMatrixFromFile(FilePath);
	}

	void ReadMatrixFromConsole()
	{
		cout << "Write elements of matrix(you can use only space):" << endl;
		CreateMatrix();
		ZeroInitializeMatrix();
		InitializeMatrixFromConsole();
		cout << endl;
	}

	void ReadMatrixFromVariable(int** variable)
	{
		CreateMatrix();
		ZeroInitializeMatrix();
	}

	void ShowMatrix()
	{
		for (int i = 0; i < lines; i++)
			for (int j = 0; j < columns; j++)
			{
				if (j == columns - 1)
				{
					cout << matrix[j][i] << endl;
				}
				else
				{
					cout << matrix[j][i] << " ";
				}
			}
		cout << endl;
	}

	void ShowMatrix(float** matrix, int rows, int cols)
	{
		for (int i = 0; i < rows; i++)
			for (int j = 0; j < cols; j++)
			{
				if (j == cols - 1)
				{
					cout << matrix[j][i] << endl;
				}
				else
				{
					cout << matrix[j][i] << " ";
				}
			}
		cout << endl;
	}

	void ConsolePrint() {
		cout << endl;
		for (int i = 0; i < lines; i++) {
			for (int j = 0; j < columns; j++)
				cout << matrix[i][j] << " ";
			cout << endl;
		}
	}

	void ChangePlaceOfLines(int FirstLine, int SecondLine)
	{
		thread th([&]()
			{
				FirstLine -= 1;
				SecondLine -= 1;
				for (int i = 0; i < columns; i++)
				{
					int s = matrix[i][FirstLine];
					matrix[i][FirstLine] = matrix[i][SecondLine];
					matrix[i][SecondLine] = s;
				}
			});
		th.join();
	}

	void MultiplyLine(int Line, int Multiplier)
	{
		thread th([&]()
			{
				Line -= 1;
				for (int i = 0; i < columns; i++)
				{
					matrix[i][Line] *= Multiplier;
				}
			});
		th.join();
	}

	void LinePlusMultiplyLine(int FirstLine, int SecondLine, int Multiplier)
	{
		thread th([&]()
			{
				FirstLine -= 1;
				SecondLine -= 1;
				for (int i = 0; i < columns; i++)
				{
					matrix[i][FirstLine] += matrix[i][SecondLine] * Multiplier;
				}
			});
		th.join();
	}

	void operator*(MatrixClass& Matrix)
	{
		if (columns != Matrix.lines)
		{
			cout << "Can't do this!(columns != Matrix.lines)" << endl;
		}

		int CountOfElements = lines * Matrix.columns;
		int HalfCountOfElements;
		int ln = lines;
		int col = Matrix.columns;
		if (CountOfElements % 2 == 0)
		{
			HalfCountOfElements = CountOfElements / 2;
		}
		else
		{
			HalfCountOfElements = CountOfElements / 2 - 1;
		}
		float** NewMat = new float* [ln];//массив указателей
		for (int i = 0; i < ln; i++)
		{
			NewMat[i] = new float[col];
		}

		int element;
		int SdvigLine = 0;
		int SdvigCol = 0;

		while (SdvigLine != ln)
		{
			while (SdvigCol != col)
			{
				element = 0;
				for (int i = 0; i < columns; i++)
				{
					element += matrix[i][SdvigLine] * Matrix.matrix[SdvigCol][i];
				}
				NewMat[SdvigLine][SdvigCol] = element;
				SdvigCol += 1;
			}
			SdvigLine += 1;
			SdvigCol = 0;
		}
		ShowMatrix(NewMat, ln, col);
	}

	void operator*(int scalar)
	{
		for (int i = 0; i < lines; i++)
			for (int j = 0; j < columns; j++)
				matrix[j][i] *= 2;

		ShowMatrix();
	}

	void operator+(MatrixClass& Matrix)
	{
		if (lines != Matrix.lines || columns != Matrix.columns)
		{
			cout << "You cant do this" << endl;
		}
		else
		{
			int CountOfElements = lines * Matrix.columns;
			int HalfCountOfElements;
			if (CountOfElements % 2 == 0)
			{
				HalfCountOfElements = CountOfElements / 2;
			}
			else
			{
				HalfCountOfElements = CountOfElements / 2 - 1;
			}
			float** NewMat = new float* [HalfCountOfElements];//массив указателей
			for (int i = 0; i < HalfCountOfElements; i++)
			{
				NewMat[i] = new float[HalfCountOfElements];
			}

			for (int i = 0; i < HalfCountOfElements; i++)
				for (int j = 0; j < HalfCountOfElements; j++)
					NewMat[j][i] = matrix[j][i] + Matrix.matrix[j][i];

			ShowMatrix(NewMat, HalfCountOfElements, HalfCountOfElements);
		}
	}

	void operator-(MatrixClass& Matrix)
	{
		if (lines != Matrix.lines || columns != Matrix.columns)
		{
			cout << "You cant do this" << endl;
		}
		else
		{
			int CountOfElements = columns * Matrix.lines;
			int HalfCountOfElements;
			if (CountOfElements % 2 == 0)
				HalfCountOfElements = CountOfElements / 2;
			else
				HalfCountOfElements = CountOfElements / 2 - 1;
			float** NewMat = new float* [HalfCountOfElements];//массив указателей
			for (int i = 0; i < HalfCountOfElements; i++)
				NewMat[i] = new float[HalfCountOfElements];

			for (int i = 0; i < HalfCountOfElements; i++)
				for (int j = 0; j < HalfCountOfElements; j++)
					NewMat[i][j] = matrix[i][j] - Matrix.matrix[i][j];

			ShowMatrix(NewMat, HalfCountOfElements, HalfCountOfElements);
		}
	}

	bool operator==(MatrixClass& Matrix)
	{
		if (lines != Matrix.lines || columns != Matrix.columns)
		{
			return false;
		}
		int CountOfElements = lines * Matrix.columns;
		int HalfCountOfElements;
		if (CountOfElements % 2 == 0)
		{
			HalfCountOfElements = CountOfElements / 2;
		}
		else
		{
			HalfCountOfElements = CountOfElements / 2 - 1;
		}

		int count = 0;
		for (int i = 0; i < HalfCountOfElements; i++)
			for (int j = 0; j < HalfCountOfElements; j++)
				if (matrix[j][i] == Matrix.matrix[j][i])
					count += 1;

		if (count != CountOfElements)
			return false;
		else
			return true;
	}

	bool operator==(int scalar)
	{
		for (int i = 1; i < lines; i++)
			if (matrix[i][i] != scalar)
				return false;

		for (int i = 0; i < lines; i++)
			for (int j = 0; j < columns; j++)
				if (i != j && matrix[j][i] != 0)
					return false;

		return true;
	}

	bool operator!=(MatrixClass& Matrix)
	{
		return !(matrix == Matrix.matrix);
	}

	void operator=(MatrixClass& Matrix)
	{
		columns = Matrix.columns;
		lines = Matrix.lines;
		CreateMatrix();
		for (int i = 0; i < lines; i++)
			for (int j = 0; j < columns; j++)
				matrix[j][i] = Matrix.matrix[j][i];
	}

	void ZeroInitializeMatrix()
	{
		for (int i = 0; i < lines; i++)
			for (int j = 0; j < columns; j++)
				matrix[j][i] = 0;
	}

	void CalculateParametrsOfMatrix(char* FilePath)
	{
		ifstream in(FilePath);// окрываем файл для чтения
		if (in.is_open())
		{
			while (!in.eof())
			{
				char ch = NULL;
				while (ch != EOF)
				{
					ch = in.get();
					if (ch == ' ' || ch == '\n' || ch == EOF)
					{
						if (ch == '\n' || ch == EOF)
							lines += 1;
						else if (ch == ' ' && lines == 0)
							columns += 1;
					}
				}
			}
			columns += 1;
		}
		else
		{
			cout << "Is not open" << endl;
		}
		in.close();     // закрываем файл
	}

	void InitializeMatrixFromFile(char* FilePath)
	{
		ifstream in(FilePath);// окрываем файл для чтения
		int n;
		if (in.is_open())
		{
			int cols = 0;
			int rows = 0;
			while (in >> n)
			{
				matrix[cols][rows] = n;
				cols += 1;
				if (cols == columns)
				{
					cols = 0;
					rows += 1;
				}
			}
		}
		else
		{
			cout << "Error to open file" << endl;
		}
		in.close();
	}

	void InitializeMatrixFromConsole()
	{
		T n;
		int elements = 0;
		int cols = 0;
		int rows = 0;

		while (cin >> n)
		{
			elements += 1;
			matrix[cols][rows] = n;
			cols += 1;
			if (cols == columns)
			{
				cols = 0;
				rows += 1;
			}
			if (elements == (lines * columns))
				break;
		}
	}

	void CreateMatrix()
	{
		matrix = new T* [lines];//массив указателей
		for (int i = 0; i < (lines >= columns ? lines : columns); i++)
		{
			matrix[i] = new T[columns];
		}
	}

	int opredelitel() {
		if (columns != lines) {
			cout << "Cant get determinant" << endl;
			return 0;
		}
		if (lines == 1) {
			return matrix[0][0];
		}
		int opred = 0;
		if (lines == 2) {
			opred = matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
		}
		else {
			for (int n = 0; n < lines; n++) {
				MatrixClass<T> algdop(lines - 1, columns - 1);
				for (int i = 1; i < lines; i++) {
					int flag = 0;
					for (int j = 0; j < columns; j++) {
						if (j == n)
							flag = 1;

						if (j != n) {
							algdop.matrix[i - 1][j - flag] = matrix[i][j];

						}
					}
				}
				opred = opred + pow(-1, 1 + (n + 1)) * matrix[0][n] * algdop.opredelitel();
			}
		}
		return opred;
	}

	MatrixClass operator!() {
		MatrixClass<T> obratn_matr(lines, columns);
		float opred = opredelitel();
		if (opred == 0) {
			cout << "Cant get reverse matrix" << endl;
			obratn_matr.ZeroInitializeMatrix();
			return obratn_matr;
		}
		else if (lines != columns) {
			cout << "Cant get reverse matrix" << endl;
			obratn_matr.ZeroInitializeMatrix();
			return obratn_matr;
		}

		else if (lines == 1 && columns == 1) {
			obratn_matr.matrix[0][0] = 1 / matrix[0][0];
			return obratn_matr;
		}
		MatrixClass<float> dop_matr(lines - 1, columns - 1);
		for (int i = 0; i < lines; i++) {
			for (int j = 0; j < columns; j++) {
				int flagi = 0;
				int flagj = 0;
				for (int n = 0; n < lines - 1; n++) {
					if (n == i)
						flagi = 1;
					flagj = 0;
					for (int m = 0; m < columns - 1; m++) {
						if (m == j)
							flagj = 1;
						dop_matr.matrix[n][m] = matrix[n + flagi][m + flagj];
					}
				}
				obratn_matr.matrix[j][i] = pow(-1, (i + j)) * dop_matr.opredelitel() * (1 / opred);
			}
		}
		cout << endl;
		return obratn_matr;
	}

	T** matrix;
	PlaceForMatrix Place = PlaceForMatrix::None;
	unsigned int columns, lines = 0;
};
