def fnv1a_hash(data: bytes) -> int:
    FNV_OFFSET_BASIS: int = 0xcbf29ce484222325
    FNV_PRIME: int = 0x100000001b3
    MASK: int = 0xFFFFFFFFFFFFFFFF

    h: int = FNV_OFFSET_BASIS
    for byte in data:
        h = (h ^ byte) & MASK
        h = (h * FNV_PRIME) & MASK
    
    return h
