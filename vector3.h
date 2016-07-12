#ifndef _VECTOR3_H_
#define _VECTOR3_H_

class Vector3
{
public:
	//Vector3() :x(0), y(0), z(0){}
	Vector3(double nx, double ny, double nz) :x(nx), y(ny), z(nz){}
	Vector3(const Vector3& v) :x(v.x), y(v.y), z(v.z){}
	Vector3& operator= (const Vector3& v){ x = v.x; y = v.y; z = v.z; return *this; }


public:
	double x, y, z;

}
#endif