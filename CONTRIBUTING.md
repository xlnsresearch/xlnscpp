# Contributing to xlnscpp

Thanks for your interest in contributing to this project.

## Before You Start

- Read [README.md](README.md) for usage and compile-time options.
- Check existing issues and pull requests before opening a new one.
- Follow our [Code of Conduct](CODE_OF_CONDUCT.md).

## Development Setup

Use a modern C++ compiler (for example `g++` or `clang++`) and Python 3 for the
validation scripts.

Common files used during development:

- Core implementation: `xlns16.cpp`, `xlns32.cpp`
- C++ tests: `xlns16test.cpp`, `xlns32test.cpp`, `xlnsbothtest.cpp`
- Python validation scripts: `sbtest.py`, `dbtest.py`, `sblptest.py`, `dblptest.py`

## Reporting Issues

Please use the issue templates and provide:

- A minimal reproducible example
- Compiler and OS details
- Compile-time macro options (for example `xlns16_ideal`, `xlns16_alt`, `xlns16_table`)
- Expected vs actual behavior

## Submitting Pull Requests

1. Fork the repository and create a focused branch.
2. Keep changes minimal and specific to one purpose.
3. Update docs when behavior or APIs change.
4. Add or update tests when relevant.
5. Fill out the PR template completely.

## Testing Guidance

Run relevant tests for your change. Examples:

- Build and run affected C++ tests
- Run Python validation scripts if your change touches sb/db related logic

If you changed behavior behind compile-time options, test the option(s) you modified.

## Style Guidance

- Match the existing code style in each file.
- Avoid unrelated refactors in the same PR.
- Prefer clear names and small, reviewable changes.

## Commit and PR Quality

- Use clear commit messages.
- Link issues in PRs with `Fixes #<number>` when appropriate.
- Include concise test evidence in the PR description.

Thanks again for improving xlnscpp.
