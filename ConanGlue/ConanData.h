#pragma once
#include <vector>
//#include "Saccade.h"
using namespace std;
//typedef vector <Saccade*> SaccadeVec;

struct ConanHeader
{
	char cona[4];
	char head[162];
	char chNames[128];
	__int32 TimeData;//           We dunno, wtf is this.
	unsigned __int16 Arec;
	unsigned __int8 nChan;//      Number of channels
	unsigned __int16 freq;
	unsigned __int8 discrExist;
	unsigned __int16 nRec;//      Number of records
	unsigned __int16 nilCalibr[32];
	unsigned __int16 maxCalibr[32]; 
  unsigned __int8 coord[32][4];
	unsigned __int16 sens[32]; 
};

struct StimulSignal
{
  unsigned __int8 Littler;
  unsigned __int8 Elder;
};

class ConanData
{
public:
	ConanData(void);
	~ConanData(void);

	ConanHeader* Header;
  
  float *** Eeg;//                 EEG data itself
  __int16 *** NotCalibrated;//                 not calibrated eeg data
  float *** EegApproximated;//     Approximated EEG data
  __int32 * TimeCreating;//        we dunno, wtf is this too
  unsigned __int16* NDataReal2;
  unsigned __int16* NDataReal;
  unsigned __int16* NDataRaw;
  StimulSignal** Discr;

  int CurChannel;//current channel
  int CurRec;//current record
  float ZoomX;
  float ZoomY;
  float XOffset;
  float YOffset;
  int Approximate;
  int AproxCoef;
  int Iterations;
  int MinExtremumPoints;
  //SaccadeVec Saccades;
  //void ResetSaccades();
};
