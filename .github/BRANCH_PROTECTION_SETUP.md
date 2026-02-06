# Branch Protection Rules Configuration for development branch

This document describes the GitHub branch protection rules that should be configured to enforce:
- Mandatory Copilot code review
- All open issues must be resolved
- Successful status checks
- Up-to-date branches before merge

## Setup Instructions

### 1. Navigate to Repository Settings

1. Go to your GitHub repository
2. Click **Settings** tab
3. In left sidebar, click **Branches**
4. Click **Add rule** under "Branch protection rules"

### 2. Configure Protection Rule for `development` branch

#### Step 1: Branch Name Pattern
- **Branch name pattern:** `development`

#### Step 2: Protect Matching Branches

✅ **Require a pull request before merging**
- Require approvals: **2** (one from Copilot, one from human reviewer)
- Dismiss stale pull request approvals when new commits are pushed: ✅
- Require review from Code Owners: ✅ (if using CODEOWNERS file)

✅ **Require code review from Copilot**
- This is validated by the GitHub Actions workflow in `.github/workflows/code-review-check.yml`
- The workflow checks for Copilot approval before allowing merge

✅ **Require status checks to pass before merging**
Status checks required:
- `Verify Copilot Code Review` (mandatory-code-review / check-code-review)
- `Verify Related Issues Are Closed` (mandatory-code-review / check-open-issues)
- `Code Quality Checks` (mandatory-code-review / check-code-quality)
- `Merge Readiness Summary` (mandatory-code-review / summary)

Require branches to be up to date before merging: ✅

✅ **Require conversation resolution before merging**
- Require all comments to be resolved: ✅

✅ **Restrict who can push to matching branches**
- Allow specified actors to bypass required pull requests: 
  - Administrators only (recommended for hotfixes)

✅ **Require deployment to specific environments before merging** (optional)
- Environment: `staging` (optional, for integration testing)

### 3. Configure CODEOWNERS (Optional but Recommended)

Create `.github/CODEOWNERS` file:

```
# Global owners
* @maintainer-username

# Specific directories
Src/ @maintainer-username
Inc/ @maintainer-username
tests/ @maintainer-username

# Configuration files
CMakeLists.txt @maintainer-username
.github/ @maintainer-username
```

### 4. GitHub Actions Configuration

The workflow file `.github/workflows/code-review-check.yml` automatically:

1. **Checks Copilot Code Review**
   - Verifies that Copilot has submitted an APPROVED review
   - Blocks merge if review is missing or not approved

2. **Verifies Linked Issues Are Closed**
   - Extracts issue references from PR body (e.g., `Fixes #123`)
   - Checks that all linked issues are in CLOSED state
   - Blocks merge if any linked issue is still open

3. **Validates Code Quality**
   - Runs static analysis (cppcheck)
   - Checks for unsafe functions
   - Verifies compilation without critical warnings
   - Non-blocking warnings for informational purposes

4. **Provides Merge Readiness Summary**
   - Shows status of all critical checks
   - Blocks merge if critical checks fail

## Pull Request Template

Create `.github/pull_request_template.md`:

```markdown
## Description
Brief description of the changes.

## Type of Change
- [ ] Bug fix
- [ ] New feature
- [ ] Breaking change
- [ ] Documentation update
- [ ] Refactoring
- [ ] Performance optimization

## Related Issues
Fixes #(issue number)
Closes #(issue number)

## Testing
- [ ] Unit tests added/updated
- [ ] Manual testing completed
- [ ] No breaking changes introduced

## Checklist
- [ ] My code follows the CERT C Coding Standard
- [ ] Code review guidelines in `.copilot-instructions.md` are met
- [ ] All linked issues are resolved
- [ ] All tests pass
- [ ] Documentation is updated
- [ ] No new compiler warnings introduced
```

## Required GitHub Permissions

Ensure the GitHub Actions workflow has necessary permissions:

```yaml
permissions:
  pull-requests: read
  issues: read
  contents: read
  checks: read
```

## Bypassing Protection (Emergency Only)

Only repository admins can bypass branch protection. To do so:
1. Go to PR
2. Click "Merge without waiting for checks to pass" (only for admins)
3. Document the reason for bypass

**Note:** Bypass should be used only for critical security or production hotfixes.

## Troubleshooting

### "Copilot code review pending"
- Wait for Copilot to complete its review
- Address any comments from Copilot
- Request Copilot approval again if needed

### "Related issues still open"
- Close all linked issues in the PR body before merge
- Or remove issue references if not related
- Update PR description with correct issue references

### "Status checks failed"
- Check the workflow run details
- Address any code quality issues
- Ensure all tests pass locally before pushing

## Best Practices

1. **Linking Issues:** Always link related issues in PR description
2. **Copilot Review:** Address all Copilot suggestions before requesting human review
3. **Testing:** Ensure all tests pass before opening PR
4. **Documentation:** Update docs and comments for significant changes
5. **Communication:** Use PR description and comments to explain design decisions

---

**Document Version:** 1.0  
**Last Updated:** February 2026
