#include "Conandata.h"

BOOL WriteConanFile(char*fname, ConanData* Conan,CEdit* log);
ConanData* ReadConanFile(char* fname,CEdit* log);
bool FileExists(CString strFilename);
void AddLog(CEdit*log,CString z);

