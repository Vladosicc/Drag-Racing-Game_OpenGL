#ifndef Collision_H
#define Collision_H

#include "MyVector3d.h"

struct Cube
{
public:
	Vector3 vec[8];

	Vector3& operator[](int i)
	{
		return vec[i];
	}

	void DrawCube();
};

/*	
	*(0)  ----------------- *(1)  
	|						|
	|						|
	|						|
	|		���				|
	|						|
	|						|
	|						|
	|						|
	*(3)  ----------------- *(2)

	*(4)  ----------------- *(5)
	|						|
	|						|
	|						|
	|		����			|
	|						|
	|						|
	|						|
	|						|
	*(7)  ----------------- *(6)
*/

class Collision
{
public:
	Cube Coll;

public:
	//���������� ��������� �������� (8 �����)
	bool AddCollision(Vector3 vec[8]);
	//���������� ��������� �������� (8 �����)
	bool AddCollision(double a[3], double b[3], double c[3], double d[3], double a1[3], double b1[3], double c1[3], double d1[3]);
	//���������� ��������� �������� (8 �����)
	bool AddCollision(Vector3 a, Vector3 b, Vector3 c, Vector3 d, Vector3 a1, Vector3 b1, Vector3 c1, Vector3 d1);
	//���������� ��������� �������� (2 ����� - 0 � 6)
	bool AddCollision2(Vector3 vec[2]);

	//���������� �������� �� obj
	bool AddCollisionFromObj(); //���� ������ �������, �����������

	//������ �������� �������
	void DrawCollision();
};

class DynamicCollision : public Collision
{
public:
	Cube DynamicCol;

	void UpdateDynamicCollision(Vector3 pos, double AngleZ);
	//������ ������������ � ����������� ��������
	bool CheckWithStaticCollision(Collision& Col);
	//������ ��� ������������ �������� (�� �����������)
	void CheckWithDynamicCollision(const DynamicCollision& Col);

	//������ �������� �������
	void DrawCollision();
};

#endif