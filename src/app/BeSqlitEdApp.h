#ifndef BESQLITED_APP_H
#define BESQLITED_APP_H

#include <Application.h>
#include <String.h>

class MainWindow;

class BeSqlitEdApp : public BApplication {
public:
	BeSqlitEdApp(int argc, char** argv);

	void ReadyToRun() override;

private:
	MainWindow* fMainWindow;
	BString fInitialDatabasePath;
};

#endif
