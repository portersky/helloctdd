---
name: git
description: Git workflow for the ctdd template project. Use when committing
  changes, creating tags for releases, or following the branching and commit
  message conventions.
---

# Git Skill

## Commit Messages

Follow [Conventional Commits](https://www.conventionalcommits.org/) with the
50/72 rule.

### Format

```
<type>: <subject>

<body>

<footer>
```

- **Subject:** max 50 characters, imperative mood, no period
- **Body:** wrapped at 72 characters, optional
- **Blank line** between subject and body
- **No co-author trailers** (`Co-Authored-By:` is forbidden)
- **No em dashes** (`—`). Use a colon or rewrite the sentence

### Types

| Type       | Use                                |
| ---------- | ---------------------------------- |
| `feat`     | New feature or module              |
| `fix`      | Bug fix                            |
| `docs`     | Documentation only                 |
| `chore`    | Build, deps, config, housekeeping  |
| `ci`       | CI/CD changes                      |
| `refactor` | Code change without feature or fix |
| `test`     | Adding or changing tests           |
| `style`    | Formatting, whitespace, semicolons |

### Examples

Good:

```
feat: add stopwatch timer

Replace Hello World with a live stopwatch that prints elapsed time
in HH:MM:SS.mmm format, updating every 10ms with color output.
```

```
fix: clear Unity INTERFACE_SYSTEM_INCLUDE_DIRECTORIES

CMake rejects the path inside the build tree on newer versions.
```

```
chore: bump CMock to v2.6.0
```

Bad:

```
added a new thing              # no type, lowercase, vague
feat: add stopwatch             # no body for a significant change
feat: add stopwatch timer.      # trailing period
feat: add timer — it's fast     # em dash not allowed
```

## Committing

Stage changes:

```sh
git add -A
```

Commit:

```sh
git commit -m "type: subject"
```

For multi-line messages:

```sh
git commit -m "type: subject

body line 1
body line 2"
```

## Tagging

This project uses semantic versioning for template releases.

### Creating a tag

Create the tag:

```sh
git tag -a v0.1.0 -m "Release v0.1.0"
```

Push the tag:

```sh
git push origin v0.1.0
```

Tag messages follow the same rules as commit messages: no em dashes,
wrapped at 72 characters.

### Tag naming

| Version  | Meaning                              |
| -------- | ------------------------------------ |
| `v0.x.0` | Minor changes, breaking template API |
| `v0.x.y` | Patch changes, backward-compatible   |
| `v1.x.0` | Major release                        |

### When to tag

- After merging a feature branch that changes the template structure
- Before sharing the template with others
- When CMakeLists.txt, deps/, or build system changes could affect
  projects using this template

### Listing tags

```sh
git tag -l
```

### Deleting a tag

Delete the local tag:

```sh
git tag -d v0.1.0
```

Remove the remote tag:

```sh
git push origin --delete v0.1.0
```

## Branching

For local development:

```sh
git checkout -b feat/add-counter
```

Branch naming:

| Prefix   | Use           |
| -------- | ------------- |
| `feat/`  | New feature   |
| `fix/`   | Bug fix       |
| `chore/` | Housekeeping  |
| `docs/`  | Documentation |

## Common Commands

| Command                 | Use                               |
| ----------------------- | --------------------------------- |
| `git log --oneline -10` | Recent history                    |
| `git diff --staged`     | Check staged changes              |
| `git status`            | Current state                     |
| `git reset HEAD~1`      | Undo last commit (keep changes)   |
| `git rebase -i HEAD~3`  | Interactive rebase last 3 commits |

## Checklist Before Committing

- [ ] Tests pass: `ninja -C build check`
- [ ] Commit message follows 50/72 rule
- [ ] Conventional commit type used
- [ ] No co-author trailers
- [ ] No em dashes
- [ ] No large unchanged regions reformatted
- [ ] No speculative code or unused imports
