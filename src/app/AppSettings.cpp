#include "AppSettings.h"

#include <Directory.h>
#include <File.h>
#include <FindDirectory.h>
#include <Message.h>
#include <Path.h>

static const uint32 kSettingsMessage = 'bsst';
static const char* kSettingsDirectoryName = "BeSqlitEd";
static const char* kSettingsFileName = "settings";
static const char* kLastDatabasePathField = "last_database_path";

AppSettings::AppSettings()
{
}

status_t
AppSettings::Load()
{
	BPath path;
	status_t result = _GetSettingsPath(path);
	if (result != B_OK)
		return result;

	BFile file(path.Path(), B_READ_ONLY);
	result = file.InitCheck();
	if (result != B_OK)
		return result;

	BMessage message;
	result = message.Unflatten(&file);
	if (result != B_OK)
		return result;

	const char* lastDatabasePath = NULL;
	if (message.FindString(kLastDatabasePathField, &lastDatabasePath) == B_OK)
		fLastDatabasePath = lastDatabasePath;

	return B_OK;
}

status_t
AppSettings::Save() const
{
	BPath path;
	status_t result = _GetSettingsPath(path);
	if (result != B_OK)
		return result;

	BFile file(path.Path(), B_WRITE_ONLY | B_CREATE_FILE | B_ERASE_FILE);
	result = file.InitCheck();
	if (result != B_OK)
		return result;

	BMessage message(kSettingsMessage);
	message.AddString(kLastDatabasePathField, fLastDatabasePath.String());

	return message.Flatten(&file);
}

const char*
AppSettings::LastDatabasePath() const
{
	return fLastDatabasePath.String();
}

void
AppSettings::SetLastDatabasePath(const char* path)
{
	if (path != NULL)
		fLastDatabasePath = path;
	else
		fLastDatabasePath = "";
}

status_t
AppSettings::_GetSettingsPath(BPath& path) const
{
	status_t result = find_directory(B_USER_SETTINGS_DIRECTORY, &path, true);
	if (result != B_OK)
		return result;

	result = path.Append(kSettingsDirectoryName);
	if (result != B_OK)
		return result;

	result = create_directory(path.Path(), 0755);
	if (result != B_OK && result != B_FILE_EXISTS)
		return result;

	return path.Append(kSettingsFileName);
}
