# UTCook

A polished, two-mode recipe platform for managing recipes, ingredients, and recommendations—built as a clean educational C++ project.

## Why UTCook?
UTCook brings together a minimal CLI engine and a small HTTP server to demonstrate core software concepts: input parsing, data management, role-based actions, and web request handling.

## Highlights
- Dual run modes: CLI + Web
- Role-based flows: Chef and Visitor
- Recipe creation and liking
- Ingredient (fridge) management
- Recommendation engine (multiple recommenders)
- "Most liked" report
- Simple session-aware web UI

## Demo Run
CLI mode (reads commands from stdin):

```bash
./UTCook ingredients.csv recipes.csv
```

Example command input:

```bash
printf "GET recipes\n\n" | ./UTCook ingredients.csv recipes.csv
```

Web mode (needs a port):

```bash
./UTCook 5000 ingredients.csv recipes.csv
```

Then open:

```
http://localhost:5000/
```

## Clone the Repository

```bash
git clone <REPO_URL>
cd f04-a6-Abolfazl-Azad
```

## Build Instructions
Requirements:
- C++ compiler with C++20 support
- `make` (Linux/WSL/macOS)
- On Windows: WSL or MinGW/MSYS2 for `make`

Build:

```bash
make
```

Output:
- `UTCook` executable in the project root

## Project Layout

```
.
+- APHTTP/            # Lightweight HTTP library
+- includes/          # Header files
+- src/               # Source files
+- static/            # Web assets (CSS/JS)
+- ingredients.csv    # Sample ingredients data
+- recipes.csv        # Sample recipes data
+- Makefile
+- README.md
```

## Usage Notes
- If CLI looks “stuck”, it is waiting for input on `stdin`. End input with an empty line.
- If you expect a web page, use the Web mode with a port argument (example above).

## License
Educational use.

## Contributing
Pull requests and issues are welcome. Keep changes minimal and well-documented.
