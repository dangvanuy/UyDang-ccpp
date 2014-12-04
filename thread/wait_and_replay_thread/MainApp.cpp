#include <stdio.h>
#include "remote.h"
#include "Threads.h"
#include "Engineer.h"
/*
* control thread via remote from user
*/

void HandleMain(unsigned long key)
{
	switch (key) {
		case Key_ENTER:
			printf("GUI >> Enter\n");
			break;
		case Key_LEFT:
			printf("GUI >> Left \n");
			break;
		case Key_RIGHT:
			printf("GUI >> Right \n");
			break;
		case Key_UP:
			printf("GUI >> Up\n");
			break;
		case Key_DOWN:
			printf("GUI >> Down\n");
			break;
		case Key_ZERO:
			UnregisterKeyEpg();
			break;
		case Key_ONE:
			ReRunGetEpg();
			break;
		default:
			printf("GUI >> 0x%08lx\n", key);
			break;
	}

}


int main()
{
	printf("Main App\n");
	//init remote
	set_handle((void*)HandleMain);
	run_remote();	
	
	StartHandleEngineer();
	
	StartGetEpg();
	
	
	//StopHandleEngineer();
	stop_remote();
	return 0;
}

