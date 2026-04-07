def fnv1a_hash(data: bytes) -> int:
    FNV_OFFSET_BASIS: int = 0x811c9dc5
    FNV_PRIME: int = 0x01000193
    MASK: int = 0xFFFFFFFF

    h: int = FNV_OFFSET_BASIS
    for byte in data:
        h = (h ^ byte) & MASK
        h = (h * FNV_PRIME) & MASK
    
    return h
