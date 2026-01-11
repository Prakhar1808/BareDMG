# Contributing to BareDMG
Thank you for your interest in contributing to **BareDMG**!

This project is built as a learning resource, and contributions that improve clarity, accuracy, or educational value are always welcome.

## How Can I Contribute?

### 1. Reporting Bugs
#### Before submitting a bug report:
- Check existing [issues](https://github.com/LilSuperUser/BareDMG/issues) to see if it's already been reported.
- Try to isolate the bug to a specific component (CPU, PPU, MMU, etc)

#### When creating a bug report, include:
- **Clear title** describing the issue.
- **Components affected** (eg: "CPU: ADD instruction sets wrong flags")
- Steps to reproduce the bug
- Expected vs Actual behavior
- Test ROM that triggers the bug (if applicable)


### 2. Suggesting Enhancements / Requesting Features
We welcome suggestions for:
- Improved hardware accuracy
- Better code organization or clarity
- Additional documentation
- New test coverage

#### When creating a feature request, include:
- What you'd like to see improved
- Does it fix any existing issue?
- Why it would be valuable


## Submitting Pull Requests
#### 1. Fork the repository

#### 2. Clone the fork locally
```zsh
git clone https://github.com/<username>/BareDMG.git
cd BareDMG
```
#### 3. Set the upstream
```zsh
git remote add upstream https://github.com/LilSuperUser/BareDMG.git
```
#### 4. Pull changes from upstream main
```zsh
git checkout main && git pull upstream main
```
#### 6. Create a feture/fix branch
```zsh
git checkout -b feat/your-feature-name
# or
git checkout -b fix/your-fix-name
```
#### 7. Make your changes
#### 8. Format your code using the provided `.clang-format`
#### 9. Test your changes. See [HOW](https://github.com/LilSuperUser/BareDMG?tab=readme-ov-file#testing)
#### 10. Commit changes with a clear message(s)
#### 11. Push your feature/fix branch to your fork
```zsh
git push origin <branch-name>
```
#### 12. Open a Pull Request with:
- Description of what changed & why?
- Key changes made
- Test results (unit tests and/or test ROMs)


## Code Style
**BareDMG** uses `.clang-format` in the project to enforce consistent formatting. Please format your code before commiting.
```zsh
clang-format -i path/to/file(s)
```

## General Guidelines
- **Follow the hardware-first philosophy**: Each component should be self-contained.
- **Document complex logic**: Add comments explaining hardware behavior.
- **Consistent naming**:
    - **Functions**: `component_action()` eg: `cpu_setp()`,  `mmu_read()`
    - **Structs**: `PascalCase` eg: `CPU`, `GameBoy`
    - **variables**: `snake_case`

### Project Structure
- `include/core/` Component headers (`.h` files)
- `src/core/` Component implementations (`.c` files)
- `tests/` Unit tests using Check framework
- **Keep frontend code separate** from core emulation logic.

### Claiming an Isse
if you'd like to work on an existing issue, comment on it to let others know. We'll assign it to you to avoid duplicate work.

### Learning Resources?
When implementing a component, refer to the [resources section](https://github.com/LilSuperUser/BareDMG?tab=readme-ov-file#resources) in `README.md`.

## License
By contributing to BareDMG, you agree that your contributions will be licensed under the [GPL v3 License](./LICENSE), the same license as the project.
