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

	//Позиция объекта
	Vector3 CarPoint;

	//Поинты колес
	Vector3 PointKolesL;
	Vector3 PointKolesR;
	Vector3 PointKolesLBack;
	Vector3 PointKolesRBack;

	//Максимальный угол поворота колес
	double MaxAngleWheel = 30;

	//Угол поворота колес
	double AngleZWheel = 0;

	//Угол поворота объекта относительно OZ
	double AngleZ = 0;

	//Скорость объекта "прямо"
	double Speed = 0.1;

	//Скорость поворота колес (Насколько большой радиус поворота || Резкость управления)
	double AngleSpeed = 4;

	//Поворот колеса относительно своей оси
	double AngleWheelNow = 0;

	//Скорость возвращения колес в прямое положение
	double AngleSpeedReverse = 0.5;

	//Высота камеры над объектом
	double HeightFromObj = 4;

	//"Сила" торможения (<< 1)
	double RateSlow = 0.1;

	//Период нажатия W (или S если в минус уходит). Симуляция ускорения
	double KeyWDuration = 0;

	//Выбранная передача
	SelectedTransmission SelectedTransmissionNow = SelectedTransmission::First;

	//Сколько передач в коробке
	int TransmissionCount = 6;

	//Обороты двигателя сейчас
	double RevsNowGlob = 800;

	//Отсечка (Оборотов)
	double MaxN1 = 8000;

	//Флаг, идет ли сейчас разгон
	bool flagAcceleration = false;

	//Флаг торможения
	bool flagDeceleration = false;

	//Флаг на разгон после торможения
	bool flagAccelerationAfterDec = false;

	//Расстояние от центра координат до колес
	double dist_x_right_wheel_front = 2.7;
	double dist_y_right_wheel_front = 1.37;
	double dist_x_left_wheel_front = 2.7;
	double dist_y_left_wheel_front = 1.37;

	double dist_x_right_wheel_rear = 1.9;
	double dist_y_right_wheel_rear = 1.37;
	double dist_x_left_wheel_rear = 1.9;
	double dist_y_left_wheel_rear = 1.37;


public:
	//Машина без колес
	ObjFile *Car;

	//Колесо правое и левое
	ObjFile *KolesR, *KolesL;

	//Текстурки
	Texture *carTex, *WheelLeftTex;

	//Флаг разрешения переключения передачи
	bool SelectTransmissionEnable = true;

	//Флаг включения автомата
	bool AutoTransmission = false;

	//Флаг зажатия ПКМ (Свободной камеры)
	bool freeCam = false;

	//Time Frame
	double TimeFrame = 0;

	//Отдаленность камеры от объекта
	double DistanceFromObj = 10;

protected:
	//Ивенты
	inline void W_ivent();
	inline void S_ivent();
	inline void A_ivent();
	inline void D_ivent();
	inline void NoneKey_ivent();
	//Типа симуляция некоторых свойств машины
	inline double RevNow();
	inline double TurboCoef();
	inline double GearRatio();
	inline void KeyW_ivent_slowDown();
	//Обработка других public флагов идет из под Render'a


public:
	//Проверка смещения машины
	void CheckPosition(double TimeLastFrame);
	//Прорисовка теней
	void DrawShadow(Vector3 lightPos);
	//Возвращает положение камеры сзади машины
	Vector3 LookAtCar();
	//Отрисовка машины
	void DrawCar();
	//Функции подвинуть колеса
	inline void setRightFrontWheel(double x, double y);
	inline void setLeftFrontWheel(double x, double y);
	inline void setRightRearWheel(double x, double y);
	inline void setLeftRearWheel(double x, double y);
	//Получить переменные
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