# BeSqlitEd Native Doctrine

BeSqlitEd is not a Qt port.

BeSqlitEd maps SQLite workbench capabilities into native Haiku user actions.

## Core Rule

Running on Haiku is not enough.

A feature is accepted only when its user-visible behavior can be expressed through native Haiku concepts.

## Native Vocabulary

BeSqlitEd prefers:

- BApplication for application lifecycle
- BWindow for windows
- BView for visible interface elements
- BMessage for commands and events
- BMessenger for communication
- BHandler and BLooper for message processing
- BFilePanel for file selection
- Tracker and MIME integration for database files
- Haiku settings conventions for preferences
- Haiku add-on patterns only where they naturally fit

## Rejected Design Direction

BeSqlitEd does not emulate:

- Qt widgets
- Qt model/view semantics
- Qt docking
- Qt MDI
- Qt plugin architecture
- foreign theme systems

When a Letos or SQLiteStudio feature is found, the question is not:

"Which Qt widget implements this?"

The question is:

"Which Haiku-native user action corresponds to this capability?"

## Guiding Sentence

Letos is the capability mine.
Haiku is the operating culture.
Qt is not the target shape.
