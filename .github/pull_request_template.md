## Description
Brief description of the changes in this pull request.

## Type of Change
- [ ] Bug fix (non-breaking change that fixes an issue)
- [ ] New feature (non-breaking change that adds functionality)
- [ ] Breaking change (fix or feature that would cause existing functionality to change)
- [ ] Documentation update
- [ ] Refactoring (code reorganization without behavior change)
- [ ] Performance optimization
- [ ] Test improvement

## Related Issues
Link any issues this PR addresses:
- Fixes #(issue number)
- Closes #(issue number)
- Related to #(issue number)

## Changes Made
Describe the specific changes:
- Change 1
- Change 2
- Change 3

## Testing Performed
- [ ] Unit tests added/updated (GoogleTest, `tests/unit/`)
- [ ] Integration tests pass (pytest, `tests/integration/`, including the C/Python cross-check)
- [ ] Reference frames still pass (`docs/protocol.md`, `tests/unit/gtest_isotp_decode.cpp`)
- [ ] No regression detected

## MISRA C:2012 Compliance Checklist
- [ ] `misra-analyze` reports no new violations
- [ ] Every remaining deviation is documented (rule, location, rationale)
- [ ] No dynamic memory allocation (Rule 21.3)
- [ ] No recursion (Rule 17.2)
- [ ] All non-void return values are used or explicitly cast to `void` (Rule 17.7)
- [ ] Essential type rules respected; no implicit narrowing (Rules 10.x)
- [ ] All `if ... else if` chains end with `else` (Rule 15.7)
- [ ] `switch` statements have a `default` clause (Rule 16.4)

## Code Review Checklist
- [ ] All Copilot code review comments addressed
- [ ] Code style is consistent (`clang-format` clean)
- [ ] Builds without warnings with `-Wall -Wextra -Wpedantic -Wconversion`
- [ ] Comments explain "why", not "what"
- [ ] Public API items have Doxygen comments
- [ ] Documentation updated if needed

## Embedded Systems Considerations
- [ ] Library code includes only `<stddef.h>`, `<stdint.h>`, `<stdbool.h>`, `<string.h>` and ISOTP headers
- [ ] No OS, driver or hardware dependency added
- [ ] No global mutable state; all state in caller-owned structures
- [ ] Stack and RAM footprint impact documented (`docs/configuration.md`)
- [ ] Behavior matches the ISO 15765-2 (ISO-TP) specification; wire-format changes are a new major version

## Breaking Changes
- [ ] This PR introduces breaking changes
- [ ] API changes documented
- [ ] Migration guide provided (if needed)

## Additional Notes
Any additional context or notes for reviewers:

---

**Before merging, ensure:**
1. ✅ Copilot code review is approved
2. ✅ All linked issues are closed or resolved
3. ✅ All status checks pass
4. ✅ At least 2 approvals (Copilot + 1 human reviewer)
5. ✅ Branch is up to date with development
