#include "MainWindow.h"

#include "Database.h"
#include "SchemaReader.h"

#include <Application.h>
#include <GroupLayout.h>
#include <ListView.h>
#include <ScrollView.h>
#include <StringItem.h>
#include <StringView.h>

#include <string>
#include <vector>

MainWindow::MainWindow(const char* databasePath)
	:
	BWindow(BRect(100, 100, 640, 420),
		"BeSqlitEd",
		B_TITLED_WINDOW,
		B_ASYNCHRONOUS_CONTROLS | B_QUIT_ON_WINDOW_CLOSE),
	fStatusView(NULL),
	fTableListView(NULL)
{
	SetLayout(new BGroupLayout(B_VERTICAL));

	fStatusView = new BStringView("status",
		"BeSqlitEd native Haiku skeleton");
	AddChild(fStatusView);

	fTableListView = new BListView("tables");
	BScrollView* scrollView = new BScrollView("tableScrollView",
		fTableListView,
		B_WILL_DRAW,
		false,
		true);
	AddChild(scrollView);

	LoadDatabase(databasePath);
}

bool
MainWindow::QuitRequested()
{
	be_app->PostMessage(B_QUIT_REQUESTED);
	return true;
}

void
MainWindow::LoadDatabase(const char* path)
{
	if (path == NULL || path[0] == '\0') {
		fStatusView->SetText("No database path supplied. Start with: BeSqlitEd <database>");
		return;
	}

	Database database;
	status_t result = database.OpenReadOnly(path);
	if (result != B_OK) {
		BString message("Could not open database: ");
		message << database.LastError();
		fStatusView->SetText(message.String());
		return;
	}

	SchemaReader reader;
	std::vector<std::string> tables;
	std::string error;

	result = reader.ReadTables(database, tables, error);
	if (result != B_OK) {
		BString message("Could not read schema: ");
		message << error.c_str();
		fStatusView->SetText(message.String());
		return;
	}

	BString message("Opened: ");
	message << database.Path();
	message << " — ";
	message << static_cast<int32>(tables.size());
	message << " table(s)";
	fStatusView->SetText(message.String());

	for (std::vector<std::string>::const_iterator it = tables.begin();
			it != tables.end(); ++it) {
		fTableListView->AddItem(new BStringItem(it->c_str()));
	}
}
