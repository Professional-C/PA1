#include "RleData.h"
#include <iostream>
#include <vector>

void RleData::Compress(const char* input, size_t inSize)
{
    // Clean up any prior data & allocate mData
    delete [] mData;
    mSize = 0;
    mData = new char[inSize*2];
    
    // Single char edge case
    if(inSize == 1){
        mData[mSize++] = 1;
        mData[mSize++] = input[0];
        return;
    }
    
    // Temp storage vector
    std::vector<char> temp;
    int tempSize = 0;
    
    // First pass -- fill vector
    char prevChar = NULL;
    int counter = 0;
    for(int i = 0; i < inSize; i++){
        char currChar = input[i];
        if(currChar == prevChar){
            counter++;
            if((i == inSize-1) || (counter == 127)){
                temp.push_back(counter);
                tempSize++;
                temp.push_back(currChar);
                tempSize++;
                counter = 0;
            }
        }
        else if(prevChar){
            temp.push_back(counter);
            tempSize++;
            temp.push_back(prevChar);
            tempSize++;
            prevChar = currChar;
            counter = 1;
            if(i == inSize-1){ //last iteration
                temp.push_back(counter);
                temp.push_back(currChar);
            }
        }
        else{ // first iteration
            prevChar = currChar;
            counter++;
        }
    }
    
    int negStart = -1;
    int negEnd = 0;
    for(int i = 0; i < temp.size(); i+=2){
        
        if((temp[i] != 1) && (negStart == -1)){ // no neg run IP & positive run
            mData[mSize++] = temp[i];
            mData[mSize++] = temp[i+1];
            continue;
        }
        else if(negStart == -1){ // start of neg run
            negStart = i;
            continue;
        }
        else if(temp[i] == 1){ // continuing neg run
            negEnd = i;
        }
        if((i == temp.size()-2) || (temp[i] != 1) || (((negEnd-negStart)+2) /2 == 127)){ //end of neg run or last run
            if((negStart - negEnd) == negStart){ // run of 1
                mData[mSize++] = 1;
                mData[mSize++] = temp[i-1];
            }
            else if(negEnd != 0){
                int numNeg = ((negEnd-negStart) + 2) / 2;
                mData[mSize++] = numNeg*-1;
                for(int j = negStart+1; j < negStart+(negEnd-negStart)+2; j+=2){
                    mData[mSize++] = temp[j];
                }
            }
            if(temp[i] != 1){
                mData[mSize++] = temp[i];
                mData[mSize++] = temp[i+1];
            }
            negStart = -1;
            negEnd = 0;
        }
    }
    
}

void RleData::Decompress(const char* input, size_t inSize, size_t outSize)
{
    // Clean up any prior data & allocate mData
    delete [] mData;
    mSize = 0;
    mData = new char[outSize];

    

    for(int i = 0; i < inSize; i+=2){
        if(input[i] > 0){ //positive run
            for(int j = 0; j < input[i]; j++){
                mData[mSize++] = input[i+1];
            }
        }
       else if(input[i] < 0){ //negative run
            for(int j = i+1; j <= i+input[i]*-1; j++){
                mData[mSize++] = input[j];
            }
            i += (input[i]*-1) -1;

        }
    }
    
}

std::ostream& operator<< (std::ostream& stream, const RleData& rhs)
{
	for (int i = 0; i < rhs.mSize; i++)
	{
		stream << rhs.mData[i];
	}
	return stream;
}
