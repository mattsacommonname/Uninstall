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

#ifndef PREF_WIN_H
#define PREF_WIN_H

#include <FilePanel.h>
#include <RadioButton.h>
#include <TextControl.h>
#include <Window.h>
#include <Box.h>

#include "SettingsFile.h"

class PrefWin :public BWindow
{
	public:
		PrefWin (SettingsFile *, BPoint);
		virtual void MessageReceived (BMessage *);
		virtual bool QuitRequested (void);
	private:
		BFilePanel * file_panel;
		BRadioButton * radio1, * radio2;
		BTextControl * log_dir_text_con;
		SettingsFile * settings;
		BBox * main_box;
};

#endif
