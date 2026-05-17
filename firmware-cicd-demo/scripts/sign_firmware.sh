#!/usr/bin/env bash
# sign_firmware.sh — Generate SHA256 checksums and optionally GPG-sign
# Usage: ./scripts/sign_firmware.sh <firmware_dir>

set -euo pipefail

FIRMWARE_DIR="${1:-build}"

echo "========================================"
echo "  Firmware Signing & Checksum"
echo "========================================"

if [ ! -d "$FIRMWARE_DIR" ]; then
    echo "[ERROR] Directory '$FIRMWARE_DIR' not found"
    exit 1
fi

for ext in bin hex elf; do
    FILE="$FIRMWARE_DIR/firmware.$ext"
    if [ -f "$FILE" ]; then
        sha256sum "$FILE" > "$FILE.sha256"
        echo "[✅] SHA256: $(cat $FILE.sha256)"
    fi
done

# GPG sign (optional — requires GPG_PRIVATE_KEY and GPG_PASSPHRASE in env)
if [ -n "${GPG_PRIVATE_KEY:-}" ] && [ -n "${GPG_PASSPHRASE:-}" ]; then
    echo ""
    echo "--- GPG signing ---"
    echo "$GPG_PRIVATE_KEY" | gpg --batch --import
    gpg --batch --yes \
        --passphrase "$GPG_PASSPHRASE" \
        --detach-sign --armor \
        "$FIRMWARE_DIR/firmware.bin"
    echo "[✅] GPG signature: $FIRMWARE_DIR/firmware.bin.asc"
else
    echo "[INFO] GPG_PRIVATE_KEY/GPG_PASSPHRASE not set — skipping GPG sign"
fi

echo ""
echo "Files produced:"
ls -lh "$FIRMWARE_DIR"/firmware.*
echo "========================================"
echo "  Signing complete"
echo "========================================"
