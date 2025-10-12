# Commiting Guidelines

## Commit Structure

- **Make Commits Atomic**: Each commit should represent a single logical change. Avoid mixing multiple changes in a single commit. If a change affects multiple files, ensure it constitutes one "logical" change.

### Writing Commit Messages

- **Clear Subject Line**: Start the commit message with a clear and concise description of the change. It should be no more than 50 characters, start with a capital letter, and be in imperative mood.

  - > ```bash
    > feat: Add function to calculate average
    > ```

- **Separate Subject from Body**: If further explanation is needed, include it in the body of the commit message, following a blank line after the subject line.

- **Explain the Change**: The body of the commit message should explain why the change was needed and how it was implemented. Keep it wrapped to 72 characters and written in present tense.

  - > ```bash
    > fix: Resolve issue with login form not submitting on Safari
    >
    > This commit fixes a bug where the login form was not submitting on Safari browsers. The issue was caused by an outdated event listener binding, which has been updated to the correct syntax.
    > ```

- **Avoid "How" Details**: Use the commit message to explain what changes were made and why, not how they were made. Well-written code should be self-explanatory.

- **Reference Issues or Pull Requests**: If the commit is related to an issue or a pull request, include the reference in the commit message. This can be done in the body of the message or at the end.

  - > ```bash
    > feat: Add user authentication feature
    >
    > Closes #123
    > ```

- **Avoid Unnecessary Punctuation**: Do not end the subject line with a period.

### Commit Message Structure Rules

- **Type Enum**: Ensure your commit type matches one of the predefined values: `build`, `chore`, `ci`, `docs`, `feat`, `fix`, `perf`, `refactor`, `revert`, `style`, `test`.

  - `build`: This type is used for changes related to build processes or tools. It includes modifications to build scripts, configuration files, and build-related dependencies.

  - `chore`: Chore changes typically involve tasks related to maintenance or housekeeping. This could include updating dependencies, cleaning up code, or configuring development tools.

  - `ci`: Changes related to Continuous Integration (CI) configuration files and scripts fall under this type. It includes modifications to CI pipelines, configurations for automated testing, and integration with CI services.

  - `docs`: Documentation changes belong to this type. It covers updates to documentation files, such as READMEs, API documentation, or inline code comments.

  - `feat`: This type is for new feature implementations or enhancements to existing features. It signifies significant additions or improvements to the codebase's functionality.

  - `fix`: Use this type for bug fixes or patches. It indicates changes made to address issues or bugs in the codebase.

  - `perf`: Performance-related changes are categorized under this type. It includes optimizations aimed at improving the codebase's performance, such as reducing execution time or memory usage.

  - `refactor`: Refactoring changes fall under this type. It involves restructuring or cleaning up the code without changing its external behavior. Refactoring aims to improve code quality, maintainability, or readability.

  - `revert`: Reverting changes made in a previous commit is indicated by this type. It's used when undoing the effects of a specific commit, often to address unexpected issues or regressions introduced by the previous change.

  - `style`: Style-related changes belong to this type. It covers modifications to code style, formatting, or whitespace, without affecting the code's functionality.

  - `test`: Test-related changes are categorized under this type. It includes additions or modifications to test cases, test suites, or testing infrastructure to ensure code correctness and reliability.

- **Type Case**: Use lowercase for commit types.

- **Subject Not Empty**: The subject line should not be empty.

- **Subject Case**: Use sentence case for the subject line.

- **Subject Length**: Keep the subject line under 100 characters.

- **Body Leading Blank**: Ensure there's a leading blank line in the commit body.

- **Body Max Line Length**: Keep each line in the commit body under 100 characters.
