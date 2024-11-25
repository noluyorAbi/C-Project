### README for `setup_pre_commit.sh`

#### Overview

`setup_pre_commit.sh` is a helper script designed to set up a Git `pre-commit` hook for your repository. The `pre-commit` hook ensures that every commit is checked for code formatting and that the `README.md` file is updated with the latest project structure.

---

#### What Does It Do?

The script creates or overwrites a `pre-commit` hook in your Git repository. The hook performs the following actions before every commit:

1. **Formats the Code**: Runs `make format` to ensure all source files are properly formatted.
2. **Updates `README.md`**: Runs `make update_readme` to regenerate the project structure section in the `README.md`.
3. **Stages the Updated `README.md`**: Automatically stages changes to the `README.md` file.

---

#### Usage Instructions

1. **Save the Script**  
   Save the script as `setup_pre_commit.sh` in the directory `scripts/setup_precommit/` of your repository.

2. **Make the Script Executable**  
   Run the following command to make the script executable:

   ```bash
   chmod +x scripts/setup_precommit/setup_pre_commit.sh
   ```

3. **Run the Script**  
   Execute the script to set up the `pre-commit` hook:

   ```bash
   scripts/setup_precommit/setup_pre_commit.sh
   ```

4. **Verify the Hook**  
   Check that the `pre-commit` hook exists in `.git/hooks/`:
   ```bash
   ls .git/hooks/pre-commit
   ```

---

#### Requirements

- A valid `Makefile` in the root of the repository with the following targets:
  - `format`: Ensures code formatting.
  - `update_readme`: Updates the project structure in the `README.md`.
- Git must be initialized in the repository (`git init`).

---

#### Troubleshooting

1. **Script Fails to Navigate to the Repository Root**  
   Ensure the script is placed in `scripts/setup_precommit/` and your repository root contains a `.git` directory.

2. **The Hook Does Not Trigger**  
   Make sure the `pre-commit` hook is executable:

   ```bash
   chmod +x .git/hooks/pre-commit
   ```

3. **Make Commands Not Found**  
   Verify that your `Makefile` includes the `format` and `update_readme` targets.

4. **Existing Pre-Commit Hook**  
   This script overwrites any existing `pre-commit` hook. If you have other functionality in your hook, consider appending instead of overwriting.

---

#### Notes

- The `pre-commit` hook is local to each developer's environment and is not automatically shared with others. For shared configurations, consider using tools like [pre-commit](https://pre-commit.com/).
- Use this script to standardize pre-commit behavior across your team by sharing it in your repository.
