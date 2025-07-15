#!/bin/sh

# Decompress assets.tar.gz into the current directory
# Usage: ./decompress.sh

ARCHIVE="assets.tar.gz"

if [ ! -f "$ARCHIVE" ]; then
    echo "Error: Archive '$ARCHIVE' not found."
    exit 1
fi

tar -xvzf "$ARCHIVE"

echo "Decompressed '$ARCHIVE'"
