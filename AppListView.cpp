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

//System Headers
#include <Window.h>

//Application Headers
#include "AppListView.h"
#include "constants.h"

//Constructor
AppListView::AppListView (BRect frame, const char * name)
	:BListView (frame, name)
{

}

void AppListView::SelectionChanged (void)
{
	bool selected = true;

	if(CurrentSelection() < 0)
	{
		selected = false;
	}

	BMessage * msg = new BMessage(SELECTION_CHANGED);
	msg->AddBool("selected", selected);

	Window()->PostMessage(msg);
}
