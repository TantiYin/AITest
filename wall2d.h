#pragma once

#include "vector2d.h"

class Wall2d
{
private:
    Vector2 mA, mB, mNormal;

public:
    Wall2d() {}

    Wall2d(Vector2 A, Vector2 B): mA(A), mB(B)
    {
        CalculateNormal();
    }
    Wall2d(Vector2 a, Vector2 b, Vector2 normal) : mA(a), mB(b), mNormal(normal) {}

    void CalculateNormal();

    Vector2 From() const { return mA; }
    void SetFrom(Vector2 from) { mA = from; CalculateNormal(); }

    Vector2 To() const { return mB; }
    void SetTo(Vector2 to) { mB = to; CalculateNormal(); }

    Vector2 Normal() const { return mNormal; }
    void SetNormal(Vector2 normal) { mNormal = normal; }

    void Render() const;
};
