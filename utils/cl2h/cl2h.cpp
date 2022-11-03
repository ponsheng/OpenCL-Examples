#include <iostream>
#include <cstdlib>
#include <string>
#include <fstream>
#include <filesystem>

/*
 * Output "kernel.h" containing:
 *   const char *KernelSource = "...";
 */

using namespace std;

void usage() {
  cout << "Usage: cl2h <cl file>\n";
}

string getHeaderBase(const string &cl_file) {
  string header_name = cl_file;
  size_t pos;

  // Get basename , TODO find \ for Windows
  pos = cl_file.rfind("/");
  if (pos != string::npos) {
    header_name = cl_file.substr(pos + 1);
  }

  // Replace all '.' to '_' in basename
  do {
    pos = header_name.find(".");
    if (pos == string::npos) {
      break;
    }
    header_name.replace(pos, 1, "_");
  } while (pos != string::npos);

  header_name += ".h";

  cout << "Header name: " << header_name << endl;

  return header_name;
}

int check_input(const string &cl_file) {
  size_t found = cl_file.rfind(".cl");
  if (found == string::npos || cl_file.size() != (found + 3)) {
    cerr << "Input file " << cl_file << " is not .cl" << endl;
    return EXIT_FAILURE;
  }

  if (!filesystem::exists(cl_file)) {
    cerr << "Input file " << cl_file << " does not exist" << endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int main(const int argc, const char **argv) {
  if (argc < 2) {
    usage();
    return EXIT_FAILURE;
  }

  // TODO accept output dir

  string cl_file(argv[1]);
  if (int ret = check_input(cl_file)) {
    return ret;
  }
  
  // Generate output file name
  string header_name = getHeaderBase(cl_file);

  string str = "static const char *KernelSource = \n";

  string line;
  ifstream cl_fd(cl_file);
  while (getline(cl_fd, line)) {
    str += "\" " + line + "\\n\"\n";
  }
  cl_fd.close();

  str += "\"\";";

  // Content
  //cout << str << endl;
   
  ofstream header_fd(header_name);
  header_fd << str;
  header_fd.close();

  return 0;
}

