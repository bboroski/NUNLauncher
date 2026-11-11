PYTHON ?= python3

.PHONY: help test verify
help:
	@printf '%s\n' \
	  'NUNLauncher boot supervisor:' \
	  '  make test    Run host-side verifier tests' \
	  '  make verify IMAGE=... MANIFEST=...    Verify a NUN image before boot'

test:
	PYTHONPATH=. $(PYTHON) -m unittest discover -s tests -v

verify:
	@test -n "$(IMAGE)" -a -n "$(MANIFEST)" || { echo 'Set IMAGE=... and MANIFEST=...' >&2; exit 2; }
	PYTHONPATH=. $(PYTHON) -m nunlauncher_boot.cli verify --image "$(IMAGE)" --manifest "$(MANIFEST)"
