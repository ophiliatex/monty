#!/bin/bash
# Define colors
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m' # No Color
# Define two variables task and count
task=3
count=-1

echo "Running tests for task $1"

# check if folder test/task_$task exists
if [ -d test/task_$task ]; then
		echo "Found test/task_$task"
else
		echo "test/task_$task not found"
		exit 1
fi

# first build the c files using gcc -Wall -Werror -Wextra -pedantic -std=c89 *.c -o monty
echo "Building monty"

# before continuing make sure the cmd is run
state=$(gcc -Wall -Wextra -pedantic -std=c89 *.c -o monty 2>&1 >/dev/null)
if [ $? -eq 0 ]; then
		echo "Build successful"
else
		echo "Build failed"
		echo "$state"
		exit 1
fi

# copy the monty file to the test folder
echo "Copying monty to test/task_$task"
state=$(cp monty test/task_$task 2>&1 >/dev/null)
if [ $? -eq 0 ]; then
		echo "Copy successful"
else
		echo "Copy failed"
		echo "$state"
		exit 1
fi

# for every file in the folder till count becones zero execute the file
echo "--------------------Running tests-----------------------"
for file in test/task_$task/*.sh; do
		if [ $count -eq 0 ]; then
				break
		fi
		./$file
		((count--))
done

# delete the monty file from the test folder
echo "Deleting monty from test/task_$task"
rm test/task_$task/monty


