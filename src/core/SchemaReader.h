#ifndef BESQLITED_SCHEMA_READER_H
#define BESQLITED_SCHEMA_READER_H

#include <SupportDefs.h>

#include <string>
#include <vector>

class Database;

class SchemaReader {
public:
	SchemaReader();

	status_t ReadTables(const Database& database,
		std::vector<std::string>& tables,
		std::string& error) const;
};

#endif
