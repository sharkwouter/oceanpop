#!/bin/bash

cd "$(dirname "$0")"

if [ -z "${1}" ]; then
	echo "Please select a language like: ${0} en_US"
fi

POTFILE="oceanpop.pot"

xgettext --from-code=UTF-8 --keyword=_ --sort-output --language=C++ ../src/*.cpp ../src/*.hpp ../src/states/*.cpp ../src/states/*.hpp -o ${POTFILE}

msginit --locale="${1}" --input=${POTFILE} -o "../assets/languages/${1}.po"