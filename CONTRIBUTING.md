# Contributing to ATM Management System

Thank you for your interest in contributing! This project is part of the Zone01 Athens curriculum.

## Getting Started

1. Fork the repository
2. Clone your fork: `git clone https://platform.zone01.gr/git/kkasdana/atm-management-system`
3. Create a branch: `git checkout -b feature/your-feature`
4. Make your changes
5. Test thoroughly
6. Commit: `git commit -m "Add: your feature description"`
7. Push: `git push origin feature/your-feature`
8. Open a Pull Request

## Code Guidelines

- Follow existing code style and formatting
- Keep functions small and focused
- Add comments for complex logic
- Test all changes before submitting
- Ensure `make` builds without errors or warnings

## Commit Messages

Use clear, descriptive commit messages:
- `Add: new feature description`
- `Fix: bug description`
- `Update: modification description`
- `Remove: deletion description`

## Testing

Before submitting:
```bash
make clean
make
./build/atm
```

Test all affected features manually using the audit guide in [Audit Guide](./audit/audit_guide.md).

## Pull Request Process

1. Update documentation if needed
2. Ensure code builds successfully
3. Describe your changes clearly
4. Reference any related issues

## Questions?

Open an issue for questions or discussions about the project.
