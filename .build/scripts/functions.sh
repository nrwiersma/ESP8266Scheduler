#!/bin/bash

COLORIZE=${COLORIZE-1}

function info() {
  if test "${COLORIZE}" -eq 1; then
    tput bold
    tput setaf 4
  fi

  echo "$@"

  if test "${COLORIZE}" -eq 1; then
    tput sgr0
  fi
}

function success() {
  if test "${COLORIZE}" -eq 1; then
    tput bold
    tput setaf 2
  fi

  echo "$@"

  if test "${COLORIZE}" -eq 1; then
    tput sgr0
  fi
}

function err() {
  if test "${COLORIZE}" -eq 1; then
    tput bold
    tput setaf 1
  fi

  echo "$@"

  if test "${COLORIZE}" -eq 1; then
    tput sgr0
  fi
}