#ifndef __FACILITYLOCPROB_CPP
#define __FACILITYLOCPROB_CPP

#include "../../../stdafx.h"
#include "FacilityLocProb.h"

FacilityLocProb::FacilityLocProb(int numOfStream)
{
	m_nNumOfStream = numOfStream;
	m_pTgood = new int[m_nNumOfStream];
}

FacilityLocProb::~FacilityLocProb(void)
{
}

int* FacilityLocProb::getTgood()
{
	return m_pTgood;
}

void FacilityLocProb::setArrBaseCost(int* arrBaseCost)
{
	this->m_arrBaseCost = arrBaseCost;
}

void FacilityLocProb::setRatioCost(DynArray<int> **ratioCost)
{
	this->m_pRatioCost = ratioCost;
}

// Purpose	: find facility location solution to minimize memory cost problem
// Return	: (int) allocated memory cost
int FacilityLocProb::findOptimalSolution()
{
	// init m_pTgood and Tgoodold and run array for caculating reduce cost
	int totalCost = 0;
	int* selectedSignal =new int[m_nNumOfStream];

	for(int i = 0; i < m_nNumOfStream; i++)
	{
		m_pTgood[i] = i;
		totalCost += m_arrBaseCost[i];
		selectedSignal[i] = 0;
	}

	// Repeat until no changes between previous and current solution
	while(true)
	{
		int maxReduceCostPos = -1;
		int maxReduceCost = 0;

		// calculate reduce cost of signal ith
		for(int i = 0; (i < m_nNumOfStream) && (selectedSignal[i] == 0); i++)
		{
			int reduceCost = 0;
			for(int j = 0; j < m_nNumOfStream && (selectedSignal[j] == 0); j++)
			{
				int delta = m_arrBaseCost[j] - m_pRatioCost[i]->getAt(j);
				if(delta > 0)
				{
					reduceCost = reduceCost + delta;
				}
			}

			if(reduceCost > 0 && reduceCost > maxReduceCost)
			{
				maxReduceCostPos = i;
				maxReduceCost = reduceCost;
			}
		}

		// Choose the signal, which has maximum reduce cost, to be base signal
		// And the remaining signals, which have reduce cost > 0, become ratio signals
		if(maxReduceCostPos != -1)
		{
			// update m_pTgood
			selectedSignal[maxReduceCostPos] = 1;
			m_pTgood[maxReduceCostPos] = maxReduceCostPos;

			// for(int j = 0; j < m_nNumOfStream && (selectedSignal[j] == 0); j++)
			for(int j = m_nNumOfStream - 1; (j >= 0) && (selectedSignal[j] == 0); j--)
			{
				int delta = m_arrBaseCost[j] - m_pRatioCost[maxReduceCostPos]->getAt(j);
				if(delta > 0)
				{
					m_pTgood[j] = maxReduceCostPos;
					selectedSignal[j] = 1;
				}
			}
		}
		else // Otherwise, all remaining signals become base signals
		{
			for(int j = 0; j < m_nNumOfStream && (selectedSignal[j] == 0); j++){
				m_pTgood[j] = j;
			}
			break;
		}
	}

	totalCost = 0;
	for (int i = 0; i < m_nNumOfStream; i++){
		totalCost += m_pRatioCost[m_pTgood[i]]->getAt(i);
	}

	delete[] selectedSignal;
	return totalCost;
}

#endif
