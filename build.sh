#!/bin/bash

VENV_DIR="./builder/.venv"
REQ_FILE="builder/requirements.txt"

if ! command -v python3 &> /dev/null; then
    echo "Error: python3 is not installed or not in your system PATH."
    exit 1
fi

if [ ! -d "$VENV_DIR" ]; then
    python3 -m venv "$VENV_DIR"
fi

source "$VENV_DIR/bin/activate"

if [ -f "$REQ_FILE" ]; then
    if ! python3 -m pip install -q -r "$REQ_FILE"; then
        echo "Error: Failed to verify or install required libraries."
        exit 1
    fi
fi

echo "Environment is healthy. Starting builder..."

python3 -m builder "$@"

deactivate
