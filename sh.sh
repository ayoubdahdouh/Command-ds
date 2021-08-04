#!/bin/bash
# For LS_COLORS, print type and description in the relevant color.

declare -A descriptions=(
    [bd]="block device"
    [ca]="file with capability"
    [cd]="character device"
    [di]="directory"
    [do]="door"
    [ex]="executable file"
    [fi]="regular file"
    [ln]="symbolic link"
    [mh]="multi-hardlink"
    [mi]="missing file"
    [no]="normal non-filename text"
    [or]="orphan symlink"
    [ow]="other-writable directory"
    [pi]="named pipe, AKA FIFO"
    [rs]="reset to no color"
    [sg]="set-group-ID"
    [so]="socket"
    [st]="sticky directory"
    [su]="set-user-ID"
    [tw]="sticky and other-writable directory"
)

IFS=:
IFS=:
for ls_color in $LS_COLORS; do
    color="${ls_color#*=}"
    type="${ls_color%=*}"

    # Add description for named types.
    desc="${descriptions[$type]}"

    # Separate each color with a newline.
    if [[ $color_prev ]] && [[ $color != "$color_prev" ]]; then
        echo
    fi

    printf "033[%s\n\t\e[%sm%s%s\e[m " "$color" "$color" "$type" "${desc:+ ($desc)}"

    # For next loop
    color_prev="$color"
done
echo*