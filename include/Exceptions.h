#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <iostream>
#include <string>
#include <format>
#include <exception>

using namespace std;

const string INPUT_EXCEPTION = "Error: invalid input provided.";
const string UNKNOWN_TOKEN_EXCEPTION = "Error: unknown input detected.";

const string HELP_KEY_IGNORED_WARNING = "Warning: The \"--help\" key is ignored when not placed after the command declaration.";
const string HELP_KEY_TYPED_WARNING = "Warning: The \"--help\" key was typed. Other data of input was ignored.";

const string LIMIT_ARGUMENT_EXCEPTION = "Error: number of command's argument exceeded.";
const string UNKNOWN_KEY_EXCEPTION = "Error: unknown key.";

const string KEY_USAGE_EXCEPTION = "Error: incorrect use of key.";

const string ARGUMENT_;
const string ARGUMENT_MISSING_VALUE_EXCEPTION = "Error: missing value for argument.";
const string ARGUMENT_DOESNT_EXISTS_EXCEPTION = "Error: command's argument doesn't found.";
const string ARGUMENT_NOT_FOUND_EXCEPTION = "Error: command's argument doesn't found.";
const string KEY_NOT_FOUND_EXCEPTION = "Error: key doesn't found.";
const string INVALID_KEY_VALUE_EXCEPTION = "Error: invalid key value.";
const string MISSING_KEY_VALUE_EXCEPTION = "Error: missing value for key.";

const string FILESYSTEM_EXCEPTION = "Error: filesystem exception.";

const string OBJECT_ALREADY_EXISTS_EXCEPTION = "Error: object already exists.";
const string DIRECTORY_ALREADY_EXISTS_EXCEPTION = "Error: directory already exists.";
const string FILE_ALREADY_EXISTS_EXCEPTION = "Error: the file already exists.";

const string OBJECT_NOT_EXIST_EXCEPTION = "Error: object doesn't exists.";
const string DIRECTORY_NOT_EXIST_EXCEPTION = "Error: directory doesn't exists.";
const string FILE_NOT_EXIST_EXCEPTION = "Error: file doesn't exists.";

const string DIRECTORY_OPEN_EXCEPTION = "Error: failed to open directory.";
const string FILE_OPEN_EXCEPTION = "Error: failed to open file.";

const string FILE_WRITE_EXCEPTION = "Error: failed to insert content to file.";

const string COMMAND_EXECUTION_EXCEPTION = "Command execution ended with an error.";

const string INVALID_TYPE_OBJECT_EXCEPTION = "Error: type object is invalid.";

void write_error(const string& error, const string& reason){
    cerr << format("{} => {}", error, reason) << endl;
}

void throw_error(const string& error, const string& reason){
    throw runtime_error(format("{} => {}", error, reason)); 
}

#endif