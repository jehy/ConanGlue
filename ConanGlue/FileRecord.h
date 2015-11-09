#pragma once
class FileRecord
{
public:
	FileRecord(TCHAR* line);
	~FileRecord(void);
	TCHAR* Fname;
	int RecNum;
};