from pathlib import Path
from subprocess import run, PIPE, TimeoutExpired

SOLVER = "z3"

CVC5_PARAMS = ["--mbqi-fast-sygus", "--incremental"]
SOLVER_PARAMS = CVC5_PARAMS if SOLVER == "cvc5" else []

BUILD_DIR = Path("build")


class colors:
    red = "\033[91m"
    green = "\033[92m"
    white = "\033[m"
    yellow = "\033[93m"


def print_ok(text):
    print(f"{colors.green}{text}{colors.white}")


def print_err(text):
    print(f"{colors.red}{text}{colors.white}")


def print_warning(text):
    print(f"{colors.yellow}{text}{colors.white}")


def error(msg):
    print_err("ERROR: " + msg)
    exit(1)


def smoke_test():
    # Verify that selected solver is available
    try:
        run([SOLVER, "--version"], stdout=PIPE, stderr=PIPE)
    except Exception as e:
        print(e)
        error(f"SMT solver (configured to {SOLVER}) is not installed or is not in PATH")


def check_build_exists():
    if not (BUILD_DIR.exists() or BUILD_DIR.is_dir()):
        error("Directory build does not exist")
    if not any(BUILD_DIR.iterdir()):
        error("Directory build is empty")


def run_file(file):
    print(f"Running {file}:")
    try:
        result = run([SOLVER, *SOLVER_PARAMS, file], capture_output=True, timeout=60)
        stdout = result.stdout.decode().strip()

        res = stdout.split("\n")
        if result.returncode == 0 and all(map(lambda x: x in ["sat", "unsat"], res)):
            print_ok("[OK] " + stdout)
        else:
            print_err("[ERR] " + stdout)

    except TimeoutExpired:
        print_err("timeout after 60 seconds")


def sanity_check():
    print(f"Running sanity checks in template:")
    try:
        result = run(
            [SOLVER, *SOLVER_PARAMS, "project2.smt2"], capture_output=True, timeout=60
        )
        stdout = result.stdout.decode().strip()

        res = stdout.split("\n")
        if result.returncode == 0 and all(map(lambda x: x in ["sat", "unsat"], res)):
            print_ok("[OK] " + stdout.replace("\n", ", "))
        else:
            print_err("[ERR] " + stdout)
            print("Sanity check failed. Ignoring further tests...")
            exit(1)

    except TimeoutExpired:
        print_err("timeout after 60 seconds")
        print("Sanity check failed. Ignoring further tests...")
        exit(1)


def run_tests():
    sanity_check()
    for file in sorted(BUILD_DIR.glob("*.smt2")):
        run_file(file)


def main():
    smoke_test()
    check_build_exists()
    run_tests()


if __name__ == "__main__":
    main()
