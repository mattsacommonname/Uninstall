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

#ifndef SELECT_WIN_H
#define SELECT_WIN_H

#include <vector>

#include <Button.h>
#include <ListView.h>
#include <String.h>
#include <Box.h>
#include <Window.h>

#include "AppListView.h"
#include "PrefWin.h"
#include "SettingsFile.h"

using namespace std;

class SelectWin :public BWindow
{
	public:
		SelectWin (SettingsFile *, BPoint);
		virtual void FrameMoved (BPoint);
		virtual void MessageReceived (BMessage *);
		virtual void Show (void);
		virtual bool QuitRequested (void);
		
	private:
		//Functions
		BRect CreateMenu();
		
		//Members	
		vector<BString> log_files;
		AppListView * app_list_view;
		BButton * u_button;
		PrefWin * pref_win;
		SettingsFile * settings;
		BBox * select_box;
};

#endif
