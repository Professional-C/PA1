#include "SrcMain.h"
#include <string>
#include "RleFile.h"

void ProcessCommandArgs(int argc, const char* argv[])
{
	// TODO: Process argc and argv, and use to compress/decompress files
    if(argc == 2){
        std::string file = argv[1];
        RleFile rFile;
        if(file.find(".rl1") != std::string::npos){
            rFile.ExtractArchive(file);
        }
        else{
            rFile.CreateArchive(file);
        }
    }
}
