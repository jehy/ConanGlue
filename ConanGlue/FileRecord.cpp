#include "StdAfx.h"
#include "FileRecord.h"


FileRecord::FileRecord(TCHAR* line)
{
	CString l;
  CString f;
	l=line;
	int s=l.Find('\t');
  if(l.Find(_T("File_Name"))!=-1)//found header
  {
    this->Fname=NULL;
    return;
  }
  if(s==-1)
  {
    l.Remove(' ');//string is empty?
    l.Remove('\n');
    l.Remove('\r');
    if(l.GetLength()==0)
    {
      this->Fname=NULL;
      return;
    }
    MessageBox(NULL,_T("—имвол табул€ции не найден в строке файла склейки!"),_T("‘атальна€ ошибка"),MB_OK);
    exit(0);
  }
	this->Fname=new TCHAR[s+1];
	f=l.Left(s);
	wcscpy_s(this->Fname, s + 1, f.GetBuffer());
	this->RecNum=_wtoi(l.Mid(s+1));
}


FileRecord::~FileRecord(void)
{
	delete[] this->Fname;
}
