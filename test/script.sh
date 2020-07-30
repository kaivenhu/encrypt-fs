#!/bin/bash
RED='\033[0;31m'
GREEN='\033[1;32m'
NC='\033[0m' # No Color

PWD=`pwd`

TEST_ROOT="${PWD}/example/root"
TEST_MOUNT="${PWD}/example/mount"

checker() {
    local ret=${1}
    local key=${2}
    if [[ $ret == 0 ]]; then
        echo -e "${GREEN} \"${key}\" PASS~${NC}"
    else
        echo -e "${RED} \"${key}\" FAILED!!!${NC}"
    fi
}


if [ ! -d "${TEST_ROOT}" ]; then
    mkdir ${TEST_ROOT};
fi

if [ ! -d "${TEST_MOUNT}" ]; then
    mkdir ${TEST_MOUNT};
fi

./encfs "${TEST_ROOT}" "${TEST_MOUNT}"
checker $? "run encfs"
sleep 1s

./test/basic-op-test.exe "${TEST_MOUNT}"
checker $? "check basic operation"

fusermount -u "${TEST_MOUNT}"

