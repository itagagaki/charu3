#pragma once

#include "stdafx.h"

#include <vector>
#include "nlomann/json.hpp"

namespace jsonHelper {

    bool GetBoolProperty(nlohmann::json& j, const char* key, bool defaultValue);
    double GetNumberProperty(nlohmann::json& j, const char* key, double defaultValue);
    std::string GetStringProperty(nlohmann::json& j, const char* key, std::string defaultValue);
    CString GetStringPropertyAsCString(nlohmann::json& j, const char* key, CString defaultValue);
    bool GetCString(nlohmann::json j, CString& cs);
    void AppendCStringArray(nlohmann::json& j, const char* key, std::vector<CString>& list);

} // namespace jsonHelper
