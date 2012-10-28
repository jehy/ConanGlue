#pragma once
class FileRecord
{
public:
	FileRecord(char* line);
	~FileRecord(void);
	char* Fname;
	int RecNum;
};