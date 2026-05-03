.PHONY: build test

build:
	@python3 scripts/init_build.py

test: build
	@python3 scripts/run_tests.py

clean:
	rm -rf $(BUILD_DIR)
