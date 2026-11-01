# FizzBuzz

A simple CLI application that prints FizzBuzz from 0 to 1000.

- **macOS:** Quad-Fat binary (ppc, i386, x86_64, arm64) — macOS 10.4+
- **iOS:** FAT binary (armv7, arm64) — iOS 5+

## Develop

Inside the Altivec container (see `compose.yml`):

```sh
make test-host          # Portable C tests (no SDK needed)
make check-project      # Metadata and plist validation
make print-config       # Show build configuration
make macOS-release      # Build macOS quad-fat ZIP
make iOS-release        # Build iOS FAT ZIP
make macOS-analyze      # Clang static analysis
make iOS-analyze        # Clang static analysis
make macOS-validate-package  # Validate package slices and metadata
make iOS-validate-package    # Validate package slices and metadata
```

After both debug and release builds, run `python3 tools/check-artifacts.py`
for archive deployment floors, dynamic dependencies, and public symbols.

## Tests

- `make test-host` — FizzBuzz unit tests, external library consumer, and
  native CLI smoke test. `SANITIZE=1` adds AddressSanitizer/UBSan.
- `make check-project` — Project metadata, plist, and artwork validation.
- `make validate-package` — Package slice, metadata, and signing validation.
- `make test-native` — Explicit SSH smoke test (requires NATIVE_HOST,
  NATIVE_STAGE, NATIVE_PLATFORM).
- `python3 tools/check-artifacts.py` — Artifact inspection after builds.

## Release

```sh
altivec-release build       # Build all configured targets
altivec-release stage 1.0.0 --dist dist  # Stage release assets
```

## GitHub Actions

See `.github/workflows/ci.yml` and `.github/workflows/release.yml`.
CI builds and tests the application on Linux using the Altivec container.
The release workflow requires `ALTIVEC_SDK_ARCHIVE_DIR` to be set as a
GitHub secret (or the SDK archives must be available in the container).
