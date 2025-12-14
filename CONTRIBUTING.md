# Contributing to UIForge

Thank you for considering contributing to UIForge.
This project prioritizes **clarity, correctness, and long-term maintainability**.

---

## Guiding Principles

* Explicit behavior over hidden logic
* Simple, readable C over clever abstractions
* XML is the single source of truth
* One responsibility per module

If a change violates these principles, it will not be accepted.

---

## Repository Structure

```
containers/   # Layout and container widgets
widgets/      # Interactive GTK widgets
view/
 ├─ signals/  # User interaction handlers
 ├─ xml_builder/ # XML export logic
 └─ properties/  # Property parsing and mapping
```

Respect these boundaries when contributing.

---

## Adding a Widget

1. Create the GTK widget implementation
2. Define supported XML attributes
3. Map attributes into `ViewConfig`
4. (Optional) Declare supported signals

Rule: **If a feature is not declared in XML, it does not exist.**

---

## Adding a Signal Handler

* Implement the callback in `view/signals/`
* Use parameters passed from XML
* Keep handlers single-purpose
* Register the handler in `connect_signales()`

Avoid hard-coded widget IDs and hidden side effects.

---

## Coding Standards

* Prefer C89-compatible code when possible
* Explicit memory ownership (no leaks, no ambiguity)
* No global state modifications outside designated modules
* Fail loudly on errors

---

## Documentation Requirements

Any contribution must include:

* An XML usage example
* A description of supported attributes
* Signal behavior documentation (if applicable)

Undocumented behavior is considered unstable.

---

## Commit Guidelines

* Small, atomic commits
* Clear and descriptive commit messages
* Separate refactors from feature changes

---

## Final Note

UIForge is an engineering-focused project.
Contributions should improve robustness, readability, or extensibility — not add unnecessary complexity.
