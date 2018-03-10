#!/bin/bash

test_fun()
{
read -r -d '' VARIABLE << EOM
qwerty
EOM

echo "Running ${PROG}"
printf "\n"
echo -e "${VARIABLE}" | ./${PROG} foo bar
printf "\n"
echo "End of ${PROG}"
}
