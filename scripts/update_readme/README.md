## Bash Script: Update README.md with Project Structure

### Overview

This script automates the process of updating the `README.md` file in a project with the latest directory structure using the `tree` command. It ensures consistency and ease of documentation for project structures, especially useful for collaborative projects.

---

### Features

1. **Terminal-Friendly Colors**:
   - Informational (`INFO`), error (`ERROR`), and update (`UPDATE`) messages are color-coded for better visibility.
   
2. **Quiet Mode**:
   - Suppresses non-essential messages with the `-q` or `--quiet` option.

3. **Automatic Installation**:
   - Detects if the `tree` command is missing and attempts to install it using the appropriate package manager (`apt-get`, `yum`, `dnf`, `pacman`, or `brew`).

4. **Customizable Exclusions**:
   - Excludes specific files and directories (e.g., `.git`, `node_modules`, etc.) from the `tree` output.

5. **Markdown Update**:
   - Searches for specific markers (`<!-- project-structure-start -->` and `<!-- project-structure-end -->`) in the `README.md` file and replaces the content between them with the latest project structure.

6. **Error Handling**:
   - Provides meaningful error messages and exits gracefully if required conditions are not met (e.g., `README.md` missing or markers not found).

7. **Temporary File Management**:
   - Ensures cleanup of temporary files even if the script is interrupted.

---

### Usage

```bash
./update_readme.sh [OPTIONS]
```

#### Options
- `-q, --quiet`  
  Suppresses non-essential informational messages.

- `-h, --help`  
  Displays usage information.

---

### Example

Run the script to update the `README.md` file in the project root:

```bash
./update_readme.sh
```

Suppress output except for errors:

```bash
./update_readme.sh --quiet
```

---

### Prerequisites

- The `tree` command must be installed (handled by the script if missing).
- A `README.md` file must exist in the project root.
- The `README.md` must contain the following markers:
  ```markdown
  <!-- project-structure-start -->
  ```sh
  # The project structure will be inserted here
  ```
  <!-- project-structure-end -->
  ```

---

### How It Works

1. **Navigates to Project Root**:  
   The script identifies the project root directory relative to its own location and navigates there.

2. **Checks for `tree` Command**:  
   If `tree` is not installed, it uses the system's package manager to install it.

3. **Generates Directory Structure**:  
   Excludes unnecessary files/directories and writes the structure to a temporary file.

4. **Updates `README.md`**:  
   Finds the defined markers in `README.md` and replaces the content between them with the updated structure.

5. **Cleans Up**:  
   Deletes temporary files and displays a success message.

---

### Error Handling

- **Missing `README.md`**:  
  Displays: `"README.md not found in project root."`

- **Markers Missing in `README.md`**:  
  Displays: `"Markers '<!-- project-structure-start -->' and/or '<!-- project-structure-end -->' not found in README.md."`

- **Unsupported Package Manager**:  
  Displays: `"No supported package manager found. Please install 'tree' manually."`

---

### Sample Output

```plaintext
[INFO] Navigated to project root: /path/to/project
[INFO] Generating project structure...
[INFO] Updating README.md with the latest project structure...
[UPDATE] README.md has been successfully updated.
```

