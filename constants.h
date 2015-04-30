/*
 * Copyright © 2015 Matthew Bishop, Alain Laporte, Bernard Krummenacher
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
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
