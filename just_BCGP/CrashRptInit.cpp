#include "stdafx.h"
// CrashRptInit.cpp – wrapper for CrashRpt with SILENT mode by default
// Toggle behavior by changing CR_SILENT_MODE to 0 (interactive)
//
// Public API (pair with your existing CrashRptInit.h):
//   bool CR_Install(const std::wstring& as_appName,
//                   const std::wstring& as_appVersion,
//                   const std::wstring& as_vendor);
//   void CR_Uninstall();
//   bool CR_AddFileIfExists(const std::wstring& as_path,
//                           const std::wstring& as_desc,
//                           const std::wstring& as_reportNameNoExt,
//                           bool ab_makeCopy = true);
//
// Notes:
// - Default is *silent catcher*: no UI, store ZIP locally, don't send.
// - Set CR_SILENT_MODE to 0 to switch to interactive (dialog + optional queued send).
// - Language file is resolved next to the EXE (crashrpt_lang.ini). Pass absolute path to avoid lookup issues.
// - Report directory: %LOCALAPPDATA%\\<vendor>\\<app>\\crash_reports
// - Dependencies next to your EXE: CrashRpt1403.dll, CrashSender1403.exe, zlib1.dll, crashrpt_lang.ini

#include <windows.h>
#include <shlobj.h>   // SHGetKnownFolderPath
#include <shellapi.h>
#include <crashrpt.h>
#include <string>

#pragma comment(lib, "Dbghelp.lib")

// =============================
// Configuration toggles
// =============================
#ifndef CR_SILENT_MODE
#define CR_SILENT_MODE 0   // 1 = silent ZIP to disk, 0 = interactive dialog/send
#endif

// Optional: emulate a crash right after successful install (for testing)
#ifndef CR_EMULATE_ON_START
#define CR_EMULATE_ON_START 0
#endif

// =============================
// Helpers (no <filesystem> required)
// =============================
static std::wstring g_saveDir;      // %LOCALAPPDATA%\vendor\app\crash_reports
static std::wstring g_langPath;     // <module_dir>\crashrpt_lang.ini

static void DbgOut(const wchar_t* as_msg)
{
    OutputDebugStringW(as_msg);
}

static std::wstring GetModuleDir()
{
    wchar_t l_path[MAX_PATH] = { 0 };
    GetModuleFileNameW(nullptr, l_path, MAX_PATH);
    std::wstring s(l_path);
    size_t pos = s.find_last_of(L"\\/");
    return (pos == std::wstring::npos) ? L"." : s.substr(0, pos);
}

static std::wstring GetLocalAppDataPath()
{
    PWSTR lp_path = nullptr;
    std::wstring ls_ret;
    if (SUCCEEDED(SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, nullptr, &lp_path)))
    {
        ls_ret = lp_path;
        CoTaskMemFree(lp_path);
    }
    return ls_ret;
}

static bool EnsureDirectoryRecursive(const std::wstring& as_dir)
{
    // Use SHCreateDirectoryExW to avoid C++17 filesystem dependency
    int rc = SHCreateDirectoryExW(nullptr, as_dir.c_str(), nullptr);
    if (rc == ERROR_SUCCESS || rc == ERROR_ALREADY_EXISTS) return true;
    if (rc == ERROR_FILE_EXISTS) return true;
    return false;
}

static bool FileExists(const std::wstring& as_path)
{
    DWORD attr = GetFileAttributesW(as_path.c_str());
    return (attr != INVALID_FILE_ATTRIBUTES && !(attr & FILE_ATTRIBUTE_DIRECTORY));
}

// =============================
// Public API
// =============================

bool CR_Install(const std::wstring& as_appName,
    const std::wstring& as_appVersion,
    const std::wstring& as_vendor)
{
    // Resolve language file next to the EXE
    g_langPath = GetModuleDir() + L"\\crashrpt_lang.ini";

    if (!FileExists(g_langPath))
    {
        DbgOut(L"[CrashRpt] Warning: crashrpt_lang.ini not found next to EXE.\n");
        // We proceed anyway; some builds still require it, so better to have it there.
    }

    // Build save directory under LocalAppData
    std::wstring base = GetLocalAppDataPath();
    if (base.empty())
    {
        DbgOut(L"[CrashRpt] ERROR: could not resolve %LOCALAPPDATA%\n");
        return false;
    }

    g_saveDir = base + L"\\" + as_vendor + L"\\" + as_appName + L"\\crash_reports";
    if (!EnsureDirectoryRecursive(g_saveDir))
    {
        DbgOut(L"[CrashRpt] ERROR: could not create crash_reports directory.\n");
        return false;
    }

    CR_INSTALL_INFOW info = {};
    info.cb = sizeof(info);
    info.pszAppName = as_appName.c_str();
    info.pszAppVersion = as_appVersion.c_str();
    info.pszLangFilePath = g_langPath.c_str();  // absolute path prevents lookup/version issues
    info.pszErrorReportSaveDir = g_saveDir.c_str();

#if CR_SILENT_MODE
    // SILENT: no UI, do not send, just store ZIP locally
    info.dwFlags = CR_INST_ALL_POSSIBLE_HANDLERS
        | CR_INST_DONT_SEND_REPORT
        | CR_INST_STORE_ZIP_ARCHIVES
        | CR_INST_NO_GUI;
#else
    // INTERACTIVE: show dialog; you can add HTTP/SMTP transport here
    info.dwFlags = CR_INST_ALL_POSSIBLE_HANDLERS
        | CR_INST_SHOW_ADDITIONAL_INFO_FIELDS
        | CR_INST_SEND_QUEUED_REPORTS; // if you later enable HTTP/SMTP

    // Force HTTP upload (disable email transports explicitly)
    info.pszUrl = L"https://vecchiastrega.proficad.net/receive_crash.php?token=CU9EqcfVFix7JJ4AU45";

    // 1) disable ALL transports first
    memset(info.uPriorities, 0, sizeof(info.uPriorities));
    // 2) enable only HTTP
    info.uPriorities[CR_HTTP] = 1;

    // 3) make sure email is OFF (both SMTP and Simple MAPI)
    info.pszEmailTo = nullptr;
    info.pszSmtpProxy = nullptr;
    info.pszSmtpLogin = nullptr;
    info.pszSmtpPassword = nullptr;
#endif

    int r = crInstallW(&info);
    if (r != 0)
    {
        wchar_t msg[1024] = { 0 };
        crGetLastErrorMsgW(msg, 1024);
        DbgOut(L"[CrashRpt] crInstallW FAILED:\n");
        DbgOut(msg);
        DbgOut(L"\n");
        return false;
    }

    // Simple metadata example
    crAddPropertyW(L"Vendor", as_vendor.c_str());

#if CR_EMULATE_ON_START
    // Deterministic test that the pipeline works
    crEmulateCrash(CR_SEH_EXCEPTION);
#endif

    return true;
}

void CR_Uninstall()
{
    crUninstall();
}

bool CR_AddFileIfExists(const std::wstring& as_path,
    const std::wstring& as_desc,
    const std::wstring& as_reportNameNoExt,
    bool ab_makeCopy)
{
    if (!FileExists(as_path)) return false;

    int rc = crAddFile2W(as_path.c_str(),
        as_reportNameNoExt.c_str(),
        as_desc.c_str(),
        ab_makeCopy);
    return (rc == 0);
}
