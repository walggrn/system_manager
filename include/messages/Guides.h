#ifndef GUIDES_H
#define GUIDES_H

#include <iostream>
#include <string>

using namespace std;

const string INITIAL_MESSAGE = R"(Welcome to the Console Manager!
This tool helps you manage files and directories directly from the terminal.
Type "help" to see detailed usage instructions, or "list" to view all available commands.
If you're unsure about how to use a specific command, try "command_name --help" for guidance.)";

const string GUIDE = R"(System Manager - Guide
This tool supports the following commands:
    copy                                         Copies objects. Default copies files.
    create                                       Creates objects. Default creates files.
    delete                                       
    dir                                          Displays directory contents.
    find                                         Finds object. Default finds files, directories and symlinks.
    insert                                       Inserts data to files.
    modify                                       Renames or moves objects.
    read                                         Displays file contents.
    list                                         Shows a list of all available commands.
    help                                         Shows a guide.
    exit                                         Ends the program.
      
For more details on any command, type "command_name --help".)";

const string LIST_COMMANDS = R"(Commands:
    copy
    create
    delete
    dir
    find
    insert
    modify
    read
    list
    help
    exit)";

const string COPY_GUIDE = R"(Usage:
    copy <args> [options]

Description:
    Copies objects. Default copies files.

Arguments:
    <PATH1>                                      Path of the object to be copied.
    <PATH2>                                      Path of the object where it will be copied.

Options:
    --recreate, -rc                              Delete an object before copying if it already exists. If key is not 
                                                 specified, raises an error. 
    --directory, -dir                            Copy a directory instead of a file.
    --recursive. -r                              Copy all contents of the directory, including subdirectories 
                                                 and their files. Required keys: --directory.)";

const string CREATE_GUIDE = R"(Usage:
    create <args> [options]

Description:
    Creates objects. Default creates files.

Arguments:
    <PATH>                                       Path of the object where it will be created.

Options:
    [--recreate, -rc]                            Delete an object before copying if it already exists. If key is not 
                                                 specified, raises an error. 
    [--directory, -dir]                          Create a directory instead of a file.)";

const string DELETE_GUIDE = R"(Usage:
    delete <args> [options]

Description:
    Deletes objects. Default deletes files.

Arguments:
    <PATH>                                       Path of the object where it will be deleted.

Options:
    [--directory, -dir]                          Delete a directory instead of a file.
                                                 Excluded keys: --line, --skip, --length, --output.
    [--line, -l] <number>                        Delete the line with the specified <number> from the file. 
                                                 Excluded keys: --directory.
    [--skip, -s] <number>                              Skip symbols before deleting. 
                                                 Required keys: --line. 
                                                 Excluded keys: --directory.
    [--length, -len] <number>                    Delete <number> symbols. If the number 
                                                 exceeds the length of the string, the string will be removed entirely. 
                                                 Required keys: --line. 
                                                 Excluded keys: --directory.
    [--output, -o]                               Display file contents after deletion. 
                                                 Required keys: --line. 
                                                 Excluded keys: --directory.)";

const string DIR_GUIDE = R"(Usage:
    dir <args> [options]

Description:
    Display directory contents.

Arguments:
    <PATH>                                       Path of the directory. if not specified, the current directory will be used.

Options:
    [--recursive, -r]                            Display also children directories contents.
    [--depth, -d]                                Don't go down into children directories below <number>. 
                                                 Required keys: --recursive.)";

const string FIND_GUIDE = R"(Usage:
    find <args> [options]

Description:
    Finds object. Default finds files, directories and symlinks.

Arguments:
    <NAME>                                       Search for objects named <NANE>.
    <PATH>                                       Directory at the specified <PATH> where to search. 
                                                 if not specified, the current directory will be used.

Options:
    [--contains, -c]                             
    [--directory, -dir]                          Search directories only. 
                                                 Excluded keys: --file, --symlink.
    [--file, -f]                                 Search files only. 
                                                 Excluded keys: --directory, --symlink.
    [--symlink, -sm]                             Search symlinks only. 
                                                 Excluded keys: --directory, --file.
    [--limit, -lim] <number>                     The number of matches does not exceed <number>.
    [--recursive, -r]                            Recursive directory traversal.
    [--depth, -d] <number>                       Don't go down into nested directories below <number>. 
                                                 Required keys: --recursive.
    [--log-visited, -lv]                         Save the list of visited directories to "./logs/". 
                                                 Required keys: --recursive.
    [--log-matches, -lm]                         Save the list of matches to "./logs/".)";

const string INSERT_GUIDE = R"(Usage:
    insert <args> [options]

Description:
    Insert data to files.

Arguments:
    <DATA>                                       Data to insert. 
    <PATH>                                       File at the specified <PATH>.

Options:
    [--line, -l] <number>                        Insert into line under <number>.
    [--skip, -s] <number>                        Skip <number> of symbols before inserting. If the number exceeds the length of the string, 
                                                 the data will be inserted at the end. 
                                                 Required keys: --line.
    [--output, -o]                               Display file contents after insertion.)";

const string MODIFY_GUIDE = R"(Usage:
    modify <args> [options]

Description:
    Rename or move objects.

Arguments:
    <PATH1>                                      Object at the current specified <PATH1> Сurrent path of the object.
    <PATH2>                                      .

Options:
    [--recreate, -rc]                            Delete an object before modifing if it already exists. If key is not 
                                                 specified, raises an error.)";

const string READ_GUIDE = R"(Usage:
    read <args> [options]

Description:
    Display file contents.

Arguments:
    <PATH>                                       File at the specified <PATH>.

Options:
    [--detail, -dt]                              Number before each line and length at the end.)";

#endif