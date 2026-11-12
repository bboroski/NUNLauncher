# Contributing to NUNLauncher

NUNLauncher owns boot-image construction, manifests, verification, and the handoff into NUN. Preserve the contract between the launcher and the operating system; do not silently broaden accepted image formats or trust boundaries.

## Required workflow

1. Open or link an Issue and identify the image format, manifest field, verifier, or target platform affected.
2. Use a focused branch: `feat/<topic>`, `fix/<topic>`, `docs/<topic>`, or `security/<topic>`.
3. Keep schema changes explicit and versioned; reject malformed or ambiguous image metadata.
4. Never commit private firmware, credentials, device identifiers, or generated images without a documented fixture requirement.
5. Test both valid and adversarial/invalid manifest cases.

## Required validation

```bash
cmake -S . -B build -DBUILD_TESTING=ON
cmake --build build --parallel
ctest --test-dir build --output-on-failure
python -m unittest discover -s tests -p 'test_*.py' -v
```

Run boot-image validation and supervisor workflows when changing image or handoff behavior.

## Commits and releases

Use Conventional Commit subjects (`feat:`, `fix:`, `docs:`, `refactor:`, `perf:`, `test:`, `build:`, `ci:`, `chore:`). Breaking changes require `!:` and a `BREAKING CHANGE:` footer. Releases use SemVer; update image/schema compatibility notes before tagging.

## Pull request checklist

- Document affected image formats and schema versions.
- Include tests for malformed, truncated, and mismatched images.
- State compatibility with NUN and target platforms.
- Provide migration and rollback steps for boot handoff changes.
