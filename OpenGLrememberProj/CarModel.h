#ifndef CAR_H
#define CAR_H

#include "MyVector3d.h"
#include "ObjLoader.h"
#include "Texture.h"

enum SelectedTransmission
{
	Reverse = -1,
	Neutral,
	First,
	Second,
	Third,
	Fourth,
	Fifth,
	Sixth,
	Seventh,
};

class CarModel
{
protected:

	//������� �������
	Vector3 CarPoint;

	//������ �����
	Vector3 PointKolesL;
	Vector3 PointKolesR;
	Vector3 PointKolesLBack;
	Vector3 PointKolesRBack;

	//������������ ���� �������� �����
	double MaxAngleWheel = 30;

	//���� �������� �����
	double AngleZWheel = 0;

	//���� �������� ������� ������������ OZ
	double AngleZ = 0;

	//�������� ������� "�����"
	double Speed = 0.1;

	//�������� �������� ����� (��������� ������� ������ �������� || �������� ����������)
	double AngleSpeed = 4;

	//������� ������ ������������ ����� ���
	double AngleWheelNow = 0;

	//�������� ����������� ����� � ������ ���������
	double AngleSpeedReverse = 0.5;

	//������ ������ ��� ��������
	double HeightFromObj = 4;

	//"����" ���������� (<< 1)
	double RateSlow = 0.1;

	//������ ������� W (��� S ���� � ����� ������). ��������� ���������
	double KeyWDuration = 0;

	//��������� ��������
	SelectedTransmission SelectedTransmissionNow = SelectedTransmission::First;

	//������� ������� � �������
	int TransmissionCount = 6;

	//������� ��������� ������
	double RevsNowGlob = 800;

	//������� (��������)
	double MaxN1 = 8000;

	//����, ���� �� ������ ������
	bool flagAcceleration = false;

	//���� ����������
	bool flagDeceleration = false;

	//���� �� ������ ����� ����������
	bool flagAccelerationAfterDec = false;

	//���������� �� ������ ��������� �� �����
	double dist_x_right_wheel_front = 2.7;
	double dist_y_right_wheel_front = 1.37;
	double dist_x_left_wheel_front = 2.7;
	double dist_y_left_wheel_front = 1.37;

	double dist_x_right_wheel_rear = 1.9;
	double dist_y_right_wheel_rear = 1.37;
	double dist_x_left_wheel_rear = 1.9;
	double dist_y_left_wheel_rear = 1.37;


public:
	//������ ��� �����
	ObjFile *Car;

	//������ ������ � �����
	ObjFile *KolesR, *KolesL;

	//���������
	Texture *carTex, *WheelLeftTex;

	//���� ���������� ������������ ��������
	bool SelectTransmissionEnable = true;

	//���� ��������� ��������
	bool AutoTransmission = false;

	//���� ������� ��� (��������� ������)
	bool freeCam = false;

	//Time Frame
	double TimeFrame = 0;

	//������������ ������ �� �������
	double DistanceFromObj = 10;

protected:
	//������
	inline void W_ivent();
	inline void S_ivent();
	inline void A_ivent();
	inline void D_ivent();
	inline void NoneKey_ivent();
	//���� ��������� ��������� ������� ������
	inline double RevNow();
	inline double TurboCoef();
	inline double GearRatio();
	inline void KeyW_ivent_slowDown();
	//��������� ������ public ������ ���� �� ��� Render'a


public:
	//�������� �������� ������
	void CheckPosition(double TimeLastFrame);
	//���������� �����
	void DrawShadow(Vector3 lightPos);
	//���������� ��������� ������ ����� ������
	Vector3 LookAtCar();
	//��������� ������
	void DrawCar();
	//������� ��������� ������
	inline void setRightFrontWheel(double x, double y);
	inline void setLeftFrontWheel(double x, double y);
	inline void setRightRearWheel(double x, double y);
	inline void setLeftRearWheel(double x, double y);
	//�������� ����������
	int ReturnSelectedGear();
	Vector3 ReturnPosition();
	double ReturnRevNow();
	double ReturnSpeed();
	double ReturnAngleZ();
	void SetPoint(double x, double y, double z);
	void SetPoint(Vector3 vec);
	void SetAngleZ(double angle);
	void SetKeyWDuration(double k);
	void SetGearNow(int n);
};

#endif