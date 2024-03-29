#ifndef __CVECTOR_H
#define __CVECTOR_H

#include <math.h>

// ����� ����� ����� ��� �������� �������� ���������� ��������� �������:
class CTexCoord
{
public:
    CTexCoord()
    {
        tu = 0.0f; tv = 0.0f;
    }

    CTexCoord(float u, float v)
    {
        tu = u; tv = v;
    }

    float tu, tv;                      // ������ ���������� ���������
};


class CVector4
{
public:
    CVector4();                                        // �����������
    CVector4(float X, float Y, float Z);               // ������������� �����������
    CVector4(float X, float Y, float Z, float W);      // ����� ������������� �����������
    CVector4(const CVector4& v);                       // � ��� ����.

    void operator=(CVector4 v);                        // ������������� �������� =
    CVector4 operator-(CVector4 v);                    // ������������� �������� - 
    CVector4 operator+(CVector4 v);                    // ������������� �������� + 
    CVector4 operator*(CVector4 v);                    // ������������� �������� * 
    CVector4 operator/(CVector4 v);                    // ������������� �������� / 
    CVector4 operator+(float f);                       // ������������� �������� + 
    CVector4 operator-(float f);                       // ������������� �������� - 
    CVector4 operator*(float f);                       // ������������� �������� * 
    CVector4 operator/(float f);                       // ������������� �������� / 

    void operator +=(CVector4 v);                      // ������������� �������� +=
    void operator -=(CVector4 v);                      // ������������� �������� -=
    void operator *=(CVector4 v);                      // ������������� �������� *=
    void operator /=(CVector4 v);                      // ������������� �������� /=
    void operator +=(float f);                         // ������������� �������� +=
    void operator -=(float f);                         // ������������� �������� -=
    void operator *=(float f);                         // ������������� �������� *=
    void operator /=(float f);                         // ������������� �������� /=

    bool operator ==(CVector4 v);                      // ������������� �������� ==
    bool operator !=(CVector4 v);                      // ������������� �������� !=

    void CrossProduct(CVector4 v1, CVector4 v2);       // ���������� Cross Product �������� v1 � v2.
    void CrossProduct3(CVector4 v1, CVector4 v2,
        CVector4 v3);                   // ���������� Cross Product �������� v1, v2, v3.
    float DotProduct3(CVector4 v1);                    // ��������� ������������ 3 �� v1
    float DotProduct4(CVector4 v1);                    // ��������� ������������ 4 �� v1
    float GetLength();                                 // ���������� ������ ������� �������
    void Normal();                                     // ����������� ������
    void Normalize(CVector4 Triangle[]);               // ������� ������� ������������
    void CalculateTangentVector(CVector4 Point1,
        CVector4 Point2,
        CVector4 Point3,
        CVector4 NormalOfA,
        CTexCoord P1Tex,
        CTexCoord P2Tex,
        CTexCoord P3Tex);      // ��������� tagent ������ � ��������� ���
    void ExtendVertexPos(CVector4 lightPos,            // ������� �����, ����������� �� ������� ���������
        float Extend);                // ����� � ������� �����, ��������� �����.
    void ExtendVertexPos(CVector4 currentVertex,       // 
        CVector4 lightPos,            // 
        float Extend);
    CVector4 GetRotatedX(double angle);                // �������� ������ x.
    CVector4 GetRotatedY(double angle);                // �������� ������ y.
    CVector4 GetRotatedZ(double angle);                // �������� ������ z.
    CVector4 GetRotatedAxis(double angle, CVector4 axis);  // �������� ������ ������������ ���.
    void CalculateBinormalVector(CVector4 tangent,
        CVector4 normal);     // ��������� ���������.
    void ClampTo01();                                  // "����������" ���� ������ ����� 0 � 1.


    float x, y, z, w;                                  // ��������� x, y, � z ����������.
};

#endif
