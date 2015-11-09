#include "stdafx.h"
#include "Conandata.h"
#include <sys/stat.h> 
using namespace std;

void AddLog(CEdit*log, CString z)
{
	CString logstr;
	log->GetWindowText(logstr);
	logstr.Append(z);
	logstr.Append(_T("\r\n"));
	log->SetWindowText(logstr);
	log->LineScroll(log->GetLineCount());
}


BOOL WriteConanFile(TCHAR* fname, ConanData* Conan, CEdit* log)
{
	FILE * pFile;
	_wfopen_s(&pFile, fname, _T("w+b"));
	if (pFile == NULL)
	{
		MessageBox(NULL, _T("Не могу открыть файл на запись!"), _T("Ошибка"), MB_OK);
		return 0;
	}
	int pos = 0;
	fwrite(Conan->Header->cona, sizeof(Conan->Header->cona), 1, pFile);
	pos += sizeof(Conan->Header->cona);
	fwrite(Conan->Header->head, sizeof(Conan->Header->head), 1, pFile);
	pos += sizeof(Conan->Header->head);
	fwrite(Conan->Header->chNames->get(), Conan->Header->chNames->getlength(), 1, pFile);
	pos += Conan->Header->chNames->getlength();
	fwrite(&Conan->Header->TimeData, sizeof(Conan->Header->TimeData), 1, pFile);
	pos += sizeof(Conan->Header->TimeData);
	fwrite(&Conan->Header->Arec, sizeof(Conan->Header->Arec), 1, pFile);
	pos += sizeof(Conan->Header->Arec);
	fwrite(" ", sizeof(__int8), 2, pFile);//empty space
	pos += sizeof(__int8) * 2;
	fwrite(&Conan->Header->nChan, sizeof(Conan->Header->nChan), 1, pFile);
	pos += sizeof(Conan->Header->nChan);
	fwrite(&Conan->Header->freq, sizeof(Conan->Header->freq), 1, pFile);
	pos += sizeof(Conan->Header->freq);
	fwrite(&Conan->Header->discrExist, sizeof(Conan->Header->discrExist), 1, pFile);
	pos += sizeof(Conan->Header->discrExist);
	fwrite(&Conan->Header->nRec, sizeof(Conan->Header->nRec), 1, pFile);
	pos += sizeof(Conan->Header->nRec);
	fwrite(Conan->Header->nilCalibr->get(), Conan->Header->nilCalibr->getlength(), 1, pFile);
	pos += Conan->Header->nilCalibr->getlength();
	fwrite(Conan->Header->maxCalibr->get(), Conan->Header->maxCalibr->getlength(), 1, pFile);
	pos += Conan->Header->maxCalibr->getlength();
	fwrite(Conan->Header->coord->get(), Conan->Header->coord->getlength(), 1, pFile);
	pos += Conan->Header->coord->getlength();
	////??? 11 or 10 bytes? reserve
	fwrite(" ", sizeof(__int8), 11, pFile);
	pos += sizeof(__int8) * 11;

	fwrite(Conan->Header->sens->get(), Conan->Header->sens->getlength(), 1, pFile);
	pos += Conan->Header->sens->getlength();

	////records begin from Arec
	//fwrite(" ",sizeof(__int8),Conan->Header->Arec-pos,pFile);

	for (int i = 0; i < Conan->Header->nRec; i++)
	{
		fwrite(&Conan->NDataRaw[i], sizeof(__int16), 1, pFile);
		fwrite(&Conan->NDataReal[i], sizeof(__int16), 1, pFile);

		for (int j = 0; j < Conan->Header->nChan; j++)
		{
			for (int k = 0; k < Conan->NDataReal[i]; k++)
			{
				fwrite(&Conan->NotCalibrated[i][j][k], sizeof(__int16), 1, pFile);
			}
		}

		// discrete channels for the current record: 
		if (Conan->Header->discrExist == 1)
		{
			for (int k = 0; k < Conan->NDataReal[i]; k++)
			{
				fwrite(&Conan->Discr[i][k].Elder, sizeof(__int8), 1, pFile);
				fwrite(&Conan->Discr[i][k].Littler, sizeof(__int8), 1, pFile);
			}
		}
		else
		{
			//no discrete channel - nothing to write
		}
		//make blank space ( which may result from deleting some data when editing the EEG in CONAN ) : 
		int spaces = (Conan->NDataRaw[i] - Conan->NDataReal[i])*Conan->Header->nChan;
		if (spaces)
			fwrite(" ", sizeof(__int8), 2 * spaces, pFile);

		fwrite(&Conan->TimeCreating[i], sizeof(__int32), 1, pFile);
		fwrite(&Conan->NDataReal2[i], sizeof(__int16), 1, pFile);
	}
	fclose(pFile);
	return 1;
}

ConanData* ReadConanFile(TCHAR* fname, CEdit* log)
{

	//CString z,logstr;

	FILE * pFile;
	long lSize;
	byte * buffer;
	size_t result;
	_wfopen_s(&pFile, fname, _T("r+b"));
	if (pFile == NULL)
	{
		MessageBox(NULL, _T("Не могу прочитать файл!"), _T("Ошибка"), MB_OK);
		return NULL;
	}
	//copy file to buffer for quicker operations
	// obtain file size:
	fseek(pFile, 0, SEEK_END);
	lSize = ftell(pFile);
	rewind(pFile);
	buffer = new byte[lSize];
	result = fread(buffer, 1, lSize, pFile);
	fclose(pFile);
	//z.Format("File size: %d",lSize);
	//AddLog(log,z);

	//can't copy memory directly into structure because it may not be compacted with some compilers's settings.
	char version[4];
	memcpy_s(version, sizeof(version), &buffer[0], sizeof(version));


	if (memcmp(version, "CA30", 4) != 0 && memcmp(version, "CA40", 4) != 0)
	{
		CString error = _T("Неизвестная версия формата. Ожидалось CA30 или CA40, получено ");
		USES_CONVERSION;
		error.Append(A2W(version));
		MessageBox(NULL, (error), _T("Ошибка"), MB_OK);
		return NULL;
	}

	ConanHeader* data = new ConanHeader();
	unsigned int pos = 0;
	memcpy_s(data->cona, sizeof(data->cona), &buffer[0], sizeof(data->cona));
	pos += sizeof(data->cona);
	memcpy_s(data->head, sizeof(data->head), &buffer[pos], sizeof(data->head));
	pos += sizeof(data->head);
	memcpy_s(data->chNames->get(), data->chNames->getlength(), &buffer[pos], data->chNames->getlength());
	pos += data->chNames->getlength();
	memcpy_s(&data->TimeData, sizeof(data->TimeData), &buffer[pos], sizeof(data->TimeData));
	pos += sizeof(data->TimeData);
	memcpy_s(&data->Arec, sizeof(data->Arec), &buffer[pos], sizeof(data->Arec));
	pos += sizeof(data->Arec);
	pos += 2;
	memcpy_s(&data->nChan, sizeof(data->nChan), &buffer[pos], sizeof(data->nChan));
	pos += sizeof(data->nChan);
	memcpy_s(&data->freq, sizeof(data->freq), &buffer[pos], sizeof(data->freq));
	pos += sizeof(data->freq);
	memcpy_s(&data->discrExist, sizeof(data->discrExist), &buffer[pos], sizeof(data->discrExist));
	pos += sizeof(data->discrExist);
	memcpy_s(&data->nRec, sizeof(data->nRec), &buffer[pos], sizeof(data->nRec));
	pos += sizeof(data->nRec);
	memcpy_s(data->nilCalibr->get(), data->nilCalibr->getlength(), &buffer[pos], data->nilCalibr->getlength());
	pos += data->nilCalibr->getlength();
	memcpy_s(data->maxCalibr->get(), data->maxCalibr->getlength(), &buffer[pos], data->maxCalibr->getlength());
	pos += data->maxCalibr->getlength();


	memcpy_s(data->coord->get(), data->coord->getlength(), &buffer[pos], data->coord->getlength());
	pos += data->coord->getlength();

	pos += 11;
	memcpy_s(data->sens->get(), data->sens->getlength(), &buffer[pos], data->sens->getlength());
	pos += data->sens->getlength();

	pos = data->Arec;

	/*z.Format("Arec: %d \r\n",data->Arec);
	logstr.Append(z);
	z.Format("nChan: %d\r\n",data->nChan);
	logstr.Append(z);
	z.Format("freq: %d\r\n",data->freq);
	logstr.Append(z);
	z.Format("nRec: %d\r\n",data->nRec);


	z.Format("coord [0,0]: %d\r\n",data->coord[0][0]);
	logstr.Append(z);
	z.Format("coord [0,1]: %d\r\n",data->coord[0][1]);
	logstr.Append(z);
	z.Format("coord [1,0]: %d\r\n",data->coord[1][0]);
	logstr.Append(z);
	z.Format("coord [31,2]: %d\r\n",data->coord[31][2]);
	logstr.Append(z);
	z.Format("sens 0: %d\r\n",data->sens[0]);
	logstr.Append(z);
	z.Format("sens last(31): %d\r\n",data->sens[31]);
	logstr.Append(z);
	AddLog(log,logstr);*/


	float *** Eeg = new float**[data->nRec];
	__int16*** NotCalibrated = new __int16**[data->nRec];

	__int32 * TimeCreating = new __int32[data->nRec];
	unsigned __int16* NDataReal2 = new unsigned __int16[data->nRec];
	unsigned __int16* NDataReal = new unsigned __int16[data->nRec];
	unsigned __int16* NDataRaw = new unsigned __int16[data->nRec];
	StimulSignal** Discr = new StimulSignal*[data->nRec];

	for (int i = 0; i < data->nRec; i++)
	{
		memcpy_s(&NDataRaw[i], sizeof(__int16), &buffer[pos], sizeof(__int16));
		pos += sizeof(__int16);
		memcpy_s(&NDataReal[i], sizeof(__int16), &buffer[pos], sizeof(__int16));
		pos += sizeof(__int16);

		//logstr.Format("recNDataRaw: %d\r\nrecNDataReal: %d",NDataRaw[i],NDataReal[i]);
		//AddLog(log,logstr);
		NotCalibrated[i] = new __int16*[data->nChan];
		Eeg[i] = new float*[data->nChan];
		for (int j = 0; j < data->nChan; j++)
		{
			NotCalibrated[i][j] = new __int16[NDataReal[i]];
			Eeg[i][j] = new float[NDataReal[i]];
			for (int k = 0; k < NDataReal[i]; k++)
			{
				NotCalibrated[i][j][k] = *(__int16*)&buffer[pos];
				Eeg[i][j][k] = (*(__int16*)&buffer[pos] - (float)data->nilCalibr->get()[j]) / (float)data->maxCalibr->get()[j] * (float)data->sens->get()[j];
				pos += sizeof(__int16);
			}
		}

		// discrete channels for the current record: 
		if (data->discrExist == 1)
		{
			Discr[i] = new StimulSignal[NDataReal[i]];
			for (int k = 0; k < NDataReal[i]; k++)
			{
				memcpy_s(&Discr[i][k].Elder, sizeof(__int8), &buffer[pos], sizeof(__int8));
				pos += 1;
				memcpy_s(&Discr[i][k].Littler, sizeof(__int8), &buffer[pos], sizeof(__int8));
				pos += 1;
			}
		}
		else
		{
			Discr[i] = new StimulSignal[data->nChan];
			for (unsigned int k = 0; k < data->nChan; k++)
			{
				Discr[i][k].Littler = 0;
				Discr[i][k].Elder = 0;
			}
		}
		///
		//test
		/*for(int v=0;v<NDataReal[i];v++)
		{
		if(v>200)
		break;
		if(i>0)
		break;
		z.Format("Discr rec %d num %d: %x %x \r\n",i,v,Discr[i][v].Elder,Discr[i][v].Littler);
		AddLog(log,z);
		}*/

		//
		/* % skip blank space ( which may result from deleting some data when
			% editing the EEG in CONAN ) : */
		pos += 2 * (NDataRaw[i] - NDataReal[i])*data->nChan;//number of 16bit integers to skip

		memcpy_s(&TimeCreating[i], sizeof(__int32), &buffer[pos], sizeof(__int32));
		pos += sizeof(TimeCreating[i]);

		memcpy_s(&NDataReal2[i], sizeof(__int16), &buffer[pos], sizeof(__int16));
		pos += sizeof(NDataReal2[i]);

		//z.Format("recNDataReal2: %d ",NDataReal2[i]);
		//AddLog(log,z);


		//z.Format("Bytes processed: %d \r\n",pos);
		//AddLog(log,z);

	}
	delete[] buffer;

	ConanData* all = new ConanData();
	all->Header = data;
	all->Eeg = Eeg;
	all->NotCalibrated = NotCalibrated;
	all->TimeCreating = TimeCreating;
	all->NDataReal2 = NDataReal2;
	all->NDataReal = NDataReal;
	all->NDataRaw = NDataRaw;
	all->Discr = Discr;

	return all;
}


bool FileExists(CString strFilename) {
	struct _stat stFileInfo;
	bool blnReturn;
	int intStat;

	// Attempt to get the file attributes
	intStat = _wstat(strFilename, &stFileInfo);
	if (intStat == 0) {
		// We were able to get the file attributes
		// so the file obviously exists.
		blnReturn = true;
	}
	else {
		// We were not able to get the file attributes.
		// This may mean that we don't have permission to
		// access the folder which contains this file. If you
		// need to do that level of checking, lookup the
		// return values of stat which will give you
		// more details on why stat failed.
		blnReturn = false;
	}

	return(blnReturn);
}