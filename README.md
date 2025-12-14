# UIForge

## Executive Summary

UIForge is a desktop application written in **C with GTK3** that enables **dynamic graphical interface design** using a **custom XML markup language**. The tool covers the complete UI lifecycle: visual design, property configuration, interaction management (signals), **XML import/export**, and **standalone execution** of the generated interface.

Clear positioning: UIForge is a **developer-oriented UI Builder**, designed for fine-grained GTK3 rendering control, generated code traceability, and extensibility.

---

## Key Capabilities

* Dynamic GTK3 interface generation
* Extensible XML markup system (widgets + containers)
* Advanced signal management (click, hover, change, key events, etc.)
* Real-time property modification (colors, margins, fonts, dimensions)
* Modular architecture focused on extensibility
* Faithful and reusable XML export
* Autonomous execution of generated UI

---

## Architecture Overview

UIForge is built on a **strictly separated modular architecture**:

```
src/
 ├─ containers/      # Layouts (box, grid, fixed, etc.)
 ├─ widgets/         # Interactive widgets (button, label, entry, …)
 ├─ view/
 │   ├─ xml_builder/ # XML generation
 │   ├─ signals/     # Interaction management
 │   └─ properties/  # Configuration models
 ├─ dialogs/         # Configuration dialogs
 └─ main.c           # Entry point
```

Structural principles:

* **Single Responsibility** per module
* **View Tree** as UI source of truth
* **XML as contract format** between design and runtime

---

## Core Concept: View Tree

The entire interface is represented by a `View` tree structure:

* Each node = 1 GTK widget
* Parent / child / sibling relationships
* Centralized configuration via `ViewConfig`

This model enables:

* Recursive traversal
* Reliable XML generation
* Automatic signal connection

---

## Custom XML Language

### General Rules

* Strict XML, no comments in attributes
* Values always in quotes
* Self-closing widgets or containers
* Unique `id` recommended (auto-generated otherwise)

### Example

```xml
<box orientation="vertical" spacing="10">
  <label label_text="Hello UIForge" font_size="18" />
  <button label="Click Me" on_click="sig_hello" />
</box>
```

---

## Supported Containers

### box

| Property       | Description            |
| -------------- | ---------------------- |
| orientation    | horizontal / vertical  |
| spacing        | Space between children |
| homogeneous    | Equal distribution     |
| width / height | Dimensions             |

### grid

| Property               | Description  |
| ---------------------- | ------------ |
| row / column           | Position     |
| row_span / column_span | Span         |
| spacing                | Grid spacing |

### fixed

Absolute positioning via `position_x`, `position_y`.

---

## Supported Widgets

### button

* label
* tooltip
* is_sensitive
* dimensions
* colors
* signals

### label

* label_text
* font_size
* font_family
* color

### entry

* placeholder_text
* max_length

### image

* file / icon / resource / animation
* opacity

### progress_bar

* progress_fraction
* is_inverted

---

## Styling System

UIForge applies styling via **dynamic GTK CSS**:

* Background color
* Text color
* Background image
* Font (size + family)

Each widget can be styled independently.

---

## Signal System

### Supported Signals

* on_click
* on_change
* on_toggle
* on_activate
* on_key_press / release
* on_enter / leave (hover)
* on_destroy
* on_response

### XML Example

```xml
<button label="OK" on_click="sig_change_color" param_1="#ff0000" />
```

### Design Principles

* 1 widget → 1 signal
* Up to 4 parameters
* Dynamic mapping to C functions

---

## Built-in Signal Handlers

UIForge provides ready-to-use handlers:

* Color change
* Font size change
* Widget destruction
* Dialog opening
* XML generation

Developers can easily add new ones.

---

## XML Export Pipeline

1. Recursive traversal of View tree
2. Detection of modified properties
3. Indented XML generation
4. Final export ready for execution

Result: **readable, versionable, and maintainable XML**.

---

## Extending UIForge

Adding a new widget requires:

1. GTK definition
2. XML mapping
3. Optional signal management

Minimal impact on existing code.

---

## Build & Run

```bash
make
./uiforge
```

Dependencies:

* GTK3
* glib

---

## Target Audience

* C / GTK developers
* Advanced academic projects
* UI prototyping tools
* Controlled code generation

---

## Roadmap (Forward-Looking)

* XML validation (schema)
* Optional drag & drop
* GTK4 support
* Global themes
* Plugin system

---

## License

MIT

---

## Final Note

UIForge prioritizes **technical mastery, readability, and extensibility**. This is not a no-code tool: it's a **UI engineering platform**.