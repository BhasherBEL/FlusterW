template<typename L>
bool DB::openDB(const std::string &path, const std::string &req, L const &content) {
    sqlite3 *db;

    //TODO Error handling
    if(sqlite3_open(path.c_str(), &db)){
        return false;
    }

    sqlite3_stmt *pStmt;

    if(sqlite3_prepare(db, req.c_str(), -1, &pStmt, nullptr)){
        return false;
    }

    while(sqlite3_step(pStmt) == SQLITE_ROW){
        content(pStmt);
    }
    sqlite3_finalize(pStmt);
    sqlite3_close(db);

    return true;
}