#ifndef _3DMATH_H
#define _3DMATH_H


#include <float.h>	// Чтобы использовать _isnan() для acos()

#define PI 3.1415926535897932		// Наш знаменитый ПИ %)


// Эта структура используется для хранения 3д точек и векторов.
struct CVector3
{
public:
    float x, y, z;
};

// Возвращает вектор, перпендикулярный двум переданным векторам (плоскости)
CVector3 Cross(CVector3 vVector1, CVector3 vVector2);

// Возвращает вектор между 2мя точками.
CVector3 Vector(CVector3 vPoint1, CVector3 vPoint2);

// Возвращает величину нормали или любого другого вектора
float Magnitude(CVector3 vNormal);

// Возвращает нормализованный вектор (его длинна становится равной 1)
CVector3 Normalize(CVector3 vNormal);

// Возвращает нормаль полигона (направление, куда повернут полигон)
CVector3 Normal(CVector3 vTriangle[]);

// Возвращает расстояние плоскости от начала координат (0,0,0).
// Принимает нормаль к плоскости и ЛЮБУЮ точку, лежащую на этой плоскости:
float PlaneDistance(CVector3 Normal, CVector3 Point);

// Принимает треугольник (плоскость) и линию, и возвращает true при их пересечении.
bool IntersectedPlane(CVector3 vPoly[], CVector3 vLine[], CVector3& vNormal, float& originDistance);



// Возвращяет dot product между двумя векторами
float Dot(CVector3 vVector1, CVector3 vVector2);

// Возвращает угол между двумя векторами
double AngleBetweenVectors(CVector3 Vector1, CVector3 Vector2);

// Возвращает точку пересечения полигона и линии (подразумевается пересечение плоскости)
CVector3 IntersectionPoint(CVector3 vNormal, CVector3 vLine[], double distance);

// Возвращает true если точка пересечения находится внутри полигона
bool InsidePolygon(CVector3 vIntersection, CVector3 Poly[], long verticeCount);

// Используйте эту функцию для теста пересечения линии и полигона
bool IntersectedPolygon(CVector3 vPoly[], CVector3 vLine[], int verticeCount);



#endif
