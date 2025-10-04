#include "stdafx.h"

#include "CrashRptInit.h"
#include <windows.h>
#include <shellapi.h>
#include <shlobj.h>
#include <crashrpt.h>   // CrashRpt API
#include <filesystem>

#pragma comment(lib, "Dbghelp.lib")

namespace fs = std::filesystem;

static std::wstring gs_saveDir;

static std::wstring ExpandEnv(const std::wstring& a_in)
{
    DWORD li_needed = ExpandEnvironmentStringsW(a_in.c_str(), nullptr, 0);
    if (li_needed == 0) return a_in;
    std::wstring ls_out(li_needed, L'\0');
    ExpandEnvironmentStringsW(a_in.c_str(), ls_out.data(), li_needed);
    if (!ls_out.empty() && ls_out.back() == L'\0') ls_out.pop_back();
    return ls_out;
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

bool CR_Install(const std::wstring& as_appName,
                const std::wstring& as_appVersion,
                const std::wstring& as_vendor)
{
    // Create save dir: %LOCALAPPDATA%\Vendor\AppName\crash_reports
    std::wstring ls_base = GetLocalAppDataPath();
    if (ls_base.empty()) return false;

    gs_saveDir = ls_base + L"\\" + as_vendor + L"\\" + as_appName + L"\\crash_reports";
    fs::create_directories(gs_saveDir);

    CR_INSTALL_INFOW l_info;
    memset(&l_info, 0, sizeof(l_info));
    l_info.cb = sizeof(l_info);
    l_info.pszAppName = as_appName.c_str();
    l_info.pszAppVersion = as_appVersion.c_str();

    // If CrashSender.exe is next to the EXE, leaving pszCrashSenderPath = nullptr is OK.
    l_info.pszCrashSenderPath = nullptr;

    // Flags: all handlers, show UI, store zips locally, send queued on next launch.
    l_info.dwFlags = CR_INST_ALL_POSSIBLE_HANDLERS
                   | CR_INST_SHOW_ADDITIONAL_INFO_FIELDS
                   | CR_INST_STORE_ZIP_ARCHIVES
                   | CR_INST_SEND_QUEUED_REPORTS;

    // Save directory for ZIPs
    l_info.pszErrorReportSaveDir = gs_saveDir.c_str();

    // Optional UI strings / privacy policy could be set here (left default for minimal sample)
    // l_info.pszPrivacyPolicyURL = L"https://example.com/privacy";

    // TODO (optional): enable HTTP or SMTP sending here (see CrashRpt docs)
    // l_info.pszUrl = L"https://your-endpoint.example.com/crash";
    // l_info.uPriorities[CR_HTTP] = 1;

    int li_res = crInstallW(&l_info);
    if (li_res != 0)
    {
        // You can use crGetLastErrorMsgW to retrieve the error text.
        return false;
    }

    // Add a basic app info property visible in CrashRpt XML
    crAddPropertyW(L"Vendor", as_vendor.c_str());

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
    std::error_code ec;
    if (!fs::exists(as_path, ec)) return false;

    // The 2nd parameter is report file name (no path). CrashRpt will copy/pack it.
    // The 3rd parameter is description shown in the UI.
    int li_res = crAddFile2W(as_path.c_str(),
                             as_reportNameNoExt.c_str(),
                             as_desc.c_str(),
                             ab_makeCopy);
    return (li_res == 0);
}
