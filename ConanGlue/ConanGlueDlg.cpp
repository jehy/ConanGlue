
// ConanGlueDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ConanGlue.h"
#include "ConanGlueDlg.h"
#include "afxdialogex.h"
#include "afxdialogex.h"
#include "funcs.h"
#include "FileRecord.h"
#include <vector>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;
typedef vector <char*> StringVec;
typedef vector <FileRecord*> FileRecordVec;
//StringVec GlueFiles;
//CString FilesDir;
FileRecordVec FileRecords;
// CConanGlueDlg dialog



CConanGlueDlg::CConanGlueDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CConanGlueDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CConanGlueDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, SettingsFile);
	DDX_Control(pDX, IDC_EDIT4, Log);
	DDX_Control(pDX, IDC_EDIT2, filedir);
	DDX_Control(pDX, IDC_EDIT3, OutputFile);
	DDX_Control(pDX, IDC_PROGRESS1, ProgressBar);
}

BEGIN_MESSAGE_MAP(CConanGlueDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CConanGlueDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CConanGlueDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CConanGlueDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CConanGlueDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDOK, &CConanGlueDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CConanGlueDlg::OnBnClickedCancel)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CConanGlueDlg message handlers

BOOL CConanGlueDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	ProgressBar.SetPos(0);

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CConanGlueDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CConanGlueDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CConanGlueDlg::OnBnClickedButton1()
{

	CString path;
	CFileDialog dlg(TRUE/*Open=TRUE Save=False*/, _T("txt")/*Filename Extension*/, _T("")/*Initial Filename*/, OFN_ENABLESIZING | OFN_EXPLORER | OFN_FILEMUSTEXIST/*Flags*/, _T("Conan glue options(*.txt)|*.txt||")/*Filetype Filter*/, this/*parent Window*/);
	int z = dlg.DoModal();
	if (z == IDOK)
		path = dlg.GetPathName();
	else
		return;
	SettingsFile.SetWindowText(path);
}


int CALLBACK BrowseForFolderCallback(HWND hwnd, UINT uMsg, LPARAM lp, LPARAM pData)
{
	TCHAR szPath[MAX_PATH];

	switch (uMsg)
	{
	case BFFM_INITIALIZED:
		SendMessage(hwnd, BFFM_SETSELECTION, TRUE, pData);
		break;

	case BFFM_SELCHANGED:
		if (SHGetPathFromIDList((LPITEMIDLIST)lp, szPath))
		{
			SendMessage(hwnd, BFFM_SETSTATUSTEXT, 0, (LPARAM)szPath);

		}
		break;
	}

	return 0;
}

void CConanGlueDlg::OnBnClickedButton2()
{



	LPITEMIDLIST pidlRoot = NULL;
	PCWSTR temp;
	CString tmp;
#ifdef _UNICODE
	temp = LPCTSTR(tmp);
#else
	USES_CONVERSION;
	temp = A2CW(LPCTSTR(tmp));
#endif
	HRESULT hR = SHParseDisplayName(temp, 0, &pidlRoot, 0, 0);


	TCHAR path[600];//more then MAX_path, just in case...
	//set begining path
	tmp = "";
	BROWSEINFO bi;
	bi.hwndOwner = NULL;
	bi.pidlRoot = pidlRoot;
	bi.pszDisplayName = path;
	bi.lpszTitle = _T("Пожалуйста, укажите путь к директории файлов записей");
	bi.ulFlags = BIF_USENEWUI + BIF_SHAREABLE;
	bi.lpfn = BrowseForFolderCallback;
	bi.lParam = (LPARAM)tmp.GetBuffer();


	LPITEMIDLIST pidl = SHBrowseForFolder(&bi);
	if (pidl != 0)//if folder selected
	{
		if (SHGetPathFromIDList(pidl, path))
		{
			filedir.SetWindowText(path);
		}
		else
			MessageBox(_T("Не получается получить путь к директории"), _T("Ошибка!"), MB_OK);

		// free memory used
		IMalloc * imalloc = 0;
		if (SUCCEEDED(SHGetMalloc(&imalloc)))
		{
			imalloc->Free(pidl);
			imalloc->Release();
		}
	}


}


void CConanGlueDlg::OnBnClickedButton3()
{

	CString path;
	CFileDialog dlg(FALSE/*Open=TRUE Save=False*/, _T("cad")/*Filename Extension*/, _T("")/*Initial Filename*/, OFN_ENABLESIZING | OFN_EXPLORER/*Flags*/, _T("Conan output file(*.cad)|*.cad||")/*Filetype Filter*/, this/*parent Window*/);
	int z = dlg.DoModal();
	if (z == IDOK)
		path = dlg.GetPathName();
	else
		return;
	OutputFile.SetWindowText(path);
}


void CConanGlueDlg::OnBnClickedButton4()
{
	ProgressBar.ShowWindow(SW_SHOW);
	ProgressBar.SetPos(0);
	ProgressBar.SetStep(1);

	CString path;
	SettingsFile.GetWindowText(path);
	FILE *stream;
	TCHAR line[500];
	if (_wfopen_s(&stream, path, _T("r")) == 0)
	{
		while (fgetws(line, 500, stream))
		{
			///int s=strlen(line);
			//char* l=new char[s+1];
			//strcpy_s(l,s+1,line);
			FileRecord* f = new FileRecord(line);
			if (f->Fname == NULL)
				delete(f);
			else
				FileRecords.push_back(f);
		}
		fclose(stream);
	}
	else
	{
		MessageBox(_T("Не удалось прочитать файл спецификации склейки!"), _T("Ошибка"), MB_OK);
		return;
	}
	CString info;
	CString fname;
	int nRec = FileRecords.size();
	info.Format(_T("%d элементов для обработки"), nRec);
	AddLog(&Log, info);
	//begin reading conan
	ProgressBar.SetRange(0, 3 + nRec);
	ProgressBar.StepIt();

	AddLog(&Log, _T("создаём файл заголовка"));
	filedir.GetWindowText(fname);
	fname.Append(_T("\\"));
	fname.Append(FileRecords.at(0)->Fname);
	ConanData* Conan = ReadConanFile(fname.GetBuffer(), &Log);
	//first one.
	//Make header structure for all the eeg later.
	//remove all extra info from object
	for (int i = 0; i < Conan->Header->nRec; i++)
	{
		for (int j = 0; j < Conan->Header->nChan; j++)
		{
			delete[] Conan->Eeg[i][j];
			delete[] Conan->NotCalibrated[i][j];
		}
		delete[] Conan->Eeg[i];
		delete[] Conan->NotCalibrated[i];
		delete[] Conan->Discr[i];
	}
	delete[] Conan->Eeg;
	delete[] Conan->TimeCreating;
	delete[] Conan->NDataReal2;
	delete[] Conan->NDataReal;
	delete[] Conan->NDataRaw;
	delete[] Conan->Discr;
	delete[] Conan->NotCalibrated;

	//create structure for new object
	Conan->Header->nRec = nRec;
	Conan->Eeg = new float**[nRec];
	Conan->NotCalibrated = new __int16**[nRec];
	Conan->TimeCreating = new __int32[nRec];
	Conan->NDataReal2 = new unsigned __int16[nRec];
	Conan->NDataReal = new unsigned __int16[nRec];
	Conan->NDataRaw = new unsigned __int16[nRec];
	Conan->Discr = new StimulSignal*[nRec];
	//now - fill records!
	ProgressBar.StepIt();
	CString fnameNew;
	for (int CurrentRecNum = 0; CurrentRecNum < nRec; CurrentRecNum++)
	{
		filedir.GetWindowText(fnameNew);
		fnameNew.Append(_T("\\"));
		fnameNew.Append(FileRecords.at(CurrentRecNum)->Fname);
		info = _T("Переписываем файл ") + fnameNew;
		AddLog(&Log, info);
		ConanData* NewConan = ReadConanFile(fnameNew.GetBuffer(), &Log);
		if (NewConan == NULL)
		{
			info = _T("Не удалось прочитать файл ") + fnameNew;
			info.Append(_T(" Склейка не может быть завершена"));
			MessageBox(info);
			ProgressBar.ShowWindow(SW_HIDE);
			return;
		}

		if (NewConan->Header->isNewVersion() && !Conan->Header->isNewVersion())
		{
			info = _T("Версия файла ") + fnameNew + _T(" новее, чем файла ") + fname;
			info.Append(_T(" Склейка не может быть завершена"));
			MessageBox(info);
			delete(NewConan);
			ProgressBar.ShowWindow(SW_HIDE);
			return;
		}

		if (!NewConan->Header->isNewVersion() && Conan->Header->isNewVersion())
		{
			info = _T("Версия файла ") + fnameNew + _T(" старее, чем файла ") + fname;
			info.Append(_T(" Склейка не может быть завершена"));
			MessageBox(info);
			delete(NewConan);
			ProgressBar.ShowWindow(SW_HIDE);
			return;
		}

		int CurrentCopyingRecord = FileRecords.at(CurrentRecNum)->RecNum - 1;//not 1 but 0-based index
		memcpy_s(&Conan->NDataRaw[CurrentRecNum], sizeof(__int16), &NewConan->NDataRaw[CurrentCopyingRecord], sizeof(__int16));
		memcpy_s(&Conan->NDataReal[CurrentRecNum], sizeof(__int16), &NewConan->NDataReal[CurrentCopyingRecord], sizeof(__int16));

		Conan->Eeg[CurrentRecNum] = new float*[NewConan->Header->nChan];
		for (int j = 0; j < NewConan->Header->nChan; j++)
		{
			Conan->Eeg[CurrentRecNum][j] = new float[NewConan->NDataReal[CurrentCopyingRecord]];
			for (int k = 0; k < NewConan->NDataReal[CurrentCopyingRecord]; k++)
			{
				memcpy_s(&Conan->Eeg[CurrentRecNum][j][k], sizeof(float), &NewConan->Eeg[CurrentCopyingRecord][j][k], sizeof(float));
			}
		}

		Conan->NotCalibrated[CurrentRecNum] = new __int16*[NewConan->Header->nChan];
		for (int j = 0; j < NewConan->Header->nChan; j++)
		{
			Conan->NotCalibrated[CurrentRecNum][j] = new __int16[NewConan->NDataReal[CurrentCopyingRecord]];
			for (int k = 0; k < NewConan->NDataReal[CurrentCopyingRecord]; k++)
			{
				memcpy_s(&Conan->NotCalibrated[CurrentRecNum][j][k], sizeof(__int16), &NewConan->NotCalibrated[CurrentCopyingRecord][j][k], sizeof(__int16));
			}
		}

		if (NewConan->Header->discrExist == 1)
		{
			Conan->Discr[CurrentRecNum] = new StimulSignal[NewConan->NDataReal[CurrentCopyingRecord]];
			for (int k = 0; k < NewConan->NDataReal[CurrentCopyingRecord]; k++)
			{
				memcpy_s(&Conan->Discr[CurrentRecNum][k].Elder, sizeof(__int8), &NewConan->Discr[CurrentCopyingRecord][k].Elder, sizeof(__int8));
				memcpy_s(&Conan->Discr[CurrentRecNum][k].Littler, sizeof(__int8), &NewConan->Discr[CurrentCopyingRecord][k].Littler, sizeof(__int8));
			}
		}
		else
		{
			Conan->Discr[CurrentRecNum] = new StimulSignal[NewConan->Header->nChan];
			for (unsigned int k = 0; k < NewConan->Header->nChan; k++)
			{
				Conan->Discr[CurrentRecNum][k].Littler = 0;
				Conan->Discr[CurrentRecNum][k].Elder = 0;
			}
		}

		memcpy_s(&Conan->TimeCreating[CurrentRecNum], sizeof(__int32), &NewConan->TimeCreating[CurrentCopyingRecord], sizeof(__int32));

		memcpy_s(&Conan->NDataReal2[CurrentRecNum], sizeof(__int16), &NewConan->NDataReal2[CurrentCopyingRecord], sizeof(__int16));


		delete(NewConan);
		ProgressBar.StepIt();
	}

	///cleanup
	for (unsigned int i = 0; i < FileRecords.size(); i++)
	{
		delete (FileRecords.at(i));
	}
	FileRecords.erase(FileRecords.begin(), FileRecords.end());

	// Now - save new conan file.
	OutputFile.GetWindowText(fname);
	if (WriteConanFile(fname.GetBuffer(), Conan, &Log) == 0)
	{
		MessageBox(_T("Не удалось записать склеенный файл!"));
	}
	//clear
	delete(Conan);
	ProgressBar.StepIt();
	AddLog(&Log, _T("запись окончена"));
	ProgressBar.ShowWindow(SW_HIDE);
}


void CConanGlueDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	///CDialogEx::OnOK();
}


void CConanGlueDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	//CDialogEx::OnCancel();
}


void CConanGlueDlg::OnClose()
{
	CDialogEx::OnOK();
	//exit(0);
	//	CDialogEx::OnClose();
}
