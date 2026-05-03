from pathlib import Path
from run_tests import print_err, print_warning

import re
import stat
import shutil

tests_dir = Path("../tests")
build_dir = Path("build")
solution = Path("project2.smt2")


def init_build_dir():
    shutil.rmtree("build", ignore_errors=True)
    build_dir.mkdir()


def read_solution():
    solution_text = solution.read_text()
    match = re.search(
        r";; START OF SOLUTION(.*?);; END OF SOLUTION", solution_text, re.DOTALL
    )
    if not match:
        print_err(
            "File project.smt2 is broken:\n"
            + 'Tags ";; START OF SOLUTION" or ";; END OF SOLUTION" are missing'
        )
        exit(1)
    return match.group(0).strip()


def copy_tests(solution):
    for test in tests_dir.glob("*.smt2"):
        test_text = test.read_text()

        if not "set-info" in test_text:
            print_warning(f"Warning: expected status missing in test {test}")
        if not "(check-sat" in test_text:
            print_warning(f"Warning: check-sat command missing in test {test}")
        if not "; <script_input>" in test_text:
            print_warning(f'Warning: "; <script_input>" tag missing in test {test}')

        test_text = test_text.replace("; <script_input>", solution)
        path = build_dir / test.name
        path.write_text(test_text)

        # Make test files read only for user
        path.chmod(stat.S_IREAD)


def main():
    init_build_dir()
    s = read_solution()
    copy_tests(s)


if __name__ == "__main__":
    main()
