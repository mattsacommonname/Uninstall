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
#include <String.h>
#include <Roster.h>
#include <AppFileInfo.h>
#include <File.h>

#include "constants.h"
#include "UApp.h"

UApp::UApp(void)
	:SpLocaleApp("application/x-vnd.Uninstall")
{
	settings = new SettingsFile;
	settings->Load();

	BPoint orgin;

	if(settings->FindPoint("orgin", & orgin) != B_OK)
	{
		orgin.Set(150, 100);
		settings->AddPoint("orgin", orgin);
	}

	select_win = new SelectWin(settings, orgin);
	select_win->Show();
}

void UApp::MessageReceived(BMessage * msg)
{
	switch(msg->what)
	{
		//New language
		case MSG_LANGUAGE_CHANGED:
	    {
			settings->Save();
			
			BPoint orgin;
						
			if(settings->FindPoint("orgin", & orgin) != B_OK)
			{
				orgin.Set(150, 100);
				settings->AddPoint("orgin", orgin);
			}

			select_win->Quit();
			select_win = new SelectWin(settings, orgin);
			select_win->Show();

	    	break;
	    } 
		case LOG_DIR_UPDATED:
		{
			select_win->Lock();
			select_win->Show();
			select_win->Unlock();
			break;
		}
		case UNINSTALL_REQUESTED:
		{
			BString log, path, title, temp;

			msg->FindString("log", &log);
			temp = log;
			temp.RemoveLast(" Install Log");
			title << SpTranslate("Uninstalling") << temp;
			msg->FindString("path", & path);
			path << "/" << log;

			BPoint orgin;
			
			settings->FindPoint("orgin", & orgin);
			
			int8 del_non_empty_dir;

			if(settings->FindInt8("del_non_empty_dir", & del_non_empty_dir) != B_OK)
			{
				del_non_empty_dir = DELETE;
				settings->ReplaceInt8("del_non_empty_dir", del_non_empty_dir);
			}

			u_win = new UWin(orgin + BPoint(10, 10), title.String(), path, del_non_empty_dir);

			break;
		}
		case UNINSTALL_COMPLETED:
		{
			select_win->Lock();
			select_win->Show();
			select_win->Unlock();
			break;
		}
		default:
		{
			SpLocaleApp::MessageReceived(msg);
			break;
		}
	}
}

void UApp::AboutRequested()
{
	//Get the name and the version of Uninstall
	BFile FileApp;
	app_info app_InfoTheApp;
	BAppFileInfo AppFileInfo;
	BString StringAbout("");
	version_info version_infoTheApp;
	
	be_app->GetAppInfo(&app_InfoTheApp);
	FileApp.SetTo(&app_InfoTheApp.ref, B_READ_ONLY);
	AppFileInfo.SetTo(&FileApp);
	AppFileInfo.GetVersionInfo(&version_infoTheApp, B_APP_VERSION_KIND);
	
	StringAbout << UNINSTALL_APP_NAME;
	StringAbout << " ";
	StringAbout << version_infoTheApp.major << "." << version_infoTheApp.middle << "." << version_infoTheApp.minor;
	StringAbout << "\n\n";
	StringAbout << SpTranslate("Uninstall is developed by Matt Bishop (mrbish@excite.com) and Alain Laporte (betroll@free.fr)");
	
	BAlert * pAlertAbout = new BAlert("About", StringAbout.String(), SpTranslate("Ok"));
	pAlertAbout->Go();
}

bool UApp::QuitRequested(void)
{
	settings->Save();
	return true;
}
