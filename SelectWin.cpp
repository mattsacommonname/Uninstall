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

//System headers
#include <Alert.h>
#include <Application.h>
#include <Button.h>
#include <Directory.h>
#include <Entry.h>
#include <Menu.h>
#include <MenuBar.h>
#include <MenuItem.h>
#include <Roster.h>
#include <ScrollView.h>

//Application headers
#include "constants.h"
#include "PrefWin.h"
#include "SelectWin.h"
#include "UApp.h"

SelectWin::SelectWin(SettingsFile * s, BPoint orgin)
	:BWindow
		(
			BRect(orgin.x, orgin.y, orgin.x + SETTINGS_WIN_WIDTH, orgin.y + SETTINGS_WIN_HEIGHT),
			UNINSTALL_APP_NAME,
			B_TITLED_WINDOW,
			B_NOT_ZOOMABLE | B_NOT_RESIZABLE | B_ASYNCHRONOUS_CONTROLS
		)
{
	settings = s;
		
	//The view
	BRect rect_select_box(Bounds());
	rect_select_box.right++;
	rect_select_box.bottom++;
	rect_select_box.left--;
	rect_select_box.top--;	
	select_box = new BBox(rect_select_box, "select_box");	

	BString log_dir_path;

	if(settings->FindString("log_dir", & log_dir_path) != B_OK)
	{
		log_dir_path = LOG_DIR_PATH;
		settings->AddString("log_dir", log_dir_path);
		settings->Save();
	}

	BEntry dir_check(log_dir_path.String(), true);

	if(!dir_check.Exists())
	{
		BString str;
		str << SpTranslate("Directory") << " \"" << log_dir_path << "\" " << SpTranslate("does not exist.\nReseting log file directory to") << " \"" << LOG_DIR_PATH << "\"";
		BAlert * a = new BAlert("alert!", str.String(), SpTranslate("Ok"));
		a->Go();

		log_dir_path = LOG_DIR_PATH;
		settings->ReplaceString("log_dir", log_dir_path);
		settings->Save();
	}

	//The list
	BRect temp_rect = CreateMenu();
	app_list_view = new AppListView(BRect(10, temp_rect.bottom + 10, 320 - B_V_SCROLL_BAR_WIDTH, 240), "app_menu_field");
	select_box->AddChild(new BScrollView("app_list_scroll_view", app_list_view, B_FOLLOW_ALL, B_WILL_DRAW, false, true));
	
	//The "Unistall" button
	u_button = new BButton(BRect(330, 120, 400, 130), "u_button", SpTranslate("Uninstall"), new BMessage(UNINSTALL_REQUESTED));
	u_button->MakeDefault(true);
	u_button->SetEnabled(false);
	select_box->AddChild(u_button);
	
	//Add the view
	AddChild(select_box);
}

BRect SelectWin::CreateMenu()
{
	//Bar
	BMenuBar * pMenuBar = new BMenuBar(BRect(0, 0, SETTINGS_WIN_WIDTH - 1, 1), "menu_bar");
	
	//File
	BMenu * pMenuFile = new BMenu(SpTranslate("File"));
	((UApp*)be_app)->AddLanguageMenuItem(pMenuFile); //Language menu
	pMenuFile->AddSeparatorItem();
	pMenuFile->AddItem(new BMenuItem(SpTranslate("Preferences..."), new BMessage(DISPLAY_PREF_WIN)));
	pMenuFile->AddSeparatorItem();
	pMenuFile->AddItem(new BMenuItem(SpTranslate("Quit"), new BMessage(B_QUIT_REQUESTED), 'Q'));
	pMenuBar->AddItem(pMenuFile);
	
	//Help
	BMenu * pMenuHelp = new BMenu(SpTranslate("Help"));
	pMenuHelp->AddItem(new BMenuItem(SpTranslate("Documentation"), new BMessage(SHOW_DOCUMENTATION)));
	pMenuHelp->AddSeparatorItem();
	pMenuHelp->AddItem(new BMenuItem(SpTranslate("About…"), new BMessage(SHOW_ABOUT_BOX)));
	pMenuBar->AddItem(pMenuHelp);
	
	//Add the bar
	AddChild(pMenuBar);
	
	return pMenuBar->Frame();
}

void SelectWin::FrameMoved(BPoint orgin)
{
	settings->ReplacePoint("orgin", orgin);
}

void SelectWin::MessageReceived(BMessage * msg)
{
	switch(msg->what)
	{
		//New language
		case MSG_LANGUAGE_CHANGED:
	    {
	    	cout << "New language!!!" << endl;
	    	break;
	    } 
		case DISPLAY_PREF_WIN:
		{
			BPoint orgin;

			settings->FindPoint("orgin", & orgin);

			pref_win = new PrefWin(settings, orgin + BPoint(10, 10));
			pref_win->Show();

			break;
		}
		case SELECTION_CHANGED:
		{
			bool selected;

			msg->FindBool("selected", & selected);

			u_button->SetEnabled(selected);
			break;
		}
		case UNINSTALL_REQUESTED:
		{
			Lock();
			int32 log_index = app_list_view->CurrentSelection();
			Unlock();

			if(log_index < 0)
			{
				BAlert * a = new BAlert("alert!", SpTranslate("Choose a program to uninstall first."), SpTranslate("Ok"));
				a->Go();
				break;
			}

			BString str;
			str << SpTranslate("Preparing to uninstall") << " " << log_files[log_index].String() << "\n" << SpTranslate("Do you really want to continue?");
			str.RemoveLast(" Install Log");

			BAlert * a = new BAlert("alert!", str.String(), SpTranslate("Cancel"), SpTranslate("Ok"), NULL, B_WIDTH_AS_USUAL, B_EVEN_SPACING, B_WARNING_ALERT);
			a->SetShortcut(0, B_ESCAPE);
			if(a->Go() == 1)
			{
				BMessage * u_msg = new BMessage(UNINSTALL_REQUESTED);

				u_msg->AddString("log", log_files[log_index].String());

				BString log_dir_path;
				settings->FindString("log_dir", &log_dir_path);
				u_msg->AddString("path", log_dir_path);
				
				log_files.erase(&log_files[log_index]);
				
				be_app->PostMessage(u_msg);
			}

			break;
		}
		//Show the documentation "Uninstall Read Me.html" file
		case SHOW_DOCUMENTATION:
		{
			app_info app_infoTheApp;
			BEntry EntryDocFile;
			BDirectory DirectoryDocFile;
			entry_ref RefDocFile;
			
			be_app->GetAppInfo(&app_infoTheApp);
			EntryDocFile.SetTo(&app_infoTheApp.ref);
			
			EntryDocFile.GetParent(&EntryDocFile);
			DirectoryDocFile.SetTo(&EntryDocFile);
			if(DirectoryDocFile.FindEntry(SpTranslate("Uninstall Read Me Eng.html"), &EntryDocFile) == B_ENTRY_NOT_FOUND)
			{
				BString StringNoDocumentation;
				StringNoDocumentation << SpTranslate("The file") << "\"" << SpTranslate("Uninstall Read Me Eng.html") << "\" " << SpTranslate("is not present in Uninstall directory.");
				BAlert * pAlert = new BAlert("NoDoc", StringNoDocumentation.String(), SpTranslate("Ok"), NULL, NULL, B_WIDTH_AS_USUAL, B_STOP_ALERT);
				pAlert->Go();
			}			
			EntryDocFile.GetRef(&RefDocFile);
			
			be_roster->Launch(&RefDocFile);
			break;
		}
		case SHOW_ABOUT_BOX:
		{
			be_app->PostMessage(B_ABOUT_REQUESTED);
			break;
		}
		default:
		{
			BWindow::MessageReceived(msg);
			break;
		}		
		Unlock();
	}
}

void SelectWin::Show(void)
{
	BString log_dir_path;

	BAlert * a;

	settings->FindString("log_dir", & log_dir_path);

	BDirectory log_dir(log_dir_path.String());

	BStringItem * item;

	while(!app_list_view->IsEmpty())
	{
		item = (BStringItem *)app_list_view->RemoveItem((int32)0);
		delete item;
	}

	BEntry * entry = new BEntry;
	char temp_str [B_FILE_NAME_LENGTH];
	BString name_str;
	int num_entries = log_dir.CountEntries();
	
	for(int i = 0; i < num_entries; ++ i)
	{
		log_dir.GetNextEntry(entry);
		entry->GetName(temp_str);
		
		name_str = temp_str;
		if(name_str.FindFirst(" Install Log") != B_ERROR)
		{
			log_files.push_back(name_str);
			name_str.RemoveLast(" Install Log");
			
			item = new BStringItem(name_str.String());
			app_list_view->AddItem(item);
		}
	}
	
	BWindow::Show();
}

//Quit the application
bool SelectWin::QuitRequested(void)
{
	be_app->PostMessage(B_QUIT_REQUESTED, be_app);
	return true;
}
