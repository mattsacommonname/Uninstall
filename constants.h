/*
 * Copyright © 2011 Matthew Bishop, Alain Laporte, Bernard Krummenacher
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#ifndef CONSTANTS_H
#define CONSTANTS_H

#include "UninstallLanguage.h"

const int PREF_WIN_WIDTH      = 335;
const int PREF_WIN_HEIGHT     = 120;
const int SETTINGS_WIN_WIDTH  = 410;
const int SETTINGS_WIN_HEIGHT = 250;
const int U_WIN_WIDTH         = 300;
const int U_WIN_HEIGHT        = 100;

const int8 DELETE    = 0;
const int8 NO_DELETE = 1;
const int8 PROMPT    = 2;

const int32 BROWSE_REQUESTED    = 'BwRq';
const int32 DISPLAY_PREF_WIN    = 'DPWn';
const int32 LOG_DIR_UPDATED     = 'LDUp';
const int32 SELECTION_CHANGED   = 'SlCh';
const int32 UNINSTALL_COMPLETED = 'UiCm';
const int32 UNINSTALL_REQUESTED = 'unIn';
const int32 SHOW_DOCUMENTATION = 'Shdo';
const int32 SHOW_ABOUT_BOX = 'ShAb';

#define LOG_DIR_PATH "/boot/home"
#define UNINSTALL_APP_NAME "Uninstall"
#endif
