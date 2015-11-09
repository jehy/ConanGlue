#include "StdAfx.h"
#include "ConanData.h"
#pragma once

ConanData::ConanData(void)
{
	this->Header = NULL;
	this->Eeg = NULL;
	this->TimeCreating = NULL;
	this->NDataReal2 = NULL;
	this->NDataReal = NULL;
	this->NDataRaw = NULL;
	this->Discr = NULL;
	EegApproximated = NULL;
	this->CurChannel = 0;
	this->CurRec = 0;
	this->ZoomX = 100;
	this->ZoomY = (float)0.1;
	this->XOffset = 0;
	this->YOffset = 0;
	Approximate = 0;
	AproxCoef = 0;
	Iterations = 0;
	MinExtremumPoints = 0;
}

ConanData::~ConanData(void)
{
	if (this->Header == NULL)
		return;//do not try do destroy non-initialized object...
	for (int i = 0; i < this->Header->nRec; i++)
	{
		for (int j = 0; j < this->Header->nChan; j++)
		{
			delete[] Eeg[i][j];
			delete[] NotCalibrated[i][j];
		}
		delete[] Eeg[i];
		delete[] NotCalibrated[i];
		delete[] Discr[i];
	}
	delete[] NotCalibrated;
	if (EegApproximated != NULL)
	{
		for (int i = 0; i < this->Header->nRec; i++)
		{
			for (int j = 0; j < this->Header->nChan; j++)
				delete[] EegApproximated[i][j];
			delete[] EegApproximated[i];
		}
	}
	delete[] EegApproximated;
	delete this->Header;
	delete[] this->Eeg;
	delete[] this->TimeCreating;
	delete[] this->NDataReal2;
	delete[] this->NDataReal;
	delete[] this->NDataRaw;
	delete[] this->Discr;
}

ConanHeader::ConanHeader(void)
{
	chNames = new ChNames(this);
	maxCalibr = new MaxCalibr(this);
	nilCalibr = new NilCalibr(this);
	sens = new Sens(this);
	coord = new Coord(this);
}

ConanHeader::~ConanHeader(void)
{
	delete chNames;
	delete maxCalibr;
	delete nilCalibr;
	delete sens;
	delete coord;
}


bool ConanHeader::isNewVersion()
{
	if (memcmp(this->cona, this->version_new, 4) == 0)
		return 1;
	return 0;
}

ChNames::ChNames(ConanHeader* parent)
{
	this->parent = parent;
}

char* ChNames::get()
{
	return _chNames;
}

int ChNames::getlength()
{

	if (parent->isNewVersion())
		return sizeof(_chNames);
	else
		return sizeof(_chNames) / 2;
}


NilCalibr::NilCalibr(ConanHeader* parent)
{
	this->parent = parent;
}

__int16* NilCalibr::get()
{
	return _nilCalibr;
}


int NilCalibr::getlength()
{

	if (parent->isNewVersion())
		return sizeof(_nilCalibr);
	else
		return sizeof(_nilCalibr) / 2;
}

MaxCalibr::MaxCalibr(ConanHeader* parent)
{
	this->parent = parent;
}


__int16* MaxCalibr::get()
{
	return _maxCalibr;
}

int MaxCalibr::getlength()
{

	if (parent->isNewVersion())
		return sizeof(_maxCalibr);
	else
		return sizeof(_maxCalibr) / 2;
}


Sens::Sens(ConanHeader* parent)
{
	this->parent = parent;
}

__int16* Sens::get()
{
	return _sens;
}

int Sens::getlength()
{

	if (parent->isNewVersion())
		return sizeof(_sens);
	else
		return sizeof(_sens) / 2;
}


Coord::Coord(ConanHeader* parent)
{
	this->parent = parent;
}


unsigned __int8* Coord::get()
{
	return _coord[0];
}

int Coord::getlength()
{

	if (parent->isNewVersion())
		return sizeof(_coord);
	else
		return sizeof(_coord) / 2;
}