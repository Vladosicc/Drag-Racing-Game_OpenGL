#include "DragOpponent.h"
#include <ctime>
#include "MyShaders.h"

double PvsevdoRandom(double Low, double Up)
{
	return Low + (Up - Low) * (static_cast<double>(rand()) / RAND_MAX);
}

double PvsevdoRandomSpeed(double speed)
{
	double dV = PvsevdoRandom(-0.005, 0.005); // Приращение скорости
	double newSpeed = 0;
	newSpeed = speed + dV; // Псевдорандомная скорость
	if (newSpeed >= 0.011) // Границы скорости
	{
		newSpeed = 0.011;
	}
	if (newSpeed <= 0.0099)
	{
		newSpeed = 0.0099;
	}
	return newSpeed;
}

inline void DragOpponent::W_ivent()
{
	CarPoint.SetCoordX(CarPoint.X() + Speed * cos(AngleZ * PI / 180) * (KeyWDuration / 5));
	CarPoint.SetCoordY(CarPoint.Y() + Speed * sin(AngleZ * PI / 180) * (KeyWDuration / 5));

	if (SelectedTransmissionNow != 0) //Если не нейтраль, то ускоряем
		KeyWDuration += TimeFrame * TurboCoef() * GearRatio();

	if (KeyWDuration > 5. * SelectedTransmissionNow && SelectedTransmissionNow != 0)
	{
		KeyWDuration = 5. * SelectedTransmissionNow;
	}
	if (SelectedTransmissionNow == -1)
	{
		KeyWDuration = 0;
	}

}

void DragOpponent::CheckPosition(double TimeLastFrame)
{
	double end = clock();
	TimeFrame = (end - TimeLastFrame) / CLOCKS_PER_SEC; // Время кадра
	if (TimeFrame == 0)
		TimeFrame = 0.001;
	// Если что то не так со временем скипаем
	if (TimeFrame > 2 || TimeFrame < 0)
		return;

	if (!start)
		return;

	//Speed = PvsevdoRandomSpeed(Speed);

	W_ivent();

	RevsNowGlob = RevNow();
	AutoTransmission = true;

	MaxTransmissionN = PvsevdoRandom(7300, 8000);

	if (AutoTransmission) // Реализация АКПП
	{
		if (RevsNowGlob > MaxTransmissionN && SelectedTransmissionNow != TransmissionCount && SelectedTransmissionNow != -1)
		{
			SelectedTransmissionNow = (SelectedTransmission)(SelectedTransmissionNow + 1);
		}

		if (RevsNowGlob < 4000 && SelectedTransmissionNow > 1)
		{
			SelectedTransmissionNow = (SelectedTransmission)(SelectedTransmissionNow - 1);
		}
	}
}

void DragOpponent::SetStart(bool flagStart)
{
	start = flagStart;
}

void DragOpponent::DrawCar()
{
	
	PointKolesL.setCoords(CarPoint.X() + 2.7 * cos(AngleZ * PI / 180) - 1.37 * sin(AngleZ * PI / 180), CarPoint.Y() + 1.37 * cos(AngleZ * PI / 180) + 2.7 * sin(AngleZ * PI / 180), CarPoint.Z() + 0.62);
	PointKolesR.setCoords(CarPoint.X() + 2.7 * cos(AngleZ * PI / 180) + 1.37 * sin(AngleZ * PI / 180), CarPoint.Y() - 1.37 * cos(AngleZ * PI / 180) + 2.7 * sin(AngleZ * PI / 180), CarPoint.Z() + 0.62);
	PointKolesLBack.setCoords(CarPoint.X() - 1.9 * cos(AngleZ * PI / 180) - 1.37 * sin(AngleZ * PI / 180), CarPoint.Y() + 1.37 * cos(AngleZ * PI / 180) - 1.9 * sin(AngleZ * PI / 180), CarPoint.Z() + 0.62);
	PointKolesRBack.setCoords(CarPoint.X() - 1.9 * cos(AngleZ * PI / 180) + 1.37 * sin(AngleZ * PI / 180), CarPoint.Y() - 1.37 * cos(AngleZ * PI / 180) - 1.9 * sin(AngleZ * PI / 180), CarPoint.Z() + 0.62);

	glPushMatrix();
	carTex->bindTexture();
	//Элементы управления
	glTranslated(CarPoint.X(), CarPoint.Y(), CarPoint.Z());
	glRotated(-90, 0, 0, 1);
	glRotated(90, 1, 0, 0);
	glRotated(180, 0, 1, 0);
	glRotated(AngleZ, 0, 1, 0);
	Car->DrawObj();
	glPopMatrix();

	Shader::DontUseShaders();

	AngleWheelNow += KeyWDuration * Speed * 40;
	if (((long long)AngleWheelNow % 360 > -1 || (long long)AngleWheelNow % 360 < 1) && (AngleWheelNow > 2160 || AngleWheelNow < -2160))
		AngleWheelNow = 0;
	glPushMatrix();
	WheelLeftTex->bindTexture();
	glTranslated(PointKolesL.X(), PointKolesL.Y(), PointKolesL.Z());
	glRotated(-90, 0, 0, 1);
	glRotated(90, 1, 0, 0);
	glRotated(180, 0, 1, 0);
	glRotated(AngleZWheel, 0, 1, 0);
	glRotated(AngleWheelNow, 1, 0, 0); //вращение вокруг оси
	KolesL->DrawObj();
	glPopMatrix();

	glPushMatrix();
	glTranslated(PointKolesR.X(), PointKolesR.Y(), PointKolesR.Z());
	glRotated(-90, 0, 0, 1);
	glRotated(90, 1, 0, 0);
	glRotated(180, 0, 1, 0);
	glRotated(AngleZWheel, 0, 1, 0);
	glRotated(AngleWheelNow, 1, 0, 0); //вращение вокруг оси
	KolesR->DrawObj();
	glPopMatrix();

	glPushMatrix();
	glTranslated(PointKolesLBack.X(), PointKolesLBack.Y(), PointKolesLBack.Z());
	glRotated(-90, 0, 0, 1);
	glRotated(90, 1, 0, 0);
	glRotated(180, 0, 1, 0);
	glRotated(AngleZ, 0, 1, 0);
	glRotated(AngleWheelNow, 1, 0, 0); //вращение вокруг оси
	KolesL->DrawObj();
	glPopMatrix();

	glPushMatrix();
	glTranslated(PointKolesRBack.X(), PointKolesRBack.Y(), PointKolesRBack.Z());
	glRotated(-90, 0, 0, 1);
	glRotated(90, 1, 0, 0);
	glRotated(180, 0, 1, 0);
	glRotated(AngleZ, 0, 1, 0);
	glRotated(AngleWheelNow, 1, 0, 0); //вращение вокруг оси
	KolesR->DrawObj();
	glPopMatrix();
}
