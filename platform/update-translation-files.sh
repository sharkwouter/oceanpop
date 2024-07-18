#!/bin/bash

cd "$(dirname "$0")"

POTFILE="oceanpop.pot"

xgettext --from-code=UTF-8 --keyword=_ --sort-output --language=C++ ../src/*.cpp ../src/*.hpp ../src/states/*.cpp ../src/states/*.hpp -o ${POTFILE}

# Update each po file
for langfile in ../assets/languages/*.po; do
	msgmerge -U "${langfile}" "${POTFILE}"
done
