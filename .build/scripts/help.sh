#!/bin/bash
set -e -o pipefail
pushd "$(dirname "${BASH_SOURCE[0]}")" >/dev/null || return 1
SCRIPT_DIR=$(pwd)
popd >/dev/null || return 1

source "${SCRIPT_DIR}/functions.sh"

function usage() {
  cat <<-EOF
Usage: ${SCRIPT_NAME} <makefile path>
Description:
   This script prints a help file for a Makefile.

EOF
}

function err_usage() {
  err "$1"
  err ""
  err "$(usage)"
}

function main() {
  local makefile="$1"

  if test -z "${makefile}"; then
    err "ERROR: No Makefile specified"
    return 1
  fi

  info "Usage"
  echo ""
  awk -f "${SCRIPT_DIR}/help.awk" "${makefile}"

  return 0
}

main "$@"
exit $?