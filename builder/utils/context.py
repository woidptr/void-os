from pathlib import Path
from dataclasses import dataclass


@dataclass(frozen=True)
class EmbedderContext:
    embed_dir: Path
    template_dir: Path
    result_dir: Path
