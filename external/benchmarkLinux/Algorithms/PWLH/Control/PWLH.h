#ifndef __PWLH_H
#define __PWLH_H

#pragma once

#include "../../Algo.h"
#include "../Data/PWLHInput.h"
#include "../Data/PWLHOutput.h"

class PWLH : public Algo
{
private:
	PWLHInput* input;
	PWLHOutput* output;

public:
	PWLH(void);
	PWLH(PWLHInput* data);
	~PWLH(void);

	PWLHOutput* getOutput();
	virtual void compress();// Compute model parameters
};

#endif
