#include "SchemaReader.h"

#include "Database.h"

#include <sqlite3.h>

SchemaReader::SchemaReader()
{
}

status_t
SchemaReader::ReadTables(const Database& database,
	std::vector<std::string>& tables,
	std::string& error) const
{
	tables.clear();
	error.clear();

	if (!database.IsOpen()) {
		error = "Database is not open.";
		return B_BAD_VALUE;
	}

	static const char* kSql =
		"SELECT name "
		"FROM sqlite_master "
		"WHERE type = 'table' "
		"AND name NOT LIKE 'sqlite_%' "
		"ORDER BY name;";

	sqlite3_stmt* statement = NULL;
	int result = sqlite3_prepare_v2(database.Handle(), kSql, -1, &statement, NULL);
	if (result != SQLITE_OK) {
		error = sqlite3_errmsg(database.Handle());
		return B_ERROR;
	}

	while ((result = sqlite3_step(statement)) == SQLITE_ROW) {
		const unsigned char* text = sqlite3_column_text(statement, 0);
		if (text != NULL)
			tables.push_back(reinterpret_cast<const char*>(text));
	}

	if (result != SQLITE_DONE) {
		error = sqlite3_errmsg(database.Handle());
		sqlite3_finalize(statement);
		return B_ERROR;
	}

	sqlite3_finalize(statement);
	return B_OK;
}
