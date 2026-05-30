#ifndef BESQLITED_MAIN_WINDOW_H
#define BESQLITED_MAIN_WINDOW_H

#include <Window.h>

class MainWindow : public BWindow {
public:
	MainWindow();

	bool QuitRequested() override;
};

#endif
