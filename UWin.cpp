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

#include <fstream>
#include <list>

#include <Alert.h>
#include <Application.h>
#include <Button.h>
#include <Directory.h>
#include <Entry.h>

#include "constants.h"
#include "UWin.h"
#include "UApp.h"

using namespace std;

UWin::UWin (BPoint orgin, const char * title, BString log_name, int8 del_non_empty_dir)
: BWindow (BRect (orgin.x, orgin.y, orgin.x + U_WIN_WIDTH, orgin.y + U_WIN_HEIGHT), title, B_TITLED_WINDOW_LOOK, B_MODAL_APP_WINDOW_FEEL, B_NOT_CLOSABLE | B_NOT_ZOOMABLE | B_NOT_RESIZABLE | B_ASYNCHRONOUS_CONTROLS) {
	progress_bar = new BStatusBar (BRect (5, 5, 294, 6), "uninstall_progress", SpTranslate("Removing : "));
	
	BRect rect_u_box(Bounds());
	rect_u_box.right++;
	rect_u_box.bottom++;
	rect_u_box.left--;
	rect_u_box.top--;	
	u_box = new BBox(rect_u_box, "u_box");
	
	u_box->AddChild (progress_bar);
	
	AddChild(u_box);
	
	ifstream log_file (log_name.String ());

	int buf_len = B_PATH_NAME_LENGTH + B_FILE_NAME_LENGTH + 1;
	BString buffer;

	for (int count = 0; count < 4; ++ count) {
		log_file.getline (buffer.LockBuffer (buf_len), buf_len, '\n');
		buffer.UnlockBuffer ();
	}

	list<BString> files;

	while (log_file) {
		log_file.getline (buffer.LockBuffer (buf_len), buf_len, '\n');
		buffer.UnlockBuffer ();

		if (buffer != "") {
			files.push_front (buffer);
			log_file.getline (buffer.LockBuffer (buf_len), buf_len, '\n');
			buffer.UnlockBuffer ();
		} else {
			break;
		}
	}

	progress_bar->SetMaxValue (files.size ());

	Show ();

	Lock ();

	int delta = 0;
	BDirectory cur_dir;
	BEntry cur_entry;
	BString progress;

	for (list<BString>::iterator i = files.begin (); i != files.end (); ++ i) {
		progress = "";
		progress << ((progress_bar->CurrentValue ()) / progress_bar->MaxValue ()) * 100 << "%";

		progress_bar->Update (delta, (* i).String (), progress.String ());

		cur_entry.SetTo ((* i).String ());

		if(cur_entry.IsDirectory ())
		{
			cur_dir.SetTo (& cur_entry);

			if(cur_dir.CountEntries ())
			{
				switch(del_non_empty_dir)
				{
					case NO_DELETE :
					{
						break;
					}
					case PROMPT :
					{
						BString str;

						str << SpTranslate("The directory") << " \"" << * i << "\" " << SpTranslate("is not empty. Do you wish to delete it (and all it's contents)?");
	
						BAlert * p = new BAlert ("prompt!", str.String (), SpTranslate("Delete"), SpTranslate("Delete All"), SpTranslate("Don't Delete"), B_WIDTH_AS_USUAL, B_STOP_ALERT);

						int32 choice = p->Go();
	
						if(choice == 2)
						{
							break;
						}
						else if(choice == 1)
						{
							del_non_empty_dir = DELETE;
						}
					}
					case DELETE :
					{
						BEntry temp_entry;

						while(cur_dir.GetNextEntry (& temp_entry) != B_ENTRY_NOT_FOUND)
						{
							temp_entry.Remove ();
						}

						cur_entry.Remove ();
					}
				}
			}
			else
			{
				cur_entry.Remove ();
			}
		}
		else
		{
			cur_entry.Remove ();
		}

		delta = 1;
	}

	progress_bar->Update(1, SpTranslate("Install Log"), "100%");

	cur_entry.SetTo(log_name.String());
	cur_entry.Remove();

	BAlert * complete = new BAlert ("complete!", SpTranslate("Uninstallation complete."), SpTranslate("Ok"));
	complete->Go ();

	Unlock();

	be_app->PostMessage(UNINSTALL_COMPLETED);

	Quit();
}

bool UWin::QuitRequested (void) {
	return true;
}
