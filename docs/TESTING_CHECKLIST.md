# Build System Testing Checklist

## Before Submitting PR

- [ ] All configurations build successfully
- [ ] All tests pass with default configuration
- [ ] CMake presets work correctly
- [ ] Documentation is complete and accurate
- [ ] validate_build.sh passes
- [ ] test_compilers.sh passes (if multiple compilers available)

## Test Matrix

| Configuration | Build | Tests | Notes |
|--------------|-------|-------|-------|
| Default | ⬜ | ⬜ | Basic configuration |
| Debug | ⬜ | ⬜ | Debug symbols |
| xlns16-ideal | ⬜ | ⬜ | High accuracy |
| xlns16-table | ⬜ | ⬜ | Table lookup |
| xlns16-alt | ⬜ | ⬜ | Alt algorithm |
| xlns16-all-opts | ⬜ | ⬜ | All optimizations |
| xlns32-ideal | ⬜ | ⬜ | High accuracy |
| xlns32-alt | ⬜ | ⬜ | Alt algorithm |
| all-ideal | ⬜ | ⬜ | Both ideal |
| minimal | ⬜ | N/A | No tests built |

## Compiler Matrix

| Compiler | Version | Status | Notes |
|----------|---------|--------|-------|
| GCC | | ⬜ | |
| Clang | | ⬜ | |
| MSVC | | ⬜ | Windows only |

## Platform Matrix

| Platform | Status | Notes |
|----------|--------|-------|
| Linux | ⬜ | |
| macOS | ⬜ | |
| Windows | ⬜ | |

## Integration Tests

- [ ] Can be used as CMake subdirectory
- [ ] Installation works correctly
- [ ] Header files installed to correct location
- [ ] Libraries linked properly

## Documentation Tests

- [ ] README instructions work
- [ ] BUILD.md is accurate
- [ ] All examples compile
- [ ] CMake options documented correctly