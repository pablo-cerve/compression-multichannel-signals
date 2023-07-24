#ifndef __GAMPSINPUT_CPP
#define __GAMPSINPUT_CPP

#include "../../../stdafx.h"
#include "GAMPSInput.h"

// Parameter:	data: multi signals data which is input for GAMPS algorithm
GAMPSInput::GAMPSInput(CMultiDataStream* data)
{
	m_pOriginalStreams = data;
	m_nNumOfStream = data->getNumOfStream();
	m_nDataLength = data->getDataLength();
}

GAMPSInput::~GAMPSInput(void)
{
	delete m_pOriginalStreams;
}

int GAMPSInput::getDataLegth()
{
	return m_nDataLength;
}

int GAMPSInput::getNumOfStream()
{
	return m_nNumOfStream;
}

CMultiDataStream* GAMPSInput::getOriginalStreams()
{
	return m_pOriginalStreams;
}

// Purpose	: Get slide window data from m_pOriginalStreams
// Parameter:
//            from:  number at which to start getting data
//            wSize: the number record be read from 'from' timestamp
GAMPSInput* GAMPSInput::getWndData(int from, int wSize)
{
	CMultiDataStream* multiStream = new CMultiDataStream(m_nNumOfStream);
	CDataStream* temp = new CDataStream();

	for(int i = 0; i < m_nNumOfStream; i++)
	{
		temp = this->m_pOriginalStreams->getDataAt(i);
		int limit = temp->size() > from+wSize ? from+ wSize : temp->size();
		CDataStream* newStream = new CDataStream();
		int tempCount = 0;

		for(int i = from; i < limit; i++)
		{
			DataItem entry = temp->getAt(i);
			DataItem tempEntry;
			tempEntry.value = entry.value;
			tempEntry.timestamp = entry.timestamp;
			newStream->add(entry);
		}
		multiStream->addSingleStream(newStream);
	}

	delete temp;
	GAMPSInput* returnData = new GAMPSInput(multiStream);
	return returnData;
}

#endif
