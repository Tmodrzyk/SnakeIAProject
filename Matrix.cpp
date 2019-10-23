#include "Matrix.h"
#include <iostream> //pour display
#include <cassert> // pour emp�cher l'utilisateur de faire des calculs sans aucun sens
#include <cstdlib>
#include <ctime> //pour le constructeur de matrice al�atoire
#include <cmath> //pour exp

Matrix::Matrix()
{
	values = nullptr;
	Width = 0;
	Height = 0;
}

Matrix::Matrix(const Matrix& other) : Matrix(other.Height, other.Width)
{
	Height = other.Height;
	Width = other.Width;
	setValues(other.values);
}

Matrix::Matrix(size_t a, size_t b)
{
	Height = a;
	Width = b;
	values = new double[Height * Width];
}

Matrix::Matrix(size_t a, size_t b, double* tab)
{
	Height = a;
	Width = b;
	values = new double[Height * Width];
	double* end = tab + Height * Width;
	double* current = values;

	for (double* it = tab; it < end; it++, current++)
	{
		*current = *it;
	}
}

Matrix::Matrix(size_t a, size_t b, double inf, double sup)
{
	Height = a;
	Width = b;
	values = new double[Height * Width];

	srand(time(NULL));
	for (size_t i = 0; i < Height; i++)
	{
		for (size_t j = 0; j < Width; j++)
		{
			at(i, j) = (sup - inf) * (double)(rand() / (RAND_MAX + 1.)) + inf;
		}
	}

}

Matrix::~Matrix()
{
	delete[] values;
}

double Matrix::at(size_t i, size_t j) const
{
	assert(i < Height && j < Width);
	return values[i + j * Height];
};

double& Matrix::at(size_t i, size_t j)
{
	return values[i + j * Height];
}

void Matrix::setValues(double* tab)
{
	if (tab != nullptr)
	{
		double* end = tab + Height * Width;
		double* current = values;
		for (double* it = tab; it < end; it++, current++)
		{
			*current = *it;
		}
	}
	else
	{
		values = nullptr;
	}
}

double Matrix::getHeight()
{
	return Height;
}

double Matrix::getWidth()
{
	return Width;
}

bool Matrix::operator==(const Matrix& other)
{
	assert(Width == other.Width && Height == other.Height);

	double* current1 = values;
	double* current2 = other.values;

	for (size_t i = 0; i < Width * Height; i++, current1++, current2++)
	{
		if (*current1 != *current2) { return false; };
	}
	return true;
}

Matrix Matrix::operator+(const Matrix& other) const
{
	Matrix res(Height, Width);
	assert(Height == other.Height && Width == other.Width);

	for (size_t i = 0; i < Width; i++)
	{
		for (size_t j = 0; j < Height; j++)
		{
			res.at(i, j) = at(i, j) + other.at(i, j);
		}
	}
	return res;
}

Matrix& Matrix::operator=(const Matrix& other)
{
	assert((Height == 0 && Width == 0) || (Height == other.Height && Width == other.Width));

	if (Height == 0 && Width == 0)
	{
		delete values;
		values = new double[other.Height * other.Width];
		Height = other.Height;
		Width = other.Width;
	}

	setValues(other.values);

	return *this;
}

Matrix Matrix::operator*(const Matrix& other) const
{
	Matrix res(Height, Width);
	assert(Height == other.Height && Width == other.Width);

	for (size_t i = 0; i < Width; i++)
	{
		for (size_t j = 0; j < Height; j++)
		{
			res.at(i, j) = at(i, j) * other.at(i, j);
		}
	}
	return res;
}

Matrix Matrix::dot(const Matrix& A, const Matrix& B)
{
	assert(A.Width == B.Height);

	Matrix res(A.Height, B.Width);
	double sum = 0;

	for (size_t i = 0; i < A.Height; i++)
	{
		for (size_t j = 0; j < B.Width; j++)
		{
			for (size_t k = 0; k < A.Width; k++)
			{
				sum += A.at(i, k) * B.at(k, j);
			}
			res.at(i, j) = sum;
			sum = 0;
		}
	}

	return res;
}

Matrix& Matrix::operator+=(const Matrix& other)
{
	assert(Height == other.Height && Width == other.Width);

	for (size_t i = 0; i < Width; i++)
	{
		for (size_t j = 0; j < Height; j++)
		{
			at(i, j) += other.at(i, j);
		}
	}
	return *this;
}

Matrix& Matrix::operator-=(const Matrix& other)
{
	assert(Height == other.Height && Width == other.Width);

	for (size_t i = 0; i < Width; i++)
	{
		for (size_t j = 0; j < Height; j++)
		{
			at(i, j) -= other.at(i, j);
		}
	}
	return *this;
}
void Matrix::uniform(const Matrix& A, double val)
{
	double* current = A.values;
	for (size_t i = 0; i < A.Height * A.Width; i++, current++)
	{
		*current = val;
	}
}

void Matrix::sigmoid(Matrix& A)
{
	double* current = A.values;
	for (size_t i = 0; i < A.Height * A.Width; i++, current++)
	{
		*current = 1 / (1 + exp(-*current));
	}
}

void Matrix::display()
{
	for (size_t i = 0; i < Height; i++)
	{
		for (size_t j = 0; j < Width; j++)
		{
			std::cout << at(i, j) << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}