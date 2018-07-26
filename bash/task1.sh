#!/bin/sh

if [ "$LANG" = '' ] ; then
    export LANG=C
fi

set -e -u

number_of_backups=''
output_file="$HOME/bash/task1.out"

print_help() {
    case "$LANG" in
        'uk_UA'*)
            echo "$(basename "$0") [-h|--help] [-n число] [файл]"
            echo
            echo "де  число -- кiлькiсть файлiв iз результатами,"
            echo "    файл -- шлях та iм\`я файла, у який треба записати результат;"
            echo
            echo "наприклад: [ws01@global ~]$ $(basename "$0") -n 9 ./test/info/info.txt"
            ;;
        *)
            echo "$(basename "$0") [-h|--help] [-n number] [file]"
            echo
            echo "where   number -- the number of backups to keep,"
            echo "        file -- the path to the file, where the information should be put;"
            echo
            echo "example: [ws01@global ~]$ $(basename "$0") -n 9 ./test/info/info.txt"
            ;;
    esac
}

print_unknown_parameter_error() {
    case "$LANG" in
        'uk_UA'*) echo "Програма отримала невiдомий параметр '$1'." >&2 ;;
        *) echo "The program received an unknown parameter '$1'." >&2 ;;
    esac
}

print_incorrect_value_error() {
    case "$LANG" in
        'uk_UA'*) echo "Програма отримала некоректне значення '$2' в '$1'." >&2 ;;
        *) echo "The program received an incorrect value '$2' as '$1'." >&2 ;;
    esac
}

ok_or_unknown() {
    if [ "$1" = '' ]; then
        echo 'Unknown'
    else
        echo "$1"
    fi
}

print_dmidecode() {
    ok_or_unknown "$( (dmidecode --string "$1" 2>/dev/null | xargs) || true)"
}

while [ "$#" -gt 0 ]; do
    case "$1" in
        -n)
            if [ "$#" -lt 2 ]; then
                print_incorrect_value_error 'number' ''
                exit 1
            fi
            number_of_backups="$2"
            case "$number_of_backups" in
                ''|*[!0-9]*)
                    print_incorrect_value_error 'number' "$number_of_backups"
                    exit 1
                    ;;
                *)
                    if [ "$number_of_backups" -lt 2 ]; then
                        print_incorrect_value_error 'number' "$number_of_backups"
                        exit 1
                    fi
                    ;;
            esac
            shift 2
            ;;

        -h|--help)
            print_help
            exit 0
            ;;

        -*)
            print_unknown_parameter_error "$1"
            exit 1
            ;;

        *)
            output_file="$1"
            break
            ;;
    esac
done

prepare_output_folder() {
    mkdir -p "$(dirname "$output_file")"

    if [ -f "$output_file" ]; then
        today="$(date -u '+%Y%m%d')"
        latest_revision_number=$( (ls -t "$output_file-$today-"* 2>/dev/null || true) | head -1 | sed 's/^.*-0\{0,3\}//')
        if [ "$latest_revision_number" = '' ]; then
            revision_number=0
        else
            revision_number=$((latest_revision_number + 1))
        fi
        backup_output_file="$output_file-$today-$(printf '%04d' "$revision_number")"
        mv "$output_file" "$backup_output_file"
    fi

    if [ "$number_of_backups" != '' ]; then
        (ls -t "$output_file-"* 2>/dev/null || true) | tail -n "+$((number_of_backups + 1))" | xargs --delimiter '\n' --no-run-if-empty rm
    fi
}

print_info() {
    echo "Date: $(date)"
    echo '---- Hardware ----'
    echo "CPU: "'"'"$(print_dmidecode 'processor-version')"'"'
    echo "RAM: $(( $(cat /proc/meminfo | grep MemTotal | awk '{print $2}') / 1024)) MB"
    echo "Motherboard: "'"'"$(print_dmidecode 'baseboard-manufacturer')"'"'", "'"'"$(print_dmidecode 'baseboard-product-name')"'"'""
    echo "System Serial Number: $(print_dmidecode 'system-serial-number')"
    echo '---- System ----'
    echo "OS Distribution: "'"'"$(. /etc/os-release && echo "$PRETTY_NAME")"'"'
    echo "Kernel version: $(uname -r)"
    echo "Installation date: $(ok_or_unknown "$(LANG=C dumpe2fs "$(LANG=C mount | grep 'on \/ ' | awk '{print $1}')" 2>/dev/null | grep 'Filesystem created' | sed 's/^[^:]*: *//')")"
    echo "Hostname: $(hostname)"
    echo "Uptime: $(LANG=C uptime | awk '{print $3}' | sed 's/,$//')"
    echo "Processes running: $(ps -axo pid= | wc -l)"
    echo "User logged in: $(users | tr ' ' '\n' | wc -l)"
    echo '---- Network ----'
    for iface in /sys/class/net/* ; do
        iface="$(basename "$iface")"
        addresses="$(ip addr show "$iface" | grep -oP 'inet6? [^ ]*' | cut -d ' ' -f 2)"
        if [ "$addresses" = '' ]; then
            echo "$iface: -/-"
        else
            for addr in $addresses ; do
                echo "$iface: $addr"
            done
        fi
    done
    echo '----"EOF"----'
}

prepare_output_folder
print_info > "$output_file"
