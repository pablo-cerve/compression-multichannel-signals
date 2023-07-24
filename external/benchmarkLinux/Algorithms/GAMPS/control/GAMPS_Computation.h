#ifndef __GAMPS_COMPUTATION_H
#define __GAMPS_COMPUTATION_H

#include "../data/GAMPSInput.h"
#include "../data/GAMPSOutput.h"
#include <vector>

class GAMPS_Computation
{
private:
	GAMPSInput* m_pInput;
	GAMPSOutput* m_pGampsOutput;
	double epsilon;

	// duplicate original data
	CMultiDataStream* copyDataToTemp(CMultiDataStream* original);

	// calculate error tolerance for ratio signal
	double computeEps2(double eps, double eps1, double c1, double c2);

	// apply APCA to compress original data with max error epsilon
	DynArray<GAMPSEntry>*  compress_APCA(CDataStream* stream,double esp);

	// apply APCA to compress ratio signal with max error epsilon
	DynArray<GAMPSEntry>*  compress_APCA(DynArray<GAMPSEntry>& listComputeRatioList, double esp);

	// given original signal, calculate its ratio signal with respect to base signal
	DynArray<GAMPSEntry>* computeRatioSignal(CDataStream* ratioSignal,CDataStream* baseSignal,double& c1, double& c2);

	// static group given input data
	int statGroup(GAMPSInput* listInputList);

	// compute output based on base signals and ratio signals
	void computeOutput(DynArray<GAMPSEntry>** baseBucketList, DynArray<GAMPSEntry>** ratioBucketList,
					   std::vector<double> listBucketEpsilon, std::vector<double> listRatioSignalBucketEpsilon);

	void print(DynArray<GAMPSEntry>* array, int spaces);
public:
	GAMPS_Computation(GAMPSInput* gampsInput, double epsilon_);
	~GAMPS_Computation(void);

	// static group current input data
	int statGroup();

	GAMPSOutput* getGampsOutput();
};

#endif
