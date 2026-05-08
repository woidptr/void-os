import sys
import shutil
import argparse
import subprocess
from typing import Any
from jinja2 import Template


def check_dependencies(*deps: str) -> None:
    for dep in deps:
        if shutil.which(dep) is None:
            print(f"[FATAL] Missing dependency: [{dep}] is not installed or not in PATH.")
            sys.exit(1)


def gather_build_context(args: argparse.Namespace) -> dict[str, Any]:
    llvm_prefix: str = subprocess.check_output(["brew", "--prefix", "llvm"]).decode().strip()
    cc: str = f"{llvm_prefix}/bin/clang"
    cxx: str = f"{llvm_prefix}/bin/clang++"

    check_dependencies(cc, "xorriso", "ninja")


def run_build(args: argparse.Namespace) -> None:
    pass