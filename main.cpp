#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"
#include "rapidjson/error/en.h"
#include "src/parser.hpp"
#include <iostream>
#include <cstdio>

using namespace rapidjson;

Document* parseFile(std::string filename) {
  FILE* fp = fopen(filename, "r");
  if (!fp) {
    std::cerr << "config.json not found, exiting" << std::endl;
    return 1;
  }
  //Create a stream buffer, process file as character stream
  char readBuffer[65536];
  FileReadStream is(fp, readBuffer, sizeof(readBuffer));
  Document* d = new Document();
  d->ParseStream(is);
  fclose(fp);
  //Return errors if present with character offset into file
  if (d->HasParseError()) {
    std::cerr << "Error (offset " << d->GetErrorOffset() << ") : "
      << GetParseError_En(d->GetParseError()) << std::endl;
    free(d);
    return NULL;
  }
  return d;
}

int main(int argc, char const* argv[]) {
  Document* config =  parseFile("config.json");
  Game::Parser parser;
  parser.LoadData(config);
  return 0;
}
