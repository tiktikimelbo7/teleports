#!/bin/bash
echo $CI_COMMIT_TAG
echo $CI_JOB_TOKEN
echo $CI_COMMIT_REF_NAME
[ -z "$CI_COMMIT_TAG" ] && exit 0
[[ $CI_COMMIT_REF_NAME == devrel* ]] && /usr/bin/curl -H "JOB_TOKEN: $CI_JOB_TOKEN" --data '{ "name": "Development Release $CI_COMMIT_TAG", "tag_name": "$CI_COMMIT_TAG", "description": "Recurring TELEports developer release, only for testing!" }' --request POST http://gitlab.com/api/v4/projects/8146239/releases
[[ $CI_COMMIT_REF_NAME == release* ]] && /usr/bin/curl -H "JOB_TOKEN: $CI_JOB_TOKEN" --data \'{ "name": "Release $CI_COMMIT_TAG", "tag_name": "$CI_COMMIT_TAG", "description": "This is the current stable release of TELEports" }\' --request POST http://gitlab.com/api/v4/projects/8146239/releases    
