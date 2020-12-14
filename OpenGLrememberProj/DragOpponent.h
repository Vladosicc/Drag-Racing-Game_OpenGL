#ifndef DRAG_H
#define DRAG_H

#include "CarModel.h"

class DragOpponent : public CarModel
{
protected:
	//flag ������
	bool start = false;

	double MaxTransmissionN = 7500;

	inline void W_ivent();

public:
	void SetStart(bool flagStart);

	void CheckPosition(double TimeLastFrame);

	void DrawCar();

	//�������� �������������� ���������
	//����� ���� ���������)
	void Randomize();
};

#endif
