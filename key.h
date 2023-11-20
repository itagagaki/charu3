#pragma once

#include "stdafx.h"

namespace KeyHelper {

    UINT HotkeyToMod(UINT hotkey);
    UINT ModToHotkey(UINT mod);
    UINT ModToVK(UINT mod);
    CString GetKeyName(UINT uVkCode, HKL keyLayout);

} // namespace KeyHelper