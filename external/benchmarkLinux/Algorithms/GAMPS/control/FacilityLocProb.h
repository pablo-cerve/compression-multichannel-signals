#ifndef __FACILITYLOCPROB_H
#define __FACILITYLOCPROB_H

#include "../../../DataStructures/DynArray/DynArray.cpp"

class FacilityLocProb
{
private:
	int m_nNumOfStream;
	int* m_arrBaseCost;
	DynArray<int>** m_pRatioCost;
	int* m_pTgood;

public:
	FacilityLocProb(int _numOfStream);
	~FacilityLocProb(void);

	int* getTgood();
	
	void setArrBaseCost(int* _arrBaseCost);
	void setRatioCost(DynArray<int>** _ratioCost);

	// Find facility location solution to minimize memory cost problem	
	int findOptimalSolution();
};

#endif
