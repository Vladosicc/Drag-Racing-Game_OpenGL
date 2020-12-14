#ifndef ListCol_H
#define ListCol_H

#include "MyVector3d.h"

/*
	*(0)  ----------------- *(1)
	|						|
	|						|
	|						|
	|		ÍÈÇ				|
	|						|
	|						|
	|						|
	|						|
	*(3)  ----------------- *(2)

	*(4)  ----------------- *(5)
	|						|
	|						|
	|						|
	|		ÂÅÐÕ			|
	|						|
	|						|
	|						|
	|						|
	*(7)  ----------------- *(6)
*/

class ListCollisions
{
public:
	Vector3* CarColRet()
	{
		Vector3* vec = new Vector3[8];
		vec[0].setCoords(3.75, 1.7, 0); //
		vec[1].setCoords(3.75,-1.7,0 );
		vec[2].setCoords( -3.75,-1.7,0 );
		vec[3].setCoords(-3.75, 1.7, 0);
		vec[4].setCoords(3.75, 1.7, 3);
		vec[5].setCoords(3.75, -1.7, 3);
		vec[6].setCoords(-3.75, -1.7, 3);
		vec[7].setCoords(-3.75, 1.7, 3);
		return vec;
	}

	Vector3* SvetoforColRet2()
	{
		Vector3* vec = new Vector3[2];
		vec[0].setCoords(13.2, 18.5, 0); 
		vec[1].setCoords(12.7, 16.5, 4);
		return vec;
	}

	Vector3* OtboinikColRet2()
	{
		Vector3* vec = new Vector3[2];
		vec[0].setCoords(13.1, 18, 0);
		vec[1].setCoords(350, 17, 1.5);
		return vec;
	}
};

#endif
