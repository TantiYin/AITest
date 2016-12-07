#pragma once

#include "matrix2d.h"

//--------------------------- WorldTransform -----------------------------
//
//  given a std::vector of 2D vectors, a position, orientation and scale,
//  this function transforms the 2D vectors into the object's world space
//------------------------------------------------------------------------
inline std::vector<Vector2> WorldTransform(std::vector<Vector2> &points,
	const Vector2   &pos,
	const Vector2   &forward,
	const Vector2   &side,
	const Vector2   &scale)
{
	//copy the original vertices into the buffer about to be transformed
	std::vector<Vector2> TranVector2s = points;

	//create a transformation matrix
	Matrix2 matTransform;

	//scale
	if ((scale.x != 1.0) || (scale.y != 1.0))
	{
		matTransform.Scale(scale.x, scale.y);
	}

	//rotate
	matTransform.Rotate(forward, side);

	//and translate
	matTransform.Translate(pos.x, pos.y);

	//now transform the object's vertices
	matTransform.TransformVector2s(TranVector2s);

	return TranVector2s;
}

//--------------------------- WorldTransform -----------------------------
//
//  given a std::vector of 2D vectors, a position and  orientation
//  this function transforms the 2D vectors into the object's world space
//------------------------------------------------------------------------
inline std::vector<Vector2> WorldTransform(std::vector<Vector2> &points,
	const Vector2   &pos,
	const Vector2   &forward,
	const Vector2   &side)
{
	//copy the original vertices into the buffer about to be transformed
	std::vector<Vector2> TranVector2s = points;

	//create a transformation matrix
	Matrix2 matTransform;

	//rotate
	matTransform.Rotate(forward, side);

	//and translate
	matTransform.Translate(pos.x, pos.y);

	//now transform the object's vertices
	matTransform.TransformVector2s(TranVector2s);

	return TranVector2s;
}
