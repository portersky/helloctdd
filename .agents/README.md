# Agent Skills

This directory contains reusable, agent-agnostic guidance for common
project tasks. The skills are Markdown documents, not instructions tied
to a particular coding-agent product. Read the relevant skill when a
task matches its description; skill discovery and loading support varies
between tools.

## Skills

- [`tdd`](skills/tdd/SKILL.md): Test-driven development with Unity and
  CMock.
- [`cmake`](skills/cmake/SKILL.md): Build configuration, modules, and
  dependencies.
- [`git`](skills/git/SKILL.md): Commit messages, tagging, and branching.

## Adding a Skill

Place a `SKILL.md` inside a named directory under `.agents/skills/`:

```
.agents/skills/
  my-skill/
    SKILL.md
```

Include frontmatter with the skill's name and a short description of
when to use it:

```yaml
---
name: my-skill
description: Describe the tasks this skill helps with.
---
```

Keep the instructions self-contained and useful across coding-agent
tools. Avoid tool-specific loading commands unless a skill is explicitly
written for that tool.
