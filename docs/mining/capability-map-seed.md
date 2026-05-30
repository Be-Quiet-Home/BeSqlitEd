# BeSqlited Capability Map Seed

This is the first seed map for transposing Letos/SQLiteStudio capabilities into Haiku-native BeSqlited capabilities.

## Rule

Do not map widgets. Map abilities.

## Capability Type candidates

| Capability Type | Letos source hints | BeSqlited target |
|---|---|---|
| DatabaseProvider | `DbPlugin`, `DbSQLite3MC`, `DbAndroid` | SQLite provider / encrypted SQLite provider / future remote providers |
| DatabaseLifecycle | `DbManager`, `Db`, `AbstractDb`, `DbSqlite3` | open, close, clone, attach, detach, recent list |
| SchemaInspection | parser, db object organizer, schema resolver | tables, views, indexes, triggers, columns, constraints |
| QueryExecution | `QueryExecutor`, `SqlQuery`, `SqlResultsRow` | async worker, result messages, pagination |
| QueryMetadata | query executor steps, select resolver | rowid/source tracking, editability hints |
| SQLParsing | lexer, parser, AST | SQL editor intelligence, completion, statement splitting |
| SQLEditing | `SqlEditor`, completion helper | native text view, syntax highlight, completion |
| ResultBrowsing | data grid, query model/view | native result table view |
| TableEditing | table window, table modifier, view modifier | constrained edit pipeline, generated DDL preview |
| ImportProvider | CSV import, RegExp import | import add-ons/providers |
| ExportProvider | CSV, SQL, HTML, JSON, XML, PDF | export add-ons/providers; PDF later |
| FormatterProvider | SQL simple, SQL enterprise | formatter providers |
| SyntaxHighlightProvider | Tcl/Python highlighters | optional editor highlighters |
| CellRendererProvider | image renderer, JSON/image multi-editors | native cell renderers/editors, later |
| ConfigurationStore | config impl, settings DB | Haiku settings + optional SQLite settings DB |
| HistoryStore | SQL history, DDL history, CLI history | local history database |
| ExtensionManager | SQLite extension manager | extension directory scanner / add-on metadata |
| ScriptingProvider | JS/Python/Tcl scripting | deferred; not MVP |
| DiagramProvider | ERD editor | deferred; heavy GUI rebuild |

## MVP cut

### Keep for MVP

- DatabaseLifecycle
- SchemaInspection
- QueryExecution
- ResultBrowsing
- SQLEditing basic
- ConfigurationStore minimal
- HistoryStore minimal

### Mine, but postpone

- SQLParsing advanced
- QueryMetadata editability
- ImportProvider
- ExportProvider
- FormatterProvider
- ExtensionManager

### Concept only / later

- ERD editor
- Scripting providers
- Printing/PDF
- Android remote DB
- Multi-editors
- Theme/style plugins

## First message set

```cpp
enum {
    BSQL_OPEN_DATABASE  = 'bsop',
    BSQL_CLOSE_DATABASE = 'bscl',
    BSQL_EXECUTE_QUERY  = 'bsex',
    BSQL_QUERY_RESULT   = 'bsqr',
    BSQL_QUERY_ERROR    = 'bsqe',
    BSQL_LIST_SCHEMA    = 'bssc',
    BSQL_SCHEMA_RESULT  = 'bssr',
    BSQL_SAVE_SETTINGS  = 'bsss',
    BSQL_LOAD_SETTINGS  = 'bsls'
};
```

## BeSqlited directory proposal

```text
/vendor/letos/                 # original upstream source, read-only
/docs/mining/                  # mining reports, maps, decisions
/docs/capabilities/            # formal Capability Type specs
/src/core/                     # Qt-free SQLite core
/src/app/                      # BApplication startup
/src/ui/                       # BWindow/BView shell
/src/providers/                # future Haiku add-ons/providers
/tests/                        # native tests, ported from Letos behavior tests where useful
```

## Capability record template

```text
Capability:
  Execute SQL query

Letos source:
  Letos/core/db/queryexecutor.*
  Letos/core/db/sqlquery.*
  Letos/core/db/sqlresultsrow.*

Haiku target:
  BSQL_EXECUTE_QUERY sent to worker BLooper
  BSQL_QUERY_RESULT reply containing columns, rows, timing, affected rows

Keep:
  behavior, metadata ideas, paging ideas

Discard:
  QObject inheritance, signals, QRunnable, Qt containers

MVP status:
  required
```
