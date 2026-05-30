#ifndef BESQLITED_DATABASE_H
#define BESQLITED_DATABASE_H

class Database {
public:
	Database();
	~Database();

	bool IsOpen() const;
};

#endif
