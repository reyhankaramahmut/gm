////////////////////////////////////////////////////////////////////
//
//	Georg Umlauf, (c) 2012
//
////////////////////////////////////////////////////////////////////

/*
 * Author: Reyhan Karamahmut
 * Date: 2026-04-29
 */

#include "quaternion.h"
#include <cmath>

Quaternion::Quaternion()
{
    Re = 1.0;
    Im = Vector(0.0, 0.0, 0.0, 0.0);
}

Quaternion::Quaternion(double re, const Vector& im)
{
    Re = re;
    Im = im;
}

Quaternion::Quaternion(const Vector& axis, double angle)
{
    Vector n = axis;
    n.normalize();

    double h = angle / 2.0;
    Re = cos(h);
    Im = n * sin(h);
}

Quaternion::~Quaternion()
{
}

Quaternion Quaternion::conjugate() const
{
    return Quaternion(Re, -Im);
}

double Quaternion::getNorm() const
{
    return sqrt(Re*Re + Im.getNorm2());
}

void Quaternion::normalize()
{
    double n = getNorm();
    Re /= n;
    Im /= n;
}

Quaternion Quaternion::operator * (const Quaternion& q) const
{
    double newRe = Re * q.Re  -  (Im * q.Im);
    Vector newIm = q.Im * Re  +   Im * q.Re  +  (Im ^ q.Im);
    return Quaternion(newRe, newIm);
}

Vector Quaternion::rotate(const Vector& v) const
{
    Quaternion p     (0.0, v);
    Quaternion result = (*this) * p * conjugate();
    return result.Im;
}

Quaternion Quaternion::operator * (double s) const
{
    return Quaternion(Re * s, Im * s);
}

Quaternion Quaternion::operator + (const Quaternion& q) const
{
    return Quaternion(Re + q.Re, Im + q.Im);
}

double Quaternion::dot(const Quaternion& q) const
{
    return Re * q.Re + (Im * q.Im);
}

Quaternion Quaternion::operator - () const
{
    return Quaternion(-Re, -Im);
}

Quaternion Quaternion::lerp(const Quaternion& q, double t) const
{
    Quaternion qE = q;
    if (dot(q) < 0.0) qE = -q; // kürzester Weg
    return (*this) * (1.0 - t) + qE * t;
}

Quaternion Quaternion::nlerp(const Quaternion& q, double t) const
{
    Quaternion result = lerp(q, t);
    result.normalize();
    return result;
}

Quaternion Quaternion::slerp(const Quaternion& q, double t) const
{
    double cosOmega = dot(q);
    Quaternion qE = q;
    if (cosOmega < 0.0) { // kürzester Weg
        qE       = -q;
        cosOmega = -cosOmega;
    }

    double omega    = acos(cosOmega); // winkel zwischen q1 und q2
    double sinOmega = sin(omega);
    if (sinOmega) {
        double w0 = sin((1.0 - t) * omega) / sinOmega;
        double w1 = sin(       t  * omega) / sinOmega;
        return (*this) * w0 + qE * w1;
    }
    return nlerp(qE, t); // fallback bei sinOmega = 0
}