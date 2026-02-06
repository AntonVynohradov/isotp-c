# Repository Configuration Guide

## GitHub Actions Workflow

The repository uses automated GitHub Actions workflows to enforce code quality and review standards.

### Active Workflows

1. **code-review-check.yml**
   - Location: `.github/workflows/code-review-check.yml`
   - Triggers: On pull requests to `development` or `main` branches
   - Checks:
     - Copilot code review is approved
     - All linked issues are closed
     - Code quality standards are met

## Branch Protection Setup

### development Branch

The `development` branch is protected and requires:

1. **Copilot Code Review**
   - Mandatory approval from Copilot
   - Validates against CERT C Coding Standard
   - Uses `.copilot-instructions.md` guidelines

2. **Related Issues Resolution**
   - All issues linked in PR must be closed
   - Prevents incomplete work from being merged

3. **Status Checks**
   - All GitHub Actions checks must pass
   - Code quality validation
   - Compilation without critical warnings

4. **Human Reviews**
   - Minimum 2 approvals required
   - At least 1 from human reviewer
   - 1 from Copilot AI review

5. **Up-to-Date Branches**
   - Branch must be up-to-date with development before merge
   - Prevents conflicts and stale code

### main Branch

The `main` branch mirrors protection rules from `development` with additional:
- Requires stable release version
- May require additional security review
- Documentation must be complete

## Manual Branch Protection Configuration

To manually configure branch protection rules (in case they're not set up):

1. Go to GitHub Repository → Settings → Branches
2. Click "Add rule"
3. Enter branch name pattern: `development`
4. Enable:
   - ✅ Require a pull request before merging (2 approvals)
   - ✅ Require status checks to pass
   - ✅ Require conversation resolution before merging
   - ✅ Require branches to be up to date

5. Select required status checks:
   - `Verify Copilot Code Review`
   - `Verify Related Issues Are Closed`
   - `Merge Readiness Summary`

## Copilot Code Review Guidelines

Copilot reviews all PRs based on:
- `.copilot-instructions.md` - Comprehensive CERT C guidelines
- CERT C Coding Standard rules
- Embedded systems best practices
- ISO-TP-C specific requirements

### Review Focus Areas

1. **Security**
   - Buffer overflows
   - Integer overflows
   - Memory leaks
   - Input validation

2. **Code Quality**
   - CERT C compliance
   - Type safety
   - Error handling
   - Memory management

3. **Performance**
   - Real-time constraints
   - Memory footprint
   - Efficiency optimizations

4. **Maintainability**
   - Code clarity
   - Documentation
   - Complexity management

## Pull Request Workflow

### 1. Create Pull Request

```bash
# Create your branch
git checkout development
git pull origin development
git checkout -b feature/your-feature-name

# Make your changes
# Commit with clear messages
git commit -m "feat: implement feature X"

# Push to remote
git push origin feature/your-feature-name
```

### 2. Submit PR

- Use the PR template (`.github/pull_request_template.md`)
- Link related issues (e.g., "Fixes #123")
- Ensure title is clear and descriptive
- Fill all checkboxes appropriately

### 3. Copilot Code Review (Automatic)

- Copilot automatically reviews your code
- Checks against CERT C standards
- Provides specific feedback
- May request changes

### 4. Address Copilot Comments

- Review all Copilot suggestions
- Make necessary code changes
- Respond to comments
- Push updated changes

### 5. Request Human Review

- After Copilot approves, request human reviews
- Assign to code owners if applicable
- Ensure PR description is complete

### 6. Verification Checks

- GitHub Actions validates:
  - Copilot approval ✅
  - Linked issues closed ✅
  - Code quality ✅
  - Tests passing ✅

### 7. Merge

- Only available after all checks pass
- Maintainer merges to development
- CI/CD pipeline may trigger deployment

## Troubleshooting

### Copilot Review Pending

**Problem:** "Copilot code review pending"

**Solution:**
1. Wait for Copilot to complete review (usually 5-15 minutes)
2. If stuck, close and reopen the PR
3. Check GitHub Actions workflow for errors

### Linked Issues Not Closed

**Problem:** "The following linked issues are still open: #123"

**Solution:**
1. Close the linked issues on GitHub
2. Or remove the issue reference from PR body if not related
3. Re-run the workflow check

### Status Checks Failed

**Problem:** "code-quality checks failed"

**Solution:**
1. Review workflow logs in GitHub Actions
2. Address code quality issues locally
3. Run `cppcheck` locally: `cppcheck --enable=all Src/ Inc/`
4. Fix compiler warnings
5. Push changes to re-run workflow

### Merge Button Disabled

**Problem:** "This branch has 1 failing check" or "Waiting for status checks"

**Solution:**
1. Click "Details" on failing check
2. Review the error
3. Address the issue
4. Push a new commit to retry

## Local Testing

Before pushing, validate locally:

```bash
# Run static analysis
cppcheck --enable=all --suppress=missingIncludeSystem Src/ Inc/

# Compile with strict warnings
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make VERBOSE=1

# Run tests
ctest --output-on-failure

# Run code review manually (if pylint/flake8 available)
python -m pylint .github/workflows/
```

## Updating Branch Protection Rules

If you need to update rules:

1. Go to Settings → Branches
2. Find the rule for `development`
3. Click the pencil icon to edit
4. Make changes (e.g., increase approval count)
5. Save

**Note:** Only repository administrators can modify branch protection rules.

## Emergency Procedures

### Critical Security Fix

If a critical security fix is needed:

1. Create PR normally
2. Get Copilot approval
3. Contact repository admin for potential bypass
4. Document reason in PR comments
5. After merge, review in retrospective

### Revert Changes

If problematic code was merged:

```bash
# Create revert PR
git revert <commit-hash>
git push origin feature/revert-<commit-hash>

# Submit as normal PR
# Follow all review processes
# Merge after approval
```

---

**Document Version:** 1.0  
**Last Updated:** February 2026  
**Maintained by:** Repository Maintainers
