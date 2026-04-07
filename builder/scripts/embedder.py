from jinja2 import Template
from dataclasses import dataclass
from pathlib import Path

from builder.utils.hash import fnv1a_hash
from builder.utils.context import EmbedderContext


@dataclass(frozen=True)
class FileContext:
    path: Path
    hash: int


def collect_files_to_embed(embed_path: Path) -> list[FileContext]:
    files_to_embed: list[FileContext] = []
    for filepath in embed_path.rglob("*"):
        if filepath.is_file():
            if filepath.stat().st_size == 0:
                continue

            relative_path: str = filepath.relative_to(embed_path).as_posix()

            file_context: FileContext = FileContext(
                path=relative_path,
                hash=fnv1a_hash(relative_path.encode("utf-8")),
            )
            files_to_embed.append(file_context)
    
    files_to_embed.sort(key=lambda x: x.path)

    return files_to_embed


def generate_file_content(template_path: Path, files_to_embed: list[FileContext]) -> str:
    template_text: str = template_path.read_text()
    template: Template = Template(template_text)
    result: str = template.render(files=files_to_embed)

    return result


def write_to_file_safely(path: Path, content: str):
    if not path.is_file():
        return

    original_content: str

    if path.exists():
        original_content = path.read_text()
    else:
        original_content = ""
    
    if original_content != content:
        path.write_text(content)


def embed(ctx: EmbedderContext) -> None:
    files_to_embed: list[FileContext] = collect_files_to_embed(ctx.embed_dir)

    content: str = generate_file_content(ctx.template_dir / "resources.c.template", files_to_embed)

    write_to_file_safely(ctx.result_dir / "resources.c", content)
