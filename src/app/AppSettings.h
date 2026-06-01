#ifndef BESQLITED_APP_SETTINGS_H
#define BESQLITED_APP_SETTINGS_H

#include <String.h>
#include <SupportDefs.h>

class BPath;

class AppSettings {
public:
	AppSettings();

	status_t Load();
	status_t Save() const;

	const char* LastDatabasePath() const;
	void SetLastDatabasePath(const char* path);

private:
	status_t _GetSettingsPath(BPath& path) const;

	BString fLastDatabasePath;
};

#endif
