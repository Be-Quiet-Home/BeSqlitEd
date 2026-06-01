#ifndef BESQLITED_DATABASE_H
#define BESQLITED_DATABASE_H

#include <String.h>
#include <SupportDefs.h>

#include <sqlite3.h>

class Database {
public:
	Database();
	~Database();

	status_t OpenReadOnly(const char* path);
	void Close();

	bool IsOpen() const;
	sqlite3* Handle() const;

	const char* Path() const;
	const char* LastError() const;

private:
	sqlite3* fHandle;
	BString fPath;
	BString fLastError;
};

#endif
