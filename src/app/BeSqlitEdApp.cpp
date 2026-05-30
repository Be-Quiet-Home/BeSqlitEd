#include "BeSqlitEdApp.h"

#include "MainWindow.h"

static const char* kApplicationSignature = "application/x-vnd.BeQuietHome-BeSqlitEd";

BeSqlitEdApp::BeSqlitEdApp()
	:
	BApplication(kApplicationSignature),
	fMainWindow(NULL)
{
}

void
BeSqlitEdApp::ReadyToRun()
{
	fMainWindow = new MainWindow();
	fMainWindow->Show();
}

int
main()
{
	BeSqlitEdApp app;
	app.Run();
	return 0;
}
