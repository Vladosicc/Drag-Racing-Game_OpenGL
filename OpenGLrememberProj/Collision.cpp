#include "Collision.h"
#include <cstdlib>
#include "MyOGL.h"

void Cube::DrawCube()
{
	glBegin(GL_LINES);
	glVertex3d(vec[0].X(),vec[0].Y(),vec[0].Z());
	glVertex3d(vec[1].X(), vec[1].Y(), vec[1].Z());
	glVertex3d(vec[1].X(), vec[1].Y(), vec[1].Z());
	glVertex3d(vec[2].X(), vec[2].Y(), vec[2].Z());
	glVertex3d(vec[2].X(), vec[2].Y(), vec[2].Z());
	glVertex3d(vec[3].X(), vec[3].Y(), vec[3].Z());
	glVertex3d(vec[3].X(), vec[3].Y(), vec[3].Z());
	glVertex3d(vec[0].X(), vec[0].Y(), vec[0].Z());
	glEnd();
	glBegin(GL_LINES);

	glVertex3d(vec[4].X(), vec[4].Y(), vec[4].Z());
	glVertex3d(vec[5].X(), vec[5].Y(), vec[5].Z());
	glVertex3d(vec[5].X(), vec[5].Y(), vec[5].Z());
	glVertex3d(vec[6].X(), vec[6].Y(), vec[6].Z());
	glVertex3d(vec[6].X(), vec[6].Y(), vec[6].Z());
	glVertex3d(vec[7].X(), vec[7].Y(), vec[7].Z());
	glVertex3d(vec[7].X(), vec[7].Y(), vec[7].Z());
	glVertex3d(vec[4].X(), vec[4].Y(), vec[4].Z());

	glVertex3d(vec[0].X(), vec[0].Y(), vec[0].Z());
	glVertex3d(vec[4].X(), vec[4].Y(), vec[4].Z());
	glVertex3d(vec[1].X(), vec[1].Y(), vec[1].Z());
	glVertex3d(vec[5].X(), vec[5].Y(), vec[5].Z());
	glVertex3d(vec[2].X(), vec[2].Y(), vec[2].Z());
	glVertex3d(vec[6].X(), vec[6].Y(), vec[6].Z());
	glVertex3d(vec[3].X(), vec[3].Y(), vec[3].Z());
	glVertex3d(vec[7].X(), vec[7].Y(), vec[7].Z());
	glEnd();
}

bool Collision::AddCollision(Vector3 vec[8])
{
	for (int j = 0; j < 8; j++)
	{
		Coll[j] = vec[j];
	}
	delete[] vec;
	return true;
}

bool Collision::AddCollision(double a[3], double b[3], double c[3], double d[3], double a1[3], double b1[3], double c1[3], double d1[3])
{
	Coll[0].setCoords(a[0], a[1], a[2]);
	Coll[1].setCoords(b[0], b[1], b[2]);
	Coll[2].setCoords(c[0], c[1], c[2]);
	Coll[3].setCoords(d[0], d[1], d[2]);
	Coll[4].setCoords(a1[0], a1[1], a1[2]);
	Coll[5].setCoords(b1[0], b1[1], b1[2]);
	Coll[6].setCoords(c1[0], c1[1], c1[2]);
	Coll[7].setCoords(d1[0], d1[1], d1[2]);
	return true;
}

bool Collision::AddCollision(Vector3 a, Vector3 b, Vector3 c, Vector3 d, Vector3 a1, Vector3 b1, Vector3 c1, Vector3 d1)
{
	Coll[0] = a;
	Coll[1] = b;
	Coll[2] = c;
	Coll[3] = d;
	Coll[4] = a1;
	Coll[5] = b1;
	Coll[6] = c1;
	Coll[7] = d1;
	return true;
}

bool Collision::AddCollision2(Vector3 vec[2])
{
	Coll[0] = vec[0];
	Coll[6] = vec[1];
	Coll[1].setCoords(vec[0].X(), vec[1].Y(), vec[0].Z());
	Coll[2].setCoords(vec[1].X(), vec[1].Y(), vec[0].Z());
	Coll[3].setCoords(vec[1].X(), vec[0].Y(), vec[0].Z());
	Coll[4].setCoords(vec[0].X(), vec[0].Y(), vec[1].Z());
	Coll[5].setCoords(vec[0].X(), vec[1].Y(), vec[1].Z());
	Coll[7].setCoords(vec[1].X(), vec[0].Y(), vec[1].Z());
	delete[] vec;
	return true;
}

void Collision::DrawCollision()
{
	Coll.DrawCube();
}

void DynamicCollision::UpdateDynamicCollision(Vector3 pos, double AngleZ)
{
	DynamicCol[0].setCoords(pos.X() + Coll[0].X() * cos(AngleZ * PI / 180) - Coll[0].Y() * sin(AngleZ * PI / 180), pos.Y() + Coll[0].Y() * cos(AngleZ * PI / 180) + Coll[0].X() * sin(AngleZ * PI / 180), pos.Z() + Coll[0].Z());
	DynamicCol[1].setCoords(pos.X() + Coll[1].X() * cos(AngleZ * PI / 180) - Coll[1].Y() * sin(AngleZ * PI / 180), pos.Y() + Coll[1].Y() * cos(AngleZ * PI / 180) + Coll[1].X() * sin(AngleZ * PI / 180), pos.Z() + Coll[1].Z());
	DynamicCol[2].setCoords(pos.X() + Coll[2].X() * cos(AngleZ * PI / 180) - Coll[2].Y() * sin(AngleZ * PI / 180), pos.Y() + Coll[2].Y() * cos(AngleZ * PI / 180) + Coll[2].X() * sin(AngleZ * PI / 180), pos.Z() + Coll[2].Z());
	DynamicCol[3].setCoords(pos.X() + Coll[3].X() * cos(AngleZ * PI / 180) - Coll[3].Y() * sin(AngleZ * PI / 180), pos.Y() + Coll[3].Y() * cos(AngleZ * PI / 180) + Coll[3].X() * sin(AngleZ * PI / 180), pos.Z() + Coll[3].Z());
	DynamicCol[4].setCoords(pos.X() + Coll[4].X() * cos(AngleZ * PI / 180) - Coll[4].Y() * sin(AngleZ * PI / 180), pos.Y() + Coll[4].Y() * cos(AngleZ * PI / 180) + Coll[4].X() * sin(AngleZ * PI / 180), pos.Z() + Coll[4].Z());
	DynamicCol[5].setCoords(pos.X() + Coll[5].X() * cos(AngleZ * PI / 180) - Coll[5].Y() * sin(AngleZ * PI / 180), pos.Y() + Coll[5].Y() * cos(AngleZ * PI / 180) + Coll[5].X() * sin(AngleZ * PI / 180), pos.Z() + Coll[5].Z());
	DynamicCol[6].setCoords(pos.X() + Coll[6].X() * cos(AngleZ * PI / 180) - Coll[6].Y() * sin(AngleZ * PI / 180), pos.Y() + Coll[6].Y() * cos(AngleZ * PI / 180) + Coll[6].X() * sin(AngleZ * PI / 180), pos.Z() + Coll[6].Z());
	DynamicCol[7].setCoords(pos.X() + Coll[7].X() * cos(AngleZ * PI / 180) - Coll[7].Y() * sin(AngleZ * PI / 180), pos.Y() + Coll[7].Y() * cos(AngleZ * PI / 180) + Coll[7].X() * sin(AngleZ * PI / 180), pos.Z() + Coll[7].Z());
}

bool DynamicCollision::CheckWithStaticCollision(Collision& Col)
{
	bool tmp = false;

	CVector3 Line[2]{
	DynamicCol[0].ConvertToCVector3(),
	DynamicCol[1].ConvertToCVector3()
	};
	CVector3 Poly[4]{
		Col.Coll[0].ConvertToCVector3(),
		Col.Coll[1].ConvertToCVector3(),
		Col.Coll[4].ConvertToCVector3(),
		Col.Coll[5].ConvertToCVector3(),
	};
	tmp += IntersectedPolygon(Poly, Line, 4);

	Line[0] = DynamicCol[1].ConvertToCVector3();
	Line[1] = DynamicCol[2].ConvertToCVector3();

	tmp += IntersectedPolygon(Poly, Line, 4);

	Line[0] = DynamicCol[2].ConvertToCVector3();
	Line[1] = DynamicCol[3].ConvertToCVector3();

	tmp += IntersectedPolygon(Poly, Line, 4);

	Line[0] = DynamicCol[3].ConvertToCVector3();
	Line[1] = DynamicCol[0].ConvertToCVector3();

	tmp += IntersectedPolygon(Poly, Line, 4);

	Poly[0] = Col.Coll[2].ConvertToCVector3();
	Poly[1] = Col.Coll[3].ConvertToCVector3();
	Poly[2] = Col.Coll[7].ConvertToCVector3();
	Poly[3] = Col.Coll[6].ConvertToCVector3();

	Line[0] = DynamicCol[0].ConvertToCVector3();
	Line[1] = DynamicCol[1].ConvertToCVector3();

	tmp += IntersectedPolygon(Poly, Line, 4);

	Line[0] = DynamicCol[1].ConvertToCVector3();
	Line[1] = DynamicCol[2].ConvertToCVector3();

	tmp += IntersectedPolygon(Poly, Line, 4);

	Line[0] = DynamicCol[2].ConvertToCVector3();
	Line[1] = DynamicCol[3].ConvertToCVector3();

	tmp += IntersectedPolygon(Poly, Line, 4);

	Line[0] = DynamicCol[3].ConvertToCVector3();
	Line[1] = DynamicCol[0].ConvertToCVector3();

	tmp += IntersectedPolygon(Poly, Line, 4);

	Poly[0] = Col.Coll[0].ConvertToCVector3();
	Poly[1] = Col.Coll[3].ConvertToCVector3();
	Poly[2] = Col.Coll[7].ConvertToCVector3();
	Poly[3] = Col.Coll[4].ConvertToCVector3();
	
	Line[0] = DynamicCol[0].ConvertToCVector3();
	Line[1] = DynamicCol[1].ConvertToCVector3();

	tmp += IntersectedPolygon(Poly, Line, 4);

	Line[0] = DynamicCol[1].ConvertToCVector3();
	Line[1] = DynamicCol[2].ConvertToCVector3();

	tmp += IntersectedPolygon(Poly, Line, 4);

	Line[0] = DynamicCol[2].ConvertToCVector3();
	Line[1] = DynamicCol[3].ConvertToCVector3();

	tmp += IntersectedPolygon(Poly, Line, 4);

	Line[0] = DynamicCol[3].ConvertToCVector3();
	Line[1] = DynamicCol[0].ConvertToCVector3();

	tmp += IntersectedPolygon(Poly, Line, 4);

	Poly[0] = Col.Coll[1].ConvertToCVector3();
	Poly[1] = Col.Coll[2].ConvertToCVector3();
	Poly[2] = Col.Coll[6].ConvertToCVector3();
	Poly[3] = Col.Coll[5].ConvertToCVector3();

	Line[0] = DynamicCol[0].ConvertToCVector3();
	Line[1] = DynamicCol[1].ConvertToCVector3();

	tmp += IntersectedPolygon(Poly, Line, 4);

	Line[0] = DynamicCol[1].ConvertToCVector3();
	Line[1] = DynamicCol[2].ConvertToCVector3();

	tmp += IntersectedPolygon(Poly, Line, 4);

	Line[0] = DynamicCol[2].ConvertToCVector3();
	Line[1] = DynamicCol[3].ConvertToCVector3();

	tmp += IntersectedPolygon(Poly, Line, 4);

	Line[0] = DynamicCol[3].ConvertToCVector3();
	Line[1] = DynamicCol[0].ConvertToCVector3();

	tmp += IntersectedPolygon(Poly, Line, 4);

	return tmp;
}

void DynamicCollision::DrawCollision()
{
	DynamicCol.DrawCube();
}
