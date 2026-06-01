#include "Database.h"

Database::Database()
	:
	fHandle(NULL)
{
}

Database::~Database()
{
	Close();
}

status_t
Database::OpenReadOnly(const char* path)
{
	Close();

	if (path == NULL || path[0] == '\0') {
		fLastError = "No database path supplied.";
		return B_BAD_VALUE;
	}

	sqlite3* handle = NULL;
	int result = sqlite3_open_v2(path, &handle, SQLITE_OPEN_READONLY, NULL);
	if (result != SQLITE_OK) {
		fLastError = handle != NULL
			? sqlite3_errmsg(handle)
			: "Could not open database.";
		if (handle != NULL)
			sqlite3_close(handle);
		return B_ERROR;
	}

	fHandle = handle;
	fPath = path;
	fLastError = "";
	return B_OK;
}

void
Database::Close()
{
	if (fHandle != NULL) {
		sqlite3_close(fHandle);
		fHandle = NULL;
	}

	fPath = "";
}

bool
Database::IsOpen() const
{
	return fHandle != NULL;
}

sqlite3*
Database::Handle() const
{
	return fHandle;
}

const char*
Database::Path() const
{
	return fPath.String();
}

const char*
Database::LastError() const
{
	return fLastError.String();
}
