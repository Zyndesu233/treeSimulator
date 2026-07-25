#!/bin/sh
set -eu

if [ "$#" -lt 1 ] || [ "$#" -gt 2 ]; then
    echo "Usage: $0 INPUT_FILE [EXPECTED_OUTPUT]" >&2
    exit 2
fi

input_file=$1
expected_file=${2:-}
binary="./bin/tree-simulator"
output_file=$(mktemp)

cleanup() {
    rm -f "$output_file"
}
trap cleanup EXIT

if [ ! -x "$binary" ]; then
    echo "Simulator binary not found. Run make first." >&2
    exit 2
fi

"$binary" simulate "$input_file" > "$output_file"

if [ -n "$expected_file" ]; then
    diff -u "$expected_file" "$output_file"
    echo "[PASS] $input_file matched $expected_file"
else
    cat "$output_file"
fi
