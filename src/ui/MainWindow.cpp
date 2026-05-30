#include "MainWindow.h"

#include <Application.h>
#include <GroupLayout.h>
#include <StringView.h>

MainWindow::MainWindow()
	:
	BWindow(BRect(100, 100, 640, 420),
		"BeSqlitEd",
		B_TITLED_WINDOW,
		B_ASYNCHRONOUS_CONTROLS | B_QUIT_ON_WINDOW_CLOSE)
{
	SetLayout(new BGroupLayout(B_VERTICAL));

	BStringView* title = new BStringView("title",
		"BeSqlitEd native Haiku skeleton");
	AddChild(title);
}

bool
MainWindow::QuitRequested()
{
	be_app->PostMessage(B_QUIT_REQUESTED);
	return true;
}
