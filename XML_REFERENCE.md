# UIForge – XML Reference

## Overview

UIForge uses a **custom XML-based language** to describe GTK3 interfaces. This document is the **authoritative reference** for all supported tags, attributes, and rules.

---

## General Syntax Rules

* Attributes must use double quotes
* No spaces between attribute name and value
* Widgets are either:

  * Self-closing (`<button />`)
  * Container (`<box>...</box>`)
* `id` should be unique (auto-generated if omitted)

Invalid XML = undefined behavior.

---

## Root Structure

There is no forced root tag, but a container is expected at top level.

Example:

```xml
<box orientation="vertical">
  <label label_text="Hello" />
</box>
```

---

## Containers

### box

| Attribute      | Type                  | Description             |
| -------------- | --------------------- | ----------------------- |
| orientation    | horizontal / vertical | Layout direction        |
| spacing        | int                   | Space between children  |
| homogeneous    | true / false          | Equal size distribution |
| width / height | int                   | Dimensions              |

---

### grid

| Attribute      | Type | Description        |
| -------------- | ---- | ------------------ |
| row            | int  | Row index          |
| column         | int  | Column index       |
| row_span       | int  | Row span           |
| column_span    | int  | Column span        |
| row_spacing    | int  | Vertical spacing   |
| column_spacing | int  | Horizontal spacing |

---

### fixed

| Attribute      | Type | Description  |
| -------------- | ---- | ------------ |
| position_x     | int  | X coordinate |
| position_y     | int  | Y coordinate |
| width / height | int  | Size         |

---

## Widgets

### button

| Attribute    | Type         | Description   |
| ------------ | ------------ | ------------- |
| label        | string       | Button text   |
| tooltip      | string       | Hover text    |
| is_sensitive | true / false | Enabled state |
| on_click     | handler name | Click signal  |

---

### label

| Attribute   | Type   | Description    |
| ----------- | ------ | -------------- |
| label_text  | string | Displayed text |
| font_size   | int    | Font size      |
| font_family | string | Font family    |

---

### entry

| Attribute        | Type   | Description    |
| ---------------- | ------ | -------------- |
| placeholder_text | string | Placeholder    |
| max_length       | int    | Max characters |

---

### image

| Attribute | Type                               | Description |
| --------- | ---------------------------------- | ----------- |
| type      | file / icon / resource / animation | Source type |
| path      | string                             | Image path  |
| opacity   | float                              | 0.0 – 1.0   |

---

### progress_bar

| Attribute         | Type         | Description |
| ----------------- | ------------ | ----------- |
| progress_fraction | float        | 0.0 – 1.0   |
| is_inverted       | true / false | Direction   |

---

## Signals

### Syntax

```xml
<button on_click="sig_handler" param_1="value" />
```

### Supported Events

* on_click
* on_toggle
* on_change
* on_activate
* on_key_press / release
* on_enter / leave
* on_destroy
* on_response

Up to **4 parameters** are supported.

---

## Styling Attributes (Global)

Available on most widgets:

* bg_color
* text_color
* bg_image
* width / height
* margin_top / bottom / left / right

Styling is applied via GTK CSS at runtime.

---

## Validation Notes

* Unknown attributes are ignored
* Invalid values may cause runtime warnings
* XML must remain well-formed

---

## Design Philosophy

The XML is:

* Declarative
* Explicit
* Human-readable

It is meant to be **edited, reviewed, and versioned**.
