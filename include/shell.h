#ifndef SHELL_H
#define SHELL_H

#include "file_system.h"
#include <string>
#include <vector>

using namespace std;

enum Input : char {
  CD,
  LS,
  CAT,
  ECHO,
  MKDIR,
  TOUCH,
  EXIT,
  HELP,
  FORMAT,
};

Input parse_input(string s);

class Shell {
public:
  Shell(char *path_to_device);
  void run();
  void exit();

private:
  vector<string> path;

  int handle_stdin();

  void display_path();

  FileSystem *sys;
};

#endif