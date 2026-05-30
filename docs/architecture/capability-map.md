# BeSqlitEd Capability Map

Every Letos capability must be mapped to a Haiku-native user action.

| Capability | Letos / SQLiteStudio Source | Haiku-native Form | Phase | Notes |
|---|---|---|---|---|
| Open database | Database lifecycle | BFilePanel + sqlite3_open_v2 | 1 | Database files are Haiku documents. |
| Close database | Database lifecycle | BMessage command | 1 | No Qt action clone. |
| List schema | Schema browser | BeSqlitEd schema model + BView | 1 | Start simple. |
| Execute SQL | SQL editor / executor | BMessage command + sqlite3_prepare/step/finalize | 1 | Core before GUI luxury. |
| Show query result | Result table | Native result grid view | 2 | Do not emulate QTableView. |
| Edit table rows | Table editor | Native table editor workflow | 3 | Needs careful UX. |
| Import CSV | Import provider | Haiku file panel + import service | 3 | Translation Kit should be evaluated. |
| Export result | Export provider | Haiku file panel + export service | 3 | Keep format logic separate. |
| SQL history | History panel | Native settings/history store | 2 | Small and inspectable. |
| Preferences | Settings dialog | Haiku-style preferences window | 2 | No Qt settings clone. |
| Extensions | Plugin system | Haiku add-on/capability provider only if justified | Later | No Qt plugin ABI. |
