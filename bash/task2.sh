#!/bin/sh

set -e

print_help() {
    echo "$(basename "$0") [-h|--help]"
}

print_unknown_parameter() {
    case "$LANG" in
        'uk_UA'*) echo "Програма отримала невiдомий параметр '$1'." >&2 ;;
        *) echo "The program received an unknown parameter '$1'." >&2 ;;
    esac
}

while [ "$#" -gt 0 ]; do
    case "$1" in
        -h|--help) print_help ; exit 0;;
        *) print_unknown_parameter "$1"; exit 1;;
    esac
done

INSTALLATION_ROOT=/usr/local/bin

mkdir -p "$INSTALLATION_ROOT"

install "$(dirname "$0")/task1.sh" "$INSTALLATION_ROOT/"
