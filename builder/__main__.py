import argparse
from pathlib import Path

from builder.utils.context import EmbedderContext
from builder.scripts.embedder import embed


def main() -> None:
    parser = argparse.ArgumentParser(description="Build Script")
    parser.add_argument(
        "--target",
        type=str,
        choices=["build", "run", "clean"],
        help="The action to perform (defaults to build)",
    )
    parser.add_argument(
        "--profile",
        type=str,
        choices=["debug", "release"],
        default="debug",
        help="Specify the build profile (choices: debug, release. default: debug)",
    )
    parser.add_argument(
        "--arch",
        type=str,
        choices=["x86", "arm64"],
        default="x86",
        help="Specify the target architecture (choices: x86, arm64. default: x86)",
    )
    parser.add_argument("--rootdir", type=Path, help="Root directory path")

    args = parser.parse_args()

    # source_dir: Path = args.rootdir

    # embed(EmbedderContext(
    #     embed_dir = source_dir / "assets",
    #     template_dir = source_dir / "builder" / "templates",
    #     result_dir = source_dir / "src" / "core",
    # ))


if __name__ == "__main__":
    main()
