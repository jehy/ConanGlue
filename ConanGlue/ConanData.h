#pragma once
#include <vector>
//using namespace std;
/*
struct ConanHeader_old
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

struct ConanHeader_new
{
char cona[4];
char head[162];
char chNames[256];
__int32 TimeData;//           We dunno, wtf is this.
unsigned __int16 Arec;
unsigned __int8 nChan;//      Number of channels
unsigned __int16 freq;
unsigned __int8 discrExist;
unsigned __int16 nRec;//      Number of records
unsigned __int16 nilCalibr[64];
unsigned __int16 maxCalibr[64];
unsigned __int8 coord[64][4];
unsigned __int16 sens[64];
};
*/
struct StimulSignal
{
	unsigned __int8 Littler;
	unsigned __int8 Elder;
};

class ChNames;
class MaxCalibr;
class NilCalibr;
class Sens;
class Coord;

class ConanHeader
{
public:
	ConanHeader(void);
	~ConanHeader(void);
	char cona[4];
	char* version_new = "CA40";
	char head[162];
	__int32 TimeData;//           We dunno, wtf is this.
	unsigned __int16 Arec;
	unsigned __int8 nChan;//      Number of channels
	unsigned __int16 freq;
	unsigned __int8 discrExist;
	unsigned __int16 nRec;//      Number of records
	ChNames* chNames;
	MaxCalibr* maxCalibr;
	NilCalibr* nilCalibr;
	Sens* sens;
	Coord* coord;
	bool isNewVersion();
};


class ChNames
{
public:
	ChNames(ConanHeader* parent);
	char* get();
	int getlength();
private:
	char _chNames[256];
	ConanHeader* parent;
};

class NilCalibr
{
private:
	__int16 _nilCalibr[64];
	ConanHeader* parent;
public:
	NilCalibr(ConanHeader* parent);
	__int16* get();
	int getlength();
};

class MaxCalibr
{
private:
	__int16 _maxCalibr[64];
	ConanHeader* parent;
public:
	MaxCalibr(ConanHeader* parent);
	__int16* get();
	int getlength();
};

class Sens
{
private:
	__int16 _sens[64];
	ConanHeader* parent;
public:
	Sens(ConanHeader* parent);
	__int16* get();
	int getlength();
};

class Coord
{
private:
	unsigned __int8 _coord[64][4];
	ConanHeader* parent;
public:
	Coord(ConanHeader* parent);
	unsigned __int8* get();
	int getlength();
};

class ConanData
{
public:
	ConanData(void);
	~ConanData(void);

	ConanHeader* Header;

	float *** Eeg;//                 EEG data itself
	float *** EegApproximated;//     Approximated EEG data
	__int16 *** NotCalibrated;//                 not calibrated eeg data
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
};
