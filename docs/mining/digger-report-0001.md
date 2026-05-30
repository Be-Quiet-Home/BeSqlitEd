# BeSqlited Digger Report 0001

Status: first-pass static mining of the uploaded `BeSqlited-master.zip` source tree.
Scope: classify Letos/SQLiteStudio code for Haiku-native transposition, not for Qt porting.

## Executive summary

The source tree is very useful as a capability mine, but it is not a ready Haiku-native base.

The important finding is that Letos has a visible three-layer shape:

1. `Letos/core` contains most of the database, parser, query, configuration, import/export and plugin abstractions.
2. `Letos/gui` is a large Qt GUI layer and should be treated as reference material, not porting material.
3. `Plugins` describe many user-visible capabilities through compact JSON manifests and plugin interfaces, but many plugin implementations still depend on QtCore, QtWidgets, QtGui or external runtimes.

The strongest BeSqlited approach is therefore:

> Keep the capability knowledge. Rebuild the body in Haiku.

## Source tree facts

First-pass inventory:

- ZIP entries: 2447
- Source tree size unpacked: about 48.7 MiB
- Files seen by extension, top groups:
  - 735 translation files (`.ts`)
  - 538 headers (`.h`)
  - 480 C++ files (`.cpp`)
  - 347 SVG assets
  - 81 Qt Designer UI files (`.ui`)
  - 24 Qt resource files (`.qrc`)
  - 22 JSON plugin manifests

Top code-bearing areas by file count:

| Area | Approx. role |
|---|---|
| `Letos/gui` | Qt GUI shell, dialogs, editor views, db tree, grid, windows |
| `Letos/core` | DB abstraction, parser, query execution, services, config, plugins |
| `Plugins/*` | Export/import/editor/scripting/formatter/DB plugins |
| `Letos/Tests` | Behavior test material for parser, formatter, DSV, table modifier, etc. |
| `Letos/letos` | Qt application entry point and single-instance behavior |

## Critical build finding

The upstream CMake structure has a useful switch:

```cmake
option(WITH_GUI "Build GUI application" ON)
...
add_subdirectory(core)
if(WITH_GUI)
    add_subdirectory(gui)
    add_subdirectory(letos)
endif()
```

That looks promising, but `coreLetos` itself is not Qt-free:

```cmake
find_package(Qt6 REQUIRED COMPONENTS Core Network Qml Widgets)
...
target_link_libraries(coreLetos PRIVATE
    Qt::Core
    Qt::Network
    Qt::Qml
)
```

So the core is separable from the GUI build, but not yet separable from Qt. The first BeSqlited mining step should not be “build Letos core on Haiku without Qt.” It should be “extract a smaller BeSqlitedCore capability subset.”

## Qt contamination map

Static symbol scan summary:

| Area | Files scanned | Lines | QtCore refs | QObject/signals refs | QtWidgets-ish refs | QML/JS refs | SQLite refs |
|---|---:|---:|---:|---:|---:|---:|---:|
| `Letos/core` | 367 | 81,748 | 10,096 | 501 | 36 | 37 | 196 |
| `Letos/gui` | 414 | 80,743 | 5,536 | 831 | 2,912 | 0 | 14 |
| `Letos/letos` | 5 | 1,039 | 32 | 25 | 7 | 0 | 0 |
| `Plugins/*` | mixed | high | 2,932 | 160+ | 500+ | low | dominated by SQLite3MC amalgamation |
| `Letos/Tests` | 26 | 4,212 | 873 | 21 | 0 | 0 | low |

Interpretation:

- `Letos/gui` is unmistakably disposable for BeSqlited implementation.
- `Letos/core` is valuable, but QtCore-shaped. It should be mined, not pasted.
- The parser and query logic are high-value but use Qt containers and shared pointer patterns.
- Plugin manifests are extremely valuable for BeSqlited capability taxonomy.

## Core subfolder assessment

| Core subfolder | Assessment | Mining strategy |
|---|---|---|
| `core/db` | Highest practical value: SQLite abstraction, execution, attach/detach, result objects | Mine behavior and SQL handling; rewrite into Haiku/STL/C++ core |
| `core/parser` | High value: lexer, parser, AST, SQL understanding | Mine grammar, tests, AST concepts; consider preserving grammar shape, not Qt types |
| `core/services` | Medium/high value: db manager, config, extension manager, plugin manager | Mine service responsibilities; rebuild with Haiku settings, messages, add-ons |
| `core/common` | Medium value: utilities, table/column helpers, serialization | Selective reuse by concept only |
| `core/plugins` | High taxonomy value; mixed implementation value | Use interfaces to define Capability Types, not Qt plugin ABI |
| `core/config_builder` | Medium value; config model is useful but Qt-bound | Map to Haiku settings and SQLite-backed config only where useful |
| `core/diff` | Possibly reusable algorithmic utility | Keep as vendor candidate if license and dependencies are acceptable |
| `core/rsa` | Low priority for MVP | Ignore unless a concrete capability needs it |
| `core/chillout` | Crash handler/support code | Not relevant for Haiku-native first pass |

## Gold veins

### 1. Database lifecycle and execution

Primary files:

- `Letos/core/db/db.h`
- `Letos/core/db/abstractdb.h/.cpp`
- `Letos/core/db/dbsqlite3.h/.cpp`
- `Letos/core/db/stdsqlite3driver.h`
- `Letos/core/db/queryexecutor.h/.cpp`
- `Letos/core/db/queryexecutorsteps/*`
- `Letos/core/db/sqlquery.h/.cpp`
- `Letos/core/db/sqlresultsrow.h/.cpp`

Capability mapping:

| Letos capability | BeSqlited target |
|---|---|
| open database | `BSQL_OPEN_DATABASE` message + `BeSqlitedConnection` |
| close database | `BSQL_CLOSE_DATABASE` |
| exec SQL | `BSQL_EXECUTE_QUERY` to worker/looper |
| async query | `BLooper`/worker thread + reply `BMessage` |
| result rows | `BeSqlitedResultSet` model feeding native views |
| attach database | explicit capability, probably not MVP-day-1 |
| SQLite extension loading | later Haiku add-on/extension panel |

### 2. SQL parser / lexer / AST

Primary files:

- `Letos/core/parser/lexer.*`
- `Letos/core/parser/parser.*`
- `Letos/core/parser/sqlite3_parse.y`
- `Letos/core/parser/sqlite3_parse.cpp`
- `Letos/core/parser/ast/*`
- `Letos/core/parser/keywords.*`
- `Letos/core/parser/token.*`

Mining value:

- SQL syntax awareness
- statement splitting
- editor completion groundwork
- schema/object extraction
- query metadata support
- table modifier support

Caution: this layer is QtCore-heavy (`QString`, `QList`, `QSharedPointer`, etc.). Treat it as an algorithm and grammar source, not as a drop-in library.

### 3. Configuration database

Primary file:

- `Letos/core/services/impl/configimpl.cpp`

Important observed schema ideas:

- `settings ([group], [key], value)`
- `sqleditor_history`
- `dblist`
- `groups`
- `ddl_history`
- `cli_history`
- `bind_params`
- `bind_param_values`
- `populate_history`
- `script_functions`

BeSqlited target:

Use Haiku-native settings location and optionally keep the SQLite-backed internal config DB idea. This is a good example where Letos has a clever capability, but the filesystem location and `QSettings` layer must be replaced.

### 4. Import/export plugins

Plugin manifests show a clean capability taxonomy:

| Plugin | Capability Type |
|---|---|
| CSV import | ImportProvider |
| RegExp import | ImportProvider |
| CSV export | ExportProvider |
| SQL export | ExportProvider |
| HTML export | ExportProvider |
| JSON export | ExportProvider |
| XML export | ExportProvider |
| PDF export | ExportProvider, but GUI/print dependent |
| SQL formatter simple | FormatterProvider |
| SQL Enterprise formatter | FormatterProvider |
| Python/Tcl syntax highlighter | SyntaxHighlightProvider |
| SQLite3MultipleCiphers | DatabaseProvider / EncryptedSQLiteProvider |

This is a strong candidate for a Haiku-style `BeSqlitedCapabilityManifest`, probably JSON initially because Letos already uses JSON manifests, later possibly mapped to Haiku attributes or add-on metadata.

## Red zones

Avoid porting these directly:

| Area | Reason |
|---|---|
| `Letos/gui/*` | Qt Widgets, Qt Designer `.ui`, MDI/window/dialog worldview |
| `Letos/letos/main.cpp` | Qt application bootstrap, `QApplication`, `SingleApplication`, `QCommandLineParser` |
| `Plugins/ErdEditor` | Heavy Qt Graphics/View style object editor |
| `Plugins/FusionDarkStyle` | Qt style plugin, irrelevant for Haiku |
| `Plugins/Printing` | Qt PrintSupport dependency |
| `Plugins/MultiEditorImage` | Qt widget/editor plugin; concept only |
| `Plugins/MultiEditorJson` | Qt widget/editor plugin; concept only |
| `Plugins/DbAndroid` | Useful capability idea, but not MVP and platform-tool dependent |
| `ScriptingPython` / `ScriptingTcl` | Too much runtime policy for first BeSqlited phase |

## Haiku-native transposition protocol

Replace these Qt idioms:

| Qt idiom | BeSqlited / Haiku idiom |
|---|---|
| `QApplication` | `BApplication` |
| `QMainWindow` | `BWindow` + native layout/view hierarchy |
| `QWidget` | `BView` / specific Haiku controls |
| signals/slots | `BMessage`, `BMessenger`, `BHandler`, `BLooper` |
| `QThread`, `QRunnable` | Haiku threads / worker `BLooper` pattern |
| `QSettings` | Haiku settings file under `/boot/home/config/settings/...` plus optional SQLite config DB |
| `QFileDialog` | `BFilePanel` |
| `QPluginLoader` | Haiku add-ons / explicit provider registry |
| Qt Designer `.ui` | Hand-built Haiku UI or archived native layouts |
| Qt resources `.qrc` | app resources / files / Haiku resource strategy |

## Recommended first extraction target

Do not start with the GUI.

Start with `BeSqlitedCore-0`:

```text
src/core/
  BeSqlitedConnection.h/.cpp
  BeSqlitedDatabase.h/.cpp
  BeSqlitedResultSet.h/.cpp
  BeSqlitedQueryWorker.h/.cpp
  BeSqlitedSchemaReader.h/.cpp
  BeSqlitedMessages.h
```

Initial message protocol:

```cpp
enum {
    BSQL_OPEN_DATABASE  = 'bsop',
    BSQL_CLOSE_DATABASE = 'bscl',
    BSQL_EXECUTE_QUERY  = 'bsex',
    BSQL_QUERY_RESULT   = 'bsqr',
    BSQL_QUERY_ERROR    = 'bsqe',
    BSQL_LIST_SCHEMA    = 'bssc',
    BSQL_SCHEMA_RESULT  = 'bssr'
};
```

Initial MVP capabilities:

1. Open SQLite database from path.
2. List tables/views/indexes/triggers.
3. Execute read-only SQL query.
4. Return column names, row values, SQLite types.
5. Show results in a minimal native Haiku window.
6. Persist recent database list under Haiku settings.

Everything else should be delayed until the skeleton proves the mapping.

## First concrete mining tasks

1. Create `docs/mining/capability-map.md` from plugin manifests and major GUI workflows.
2. Create `docs/mining/qt-contamination-index.csv` from the generated scan.
3. Define `src/core/BeSqlitedMessages.h` before any GUI work.
4. Build a minimal SQLite-only core independent of Letos Qt code.
5. Use Letos tests as behavior references, especially:
   - `LexerTest`
   - `ParserTest`
   - `DsvFormatsTest`
   - `UtilsTest`
   - `TableModifierTest`
6. Only after that: build a small `BApplication` shell.

## Verdict

The diggers found gold, but also clay.

Gold:

- parser/lexer/AST concepts
- query executor behavior
- DB abstraction patterns
- configuration schema ideas
- plugin capability taxonomy
- import/export/formatter capability list
- test cases as behavioral fossils

Clay:

- QtCore in the core
- QObject/signals in service and query code
- QML/JavaScript scripting inside core
- Qt Widgets in GUI and some plugin-facing interfaces
- Qt plugin ABI assumptions

Final judgment:

> Letos is not the blueprint. Letos is the mine. BeSqlited must be the Haiku-native casting.
