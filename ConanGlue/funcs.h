#include "Conandata.h"

BOOL WriteConanFile(TCHAR* fname, ConanData* Conan,CEdit* log);
ConanData* ReadConanFile(TCHAR* fname,CEdit* log);
bool FileExists(CString strFilename);
void AddLog(CEdit*log,CString z);

