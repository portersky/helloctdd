# Pi Skills

Project-level skills loaded by Pi on-demand.

## Skills

| Skill                          | Description                                          |
| ------------------------------ | ---------------------------------------------------- |
| [tdd](skills/tdd/SKILL.md)     | TDD workflow with Unity and CMock                    |
| [cmake](skills/cmake/SKILL.md) | Build configuration, adding modules and dependencies |
| [git](skills/git/SKILL.md) | Commit messages, tagging, and branching workflow |

## Installation

Skills are auto-discovered by Pi from `.pi/skills/`. No extra setup
required.

To install this project's skills into another project:

```sh
pi install -l git:github.com/portersky/helloctdd
```

Or as a temporary package for the current session:

```sh
pi -e git:github.com/portersky/helloctdd
```

Pi auto-discovers `skills/` directories from installed packages.

## Adding a New Skill

Place a `SKILL.md` inside a directory under `.pi/skills/`:

```
.pi/skills/
  my-skill/
    SKILL.md
```

Pi validates the frontmatter and registers `/skill:my-skill` in the
TUI.
