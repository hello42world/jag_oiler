#!/bin/bash

# Check if we're on a tag
TAG=$(git describe --exact-match --tags 2>/dev/null)

if [ -n "$TAG" ]; then
  # We're on a tag, print it
  echo -n "$TAG"
else
  # Not on a tag, get commit hash
  HASH=$(git rev-parse --short=7 HEAD)
  echo -n "${HASH}-dev"
fi

# Check for uncommitted changes
if ! git diff-index --quiet HEAD --; then
  echo -n "-d"
fi

echo ""
