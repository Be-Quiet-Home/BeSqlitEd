#include "BeSqlitEdApp.h"

#include "MainWindow.h"

static const char* kApplicationSignature = "application/x-vnd.BeQuietHome-BeSqlitEd";

BeSqlitEdApp::BeSqlitEdApp(int argc, char** argv)
	:
	BApplication(kApplicationSignature),
	fMainWindow(NULL)
{
	if (argc > 1 && argv[1] != NULL)
		fInitialDatabasePath = argv[1];
}

void
BeSqlitEdApp::ReadyToRun()
{
	const char* path = fInitialDatabasePath.Length() > 0
		? fInitialDatabasePath.String()
		: NULL;

	fMainWindow = new MainWindow(path);
	fMainWindow->Show();
}

int
main(int argc, char** argv)
{
	BeSqlitEdApp app(argc, argv);
	app.Run();
	return 0;
}
