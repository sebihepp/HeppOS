# Copilot Instructions for Google C++ Style Guide

You are assisting with a C++ project that follows the Google C++ Style Guide.

## Key Guidelines

### Naming Conventions
- Use `CamelCase` for class names, function names, and variables
- Use `snake_case` for file names and namespace names
- Use `UPPER_CASE` for constants and macros
- Use descriptive names that convey intent

### Formatting
- Indent with 2 spaces (not tabs)
- Line length should not exceed 80 characters where practical
- Place opening braces on the same line (K&R style)
- Use one blank line between function definitions

### Comments
- Use `//` for single-line comments
- Use `/* */` for multi-line comments
- Write comments that explain **why**, not what the code does
- Place comments above the code they describe

### Classes and Functions
- Declare class data members as private
- Provide public getter/setter methods as needed
- Keep functions small and focused on a single task
- Use `const` for methods that don't modify state

### Headers and Includes
- Use `#include` guards or `#pragma once`
- Order includes: C++ standard library, third-party libraries, local headers
- Minimize dependencies between headers
- No standard libraries available for this project

### Modern C++
- Prefer standard library containers and algorithms
- Use smart pointers (`std::unique_ptr`, `std::shared_ptr`) for memory management
- Avoid raw pointers for ownership
- Use range-based for loops where applicable

When generating or reviewing code, ensure compliance with these guidelines.