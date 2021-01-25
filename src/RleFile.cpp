#include "RleFile.h"
#include "RleData.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>

void RleFile::CreateArchive(const std::string& source)
{
    // Requires <fstream>
    std::ifstream::pos_type size;
    char* memblock;
    // Open the file for input, in binary mode, and start ATE (at the end)
    std::ifstream file (source, std::ios::in|std::ios::binary|std::ios::ate);
    if (file.is_open()){
        
        size = file.tellg(); // Save the size of the file
        memblock = new char[static_cast<unsigned int>(size)];
        file.seekg(0, std::ios::beg); // Seek back to start of file
        file.read(memblock, size);
        file.close();
        // File data is now in memblock array
        // (Do something with it here...)
        mRleData.Compress(memblock, size);
            
        // Write to file
        std::string outFile = source + ".rl1";
        mHeader.mFileNameLength = outFile.size();
        mHeader.mFileName = outFile;
        mHeader.mFileSize = size;
        
        std::ofstream arc(outFile, std::ios::out|std::ios::binary|std::ios::trunc);
        if (arc.is_open())
        {
            arc.write(mHeader.mSig, 4);
            arc.write(reinterpret_cast<char*>(&(mHeader.mFileSize)),4);
            arc.write(reinterpret_cast<char*>(&(mHeader.mFileNameLength)),4);
            arc.write(mHeader.mFileName.c_str(),outFile.size());
            arc.write(mRleData.mData,mRleData.mSize);
            
            // Write to file
            std::string outFile = source + ".rl1";
            std::cout << "output name: " << outFile << std::endl;
            mHeader.mFileNameLength = outFile.size();
            mHeader.mFileName = outFile;
            mHeader.mFileSize = size;
            
         // Use arc.write function to write data here
        }
        

        // Make sure to clean up!
        delete[] memblock;
    }

}

void RleFile::ExtractArchive(const std::string& source)
{
    // Requires <fstream>
    std::ifstream::pos_type size;
    char* memblock;
    // Open the file for input, in binary mode, and start ATE (at the end)
    std::ifstream file (source, std::ios::in|std::ios::binary|std::ios::ate);
    if (file.is_open()){
        
        size = file.tellg(); // Save the size of the file
        memblock = new char[static_cast<unsigned int>(size)];
        file.seekg(0, std::ios::beg); // Seek back to start of file
        file.read(memblock, size);
        file.close();
        
    }
}
