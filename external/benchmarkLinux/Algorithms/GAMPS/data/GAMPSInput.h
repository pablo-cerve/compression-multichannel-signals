#ifndef __GAMPSINPUT_H
#define __GAMPSINPUT_H

#pragma once

#include "../../../DataManagementLayer/Data/MultiDataStream.h"

class GAMPSInput
{
private:
	int m_nDataLength;
	int m_nNumOfStream;
	CMultiDataStream* m_pOriginalStreams;

public:
	GAMPSInput(CMultiDataStream* data);
	~GAMPSInput(void);

	int getDataLegth();
	int getNumOfStream();
	CMultiDataStream* getOriginalStreams();

	// Get slide window data from m_pOriginalStreams
	GAMPSInput* getWndData(int from, int wSize);
};

#endif
