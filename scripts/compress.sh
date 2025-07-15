#!/bin/sh

# Compress the "assets" directory into assets.tar.gz
# Usage: ./compress.sh

ASSET_DIR="assets"
OUTPUT="assets.tar.gz"

if [ ! -d "$ASSET_DIR" ]; then
    echo "Error: Directory '$ASSET_DIR' does not exist."
    exit 1
fi

tar -czvf "$OUTPUT" "$ASSET_DIR"

echo "Compressed '$ASSET_DIR' into '$OUTPUT'"
