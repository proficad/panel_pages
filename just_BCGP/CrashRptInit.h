
#pragma once
#include <string>

// Minimal CrashRpt wrapper. No MFC dependency here.
// Link with: Dbghelp.lib and CrashRpt lib (e.g., CrashRpt1403.lib).
// Ensure CrashSender.exe is located next to your EXE at runtime.

bool CR_Install(const std::wstring& as_appName,
                const std::wstring& as_appVersion,
                const std::wstring& as_vendor);

void CR_Uninstall();

// Convenience: attach a file if it exists (e.g., your application log).
bool CR_AddFileIfExists(const std::wstring& as_path,
                        const std::wstring& as_desc,
                        const std::wstring& as_reportNameNoExt,
                        bool ab_makeCopy = true);
