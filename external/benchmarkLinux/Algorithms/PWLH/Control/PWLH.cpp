#ifndef __PWLH_CPP
#define __PWLH_CPP

#include "../../../stdafx.h"
#include "PWLH.h"
#include "LinearBucket.h"

// Add ----> A
//#include <crtdbg.h>
// Add <---- A

//Add ----> A
#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif
// Add <---- A

PWLH::PWLH(void)
{
	input = NULL;
	output = NULL;
}

PWLH::PWLH(PWLHInput *data)
{
	input = data;
	output = new PWLHOutput(input);
}

PWLH::~PWLH(void)
{
	delete input;
	delete output;
}

PWLHOutput* PWLH::getOutput()
{
	return output;
}

// Compute model parameters
void PWLH::compress(void)
{
	int size = input->getDataLength();
	double esp = input->getEsp();
	LinearBucket bucket(esp);
	Point p1, p2;
	int i = 0;

	while (i < size)
	{
		//Last bucket has only one point
		if (i == (size - 1) && bucket.getSize() == 0)
		{
			output->getCompressData()->add(Point(input->getAt(i).value, i + 1));
			break;
		}

		//Add point into bucket
		bucket.addPoint(input->getAt(i).value);

		//Bucket is valid
		if (bucket.checkEpsConstraint())
		{
			i++;
			if (i != size)	continue;
		}
		else //Bucket is invalid
		{
			bucket.removePoint();
		}

		bucket.getAproximatedLine(p1, p2);

		//Update time of p1 and p2
		p1.x = i - bucket.getSize() + 1;
		p2.x = i;

		//Reset to create new bucket
		bucket.resetBucket();

		//add compressed data
		output->getCompressData()->add(p1);
		output->getCompressData()->add(p2);
	}
}

#endif
