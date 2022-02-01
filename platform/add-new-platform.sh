#!/bin/bash

usage() {
    echo "Usage: ${0} name shellSize resolution"
    echo ""
    echo "name: the name of the platform"
    echo "shellSize: the size of the shells. The screen should be at least 8x8 shell sizes"
    echo "resolution: the resolution of the target device's screen"
    exit 1
}

if [ -z "${1}" ]||[ -z "${2}" ]||[ -z "${3}" ]; then
    echo "Please fill in all required fields"
    usage
fi

# Set working directory to the repo root
cd "$(dirname "$0")"/..
WORKDIR="${PWD}"

# Store input under readable names
PLATFORM=${1}
SHELLSIZE=${2}
RESOLUTION=${3}
SHELLRESOLUTION="$((${SHELLSIZE} * 7))x${SHELLSIZE}"

BACKGROUNDS_DIR="platform/${PLATFORM}/assets/backgrounds"
IMAGES_DIR="platform/${PLATFORM}/assets/images"

# Make directories
mkdir -p "${BACKGROUNDS_DIR}" "${IMAGES_DIR}"

# Create image
convert -background none -density 1200 -resize "${SHELLRESOLUTION}" platform/shells.svgz assets/images/shells${SHELLSIZE}.png

# Convert backgrounds to right size
for background in $(find assets/backgrounds/ -type f); do
    convert -resize "${RESOLUTION}" "${background}" "platform/${PLATFORM}/${background}"
done

echo "The platform directory has been created in platform/${PLATFORM}."
echo "Please change the shell size in src/constants.h and make sure the assets are copied at build in CMakeLists.txt."
