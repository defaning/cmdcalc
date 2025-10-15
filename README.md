# cmdcalc

A small command-line calculator in C++17.

Features:
- Evaluate arithmetic expressions with + - * / ^ and parentheses
- Unary minus
- Constants: pi, e
- REPL mode or one-shot via command-line args

Build

```bash
mkdir -p build && cd build
cmake ..
cmake --build . --config Release

# run tests
ctest --output-on-failure

# or run the CLI
./cmdcalc "2*(3+4)"
```
