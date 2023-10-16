#include "../include/shell.h"
#include "../include/utils.h"

#include <iostream>

using namespace std;

Shell::Shell(char *path_to_device) {
  sys = new FileSystem();
  sys->mount(path_to_device);
}

void Shell::run() {
  cout << "\n\nWelcome to the FAT32 shell!" << endl;
  cout << "Type 'help' for a list of commands." << endl;
  path = vector<string>();
  path.push_back("~");
  while (true) {
    display_path();
    cout << " > ";
    if (handle_stdin() != 0)
      break;
  }
  sys->unmount();
}

void Shell::display_path() {
  for (int i = 0; i < (int)path.size(); i++) {
    cout << path[i] << "/";
  }
}

int Shell::handle_stdin() {
  string s_cmd;
  getline(cin, s_cmd);
  vector<string> tokens = split(s_cmd, ' ');
  Input input = parse_input(tokens[0]);

  switch (input) {
  case CD: {
    if (tokens.size() < 2) {
      cout << "cd: missing operand" << endl;
      return 0;
    }
    string name = tokens[1];
    int index = sys->cd(name);
    if (index == -1) {
      cout << "cd: " << name << ": No such file or directory" << endl;
    } else {
      if (name == "..") {
        path.pop_back();
      } else if (name != ".") {
        path.push_back(name);
      }
    }
    break;
  }
  case LS: {
    string result = sys->ls();
    cout << result << endl;
    break;
  }
  case CAT: {
    if (tokens.size() < 2) {
      cout << "cat: missing operand" << endl;
      return 0;
    }
    string name = tokens[1];
    string result = sys->cat(name);
    cout << result << endl;
    break;
  }
  case ECHO: {
    if (tokens.size() < 3) {
    }
    string name = tokens[1];
    int data_index = -1;
    int i = 0;
    for (; i < (int)s_cmd.size(); i++) {
      if (s_cmd[i] == '\'') {
        data_index = i + 1;
        break;
      }
    }
    if (data_index == -1) {
      cout << "echo: expected \' before data to insert" << endl;
      return 0;
    }
    if (s_cmd[s_cmd.size() - 1] != '\'') {
      cout << "echo: expected \' after data to insert" << endl;
      return 0;
    }

    sys->echo(name, s_cmd.substr(data_index, s_cmd.size() - data_index - 1));
    break;
  }
  case MKDIR: {
    if (tokens.size() < 2) {
      cout << "mkdir: missing operand" << endl;
      return 0;
    }
    string name = tokens[1];
    sys->mkdir(name);
    break;
  }
  case TOUCH: {
    if (tokens.size() < 2) {
      cout << "touch: missing operand" << endl;
      return 0;
    }
    string name = tokens[1];
    sys->touch(name);
    break;
  }
  case EXIT: {
    return -1;
  }
  case FORMAT: {
    sys->format(512, 2, 32, 4);
    break;
  }
  case HELP: {
    cout << "cd <directory> - change directory" << endl;
    cout << "ls - list directory contents" << endl;
    cout << "cat <file> - print file contents" << endl;
    cout << "echo <file> '<data>' - write data to file" << endl;
    cout << "mkdir <directory> - create directory" << endl;
    cout << "touch <file> - create file" << endl;
    cout << "format - format device" << endl;
    cout << "exit - exit shell" << endl;
    break;
  }
  }
  return 0;
}

Input parse_input(string s) {
  if (s == "cd") {
    return CD;
  } else if (s == "ls") {
    return LS;
  } else if (s == "cat") {
    return CAT;
  } else if (s == "echo") {
    return ECHO;
  } else if (s == "mkdir") {
    return MKDIR;
  } else if (s == "touch") {
    return TOUCH;
  } else if (s == "exit") {
    return EXIT;
  } else if (s == "help") {
    return HELP;
  } else if (s == "format") {
    return FORMAT;
  } else {
    return HELP;
  }
}
