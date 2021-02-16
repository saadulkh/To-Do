# To Do Program

A to do program in C.

## Basic Functionality:

 - Add new tasks
 - See existing tasks
 - Edit existing tasks
 - Mark tasks as done or todo
 - Delete existing tasks

## Usage:

Use `todo help` to see usage.

### Commands:

| Command             | Detail |
|:--------------------|:-------|
| with no command     | Show todo tasks list |
| add                 | Add new task to the list |
| edit                | Edit existing task |
| remove              | Remove task from the list |
| list                | Show tasks list |
| mark                | Mark a task as done or todo |
| version             | See version of todo |
| help                | See this help |

### Options:

| Option              | Detail |
|:--------------------|:-------|
| {NUMBER}            | Task no/position of the task in the list |
| {TITLE}             | Title of the task. Number/digit at the start is not allowed |
| -d --done           | Mark tasks as done or list done tasks |
| -t --todo           | Mark tasks as todo or list todo tasks |
| -a --all            | Perform operation on all tasks |
| -v --version        | See version of todo |
| -h --help           | See this help |

Use `todo help <command>` to see help for any command.
