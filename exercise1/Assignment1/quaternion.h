////////////////////////////////////////////////////////////////////
//
//	Georg Umlauf, (c) 2012
//
////////////////////////////////////////////////////////////////////
#pragma once

#include "AffineGeometry.h"
#include "AffineMap.h"

class Quaternion
{
private:
    double Re;	// real      part
    Vector Im;	// imaginary part

public:
    // constructors/destructors
    Quaternion();        // default constructor
    Quaternion(      double     re, const Vector& im);
    Quaternion(const Vector&  axis,       double angle);
    ~Quaternion();

    Quaternion conjugate() const;
    double     getNorm  () const;
    void       normalize();

    Quaternion operator * (const Quaternion& q) const;
    Quaternion operator * (      double      s) const;
    Quaternion operator + (const Quaternion& q) const;
    Quaternion operator - () const;
    double     dot        (const Quaternion& q) const;

    Vector rotate(const Vector& v) const;

    // interpolation methods
    Quaternion lerp (const Quaternion& q, double t) const;
    Quaternion nlerp(const Quaternion& q, double t) const;
    Quaternion slerp(const Quaternion& q, double t) const;
};