#include "Render.h"

#include <windows.h>
#include <iostream>
#include <sstream>
#include <ctime>

#include <GL\gl.h>
#include <GL\glu.h>
#include "GL\glext.h"

#include "MyOGL.h"

#include "CMatrix.h"
#include "CVector.h"

#include "Camera.h"
#include "Light.h"
#include "Primitives.h"

#include "MyShaders.h"

#include "ObjLoader.h"
#include "GUItextRectangle.h"

#include "Texture.h"

#include "CarModel.h"
#include "DragOpponent.h"
#include "Collision.h"
#include "ListCollision.h"

GuiTextRectangle rec;

bool textureMode = true;
bool lightMode = true;
bool shadow = false;
bool SettingCamera = false;
bool FirstStart = true;
bool DragTrigger = false;
bool DragIventStartFlag = false;
bool RaceIsStartedFlag = false;
bool FalseStartFlag = false;
bool LeaveBorderTrack = false;
bool SuccsessFinish = false;
bool YouWon = false;
bool CollisionVisible = false;


//небольшой дефайн для упрощения кода
#define POP glPopMatrix()
#define PUSH glPushMatrix()


ObjFile *model;

Texture texture1;
Texture sTex;
Texture rTex;
Texture tBox;

Shader s[10];  //массивчик для десяти шейдеров
Shader frac;
Shader cassini;

//Высота экрана
int WindowHeight;

//Ширина экрана
int WindowWidht;

CarModel car;
DragOpponent dragOp;
ListCollisions listCol; //Все коллизии

//класс для настройки камеры
class CustomCamera : public Camera
{
public:
	//дистанция камеры
	double camDist;
	//углы поворота камеры
	double fi1, fi2;

	Vector3 cameraFront;

	//значния масеры по умолчанию
	CustomCamera()
	{
		camDist = 15;
		fi1 = 1;
		fi2 = 1;
	}


	//считает позицию камеры, исходя из углов поворота, вызывается движком
	void SetUpCamera()
	{
		//отвечает за поворот камеры мышкой
		lookPoint.setCoords(car.ReturnPosition().X(), car.ReturnPosition().Y(), car.ReturnPosition().Z());

		if (car.freeCam)
		{
			camera.pos.setCoords(car.ReturnPosition().X() + camera.camDist * cos(camera.fi2) * cos(camera.fi1),
				car.ReturnPosition().Y() + camera.camDist * cos(camera.fi2) * sin(camera.fi1),
				car.ReturnPosition().Z() + camera.camDist * sin(camera.fi2));
		}

		if (cos(fi2) <= 0)
			normal.setCoords(0, 0, -1);
		else
			normal.setCoords(0, 0, 1);

		LookAt();
	}

	void CustomCamera::LookAt()
	{
		//функция настройки камеры
		gluLookAt(pos.X(), pos.Y(), pos.Z(), lookPoint.X(), lookPoint.Y(), lookPoint.Z(), normal.X(), normal.Y(), normal.Z());
	}

	void SetLookPoint(double x, double y, double z)
	{
		lookPoint.setCoords(x, y, z);
	}

	void SetLookPoint(Vector3 vec)
	{
		lookPoint = vec;
	}

}  camera;   //создаем объект камеры

//Класс для настройки света
class CustomLight : public Light
{
public:
	CustomLight()
	{
		//начальная позиция света
		pos = Vector3(1, 1, 3);
	}

	
	//рисует сферу и линии под источником света, вызывается движком
	void  DrawLightGhismo()
	{
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);
		Shader::DontUseShaders();
		bool f1 = glIsEnabled(GL_LIGHTING);
		glDisable(GL_LIGHTING);
		bool f2 = glIsEnabled(GL_TEXTURE_2D);
		glDisable(GL_TEXTURE_2D);
		bool f3 = glIsEnabled(GL_DEPTH_TEST);
		
		glDisable(GL_DEPTH_TEST);
		glColor3d(0.9, 0.8, 0);
		Sphere s;
		s.pos = pos;
		s.scale = s.scale*0.08;
		s.Show();

		if (OpenGL::isKeyPressed('G'))
		{
			glColor3d(0, 0, 0);
			//линия от источника света до окружности
				glBegin(GL_LINES);
			glVertex3d(pos.X(), pos.Y(), pos.Z());
			glVertex3d(pos.X(), pos.Y(), 0);
			glEnd();

			//рисуем окруность
			Circle c;
			c.pos.setCoords(pos.X(), pos.Y(), 0);
			c.scale = c.scale*1.5;
			c.Show();
		}
		/*
		if (f1)
			glEnable(GL_LIGHTING);
		if (f2)
			glEnable(GL_TEXTURE_2D);
		if (f3)
			glEnable(GL_DEPTH_TEST);
			*/
	}

	void SetUpLight()
	{
		GLfloat amb[] = { 0.2, 0.2, 0.2, 0 };
		GLfloat dif[] = { 1.0, 1.0, 1.0, 0 };
		GLfloat spec[] = { .7, .7, .7, 0 };
		GLfloat position[] = { pos.X(), pos.Y(), pos.Z(), 1. };

		// параметры источника света
		glLightfv(GL_LIGHT0, GL_POSITION, position);
		// характеристики излучаемого света
		// фоновое освещение (рассеянный свет)
		glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
		// диффузная составляющая света
		glLightfv(GL_LIGHT0, GL_DIFFUSE, dif);
		// зеркально отражаемая составляющая света
		glLightfv(GL_LIGHT0, GL_SPECULAR, spec);

		glEnable(GL_LIGHT0);
	}


} light;  //создаем источник света

//старые координаты мыши
int mouseX = 0, mouseY = 0;

//обработчик движения мыши
void mouseEvent(OpenGL *ogl, int mX, int mY)
{
	int dx = mouseX - mX;
	int dy = mouseY - mY;
	mouseX = mX;
	mouseY = mY;

	//меняем углы камеры при нажатой левой кнопке мыши
	if (OpenGL::isKeyPressed(VK_RBUTTON))
	{
		camera.fi1 += 0.005 * dx;
		camera.fi2 += -0.005 * dy;
		camera.pos.setCoords(car.ReturnPosition().X() + camera.camDist * cos(camera.fi2) * cos(camera.fi1),
			car.ReturnPosition().Y() + camera.camDist * cos(camera.fi2) * sin(camera.fi1),
			car.ReturnPosition().Z() + camera.camDist * sin(camera.fi2));
		//camera.camDist = 10;
		car.freeCam = true;
		camera.SetLookPoint(car.ReturnPosition());

	}
	
	//двигаем свет по плоскости, в точку где мышь
	if (OpenGL::isKeyPressed('G') && !OpenGL::isKeyPressed(VK_LBUTTON))
	{
		LPPOINT POINT = new tagPOINT();
		GetCursorPos(POINT);
		ScreenToClient(ogl->getHwnd(), POINT);
		POINT->y = ogl->getHeight() - POINT->y;

		Ray r = camera.getLookRay(POINT->x, POINT->y,60,ogl->aspect);

		double z = light.pos.Z();

		double k = 0, x = 0, y = 0;
		if (r.direction.Z() == 0)
			k = 0;
		else
			k = (z - r.origin.Z()) / r.direction.Z();

		x = k*r.direction.X() + r.origin.X();
		y = k*r.direction.Y() + r.origin.Y();

		light.pos = Vector3(x, y, z);
	}

	if (OpenGL::isKeyPressed('G') && OpenGL::isKeyPressed(VK_LBUTTON))
	{
		light.pos = light.pos + Vector3(0, 0, 0.02*dy);
	}

	
}

//обработчик вращения колеса  мыши
void mouseWheelEvent(OpenGL *ogl, int delta)
{
	//Тут просто скролл при вращении камерой
	if (OpenGL::isKeyPressed(VK_RBUTTON))
	{
		camera.camDist += 0.005 * delta;
		camera.pos.setCoords(car.ReturnPosition().X() + camera.camDist * cos(camera.fi2) * cos(camera.fi1),
			car.ReturnPosition().Y() + camera.camDist * cos(camera.fi2) * sin(camera.fi1),
			car.ReturnPosition().Z() + camera.camDist * sin(camera.fi2));
		camera.SetLookPoint(car.ReturnPosition());
	}
	//Тут скролл приближает и отдаляет камеру от объекта
	else
	{
		if (car.DistanceFromObj + 0.005 * delta < 2)
			return;
		car.DistanceFromObj += 0.005 * delta;
		Vector3 tmp{
				car.ReturnPosition().X() - car.DistanceFromObj * cos(car.ReturnAngleZ() * PI / 180),
				car.ReturnPosition().Y() - car.DistanceFromObj * sin(car.ReturnAngleZ() * PI / 180),
				car.ReturnPosition().Z() + 2
		};
		camera.pos = tmp;
		//camera.camDist = 30;
		camera.SetLookPoint(car.ReturnPosition());
	}
}

//обработчик нажатия кнопок клавиатуры
void keyDownEvent(OpenGL *ogl, int key)
{
	if (OpenGL::isKeyPressed('L'))
	{
		lightMode = !lightMode;
	} 

	if (OpenGL::isKeyPressed('R'))
	{
		camera.fi1 = 1;
		camera.fi2 = 1;
		camera.camDist = 15;

		light.pos = Vector3(1, 1, 3);
	}

	if (OpenGL::isKeyPressed('F'))
	{
		light.pos = camera.pos;
	}

	if (OpenGL::isKeyPressed('U'))
	{
		shadow = !shadow;
	}

	if (OpenGL::isKeyPressed('C'))
	{
		CollisionVisible = !CollisionVisible;
	}

	if (OpenGL::isKeyPressed('G'))
	{
		car.AutoTransmission = !(car.AutoTransmission);
	}

	if (OpenGL::isKeyPressed('Z') && (LeaveBorderTrack || SuccsessFinish || FalseStartFlag))
	{
		FalseStartFlag = false;
		LeaveBorderTrack = false;
		DragIventStartFlag = true;
		SuccsessFinish = false;
		YouWon = false;
	}

	if (OpenGL::isKeyPressed(13) && DragIventStartFlag)
	{
		RaceIsStartedFlag = true;
	}

	if (OpenGL::isKeyPressed(13) && DragTrigger)
	{
		FalseStartFlag = false;
		LeaveBorderTrack = false;
		SuccsessFinish = false;
		DragIventStartFlag = true;
	}

	if (OpenGL::isKeyPressed('Q') && DragIventStartFlag)
	{
		DragIventStartFlag = false;
	}

	if (key == 'V')
	{
		car.freeCam = false;
		Vector3 tmp{
				car.ReturnPosition().X() - car.DistanceFromObj * cos(car.ReturnAngleZ() * PI / 180),
				car.ReturnPosition().Y() - car.DistanceFromObj * sin(car.ReturnAngleZ() * PI / 180),
				car.ReturnPosition().Z() + 2
		};
		camera.pos = tmp;
		camera.camDist = 30;
		camera.SetLookPoint(car.ReturnPosition());
	}

}

void keyUpEvent(OpenGL *ogl, int key)
{

	if (key == 17 || key == 16)
	{
		car.SelectTransmissionEnable = true;
	}
}

ObjFile Car, KolesR, KolesL, KolesPered;
DynamicCollision CarCol;
Collision SvetoforCol, OtboinikCol;
Texture carTex, WheelLeftTex;

ObjFile Svetofor, Otboinik, StartFrame;
Texture SvetoforTexNonreary, SvetoforTexOponentReady, SvetoforTexRaceReady, SvetoforTexReady1, SvetoforTexReady2, SvetoforTexReadyGo, SvetoforTexFalse; //Набор событий светофора
Texture OtboinikTex, StartFrameTex;

//выполняется перед первым рендером
void initRender(OpenGL *ogl)
{

	//настройка текстур

	//4 байта на хранение пикселя
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	//настройка режима наложения текстур
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	//включаем текстуры
	glEnable(GL_TEXTURE_2D);
	
	


	//камеру и свет привязываем к "движку"
	ogl->mainCamera = &camera;
	//ogl->mainCamera = &WASDcam;
	ogl->mainLight = &light;

	// нормализация нормалей : их длины будет равна 1
	glEnable(GL_NORMALIZE);

	// устранение ступенчатости для линий
	glEnable(GL_LINE_SMOOTH); 


	//   задать параметры освещения
	//  параметр GL_LIGHT_MODEL_TWO_SIDE - 
	//                0 -  лицевые и изнаночные рисуются одинаково(по умолчанию), 
	//                1 - лицевые и изнаночные обрабатываются разными режимами       
	//                соответственно лицевым и изнаночным свойствам материалов.    
	//  параметр GL_LIGHT_MODEL_AMBIENT - задать фоновое освещение, 
	//                не зависящее от сточников
	// по умолчанию (0.2, 0.2, 0.2, 1.0)

	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 0);

	 //так как гит игнорит модели *.obj файлы, так как они совпадают по расширению с объектными файлами, 
	 // создающимися во время компиляции, я переименовал модели в *.obj_m

	frac.VshaderFileName = "shaders\\v.vert"; //имя файла вершинного шейдер
	frac.FshaderFileName = "shaders\\frac.frag"; //имя файла фрагментного шейдера
	frac.LoadShaderFromFile(); //загружаем шейдеры из файла
	frac.Compile(); //компилируем

	cassini.VshaderFileName = "shaders\\v.vert"; //имя файла вершинного шейдер
	cassini.FshaderFileName = "shaders\\cassini.frag"; //имя файла фрагментного шейдера
	cassini.LoadShaderFromFile(); //загружаем шейдеры из файла
	cassini.Compile(); //компилируем


	s[0].VshaderFileName = "shaders\\v.vert"; //имя файла вершинного шейдер
	s[0].FshaderFileName = "shaders\\light.frag"; //имя файла фрагментного шейдера
	s[0].LoadShaderFromFile(); //загружаем шейдеры из файла
	s[0].Compile(); //компилируем

	s[1].VshaderFileName = "shaders\\v.vert"; //имя файла вершинного шейдер
	s[1].FshaderFileName = "shaders\\textureShader.frag"; //имя файла фрагментного шейдера
	s[1].LoadShaderFromFile(); //загружаем шейдеры из файла
	s[1].Compile(); //компилируем

	glActiveTexture(GL_TEXTURE0);
	loadModel("models\\CarUV.obj_m", &Car);
	carTex.loadTextureFromFile("textures//CarTex.bmp");

	glActiveTexture(GL_TEXTURE0);
	loadModel("models\\WheelRight.obj_m", &KolesR);

	glActiveTexture(GL_TEXTURE0);
	loadModel("models\\WheelLeft.obj_m", &KolesL);
	WheelLeftTex.loadTextureFromFile("textures//WheelLeft.bmp");

	car.Car = &Car;
	car.KolesL = &KolesL;
	car.KolesR = &KolesR;
	car.carTex = &carTex;
	car.WheelLeftTex = &WheelLeftTex;

	dragOp.Car = &Car;
	dragOp.KolesL = &KolesL;
	dragOp.KolesR = &KolesR;
	dragOp.carTex = &carTex;
	dragOp.WheelLeftTex = &WheelLeftTex;

	glActiveTexture(GL_TEXTURE0);
	loadModel("models\\Svetofor.obj_m", &Svetofor);
	SvetoforTexNonreary.loadTextureFromFile("textures//SvetoforTexNonreary.bmp");
	SvetoforTexOponentReady.loadTextureFromFile("textures//SvetoforTexOponentReady.bmp");
	SvetoforTexRaceReady.loadTextureFromFile("textures//SvetoforTexRaceReady.bmp");
	SvetoforTexReady1.loadTextureFromFile("textures//SvetoforTexReady1.bmp");
	SvetoforTexReadyGo.loadTextureFromFile("textures/SvetoforTexReadyGo.bmp");
	SvetoforTexReady2.loadTextureFromFile("textures//SvetoforTexReady2.bmp");
	SvetoforTexFalse.loadTextureFromFile("textures//SvetoforTexFalse.bmp");

	glActiveTexture(GL_TEXTURE0);
	loadModel("models\\otboinik.obj_m", &Otboinik);
	OtboinikTex.loadTextureFromFile("textures//Otboinik.bmp");

	glActiveTexture(GL_TEXTURE0);
	loadModel("models\\StartFrame.obj_m", &StartFrame);
	StartFrameTex.loadTextureFromFile("textures//FrameStart.bmp");
}

//Включает режим рисования теней (Все объекты, рисуемые после этой функции, будут рисоваться как тени)
CMatrix4x4 ReturnShadowMatrix()
{
	// Эта матрица будет использоватся для хранения матрицы тени
	CMatrix4x4 ShadowMatrix;

	//Первая - позиция источника света, вторая - нормаль поверхности, на которую отбрасывается тень.
	CVector4 lightPos(light.pos.X(), light.pos.Y(), light.pos.Z(), 1.0f);
	CVector4 planeNormal(0.0f, 0.0f, 1.0f, 0.0f);

	// Создадим матрицу тени на основе нормали поверхности и позиции источника света
	ShadowMatrix.CreateShadowMatrix(planeNormal, lightPos);

	return ShadowMatrix;
}

void SetLightSun()
{
	light.pos = car.ReturnPosition();
	light.pos.SetCoordZ(30);
}

double AngleCam = 270;
double AngleCamHeight = 90;

void WASD_cam()
{
	double AngleCamSpeed = 2;
	static double r = 5;
	static double R = 10;
	if (OpenGL::isKeyPressed('D'))
	{
		AngleCam += AngleCamSpeed;
		camera.pos.SetCoordX(car.ReturnPosition().X() + R * cos(AngleCam * PI / 180));
		camera.pos.SetCoordY(car.ReturnPosition().Y() + r * sin(AngleCam * PI / 180));
		camera.pos.SetCoordZ((car.ReturnPosition().Z() + 1) + R * sin(AngleCamHeight * PI / 180));
		camera.SetLookPoint(car.ReturnPosition());
	}

	if (OpenGL::isKeyPressed('A'))
	{
		AngleCam -= AngleCamSpeed;
		camera.pos.SetCoordX(car.ReturnPosition().X() + R * cos(AngleCam * PI / 180));
		camera.pos.SetCoordY(car.ReturnPosition().Y() + r * sin(AngleCam * PI / 180));
		camera.pos.SetCoordZ((car.ReturnPosition().Z() + 1) + R * sin(AngleCamHeight * PI / 180));
		camera.SetLookPoint(car.ReturnPosition());
	}

	if (OpenGL::isKeyPressed('W'))
	{
		R -= 0.05;
		r -= 0.05;
		camera.pos.SetCoordX(car.ReturnPosition().X() + R * cos(AngleCam * PI / 180));
		camera.pos.SetCoordY(car.ReturnPosition().Y() + r * sin(AngleCam * PI / 180));
		camera.pos.SetCoordZ((car.ReturnPosition().Z() + 1) + R * sin(AngleCamHeight * PI / 180));
		camera.SetLookPoint(car.ReturnPosition());
	}

	if (OpenGL::isKeyPressed('S'))
	{
		R += 0.05;
		r += 0.05;
		camera.pos.SetCoordX(car.ReturnPosition().X() + R * cos(AngleCam * PI / 180));
		camera.pos.SetCoordY(car.ReturnPosition().Y() + r * sin(AngleCam * PI / 180));
		camera.pos.SetCoordZ((car.ReturnPosition().Z() + 1) + R * sin(AngleCamHeight * PI / 180));
		camera.SetLookPoint(car.ReturnPosition());
	}

	if (OpenGL::isKeyPressed(16))
	{
		if (AngleCamHeight + AngleCamSpeed > 180)
		{
			AngleCamHeight = 180;
		}
		else
		{
			AngleCamHeight += AngleCamSpeed;
		}
		camera.pos.SetCoordX(car.ReturnPosition().X() + R * cos(AngleCam * PI / 180));
		camera.pos.SetCoordY(car.ReturnPosition().Y() + r * sin(AngleCam * PI / 180));
		camera.pos.SetCoordZ((car.ReturnPosition().Z() + 1) + R * sin(AngleCamHeight * PI / 180));
		camera.SetLookPoint(car.ReturnPosition());
	}

	if (OpenGL::isKeyPressed(17))
	{
		if (AngleCamHeight - AngleCamSpeed < 0)
		{
			AngleCamHeight = 0;
		}
		else
		{
			AngleCamHeight -= AngleCamSpeed;
		}
		camera.pos.SetCoordX(car.ReturnPosition().X() + R * cos(AngleCam * PI / 180));
		camera.pos.SetCoordY(car.ReturnPosition().Y() + r * sin(AngleCam * PI / 180));
		camera.pos.SetCoordZ((car.ReturnPosition().Z() + 1) + R * sin(AngleCamHeight * PI / 180));
		camera.SetLookPoint(car.ReturnPosition());
	}
}

double TimeLastFrame = 0;
double TimeFrame = 0;
double TimeStart = 3;

void StartFrameDraw()
{
	static bool flagReverse = false;
	static double TimeForStartFrame = 0;
	if (!flagReverse)
	{
		if (TimeForStartFrame + TimeFrame * 50 > 4)
		{
			TimeForStartFrame = 4;
			flagReverse = true;
		}
		else
		{
			TimeForStartFrame += TimeFrame * 50;
		}
	}
	else
	{
		if (TimeForStartFrame - TimeFrame * 50 < 0)
		{
			TimeForStartFrame = 0;
			flagReverse = false;
		}
		else
		{
			TimeForStartFrame -= TimeFrame * 50;
		}
	}
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	PUSH;
	glTranslated(6, 15, TimeForStartFrame);
	glRotated(-180, 0, 0, 1);
	StartFrameTex.bindTexture();
	StartFrame.DrawObj();
	POP;
}

void DragTriggerIv()
{
	bool flagX = false, flagY = false;

	if (car.ReturnPosition().X() > 2.3 && car.ReturnPosition().X() < 10.2)
	{
		flagX = true;
	}
	if (car.ReturnPosition().Y() > 11.3 && car.ReturnPosition().Y() < 15.1)
	{
		flagY = true;
	}

	if (flagX && flagY)
	{
		DragTrigger = true;
	}
	else
	{
		DragTrigger = false;
	}
}

double TimeBeforeRace = 5;

bool CheckBorderTrack()
{ 
	if (!RaceIsStartedFlag)
		return false;
	if (car.ReturnPosition().Y() > 15.6 || car.ReturnPosition().Y() < 9.7)
	{
		LeaveBorderTrack = true;
		return true;
	} 
	if (car.ReturnPosition().X() >= 300 || dragOp.ReturnPosition().X() >= 300)
	{
		SuccsessFinish = true;
		if (car.ReturnPosition().X() > dragOp.ReturnPosition().X())
		{
			YouWon = true;
		}
		else
		{
			YouWon = false;
		}
		return true;
	}
	return false;
}

double fora = -0.3; //Фора (противник стартует с задержкой)

void DragIvent()
{
	if (DragIventStartFlag)
	{
		car.SetPoint(6.25, 13.2, 0);
		car.SetAngleZ(0);
		car.SetKeyWDuration(0);
		car.SetGearNow(1);
		dragOp.SetPoint(6.25, 20.2, 0);
		dragOp.SetAngleZ(0);
		dragOp.SetKeyWDuration(0);
		car.SetGearNow(1);
		TimeBeforeRace = 5;
	}

	if (RaceIsStartedFlag && TimeBeforeRace > fora)
	{
		DragIventStartFlag = false;
		TimeBeforeRace -= TimeFrame * 40;
		if (OpenGL::isKeyPressed('W') && TimeBeforeRace >= 0.05)
		{
			RaceIsStartedFlag = false;
			DragIventStartFlag = false;
			FalseStartFlag = true; //
		}
		dragOp.SetStart(false);
	}
	else
	{
		if (CheckBorderTrack())
		{
			RaceIsStartedFlag = false;
			DragIventStartFlag = false;
			dragOp.SetStart(false);
			dragOp.SetPoint(6.25, 20.2, 0);
			dragOp.SetAngleZ(0);
			dragOp.SetKeyWDuration(0);
		}
		dragOp.SetStart(true);
	}
}

void SvetoforBindTex()
{
	if (DragIventStartFlag && !RaceIsStartedFlag)
	{
		SvetoforTexOponentReady.bindTexture();
		return;
	}
	if (RaceIsStartedFlag && TimeBeforeRace >= 2)
	{
		SvetoforTexRaceReady.bindTexture();
		return;
	}
	if (RaceIsStartedFlag && TimeBeforeRace < 2 && TimeBeforeRace >= 1)
	{
		SvetoforTexReady1.bindTexture();
		return;
	}
	if (RaceIsStartedFlag && TimeBeforeRace < 1 && TimeBeforeRace > 0)
	{
		SvetoforTexReady2.bindTexture();
		return;
	}
	if (RaceIsStartedFlag && TimeBeforeRace <= 0)
	{
		SvetoforTexReadyGo.bindTexture();
		return;
	}
	if (FalseStartFlag)
	{
		SvetoforTexFalse.bindTexture();
		return;
	}
	SvetoforTexNonreary.bindTexture();
}

void FirstFunc() //Первое выполнение
{
	CarCol.AddCollision(listCol.CarColRet());
	SvetoforCol.AddCollision2(listCol.SvetoforColRet2());
	OtboinikCol.AddCollision2(listCol.OtboinikColRet2());
	dragOp.SetStart(false);
	srand(time(NULL));
}

//Флаг пересечения коллизии
bool tmp = false;

//Для возврата Машины при столкновении
Vector3 PosOld;
double AngleOld;

//Задачи
//https://masandilov.ru/opengl/line-poly-collision Штука для коллизии +
//Реализовать систему столкновений (Не просто стопом)
//Объединить Vector3, CVector3
//ОПТИМИЗАЦИЯ (Соединение тескстур, Удаление лишнего в Car.cpp, Замена тысячи вызовов косинусов синусов одним, заредачить светофор)
void Render(OpenGL *ogl)
{  
	if (FirstStart)
	{
		FirstFunc();
		FirstStart = false;
	}

	PosOld = car.ReturnPosition();
	AngleOld = car.ReturnAngleZ();

	if(!DragIventStartFlag || RaceIsStartedFlag)
		car.CheckPosition(TimeLastFrame);
	if(RaceIsStartedFlag)
		dragOp.CheckPosition(TimeLastFrame);

	TimeLastFrame = clock();

	SetLightSun();

	//WASD_cam();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);

	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();

	glEnable(GL_DEPTH_TEST);
	if (textureMode)
		glEnable(GL_TEXTURE_2D);

	if (lightMode)
		glEnable(GL_LIGHTING);

	//альфаналожение
	glDisable(GL_BLEND);

	//настройка материала
	GLfloat amb[] = { 0.6, 0.6, 0.6, 1. };
	GLfloat dif[] = { 0.9, 0.9, 0.9, 1. };
	GLfloat spec[] = { 0.9, 0.9, 0.9, 1. };
	GLfloat sh = 0.1f * 256;

	//фоновая
	glMaterialfv(GL_FRONT, GL_AMBIENT, amb);
	//дифузная
	glMaterialfv(GL_FRONT, GL_DIFFUSE, dif);
	//зеркальная
	glMaterialfv(GL_FRONT, GL_SPECULAR, spec);
	//размер блика
	glMaterialf(GL_FRONT, GL_SHININESS, sh);

	//===================================
	//Прогать тут  

	Shader::DontUseShaders();

	tmp = false;
	CarCol.UpdateDynamicCollision(car.ReturnPosition(), car.ReturnAngleZ());
	if (CollisionVisible)
	{
		CarCol.DrawCollision();
		SvetoforCol.DrawCollision();
		OtboinikCol.DrawCollision();
	}
	tmp += CarCol.CheckWithStaticCollision(SvetoforCol);
	tmp += CarCol.CheckWithStaticCollision(OtboinikCol);

	if (tmp)
	{
		car.SetKeyWDuration(0);
		car.SetPoint(PosOld);
		car.SetAngleZ(AngleOld);
	}

	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glColor3d(0.8, 0.8, 0.8);
	glBegin(GL_QUADS);
	glVertex3d(400, 400, 0);
	glVertex3d(400, -400, 0);
	glVertex3d(-400, -400, 0);
	glVertex3d(-400, 400, 0);
	glEnd();
	glColor3d(0.6, 0.6, 0.6);
	glBegin(GL_QUADS);


	glVertex3d(400, 25, 0.01);
	glVertex3d(400, 10, 0.01);
	glVertex3d(0, 10, 0.01);
	glVertex3d(0, 25, 0.01);

	glColor3d(0.8, 0.8, 0.8);
	for (int i = 0; i <= 400; i += 5)
	{
		glVertex3d(i + 5, 18, 0.015);
		glVertex3d(i + 5, 17, 0.015);
		glVertex3d(i, 17, 0.015);
		glVertex3d(i, 18, 0.015);
		i += 5;
	}
	glEnd();

	glDisable(GL_TEXTURE_2D);
	if (shadow)
	{

		car.DrawShadow(light.pos);
		if (DragIventStartFlag || RaceIsStartedFlag)
			dragOp.DrawShadow(light.pos);
		CMatrix4x4 ShadowMatrix = ReturnShadowMatrix();
		//Рисуем тени
		PUSH;
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_LIGHTING);
		glDisable(GL_TEXTURE_2D);
		for (int i = 18; i <= 400; i += 8.6)
		{
			PUSH;
			glMultMatrixf(ShadowMatrix.matrix);
			glTranslated(i, 17.5, 0);
			glRotated(-90, 0, 0, 1);
			glRotated(90, 1, 0, 0);
			Otboinik.DrawObj();
			POP;
			PUSH;
			glMultMatrixf(ShadowMatrix.matrix);
			glTranslated(13, 17.5, 0);
			glRotated(-90, 0, 0, 1);
			glRotated(90, 1, 0, 0);
			Svetofor.DrawObj();
			POP;
		}
		//
		POP;

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LIGHTING);
	}
	glEnable(GL_TEXTURE_2D);

	OtboinikTex.bindTexture();
	for (int i = 18; i <= 400; i += 8.6)
	{
		PUSH;
		glTranslated(i, 17.5, 0);
		glRotated(-90, 0, 0, 1);
		glRotated(90, 1, 0, 0);
		Otboinik.DrawObj();
		POP;
	}

	if (!car.freeCam)
	{
		camera.pos = car.LookAtCar();
		camera.camDist = 30;
	}

	car.DrawCar();
	if(DragIventStartFlag || RaceIsStartedFlag)
		dragOp.DrawCar();

	PUSH;
	glTranslated(13, 17.5, 0);
	glRotated(-90, 0, 0, 1);
	glRotated(90, 1, 0, 0);
	SvetoforBindTex();
	Svetofor.DrawObj();
	POP;

	StartFrameDraw();

	DragTriggerIv();
	DragIvent();

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBegin(GL_QUADS);
	glColor4d(0.8, 0.8, 0.8, 0.5);

	glVertex3d(300, 25, 0.00);
	glVertex3d(300, 10, 0.00);
	glVertex3d(300, 10, 3.01);
	glVertex3d(300, 25, 3.01);

	glEnd();

	TimeFrame = (clock() - TimeLastFrame) / CLOCKS_PER_SEC; // Время кадра
	if (TimeFrame == 0)
		TimeFrame = 0.001;
}   //конец тела функции


bool gui_init = false;

//рисует интерфейс, вызывется после обычного рендера
void RenderGUI(OpenGL* ogl)
{

	Shader::DontUseShaders();

	glMatrixMode(GL_PROJECTION);	//Делаем активной матрицу проекций. 
									//(всек матричные операции, будут ее видоизменять.)
	glPushMatrix();   //сохраняем текущую матрицу проецирования (которая описывает перспективную проекцию) в стек 				    
	glLoadIdentity();	  //Загружаем единичную матрицу
	glOrtho(0, ogl->getWidth(), 0, ogl->getHeight(), 0, 1);	 //врубаем режим ортогональной проекции

	glMatrixMode(GL_MODELVIEW);		//переключаемся на модел-вью матрицу
	glPushMatrix();			  //сохраняем текущую матрицу в стек (положение камеры, фактически)
	glLoadIdentity();		  //сбрасываем ее в дефолт

	glDisable(GL_LIGHTING);



	GuiTextRectangle rec;
	rec.setSize(300, 250);
	rec.setPosition(10, ogl->getHeight() - 250 - 10);


	std::stringstream ss;
	ss << "C - Показать коллизии" << std::endl;
	ss << "U - Включить тени" << std::endl;
	ss << "Число оборотов = " << car.ReturnRevNow() << std::endl;
	ss << "SelectedTransmissionNow = " << car.ReturnSelectedGear() << std::endl;
	ss << "Speed = " << car.ReturnSpeed() << std::endl;
	ss << "АКПП = " << car.AutoTransmission << std::endl;
	ss << "collision = " << tmp << std::endl;
	ss << "L - вкл/выкл освещение" << std::endl;
	ss << "F - Свет из камеры" << std::endl;
	ss << "Координаты объекта: (" << car.ReturnPosition().ToString() << ")" << std::endl;

	rec.setText(ss.str().c_str());
	rec.Draw();

	ss.clear();
	//
	if (DragTrigger && !DragIventStartFlag && !RaceIsStartedFlag && !FalseStartFlag)
	{
		std::stringstream s;
		GuiTextRectangle start;
		start.setSize(200, 100);
		start.setPosition(ogl->getWidth() / 2 - 10, ogl->getHeight() - 200 - 10);

		s << "* Enter чтобы начать гонку *" << std::endl;

		start.setText(s.str().c_str());
		start.Draw();
	}
	if (DragIventStartFlag && !RaceIsStartedFlag)
	{
		std::stringstream s;
		GuiTextRectangle start;
		start.setSize(200, 100);
		start.setPosition(ogl->getWidth() / 2 - 10, ogl->getHeight() - 200 - 10);

		s << "* Shift - повышение\nCtrl - понижение\nОтсечка - 8000\nEnter - начать гонку *" << std::endl;

		start.setText(s.str().c_str());
		start.Draw();
	}
	if (RaceIsStartedFlag)
	{
		std::stringstream s;
		GuiTextRectangle start;
		start.setSize(200, 100);
		start.setPosition(ogl->getWidth() / 2 - 10, ogl->getHeight() - 200 - 10);
		if (TimeBeforeRace >= 0)
		{
			s << "Отсчет - " << TimeBeforeRace << std::endl;
		}
		else
		{
			s << "Race" << std::endl;
		}
		start.setText(s.str().c_str());
		start.Draw();
	}
	if (FalseStartFlag)
	{
		std::stringstream s;
		GuiTextRectangle start;
		start.setSize(200, 100);
		start.setPosition(ogl->getWidth() / 2 - 10, ogl->getHeight() - 200 - 10);
		s << "Фальстарт Z - Начать заново" << std::endl;
		TimeBeforeRace = 5;
		start.setText(s.str().c_str());
		start.Draw();
	}
	if (LeaveBorderTrack)
	{
		std::stringstream s;
		GuiTextRectangle start;
		start.setSize(200, 100);
		start.setPosition(ogl->getWidth() / 2 - 10, ogl->getHeight() - 230 - 10);
		s << "Нарушены границы\nZ - Начать заново" << std::endl;
		TimeBeforeRace = 5;
		start.setText(s.str().c_str());
		start.Draw();
	}
	if (SuccsessFinish && !LeaveBorderTrack && !FalseStartFlag)
	{
		std::stringstream s;
		GuiTextRectangle start;	
		start.setSize(200, 100);
		start.setPosition(ogl->getWidth() / 2 - 10, ogl->getHeight() - 200 - 10);
		if (YouWon)
		{
			s << "Победа!! Z - Начать заново" << std::endl;
			fora += 0.05; //Усложнение противника
			if (fora >= 0)
				fora = -0.001; 
		}
		else
		{
			s << "Поражение( Z - Начать заново" << std::endl;
			fora -= 0.05; //Облегчение гонки
		}
		TimeBeforeRace = 5;
		start.setText(s.str().c_str());
		start.Draw();
	}
	//
	if (RaceIsStartedFlag)
	{
		GuiTextRectangle Op;
		Op.setSize(300, 250);
		Op.setPosition(ogl->getWidth() - 300 - 10, ogl->getHeight() - 250 - 10);

		std::stringstream sss;
		sss << "Число оборотов = " << dragOp.ReturnRevNow() << std::endl;
		sss << "SelectedTransmissionNow = " << dragOp.ReturnSelectedGear() << std::endl;
		sss << "Speed = " << dragOp.ReturnSpeed() << std::endl;
		sss << "АКПП = " << dragOp.AutoTransmission << std::endl;
		sss << "Координаты объекта: (" << dragOp.ReturnPosition().ToString() << ")" << std::endl;

		Op.setText(sss.str().c_str());
		Op.Draw();
	}
		
	glMatrixMode(GL_PROJECTION);	  //восстанавливаем матрицы проекции и модел-вью обратьно из стека.
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	Shader::DontUseShaders(); 
}

void resizeEvent(OpenGL *ogl, int newW, int newH)
{
	rec.setPosition(10, newH - 100 - 10);
}

