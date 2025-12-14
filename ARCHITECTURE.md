# UIForge – Architecture

## Purpose

This document explains **how UIForge is built**, **why the architecture is structured this way**, and **how components interact**. It is written for developers who want to understand, extend, or audit the system.

---

## High-Level Architecture

UIForge follows a **modular, tree-based architecture** centered around a single source of truth: the **View Tree**.

```
User Actions
    ↓
Dialogs / UI Controls
    ↓
View Tree (Model)
    ↓              ↘
GTK Widgets       XML Builder
    ↓              ↓
Live Preview   Exported XML
```

Key idea: **the View Tree drives everything**.

---

## Core Components

### 1. View Tree (Core Model)

The entire UI is represented as a tree of `View` nodes.

Responsibilities:

* Represent widget hierarchy
* Store configuration (`ViewConfig`)
* Enable traversal (rendering, export, signals)

Each node contains:

* GTK widget pointer
* Parent / child / sibling links
* Configuration and signal metadata

This design guarantees:

* Deterministic rendering
* Lossless XML export
* Predictable signal binding

---

### 2. Containers Module (`containers/`)

Handles **layout widgets**:

* box
* grid
* fixed
* flowbox
* paned
* notebook

Responsibilities:

* Create GTK container widgets
* Apply layout-specific configuration
* Insert children based on View Tree relationships

Containers do **not** handle signals or styling logic.

---

### 3. Widgets Module (`widgets/`)

Handles **interactive UI elements**:

* button
* label
* entry
* image
* progress_bar
* menus

Responsibilities:

* GTK widget creation
* Default property initialization
* Signal eligibility declaration

Widgets are **stateless** beyond their GTK instance.

---

### 4. Properties & Styling (`global.*`)

All visual configuration is applied through:

* GTK CSS providers
* Runtime-generated styles

Supported styling:

* Background color / image
* Text color
* Font family & size
* Margins & dimensions

Design choice: **no global theme lock-in**.
Each widget can be styled independently.

---

### 5. Signal System (`view/signals/`)

Signals connect **user events → behavior**.

Flow:

1. XML defines signal + handler name
2. ViewConfig stores metadata
3. `connect_signales()` maps handler names to C callbacks
4. GTK signal is connected dynamically

Benefits:

* Declarative behavior
* Centralized signal mapping
* Easy extensibility

---

### 6. XML Builder (`view/xml_builder/`)

Responsible for **exporting the UI**.

Process:

* Recursive traversal of View Tree
* Detection of non-default properties
* Structured, indented XML output

Guarantees:

* No UI information loss
* Readable, versionable XML
* Rebuildable UI at runtime

---

## Architectural Principles

* Single Source of Truth (View Tree)
* Strict separation of concerns
* Declarative UI definition
* Runtime configurability
* Minimal coupling between modules

---

## Why This Architecture Works

* Predictable behavior
* Easy debugging
* Scales with new widgets
* Safe XML round-trip (import/export)

This is not accidental complexity. It is **intentional engineering**.
