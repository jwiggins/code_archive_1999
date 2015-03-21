#include <Application.h>
#include <Window.h>
#include <stdio.h>

#define MAGIC_WORKSPACE_WINDOW_FLAG 0x00008000

int main(void)
{
	BApplication app("application/x-workspace-flag-finder");
	BWindow *window = new BWindow(BRect(20,20,249,249), "Workspaces", B_TITLED_WINDOW_LOOK, B_NORMAL_WINDOW_FEEL, MAGIC_WORKSPACE_WINDOW_FLAG);
	window->Show();
	app.Run();
	return B_OK;
}