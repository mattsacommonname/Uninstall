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

#include <Alert.h>
#include <Application.h>
#include <Button.h>
#include <Entry.h>
#include <Path.h>
#include <String.h>
#include <Font.h>

#include "constants.h"
#include "PrefWin.h"
#include "UApp.h"

#include <iostream.h>

PrefWin::PrefWin (SettingsFile * s, BPoint orgin)
: BWindow (BRect (orgin.x, orgin.y, orgin.x + PREF_WIN_WIDTH, orgin.y + PREF_WIN_HEIGHT), SpTranslate("Preferences"), B_TITLED_WINDOW_LOOK, B_MODAL_APP_WINDOW_FEEL, B_NOT_ZOOMABLE | B_NOT_RESIZABLE | B_ASYNCHRONOUS_CONTROLS) {
	settings = s;

	BString log_dir_path;
	BFont FontWidth(be_plain_font);
	
	BRect rect_main_box(Bounds());
	rect_main_box.right++;
	rect_main_box.bottom++;
	rect_main_box.left--;
	rect_main_box.top--;	
	main_box = new BBox(rect_main_box, "main_box");
	
	settings->FindString("log_dir", &log_dir_path);

	log_dir_text_con = new BTextControl(BRect(5, 8, 265, 23), "log_dir_text_con", SpTranslate("Log files directory : "), log_dir_path.String(), NULL);
	log_dir_text_con->SetDivider(FontWidth.StringWidth(SpTranslate("Log files directory : ")));
	main_box->AddChild(log_dir_text_con);

	BButton * browse = new BButton (BRect (270, 5, 330, 20), "browse_button", SpTranslate("Browse..."), new BMessage (BROWSE_REQUESTED));
//	browse->SetEnabled (false);
	main_box->AddChild (browse);

	BBox * box = new BBox (BRect (5, 35, (PREF_WIN_WIDTH - 5), (PREF_WIN_HEIGHT - 5)));
	box->SetLabel (SpTranslate("When a directory isn't empty..."));
	box->SetViewColor (255, 255, 255);

	int8 del_non_empty_dir;

	if (settings->FindInt8 ("del_non_empty_dir", & del_non_empty_dir) != B_OK) {
		del_non_empty_dir = DELETE;
		settings->AddInt8 ("del_non_empty_dir", del_non_empty_dir);
	}

	radio1 = new BRadioButton (BRect (5, 18, 200, 33), "delete_radio", SpTranslate("delete it"), NULL);
	if (del_non_empty_dir == DELETE) {
		radio1->SetValue (1);
	}
	box->AddChild (radio1);

	radio2 = new BRadioButton (BRect (5, 38, 200, 53), "no_delete_radio", SpTranslate("don't delete it"), NULL);
	if (del_non_empty_dir == NO_DELETE) {
		radio2->SetValue (1);
	}
	box->AddChild (radio2);

	BRadioButton * radio3 = new BRadioButton (BRect (5, 58, 200, 68), "delete_radio", SpTranslate("prompt me"), NULL);
	if (del_non_empty_dir == PROMPT) {
		radio3->SetValue (1);
	}
	box->AddChild (radio3);

	main_box->AddChild (box);
	
	AddChild(main_box);
}

void PrefWin::MessageReceived (BMessage * msg)
{
	switch (msg->what)
	{
		case B_CANCEL :
		{
			break;
		}
		case B_REFS_RECEIVED:
		{
			//Get the new log file
			entry_ref ref;
			msg->FindRef("refs", &ref);
			
			BPath PathLogDir(&ref);
			
			//Set the BTextControl
			log_dir_text_con->SetText(PathLogDir.Path());
			
			break;
		}
		//Show the BFilePanel
		case BROWSE_REQUESTED:
		{
			//Get the directory wich in the settings
			BString log_dir_path;
			settings->FindString("log_dir", &log_dir_path);
			BEntry log_dir(log_dir_path.String());
			
			//Verify if the directory exist
			if(log_dir.Exists() != true)
			{
				log_dir.SetTo(LOG_DIR_PATH);
			}
			
			//get the entry_ref
			entry_ref log_dir_ref;
			log_dir.GetRef(&log_dir_ref);
			
			//Create a BMessenger on the window
			BMessenger MessengerThis(this);
			
			file_panel = new BFilePanel(B_OPEN_PANEL, &MessengerThis, &log_dir_ref, B_DIRECTORY_NODE, false, NULL, NULL, true);
			file_panel->Show();
			
			break;
		}
		default:
		{
			BWindow::MessageReceived(msg);
			break;
		}
	}
}

bool PrefWin::QuitRequested (void)
{
	if(radio1->Value())
	{
		settings->ReplaceInt8("del_non_empty_dir", DELETE);
	}
	else	if(radio2->Value())
	{
			settings->ReplaceInt8("del_non_empty_dir", NO_DELETE);
	}
	else
	{
		settings->ReplaceInt8("del_non_empty_dir", PROMPT);
	}
	
	BString log_dir_path = log_dir_text_con->Text();
	BEntry dir_check(log_dir_path.String(), true);

	if(!dir_check.Exists ())
	{
		BString str;
		str << SpTranslate("Directory") << " \"" << log_dir_path << "\" " << SpTranslate("does not exist.\nPlease enter another directory.");
		BAlert * a = new BAlert ("alert!", str.String (), SpTranslate("Ok"));
		a->Go ();

		log_dir_path = LOG_DIR_PATH;
		settings->ReplaceString("log_dir", log_dir_path);
		settings->Save();

		return false;
	}

	settings->ReplaceString("log_dir", log_dir_path);
	settings->Save();

	be_app->PostMessage(LOG_DIR_UPDATED);

	return true;
}
