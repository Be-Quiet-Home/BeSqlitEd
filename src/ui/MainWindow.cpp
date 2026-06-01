#include "MainWindow.h"

#include "Database.h"
#include "SchemaReader.h"

#include <Application.h>
#include <Entry.h>
#include <FilePanel.h>
#include <GroupLayout.h>
#include <ListItem.h>
#include <ListView.h>
#include <Menu.h>
#include <MenuBar.h>
#include <MenuItem.h>
#include <Messenger.h>
#include <Node.h>
#include <Path.h>
#include <ScrollView.h>
#include <StringItem.h>
#include <StringView.h>

#include <string>
#include <vector>

enum {
	kMessageOpenDatabase = 'opdb',
	kMessageDatabaseSelected = 'dbsl'
};

MainWindow::MainWindow(const char* databasePath)
	:
	BWindow(BRect(100, 100, 640, 420),
		"BeSqlitEd",
		B_TITLED_WINDOW,
		B_ASYNCHRONOUS_CONTROLS | B_QUIT_ON_WINDOW_CLOSE),
	fStatusView(NULL),
	fTableListView(NULL),
	fOpenPanel(NULL)
{
	SetLayout(new BGroupLayout(B_VERTICAL));

	BMenuBar* menuBar = new BMenuBar("menuBar");
	BMenu* fileMenu = new BMenu("File");
	fileMenu->AddItem(new BMenuItem("Open...", new BMessage(kMessageOpenDatabase), 'O'));
	menuBar->AddItem(fileMenu);
	AddChild(menuBar);

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

MainWindow::~MainWindow()
{
	delete fOpenPanel;
}

void
MainWindow::MessageReceived(BMessage* message)
{
	switch (message->what) {
		case kMessageOpenDatabase:
			ShowOpenPanel();
			break;

		case kMessageDatabaseSelected:
		{
			entry_ref ref;
			if (message->FindRef("refs", &ref) != B_OK) {
				fStatusView->SetText("No database file selected.");
				break;
			}

			BPath path(&ref);
			if (path.InitCheck() != B_OK) {
				fStatusView->SetText("Could not resolve selected database path.");
				break;
			}

			LoadDatabase(path.Path());
			break;
		}

		default:
			BWindow::MessageReceived(message);
			break;
	}
}

bool
MainWindow::QuitRequested()
{
	be_app->PostMessage(B_QUIT_REQUESTED);
	return true;
}

void
MainWindow::ShowOpenPanel()
{
	if (fOpenPanel == NULL) {
		BMessenger target(this);
		fOpenPanel = new BFilePanel(B_OPEN_PANEL,
			&target,
			NULL,
			B_FILE_NODE,
			false,
			new BMessage(kMessageDatabaseSelected));
	}

	fOpenPanel->Show();
}

void
MainWindow::LoadDatabase(const char* path)
{
	ClearTableList();

	if (path == NULL || path[0] == '\0') {
		fStatusView->SetText("No database open. Choose File > Open...");
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

void
MainWindow::ClearTableList()
{
	while (fTableListView->CountItems() > 0)
		delete fTableListView->RemoveItem((int32)0);
}
