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


//--------------------- PointToWorldSpace --------------------------------
//
//  Transforms a point from the agent's local space into world space
//------------------------------------------------------------------------
inline Vector2 PointToWorldSpace(const Vector2 &point,
	const Vector2 &AgentHeading,
	const Vector2 &AgentSide,
	const Vector2 &AgentPosition)
{
	//make a copy of the point
	Vector2 TransPoint = point;

	//create a transformation matrix
	Matrix2 matTransform;

	//rotate
	matTransform.Rotate(AgentHeading, AgentSide);

	//and translate
	matTransform.Translate(AgentPosition.x, AgentPosition.y);

	//now transform the vertices
	matTransform.TransformVector2s(TransPoint);

	return TransPoint;
}


//--------------------- PointToLocalSpace --------------------------------
//
//------------------------------------------------------------------------
inline Vector2 PointToLocalSpace(const Vector2 &point,
	Vector2 &AgentHeading,
	Vector2 &AgentSide,
	Vector2 &AgentPosition)
{

	//make a copy of the point
	Vector2 TransPoint = point;

	//create a transformation matrix
	Matrix2 matTransform;

	double Tx = -AgentPosition.Dot(AgentHeading);
	double Ty = -AgentPosition.Dot(AgentSide);

	//create the transformation matrix
	matTransform._11(AgentHeading.x); matTransform._12(AgentSide.x);
	matTransform._21(AgentHeading.y); matTransform._22(AgentSide.y);
	matTransform._31(Tx);           matTransform._32(Ty);

	//now transform the vertices
	matTransform.TransformVector2s(TransPoint);

	return TransPoint;
}

//--------------------- VectorToWorldSpace --------------------------------
//
//  Transforms a vector from the agent's local space into world space
//------------------------------------------------------------------------
inline Vector2 VectorToWorldSpace(const Vector2& vec,
	const Vector2& AgentHeading,
	const Vector2& AgentSide)
{
	//make a copy of the point
	Vector2 TransVec = vec;

	//create a transformation matrix
	Matrix2 matTransform;

	//rotate
	matTransform.Rotate(AgentHeading, AgentSide);

	//now transform the vertices
	matTransform.TransformVector2s(TransVec);

	return TransVec;
}

//-------------------------- Vec2DRotateAroundOrigin --------------------------
//
//  rotates a vector ang rads around the origin
//-----------------------------------------------------------------------------
inline void Vec2DRotateAroundOrigin(Vector2& v, double ang)
{
    //create a transformation matrix
    Matrix2 mat;

    //rotate
    mat.Rotate(ang);

    //now transform the object's vertices
    mat.TransformVector2s(v);
}
