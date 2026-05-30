#ifndef BESQLITED_APP_H
#define BESQLITED_APP_H

#include <Application.h>

class MainWindow;

class BeSqlitEdApp : public BApplication {
public:
	BeSqlitEdApp();

	void ReadyToRun() override;

private:
	MainWindow* fMainWindow;
};

#endif
