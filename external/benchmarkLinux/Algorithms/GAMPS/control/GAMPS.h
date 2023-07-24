#ifndef __GAMPS_H
#define __GAMPS_H

#include "../data/GAMPSInput.h"
#include "../data/GAMPSOutput.h"
#include "GAMPS_Computation.h"

class GAMPS
{
private:
	GAMPSInput* m_pGampsInput;
	GAMPSOutput* m_pGampsOutput;
	GAMPS_Computation* m_pGampsCompute;

	int m_nNumOfStream;
	double epsilon;

public:
	GAMPS(double epsilon_,GAMPSInput* data);
	~GAMPS(void);

	// Execute GAMPS algorithm
	void compute();

	GAMPSOutput* getOutput();
};

#endif
