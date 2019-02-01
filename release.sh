#!/bin/sh

[ -z "$CI_COMMIT_TAG" ] && exit 1
[ "$CI_COMMIT_REF_NAME" eq "release-dev" ] && /usr/bin/curl -H "JOB_TOKEN: $CI_JOB_TOKEN" --data '{ "name": "Development Release $CI_COMMIT_TAG", "tag_name": "$CI_COMMIT_TAG", "description": "Recurring TELEports developer release, only for testing!" }' --request POST http://gitlab.com/api/v4/projects/8146239/releases
[ "$CI_COMMIT_REF_NAME" eq "release" ] && /usr/bin/curl -H "JOB_TOKEN: $CI_JOB_TOKEN" --data \'{ "name": "Release $CI_COMMIT_TAG", "tag_name": "$CI_COMMIT_TAG", "description": "This is the current stable release of TELEports" }\' --request POST http://gitlab.com/api/v4/projects/8146239/releases    