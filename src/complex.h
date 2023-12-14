#pragma once
#include "glIncludes.h"

/// <summary>
/// A complex variable a + bi
/// </summary>
struct Complex
{
	float real;
	float imaginary;

	Complex Conjugate()
	{
		return Complex(real, -imaginary);
	}

	Complex operator+(const Complex& other) const
	{
		return Complex(real + other.real, imaginary + other.imaginary);
	}

	Complex operator-(const Complex& other) const
	{
		return Complex(real - other.real, imaginary - other.imaginary);
	}

	Complex operator*(const Complex& other) const
	{
		return Complex(real * other.real - imaginary * other.imaginary, real * other.imaginary + imaginary * other.real);
	}

	Complex operator-() const
	{
		return Complex(-real, -imaginary);
	}

	Complex operator*(float other) const
	{
		return Complex(real * other, imaginary * other);
	}

	Complex& operator=(const Complex& other)
	{
		real = other.real;
		imaginary = other.imaginary;
		return *this;
	}

	operator glm::vec2()
	{
		return glm::vec2(real, imaginary);
	}

	Complex()
		: real(0), imaginary(0)
	{}

	explicit Complex(float _real, float _imaginary)
		: real(_real), imaginary(_imaginary)
	{}
};