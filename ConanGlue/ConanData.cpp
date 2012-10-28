#include "StdAfx.h"
#include "Conandata.h"

ConanData::ConanData(void)
{
  this->Header=NULL;
  this->Eeg=NULL;
  this->TimeCreating=NULL;
  this->NDataReal2=NULL;
  this->NDataReal=NULL;
  this->NDataRaw=NULL;
  this->Discr=NULL;
  EegApproximated=NULL;
  this->CurChannel=0;
  this->CurRec=0;
  this->ZoomX=100;
  this->ZoomY=(float)0.1;
  this->XOffset=0;
  this->YOffset=0;
  Approximate=0;
  AproxCoef=0;
  Iterations=0;
  MinExtremumPoints=0;
}

ConanData::~ConanData(void)
{
  if(this->Header==NULL)
    return;//do not try do destroy non-initialized object...
  for (int i=0;i<this->Header->nRec;i++)
  {
	  for (int j=0;j<this->Header->nChan;j++)
    {
      delete[] Eeg[i][j];
      delete[] NotCalibrated[i][j];
    }
    delete[] NotCalibrated[i];
    delete[] Eeg[i];
    delete[] Discr[i];
  }
  if(EegApproximated!=NULL)
  {
    for (int i=0;i<this->Header->nRec;i++)
    {
	    for (int j=0;j<this->Header->nChan;j++)
        delete[] EegApproximated[i][j];
      delete[] EegApproximated[i];
    }
  }
  delete[] EegApproximated;
  delete[] this->Header;
  delete[] this->Eeg;
  delete[] this->NotCalibrated;
  delete[] this->TimeCreating;
  delete[] this->NDataReal2;
  delete[] this->NDataReal;
  delete[] this->NDataRaw;
  delete[] this->Discr;
//  ResetSaccades();
}
/*
void ConanData::ResetSaccades()
{

  int s=Saccades.size();
  for(int i=0;i<s;i++)//remove found saccades
  {
    delete(Saccades.at(i));
  }
  Saccades.clear();
}
*/