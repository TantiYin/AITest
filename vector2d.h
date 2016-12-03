#pragma once


#include <limits>



struct  Vector2
{
	double x, y;
	Vector2() :x(0.0), y(0.0){}
	Vector2(double a, double b) :x(a), y(b){}

	//set x y to zero
	void Zero(){ x = 0.0; y = 0.0; }
	//returns true if both x and y are zero
	bool isZero()const{ return (x*x + y*y) < (std::numeric_limits<double>::min)();}
	//returns the length of the vector
	inline double Length() const;
	//returns the squared length of the vector (thereby avoiding the sqrt)
	inline double LengthSq() const;

	inline void Normalize();
	//adjusts x and y so that the length of the vector does not exceed max
	inline void      Truncate(double max);

	//we need some overloaded operators
	const Vector2& operator+=(const Vector2 &rhs)
	{
		x += rhs.x;
		y += rhs.y;

		return *this;
	}

	const Vector2& operator-=(const Vector2 &rhs)
	{
		x -= rhs.x;
		y -= rhs.y;

		return *this;
	}

	const Vector2& operator*=(const double& rhs)
	{
		x *= rhs;
		y *= rhs;

		return *this;
	}

	const Vector2& operator/=(const double& rhs)
	{
		x /= rhs;
		y /= rhs;

		return *this;
	}

};

//------------------------------------------------------------------------member functions

//------------------------- Length ---------------------------------------
//
//  returns the length of a 2D vector
//------------------------------------------------------------------------
inline double Vector2::Length()const
{
	return sqrt(x * x + y * y);
}


//------------------------- LengthSq -------------------------------------
//
//  returns the squared length of a 2D vector
//------------------------------------------------------------------------
inline double Vector2::LengthSq()const
{
	return (x * x + y * y);
}

//------------------------- Normalize ------------------------------------
//
//  normalizes a 2D Vector
//------------------------------------------------------------------------
inline void Vector2::Normalize()
{
	double vector_length = this->Length();

	if (vector_length > std::numeric_limits<double>::epsilon())
	{
		this->x /= vector_length;
		this->y /= vector_length;
	}
}

//----------------------------- Truncate ---------------------------------
//
//  truncates a vector so that its length does not exceed max
//------------------------------------------------------------------------
inline void Vector2::Truncate(double max)
{
	if (this->Length() > max)
	{
		this->Normalize();

		*this *= max;
	}
}

//------------------------------------------------------------------------non member functions

inline Vector2 Vec2DNormalize(const Vector2 &v)
{
	Vector2 vec = v;

	double vector_length = vec.Length();

	if (vector_length > std::numeric_limits<double>::epsilon())
	{
		vec.x /= vector_length;
		vec.y /= vector_length;
	}

	return vec;
}

//------------------------------------------------------------------------operator overloads
inline Vector2 operator*(const Vector2 &lhs, double rhs)
{
	Vector2 result(lhs);
	result *= rhs;
	return result;
}

inline Vector2 operator*(double lhs, const Vector2 &rhs)
{
	Vector2 result(rhs);
	result *= lhs;
	return result;
}

//overload the - operator
inline Vector2 operator-(const Vector2 &lhs, const Vector2 &rhs)
{
	Vector2 result(lhs);
	result.x -= rhs.x;
	result.y -= rhs.y;

	return result;
}

//overload the + operator
inline Vector2 operator+(const Vector2 &lhs, const Vector2 &rhs)
{
	Vector2 result(lhs);
	result.x += rhs.x;
	result.y += rhs.y;

	return result;
}

//overload the / operator
inline Vector2 operator/(const Vector2 &lhs, double val)
{
	Vector2 result(lhs);
	result.x /= val;
	result.y /= val;

	return result;
}

