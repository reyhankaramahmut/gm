////////////////////////////////////////////////////////////////////
//
//	Georg Umlauf, (c) 2012
//
////////////////////////////////////////////////////////////////////

#include "viewSystem.h"
#include <iostream>
#include <cmath>

using enum RotationMode;

// constructors
viewSystem::viewSystem()
{
    RotMode = VIEW_MATRIX_MODE;
}

viewSystem::viewSystem(const double origin[4], const double dir[4], const double up[4], double f)
{
    F       = f;
    ViewOri = origin;
    ViewDir = dir;
    ViewUp  = up;
    RotMode = VIEW_MATRIX_MODE;
}

viewSystem::viewSystem(const Point origin, const Vector dir, const Vector up, double f)
{
    F       = f;
    ViewOri = origin;
    ViewDir = dir;
    ViewUp  = up;
    RotMode = VIEW_MATRIX_MODE;
}

// destructor
viewSystem::~viewSystem()
{
}

// setter methods
void viewSystem::setData(const double origin[4], const double dir[4], const double up[4], double f)
{
    *this = viewSystem(origin,dir,up,f);
}

void viewSystem::setData(const Point origin, const Vector dir, const Vector up, double f)
{
    *this = viewSystem(origin,dir,up,f);
}

// rotation mode methods
void viewSystem::setMode(RotationMode mode)
{
    if (mode<RotationMode::MAX_ROTATION_MODE) RotMode = mode;
}

RotationMode viewSystem::getMode() const
{
    return RotMode;
}

void viewSystem::stepMode()
{
    switch(RotMode) {
    case VIEW_MATRIX_MODE:     RotMode = VIEW_FORMULA_MODE;    break;
    case VIEW_FORMULA_MODE:    RotMode = VIEW_QUATERNION_MODE; break;
    case VIEW_QUATERNION_MODE: RotMode = VIEW_MATRIX_MODE;     break;
    case MAX_ROTATION_MODE:
    default:				   RotMode = VIEW_FORMULA_MODE;    break;
    }
}

// affine transforms of the view system
void viewSystem::Zoom(double f)
{
    F+=f;
}

void viewSystem::RotateX(double angle)
{
    switch (RotMode) {
    case VIEW_FORMULA_MODE:
    case VIEW_MATRIX_MODE:  {
        AffineMap Rot;
        double c = cos(angle);
        double s = sin(angle);

        Rot(1,1) = c; Rot(1,2) = -s;
        Rot(2,1) = s; Rot(2,2) =  c;

        Rotate(Rot);
        break;
    }
    case VIEW_QUATERNION_MODE:
    {
        // AUFGABE02
        Quaternion q(Vector(1,0,0), angle);
        Rotate(q);
        break;
    }
    case MAX_ROTATION_MODE:
    default:
        // Correct the ViewMode
        RotMode = VIEW_FORMULA_MODE;
        break;
    }
}

void viewSystem::RotateY(double angle)
{
    switch (RotMode) {
    case VIEW_FORMULA_MODE:
    case VIEW_MATRIX_MODE:
    {
        AffineMap Rot;
        double c = cos(angle);
        double s = sin(angle);

        Rot(0,0) =  c; Rot(0,2) = s;
        Rot(2,0) = -s; Rot(2,2) = c;

        Rotate(Rot);
        break;
    }
    case VIEW_QUATERNION_MODE:
    {
        // AUFGABE02
        Quaternion q(Vector(0,1,0), angle);
        Rotate(q);
        break;
    }
    case MAX_ROTATION_MODE:
    default:
        // Correct the ViewMode
        RotMode = VIEW_FORMULA_MODE;
        break;
    }
}

void viewSystem::RotateZ(double angle)
{
    switch (RotMode) {
    case VIEW_FORMULA_MODE:
    case VIEW_MATRIX_MODE:
    {
        AffineMap Rot;
        double c = cos(angle);
        double s = sin(angle);

        Rot(0,0) = c; Rot(0,1) = -s;
        Rot(1,0) = s; Rot(1,1) =  c;

        Rotate(Rot);
        break;
    }
    case VIEW_QUATERNION_MODE:
    {
        // AUFGABE02
        Quaternion q(Vector(0,0,1), angle);
        Rotate(q);
        break;
    }
    case MAX_ROTATION_MODE:
    default:
        // Correct the ViewMode
        RotMode = VIEW_FORMULA_MODE;
        break;
    }
}

void viewSystem::RotateDir(double angle)
{
    switch (RotMode) {
    case VIEW_FORMULA_MODE:
    case VIEW_MATRIX_MODE:
        ViewUp = RotationMatrix(ViewDir,angle)*ViewUp;
        break;
    case VIEW_QUATERNION_MODE:
    {
        // AUFGABE02
        Quaternion q(ViewDir, angle);
        ViewUp = q.rotate(ViewUp);
        break;
    }
    case MAX_ROTATION_MODE:
    default:
        // Correct the ViewMode
        RotMode = VIEW_FORMULA_MODE;
        break;
    }
    ViewUp.normalize();
}

void viewSystem::RotateUp(double angle)
{
    switch (RotMode) {
    case VIEW_FORMULA_MODE:
    case VIEW_MATRIX_MODE:
        ViewDir = RotationMatrix(ViewUp,angle)*ViewDir;
        break;
    case VIEW_QUATERNION_MODE:
    {
        // AUFGABE02
        Quaternion q(ViewUp, angle);
        ViewDir = q.rotate(ViewDir);
        break;
    }
    case MAX_ROTATION_MODE:
    default:
        // Correct the ViewMode
        RotMode = VIEW_FORMULA_MODE;
        break;
    }
    ViewDir.normalize();
}

void viewSystem::RotateHor(double angle)
{
    Vector ViewHor = ViewDir^ViewUp;
    ViewHor.normalize();

    switch (RotMode) {
    case VIEW_FORMULA_MODE:
    case VIEW_MATRIX_MODE:
    {
        AffineMap Rot = RotationMatrix(ViewHor,angle);
        ViewUp        = Rot*ViewUp;
        ViewDir       = Rot*ViewDir;
        break;
    }
    case VIEW_QUATERNION_MODE:
    {
        // AUFGABE02
        Quaternion q(ViewHor, angle);
        ViewUp  = q.rotate(ViewUp );
        ViewDir = q.rotate(ViewDir);
        break;
    }
    case MAX_ROTATION_MODE:
    default:
        // Correct the ViewMode
        RotMode = VIEW_FORMULA_MODE;
        break;
    }
    ViewUp .normalize();
    ViewDir.normalize();
}

void viewSystem::Rotate(const AffineMap& mat)
{
    ViewOri = mat * ViewOri;
    ViewDir = mat * ViewDir;
    ViewUp  = mat * ViewUp;

    ViewDir.normalize();
    ViewUp .normalize();
}

void viewSystem::Rotate(const Quaternion& q)	// rotate by quaternion quat
{
    Vector ori = ViewOri - Point(0, 0, 0); // Ortsvektor
    ViewOri    = Point(0, 0, 0) + q.rotate(ori);
    ViewDir    = q.rotate(ViewDir);
    ViewUp     = q.rotate(ViewUp );

    ViewDir.normalize();
    ViewUp .normalize();
}

void viewSystem::Rotate(const Vector& axis, double angle)
{
    switch (RotMode) {
    case VIEW_FORMULA_MODE:
    case VIEW_MATRIX_MODE:
        Rotate(RotationMatrix(axis,angle));
        break;
    case VIEW_QUATERNION_MODE:
    {
        // AUFGABE02
        Quaternion q(axis, angle);
        Rotate(q);
        break;
    }
    case MAX_ROTATION_MODE:
    default:
        // Correct the ViewMode
        RotMode = VIEW_FORMULA_MODE;
        break;
    }
}

void viewSystem::Translate(const Vector& vec)
{
    ViewOri += vec;
}

// projection methods (in the view system coordinates)
Point viewSystem::Project(Point pnt) const
{	// central projection of Point
    AffineMap M;
    M(2,2) = 0.0;
    M(3,2) = -1/F;
    pnt    = M * pnt;
    pnt.homogenize();	// re-homogenize
    return pnt;
}

// view-to-world/world-to-view transformations
AffineMap viewSystem::getViewToWorld() const
{
    // AUFGABE01
    Vector ViewHor = ViewDir ^ViewUp;
    ViewHor.normalize();
    AffineMap M;

    M.setCol(ViewHor,  0);
    M.setCol(ViewUp,   1);
    M.setCol(-ViewDir, 2);
    M.setCol(ViewOri,  3);
    return M;
}

AffineMap viewSystem::getWorldToView() const
{
    // AUFGABE01
    AffineMap M = getViewToWorld();
    M.inverse();
    return M;
}

AffineMap viewSystem::RotationMatrix(Vector axis, double angle)
{
    AffineMap Rot;

    switch (RotMode) {
    case VIEW_FORMULA_MODE:
    {
        double c = cos(angle);
        double s = sin(angle);

        Rot(0,0) = axis[0]*axis[0]*(1-c) +         c;
        Rot(0,1) = axis[0]*axis[1]*(1-c) - axis[2]*s;
        Rot(0,2) = axis[0]*axis[2]*(1-c) + axis[1]*s;
        Rot(1,0) = axis[1]*axis[0]*(1-c) + axis[2]*s;
        Rot(1,1) = axis[1]*axis[1]*(1-c) +         c;
        Rot(1,2) = axis[1]*axis[2]*(1-c) - axis[0]*s;
        Rot(2,0) = axis[2]*axis[0]*(1-c) - axis[1]*s;
        Rot(2,1) = axis[2]*axis[1]*(1-c) + axis[0]*s;
        Rot(2,2) = axis[2]*axis[2]*(1-c) +         c;

        break;
    }
    case VIEW_MATRIX_MODE:
    {
        double d = sqrt(axis[0]*axis[0]+axis[1]*axis[1]);
        double a = axis[0]/d;
        double b = axis[1]/d;
        double c = cos(angle);
        double s = sin(angle);

        AffineMap Rot1;
        if (d) {
            Rot1(0,0) = a; Rot1(1,0) = -b;
            Rot1(0,1) = b; Rot1(1,1) =  a;
        }

        AffineMap Rot2;
        Rot2(0,0) =  axis[2]; Rot2(2,0) = d;
        Rot2(0,2) = -d;       Rot2(2,2) = axis[2];

        AffineMap Rot3;
        Rot3(0,0) =  c; Rot3(1,0) = s;
        Rot3(0,1) = -s; Rot3(1,1) = c;

        AffineMap Rot4(Rot2);
        Rot4(2,0) = Rot2(0,2); Rot4(0,2) = Rot2(2,0);

        AffineMap Rot5(Rot1);
        Rot5(1,0) = Rot1(0,1); Rot5(0,1) = Rot1(1,0);

        Rot = Rot5*Rot4*Rot3*Rot2*Rot1;
        break;
    }
    case VIEW_QUATERNION_MODE:
        std::cerr << "Error in rotation mode: For quaternions no rotation matrix required.\n";
        break;
    case MAX_ROTATION_MODE:
    default:
        // Correct the ViewMode
        RotMode = VIEW_FORMULA_MODE;
        break;
    }

    return Rot;
}

// assignment operator
viewSystem &viewSystem::operator = (const viewSystem &vs)
{
    if (this==&vs) return (*this); // ok, it's me, so no l-value action
    // not me, so l-value action: copy data
    F       = vs.F;
    ViewOri = vs.ViewOri;
    ViewDir = vs.ViewDir;
    ViewUp  = vs.ViewUp;
    RotMode = vs.RotMode;
    return (*this);
}

// output
std::ostream &operator<< (std::ostream &ostr, const viewSystem &u)
{
    switch (u.getMode()) {
    case VIEW_MATRIX_MODE:     std::cout << "Matrix-Rotation";     break;
    case VIEW_FORMULA_MODE:    std::cout << "Formel-Rotation";     break;
    case VIEW_QUATERNION_MODE: std::cout << "Quaternion-Rotation"; break;
    case MAX_ROTATION_MODE:
    default:                   std::cout << "Undefinierter Rotation-Mode"; break;
    }
    return ostr;
}