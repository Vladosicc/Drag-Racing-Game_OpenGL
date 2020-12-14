#ifndef _3DMATH_H
#define _3DMATH_H


#include <float.h>	// ����� ������������ _isnan() ��� acos()

#define PI 3.1415926535897932		// ��� ���������� �� %)


// ��� ��������� ������������ ��� �������� 3� ����� � ��������.
struct CVector3
{
public:
    float x, y, z;
};

// ���������� ������, ���������������� ���� ���������� �������� (���������)
CVector3 Cross(CVector3 vVector1, CVector3 vVector2);

// ���������� ������ ����� 2�� �������.
CVector3 Vector(CVector3 vPoint1, CVector3 vPoint2);

// ���������� �������� ������� ��� ������ ������� �������
float Magnitude(CVector3 vNormal);

// ���������� ��������������� ������ (��� ������ ���������� ������ 1)
CVector3 Normalize(CVector3 vNormal);

// ���������� ������� �������� (�����������, ���� �������� �������)
CVector3 Normal(CVector3 vTriangle[]);

// ���������� ���������� ��������� �� ������ ��������� (0,0,0).
// ��������� ������� � ��������� � ����� �����, ������� �� ���� ���������:
float PlaneDistance(CVector3 Normal, CVector3 Point);

// ��������� ����������� (���������) � �����, � ���������� true ��� �� �����������.
bool IntersectedPlane(CVector3 vPoly[], CVector3 vLine[], CVector3& vNormal, float& originDistance);



// ���������� dot product ����� ����� ���������
float Dot(CVector3 vVector1, CVector3 vVector2);

// ���������� ���� ����� ����� ���������
double AngleBetweenVectors(CVector3 Vector1, CVector3 Vector2);

// ���������� ����� ����������� �������� � ����� (��������������� ����������� ���������)
CVector3 IntersectionPoint(CVector3 vNormal, CVector3 vLine[], double distance);

// ���������� true ���� ����� ����������� ��������� ������ ��������
bool InsidePolygon(CVector3 vIntersection, CVector3 Poly[], long verticeCount);

// ����������� ��� ������� ��� ����� ����������� ����� � ��������
bool IntersectedPolygon(CVector3 vPoly[], CVector3 vLine[], int verticeCount);



#endif
