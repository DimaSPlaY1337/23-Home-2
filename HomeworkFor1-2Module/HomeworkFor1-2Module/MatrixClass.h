#include <iostream>
#include <fstream>
#include <future>
#include <thread>
#include "Types.h"
#include <queue>
#include <vector>
#include <string> 
#include <chrono>
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

	MatrixClass(MatrixClass& mat1, MatrixClass& mat2, bool Place)
	{
		CreateMatrixWithBlocks(mat1, mat2, Place);
	}

	MatrixClass(TheOriginOfTheMatrix Place)
	{
		switch (Place)
		{
		case TheOriginOfTheMatrix::None:
			break;
		case TheOriginOfTheMatrix::Console:
		{
			cout << "Write count of lines:" << endl;
			cin >> lines;

			cout << "Write count of columns:" << endl;
			cin >> columns;
			ReadMatrixFromConsole();
			break;
		}
		case TheOriginOfTheMatrix::File:
		{
			break;
		}

		default:
			break;
		}
	}

	void CreateMatrixWithBlocks(MatrixClass& mat1, MatrixClass& mat2, bool Place)
	{
		lines1 = mat1.lines;
		lines2 = mat2.lines;
		columns1 = mat1.columns;
		columns2 = mat2.columns;
		flag = true;
		//снизу приписываем
		if (Place == true && mat1.columns == mat2.columns)
		{
			int CachedLines = mat1.lines;
			columns = mat1.columns;
			lines = mat1.lines + mat2.lines;
			CreateMatrix();

			block1 = new T * [lines1];//массив указателей
			for (int i = 0; i < lines1; i++)
			{
				block1[i] = new T[columns1];
			}
			block2 = new T * [lines2];//массив указателей
			for (int i = 0; i < lines2; i++)
			{
				block2[i] = new T[columns2];
			}

			for (int i = 0; i < CachedLines; i++)
				for (int j = 0; j < columns; j++)
				{
					block1[i][j] = mat1.matrix[i][j];
					matrix[i][j] = mat1.matrix[i][j];
				}

			for (int i = CachedLines; i < lines; i++)
				for (int j = 0; j < columns; j++)
				{
					block2[i - columns][j] = mat2.matrix[i - columns][j];
					matrix[i][j] = mat2.matrix[i - columns][j];
				}

			//ShowMatrix(matrix, lines, columns);
		}
		//справа приписываем
		else if (Place == false && mat1.lines == mat2.lines)
		{
			int CachedColumns = mat1.columns;
			lines = mat1.lines;
			columns = mat1.columns + mat2.columns;
			CreateMatrix();

			block1 = new T * [lines1];//массив указателей
			for (int i = 0; i < lines1; i++)
			{
				block1[i] = new T[columns1];
			}
			block2 = new T * [lines2];//массив указателей
			for (int i = 0; i < lines2; i++)
			{
				block2[i] = new T[columns2];
			}

			for (int i = 0; i < lines; i++)
				for (int j = 0; j < CachedColumns; j++)
				{
					block1[i][j] = mat1.matrix[i][j];
					matrix[i][j] = mat1.matrix[i][j];
				}

			for (int i = 0; i < lines; i++)
				for (int j = CachedColumns; j < columns; j++)
				{
					block2[i][j - CachedColumns] = mat2.matrix[i][j - CachedColumns];
					matrix[i][j] = mat2.matrix[i][j - CachedColumns];
				}

			//ShowMatrix(matrix, lines, columns);
		}
		else
		{
			flag = false;
			cout << "Bad matrix added" << endl;
		}
	}

	MatrixClass(MatrixClass& mat)
	{
		lines = mat.lines;
		columns = mat.columns;
		matrix = new T * [mat.lines];
		for (int i = 0; i < mat.lines; i++)
			matrix[i] = new T[mat.columns];
	}

	MatrixClass(int columns, int lines)
	{
		this->lines = lines;
		this->columns = columns;
		matrix = new T * [lines];
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
					cout << matrix[i][j] << endl;
				}
				else
				{
					cout << matrix[i][j] << " ";
				}
			}
		cout << endl;
	}

	void ShowMatrix(T** matrix, int rows, int cols)
	{
		for (int i = 0; i < rows; i++)
			for (int j = 0; j < cols; j++)
			{
				if (j == cols - 1)
				{
					cout << matrix[i][j] << endl;
				}
				else
				{
					cout << matrix[i][j] << " ";
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
		int col = columns;
		vector<thread> threads;
		for (int i = 0; i < lines; i++)
		{
			threads.emplace_back([this, &scalar, i, col] {
				for (int j = 0; j < col; j++)
					matrix[i][j] *= scalar;
				});
		}
		for (int i = 0; i < threads.size(); i++)
			threads[i].join();

		ShowMatrix();
	}

	MatrixClass operator+(MatrixClass& Matrix)
	{
		MatrixClass<T> MClass(Matrix.columns, Matrix.lines);
		vector<thread> threads;
		int col = columns;
		if (lines != Matrix.lines || columns != Matrix.columns)
		{
			cout << "You cant do this" << endl;
		}
		else
		{
			for (int i = 0; i < lines; i++)
			{
				threads.emplace_back([this, i, col, &MClass, &Matrix] {
					for (int j = 0; j < columns; j++)
						MClass.matrix[i][j] = matrix[i][j] + Matrix.matrix[i][j];
					});
			}

			for (int i = 0; i < threads.size(); i++)
				threads[i].join();

			ShowMatrix(MClass.matrix, lines, columns);
			return MClass;
		}
	}

	void InitializeBlocks(int lines1, int columns1, int lines2, int columns2)
	{
		block1 = new T * [lines1];//массив указателей
		for (int i = 0; i < lines1; i++)
		{
			block1[i] = new T[columns1];
		}
		block2 = new T * [lines2];//массив указателей
		for (int i = 0; i < lines2; i++)
		{
			block2[i] = new T[columns2];
		}
	}

	void MatrixSum(int lines, int columns, MatrixClass& mat1, MatrixClass& mat2, MatrixClass& mat3, int index)
	{
		if (index == 1)
		{
			for (int i = 0; i < lines; i++)
				for (int j = 0; j < columns; j++)
					mat1.block1[i][j] = mat2.block1[i][j] + mat3.block1[i][j];
		}
		else if (index == 2)
		{
			for (int i = 0; i < lines; i++)
				for (int j = 0; j < columns; j++)
					mat1.block2[i][j] = mat2.block2[i][j] + mat3.block2[i][j];
		}
		else
		{
			for (int i = 0; i < lines; i++)
				for (int j = 0; j < columns; j++)
					mat1.matrix[i][j] = mat2.matrix[i][j] + mat3.matrix[i][j];
		}
	}

	MatrixClass Plus(MatrixClass& Matrix)
	{
		MatrixClass<T> MClass(Matrix.columns, Matrix.lines);
		auto begin = chrono::high_resolution_clock::now();
		queue<future<void>> queue;
		MClass.InitializeBlocks(Matrix.lines1, Matrix.columns1, Matrix.lines2, Matrix.columns2);

		if (lines1 == Matrix.lines1 && lines2 == Matrix.lines2 &&
			columns1 == Matrix.columns1 && columns2 == Matrix.columns2 && flag)
		{

			queue.push(async(launch::async, &MatrixClass::MatrixSum, this, lines1, columns1, ref(MClass), ref(*this), ref(Matrix), 1));
			queue.push(async(launch::async, &MatrixClass::MatrixSum, this, lines2, columns2, ref(MClass), ref(*this), ref(Matrix), 2));
			queue.push(async(launch::async, &MatrixClass::MatrixSum, this, lines, columns, ref(MClass), ref(*this), ref(Matrix), 0));

			while (!queue.empty())
			{
				queue.front().wait();
				queue.pop();
			}


			auto end = chrono::high_resolution_clock::now();
			auto elapsed_ms = chrono::duration_cast<chrono::milliseconds>(end - begin);
			std::cout << "The time: " << elapsed_ms.count() << " ms\n";
			//ShowMatrix(MClass.matrix, lines, columns);
			return MClass;
		}
		else
		{
			cout << "You cant do this" << endl;
			return MClass;
		}

	}

	void MatrixMultiply(int lines, int columns, MatrixClass& mat1, MatrixClass& mat2, MatrixClass& mat3, int index)
	{
		int element;
		int SdvigLine = 0;
		int SdvigCol = 0;

		if (index == 1)
		{
			if (mat2.columns1 != mat3.lines1)
			{
				cout << "You cant multiply" << endl;
			}
			else
			{
				while (SdvigLine != lines)
				{
					while (SdvigCol != columns)
					{
						element = 0;
						for (int i = 0; i < lines; i++)
						{
							element += mat2.block1[SdvigLine][i] * mat3.block1[i][SdvigCol];
						}
						mat1.block1[SdvigLine][SdvigCol] = element;
						SdvigCol += 1;
					}
					SdvigLine += 1;
					SdvigCol = 0;
				}
			}
		}
		else if (index == 2)
		{
			if (mat2.columns1 != mat3.lines1)
			{
				cout << "You cant multiply" << endl;
			}
			else
			{
				while (SdvigLine != lines)
				{
					while (SdvigCol != columns)
					{
						element = 0;
						for (int i = 0; i < lines; i++)
						{
							element += mat2.block2[SdvigLine][i] * mat3.block2[i][SdvigCol];
						}
						mat1.block2[SdvigLine][SdvigCol] = element;
						SdvigCol += 1;
					}
					SdvigLine += 1;
					SdvigCol = 0;
				}
			}
		}
		else
		{
			for (int i = 0; i < lines; i++)
				for (int j = 0; j < columns; j++)
					mat1.matrix[i][j] = mat1.block1[i][j] + mat1.block2[i][j];
		}
	}

	MatrixClass Multiply(MatrixClass& Matrix)
	{
		MatrixClass<T> MClass(lines, Matrix.columns);
		MClass.InitializeBlocks(Matrix.lines1, Matrix.columns1, Matrix.lines2, Matrix.columns2);
		if (flag && lines == Matrix.columns)
		{
			queue<future<void>> queue;

			auto begin = chrono::high_resolution_clock::now();

			queue.push(async(launch::async, &MatrixClass::MatrixMultiply, this, lines1, Matrix.columns1, ref(MClass), ref(*this), ref(Matrix), 1));
			//queue.push(async(launch::async, &MatrixClass::MatrixMultiply, this, lines2, Matrix.columns2, ref(MClass), ref(*this), ref(Matrix), 2));
			//queue.push(async(launch::async, &MatrixClass::MatrixMultiply, this, lines, Matrix.columns, ref(MClass), ref(*this), ref(Matrix), 0));

			while (!queue.empty())
			{
				queue.front().wait();
				queue.pop();
			}
			//MatrixMultiply(lines1, Matrix.columns1, MClass, *this, Matrix, 1);
			MatrixMultiply(lines2, Matrix.columns2, MClass, *this, Matrix, 2);
			MatrixMultiply(lines, Matrix.columns, MClass, *this, Matrix, 0);

			auto end = chrono::high_resolution_clock::now();
			auto elapsed_ms = chrono::duration_cast<chrono::milliseconds>(end - begin);
			std::cout << "The time: " << elapsed_ms.count() << " ms\n";
			//ShowMatrix(MClass.matrix, lines, Matrix.columns);

		}
		else
		{
			cout << "lines != Matrix.columns" << endl;
		}
		return MClass;
	}

	void operator-(MatrixClass& Matrix)
	{
		int col = columns;
		vector<thread> threads;
		if (lines != Matrix.lines || columns != Matrix.columns)
		{
			cout << "You cant do this" << endl;
		}
		else
		{
			float** NewMat = new float* [lines];//массив указателей
			for (int i = 0; i < columns; i++)
				NewMat[i] = new float[columns];

			for (int i = 0; i < lines; i++)
			{
				threads.emplace_back([this, i, col, &NewMat, &Matrix] {
					for (int j = 0; j < col; j++)
						NewMat[i][j] = matrix[i][j] - Matrix.matrix[i][j];
					});
			}
				
			for (int i = 0; i < threads.size(); i++)
				threads[i].join();

			ShowMatrix(NewMat, lines, columns);
		}
	}

	bool operator==(MatrixClass& Matrix)
	{
		int col = columns;
		vector<thread> threads;
		if (lines != Matrix.lines || columns != Matrix.columns)
		{
			return false;
		}
		int CountOfElements = lines * Matrix.columns;
		int count = 0;
		for (int i = 0; i < lines; i++)
		{
			threads.emplace_back([this, i, col, &count, &Matrix] {
				for (int j = 0; j < col; j++)
					if (matrix[i][j] == Matrix.matrix[i][j])
						count += 1;
				});
		}

		for (int i = 0; i < threads.size(); i++)
			threads[i].join();

		if (count != CountOfElements)
			return false;
		else
			return true;
	}

	bool operator==(int scalar)
	{
		int col = columns;
		vector<thread> threads;
		for (int i = 1; i < lines; i++)
			if (matrix[i][i] != scalar)
				return false;

		for (int i = 0; i < lines; i++)
		{
			threads.emplace_back([this, i, col] {
				for (int j = 0; j < col; j++)
					if (i != j && matrix[i][j] != 0)
						return false;
				});
		}

		for (int i = 0; i < threads.size(); i++)
			threads[i].join();

		return true;
	}

	bool operator!=(MatrixClass& Matrix)
	{
		return !(matrix == Matrix.matrix);
	}

	void operator=(MatrixClass& Matrix)
	{
		int col = columns;
		vector<thread> threads;
		columns = Matrix.columns;
		lines = Matrix.lines;
		CreateMatrix();
		for (int i = 0; i < lines; i++)
		{
			threads.emplace_back([this, i, col] {
				for (int j = 0; j < columns; j++)
					matrix[j][i] = Matrix.matrix[j][i];
				});
		}
		for (int i = 0; i < threads.size(); i++)
			threads[i].join();
	}

	void ZeroInitializeMatrix()
	{
		for (int i = 0; i < lines; i++)
			for (int j = 0; j < columns; j++)
				matrix[i][j] = 0;
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
				matrix[rows][cols] = n;
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
		matrix = new T * [lines];//массив указателей
		for (int i = 0; i < lines; i++)
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

	bool flag = false;
	T** block1;
	T** block2;
	T** matrix;
	unsigned int columns, lines = 0;
	unsigned int columns1, lines1 = 0;
	unsigned int columns2, lines2 = 0;
};
