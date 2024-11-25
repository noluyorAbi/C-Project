#!/bin/bash

# Navigate two directories up from the current script's location
# "$(dirname "$0")" gives the directory where this script resides.
# "/../.." moves two levels up to reach the root of the repository.
cd "$(dirname "$0")/../.."
if [ $? -ne 0 ]; then
    # If the navigation fails, print an error message and exit with status 1
    echo "Error: Failed to navigate two directories up. Ensure the script is placed in the correct directory."
    exit 1
fi

# Define the path to the pre-commit hook file in the Git hooks directory
HOOK_PATH=".git/hooks/pre-commit"

# Define the content of the pre-commit hook
# This script ensures code formatting and README updates before every commit
HOOK_CONTENT='#!/bin/bash
make format         # Format the code according to the project's style rules
make update_readme  # Update the project structure in the README.md file
git add README.md   # Stage the updated README.md file for the commit
'

# Check if the .git directory exists in the current location
# This confirms that the script is running in a Git repository's root directory
if [ ! -d ".git" ]; then
    # Print an error message if the .git directory is missing and exit with status 1
    echo "Error: This script must be executed from the root of a Git repository."
    exit 1
fi

# Write the defined content into the pre-commit hook file
# This creates or overwrites the file with the specified content
echo "$HOOK_CONTENT" > "$HOOK_PATH"

# Make the pre-commit hook executable so that Git can run it
chmod +x "$HOOK_PATH"

# Print a success message to indicate the hook has been set up
echo "The pre-commit hook has been successfully configured."
