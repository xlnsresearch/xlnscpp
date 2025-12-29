# Contributing to xlnscpp

Thank you for your interest in contributing to xlnscpp!

## Building the Project

See [BUILD.md](docs/BUILD.md) for detailed build instructions.

Quick start:
```bash
mkdir build && cd build
cmake ..
make
ctest
```

## Making Changes

1. Fork the repository
2. Create a feature branch: `git checkout -b feature/my-feature`
3. Make your changes
4. Build and test: `./scripts/validate_build.sh`
5. Commit your changes: `git commit -am 'Add some feature'`
6. Push to the branch: `git push origin feature/my-feature`
7. Submit a pull request

## Code Style

- Follow existing code style
- Use meaningful variable names
- Add comments for complex algorithms
- Keep functions focused and concise

## Testing

All changes should:
- Build successfully with all configurations
- Pass all existing tests
- Include new tests for new functionality
- Not decrease code coverage

Run tests with:
```bash
cd build
ctest --output-on-failure
```

## Commit Messages

Use clear, descriptive commit messages:
- Start with a verb (Add, Fix, Update, etc.)
- Keep the first line under 72 characters
- Provide details in the body if needed

Good: "Add CMake build system with presets"
Bad: "fixed stuff"

## Pull Request Process

1. Update documentation for any changed functionality
2. Update README.md if needed
3. Ensure all tests pass
4. Request review from maintainers

## Questions?

Open an issue or discussion on GitHub!