#ifndef BESQLITED_MAIN_WINDOW_H
#define BESQLITED_MAIN_WINDOW_H

#include <Window.h>

class BListView;
class BStringView;

class MainWindow : public BWindow {
public:
	explicit MainWindow(const char* databasePath);

	bool QuitRequested() override;

private:
	void LoadDatabase(const char* path);

	BStringView* fStatusView;
	BListView* fTableListView;
};

#endif
