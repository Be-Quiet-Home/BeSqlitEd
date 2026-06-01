#ifndef BESQLITED_MAIN_WINDOW_H
#define BESQLITED_MAIN_WINDOW_H

#include <Window.h>

class BFilePanel;
class BListView;
class BStringView;

class MainWindow : public BWindow {
public:
	explicit MainWindow(const char* databasePath);
	~MainWindow();

	void MessageReceived(BMessage* message) override;
	bool QuitRequested() override;

private:
	void ShowOpenPanel();
	void LoadDatabase(const char* path);
	void ClearTableList();

	BStringView* fStatusView;
	BListView* fTableListView;
	BFilePanel* fOpenPanel;
};

#endif
