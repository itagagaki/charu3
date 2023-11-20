#include "stdafx.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include <vector>
#include "nlomann/json.hpp"
#include "jsonHelper.h"

bool jsonHelper::GetBoolProperty(nlohmann::json& j, const char* key, bool defaultValue)
{
    nlohmann::json::iterator it = j.find(key);
    return (it != j.end() && it.value().is_boolean()) ? it.value().get<bool>() : defaultValue;
}

double jsonHelper::GetNumberProperty(nlohmann::json& j, const char* key, double defaultValue)
{
    nlohmann::json::iterator it = j.find(key);
    return (it != j.end() && it.value().is_number()) ? it.value().get<double>() : defaultValue;
}

std::string jsonHelper::GetStringProperty(nlohmann::json& j, const char* key, std::string defaultValue)
{
    nlohmann::json::iterator it = j.find(key);
    return (it != j.end() && it.value().is_string()) ? it.value().get<std::string>() : defaultValue;
}

CString jsonHelper::GetStringPropertyAsCString(nlohmann::json& j, const char* key, CString defaultValue)
{
    nlohmann::json::iterator it = j.find(key);
    if (it == j.end()) {
        return defaultValue;
    }
    CString cs;
    if (!GetCString(it.value(), cs)) {
        return defaultValue;
    }
    return cs;
}

bool jsonHelper::GetCString(nlohmann::json j, CString& cs)
{
    if (!j.is_string()) {
        return false;
    }
    std::string s = j.get<std::string>();
    const char* cptr = s.c_str();
    int size = MultiByteToWideChar(CP_UTF8, 0, cptr, -1, nullptr, 0);
    wchar_t* wbuf = new wchar_t[size];
    if (!wbuf) {
        return false;
    }
    MultiByteToWideChar(CP_UTF8, 0, cptr, -1, wbuf, size);
    cs = CString(wbuf);
    delete[] wbuf;
    return true;
}

void jsonHelper::AppendCStringArray(nlohmann::json& j, const char* key, std::vector<CString>& list)
{
    nlohmann::json::iterator it = j.find(key);
    if (it != j.end() && it.value().is_array()) {
        nlohmann::json array = it.value();
        for (nlohmann::json& item : array) {
            CString cs;
            if (GetCString(item, cs)) {
                list.push_back(cs);
            }
        }
    }
}
