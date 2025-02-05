#ifndef GUIDES_H
#define GUIDES_H

#include <iostream>
#include <string>

using namespace std;

const string COPY_GUIDE = R"(
Usage:
    copy <args> [options]

Description:
    Copyis objects. Default object is a file.

Arguments:
    <PATH1>                                      Path of the object to be copied.
    <PATH2>                                      Path of the object where it will be copied.

Options:
    --recreate, -rc                              Delete an object before copying if it already exists. If key is not 
                                                 specified, raises an error. 
    --directory, -dir                            Copy a directory instead of a file.
    --recursive. -r                              Copy all contents of the directory, including subdirectories 
                                                 and their files. Required keys: --directory.
            )";

const string CREATE_GUIDE = R"(
Usage:
    create <args> [options]

Description:
    Creaties objects. Defaul object is a file.

Arguments:
    <PATH>                                       Path of the object where it will be created.

Options:
    --recreate, -rc                              Delete an object before copying if it already exists. If key is not 
                                                 specified, raises an error. 
    --directory, -dir                            Create a directory instead of a file.
            )";

const string DELETE_GUIDE = R"(
Usage:
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
                                                 Excluded keys: --directory.
            )";

const string DIR_GUIDE = R"(
Usage:
    dir <args> [options]

Description:
    Displays directory contents.

Arguments:
    <PATH>                                       Path of the directory.

Options:
    [--recursive, -r]                              -
    [--depth, -d]                                  -
            )";

const string FIND_GUIDE = R"(
Usage:
    find <args> [options]

Description:
    Finds object. Default find files, directories and symlinks.

Arguments:
    <NAME>                                       Name of the object to search for.
    <PATH>                                       Path where to search.

Options:
    --directory                                  Search directories only. 
                                                 Excluded keys: --file, --symlink.
    --file                                       Search files only. 
                                                 Excluded keys: --directory, --symlink.
    --symlink                                    Search symlinks only. 
                                                 Excluded keys: --directory, --file.
    --limit <number>                             The number of matches does not exceed number.
    --recursive                                  Recursive directory traversal.
    --depth <number>                             Don't go down into nested directories below number. 
                                                 Required keys: --recursive.
    --log-visited                                Save the list of visited directories to "./logs/". 
                                                 Required keys: --recursive.
    --log-matches                                Save the list of matches to "./logs/".
            )";

const string INSERT_GUIDE = R"(
Usage:
    insert <args> [options]

Description:
    Inserts data to a file.

Arguments:
    <DATA>                                       Data to insert. 
    <PATH>                                       Path of the file.

Options:
    --line <number>                              Insert into line under number .
    --skip <number>                              Skip number of symbols before inserting. If the number exceeds the length of the string, 
                                                 the data will be inserted at the end. 
                                                 Required keys: --line.
    --output                                     Display file contents after insertion.
            )";

const string MODIFY_GUIDE = R"(
Usage:
    modify <args> [options]

Description:
    Renames or moves objects.

Arguments:
    <PATH1>                                      Сurrent path of the object.
    <PATH2>                                      New path of the object.

Options:
    --recreate                                   Delete an object before modifing if it already exists. If key is not 
                                                 specified, raises an error.
            )";

const string READ_GUIDE = R"(
Usage:
    read <args> [options]

Description:
    Displays file contents.

Arguments:
    <PATH>                                       Path of the file.

Options:
    --detail                                     Number before each line and length at the end.
            )";

#endif