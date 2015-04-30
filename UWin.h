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

#ifndef U_WIN_H
#define U_WIN_H

#include <StatusBar.h>
#include <String.h>
#include <Window.h>
#include <Box.h>

#include "SettingsFile.h"

class UWin
 : public BWindow {
	public:
		UWin (BPoint, const char *, BString, int8);
		virtual bool QuitRequested (void);
	private:
		BStatusBar * progress_bar;
		BBox * u_box;
};

#endif
