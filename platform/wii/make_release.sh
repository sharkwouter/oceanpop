#!/bin/bash

set -e

usage() {
    echo "Usage: ${0} builddir version"
    echo ""
    echo "builddir: path to the build directory"
    echo "version: package version"
    exit 1
}

if [ -z "${1}" ]||[ -z "${2}" ]; then
    echo "Please fill in all required fields"
    usage
fi

BUILDDIR="$1"
VERSION="$2"

PLATFORMDIR="$(dirname "$0")"
RELEASEDIR="$BUILDDIR/oceanpop"

rm -rf "$RELEASEDIR"
mkdir -p "$RELEASEDIR"

cp "$BUILDDIR/oceanpop.dol" "$RELEASEDIR/boot.dol"
cp "$PLATFORMDIR/icon.png" "$RELEASEDIR"
sed "s/VERSION/$VERSION/" "$PLATFORMDIR/meta.xml" > "$RELEASEDIR/meta.xml"
cp -a "$BUILDDIR/assets" "$RELEASEDIR"

cd "$BUILDDIR"
rm -f "oceanpop_$VERSION.zip"
zip -r "oceanpop_$VERSION.zip" "oceanpop"

echo "The release is ready at"
echo
echo "   $BUILDDIR/oceanpop_$VERSION.zip"
echo
echo "You can now upload this file to the releases page:"
echo " https://github.com/sharkwouter/oceanpop/releases"

