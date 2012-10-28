#include "StdAfx.h"
#include "FileRecord.h"


FileRecord::FileRecord(char* line)
{
	CString l;
  CString f;
	l=line;
	int s=l.Find('\t');
  if(l.Find("File_Name")!=-1)//found header
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
    MessageBox(NULL,"—имвол табул€ции не найден в строке файла склейки!","‘атальна€ ошибка",MB_OK);
    exit(0);
  }
	this->Fname=new char[s+1];
	f=l.Left(s);
	strcpy_s(this->Fname,s+1,f.GetBuffer());
	this->RecNum=atoi(l.Mid(s+1));
}


FileRecord::~FileRecord(void)
{
	delete[] this->Fname;
}
