#!/bin/bash

usage() {
    echo "Usage: ${0} name resolution [music]"
    echo ""
    echo "name: the name of the platform"
    echo "resolution: the resolution of the target device's screen"
    echo "music (optional): the file type the music, ogg for example"
    exit 1
}

if [ -z "${1}" ]||[ -z "${2}" ]; then
    echo "Please fill in all required fields"
    usage
fi

# Set working directory to the repo root
cd "$(dirname "$0")"/..
WORKDIR="${PWD}"

# Store input under readable names
PLATFORM=${1}
RESOLUTION=${2}
MUSICFILETYPE=${3}

BACKGROUNDS_DIR="platform/${PLATFORM}/assets/backgrounds"

# Make directories
mkdir -p "${BACKGROUNDS_DIR}"

FILES_CHANGED=false

# Convert backgrounds to right size
for background in $(find assets/backgrounds/ -type f); do
    background_new="platform/${PLATFORM}/${background}"
    if [ ! -f "${background_new}" ]; then
        convert -resize "${RESOLUTION}!" "${background}" "${background_new}"
        FILES_CHANGED=true
    fi
done

if [ -n "${MUSICFILETYPE}" ]; then
    mkdir -p "platform/${PLATFORM}/assets/music"
    for music in $(find assets/music/ -type f); do
        music_new="platform/${PLATFORM}/$(echo "${music}"|cut -f1 -d".").${MUSICFILETYPE}"
        if [ ! -f "${music_new}" ]; then
            ffmpeg -i "${music}" "${music_new}"
            FILES_CHANGED=true
        fi
    done
fi

if [ "${FILES_CHANGED}" = true ]; then
    echo "The platform ${PLATFORM} was updated, changes can be found in platform/${PLATFORM}"
else
    echo "The platform ${PLATFORM} was up to date already, nothing was changed"
fi
