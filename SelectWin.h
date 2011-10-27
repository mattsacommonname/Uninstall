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
