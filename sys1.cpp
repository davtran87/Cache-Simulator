
/* 
 * File:   sys1.cpp
 * Author: David Tran
 * Purpose: The purpose of this program is to read a trace file of data accesses. The program will
 * simulate a direct mapped cache, with a choice to turn on verbose mode and select ranges for verbose mode.
 * The program will calculate miss rates and other necessary information.
 * Created on April 26, 2017, 10:38 PM
 * Version: 1.0
 * Compilation method: dtran7@unixlab ~]$ g++ sys1.cpp -o sys1
 * Execution method: 
./sys1 ~whsu/csc656/Traces/MiBench/patricia.xex 1
./sys1 ~whsu/csc656/Traces/MiBench/patricia.xex 1 -v 0 100000 // for printing verbose mode must have bounds
./sys1 ~whsu/csc656/Traces/MiBench/patricia.xex 2
./sys1 ~whsu/csc656/Traces/MiBench/patricia.xex 2 -v 0 100000 // for printing verbose mode must have bounds

 * The -v is verbose mode and it is optional, with -v used as an argument, it will print out
 * verbose information given ranges ic1 and ic2.




 */

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string.h>
#include <vector>
#include <bitset>
#include <math.h>
#include <string.h>
#include <iomanip>
#include <limits>


using namespace std;

/* Class for storing information from the trace file*/
class FileInfo {
public:
    unsigned int long instrAdd, memAddress, dataRW;
	int byteRWSize;
    char colon, accessType;
};
/* Class for storing cache block information*/
class CacheInfo {
public:
    unsigned int long tag, data;
    int validBit, dirtyBit;

};

/* Main Program*/
int main(int argc, char** argv) 
{
    
    string caseNum; // case number ex. 1, 2a, 2b
	/* Initialization of needed Ints*/
    int cacheSize = atoi(argv[2]) * 1024, numBlocks=0, numHits = 0, numMiss = 0, numReads = 0, numWrites = 0, numDataReadMiss = 0, numDataWriteMiss = 0, numDirtyReadMiss=0, numDirtyWriteMiss=0,
        numBytesRead=0, numBytesWrite=0, cyclesRead = 0, cyclesWrite = 0, indexSize = 0, status = 0, verboseMode = 0, missPen=80;
	unsigned int ic1=0, ic2=0; // variables for range of verbose mode.
    numBlocks = cacheSize / 16; // calculate number of blocks in cache.
	char colon;
	
	/* Declaration of Vectors for FileInfo and CacheInfo classes*/
    std::vector<FileInfo> fInfoVec; // vector to hold FileInfo object
    std::vector<CacheInfo> cInfoVec; // vector to hold cache info aka blocks object.
    FileInfo fInfo; // object creation of FileInfo
    CacheInfo cache; // object creation of CacheInfo
    indexSize = log2(numBlocks); // how many bits is in the index
	
	/* Open File and Read*/
	ifstream traceFile; 
	traceFile.open(argv[1]); // open trace file from argument
    if (traceFile.fail()) // check if open file fails
        cout << "error" << colon << " opening file" << endl;
	
    while (traceFile.good()) // while loop used to read file and store BranchInfo objects in a vector
    {
        traceFile >> std::hex >> fInfo.instrAdd >> colon >> fInfo.accessType >> std::hex >> fInfo.memAddress >> fInfo.byteRWSize >> std::hex >> fInfo.dataRW;
        fInfoVec.push_back(fInfo);
    }
	/* Verbose mode on or off Check */
	if(argc > 3) //verbose mode on
	{
		verboseMode = 1;
		ic1 = atoi(argv[4]);
		ic2 = atoi(argv[5]);
	}
	else //verbose mode off
	{
		ic1 = 0;
		ic2 = fInfoVec.size();
	}

	/* Initialize cache info and push to vector */
    for (int i = 0; i < numBlocks; i++)
    {
        cache.data = 0;
        cache.tag = 0;
        cache.validBit = 0; // set all bits valid to false
        cache.dirtyBit = 0;
        cInfoVec.push_back(cache);
    }

	/* Start of main loop for Program */
    for (unsigned int i = 0; i < fInfoVec.size(); i++) //loop until the end of vector
    {
		/*Initialization of Local Variables */
        unsigned int long temp = 0, prevTag, tag = 0; 
		int index = 0, currentValidBit, currentDirtyBit;
		
		/* Breaking up the memory address into tag and index */
        temp = fInfoVec[i].memAddress >> 4; // shifts by log16 for offset
        index = temp % numBlocks; // modulous by 64 to get last 6 bits
        tag = temp >> indexSize; // shift by index bit size to get tag
		
		/* Copy previous information in cache before data access */
        prevTag = cInfoVec[index].tag;
		currentValidBit = cInfoVec[index].validBit;
		currentDirtyBit = cInfoVec[index].dirtyBit;

		/* Case of hit, needed block is in cache */
        if (tag == cInfoVec[index].tag && cInfoVec[index].validBit == 1)// Checks if tag of address equals tag in cache and if valid bit is on (HIT)
        {
			caseNum = "1";
            status = 1; // used for verbose mode status=1=hit
            numHits++;
            if (fInfoVec[i].accessType == 'R') // if access is Reading.
            {
                numReads++;
                cyclesRead++;
				//numBytesWrite += fInfoVec[i].byteRWSize;				
            } 
            else //if access is Writing
            {
                cInfoVec[index].dirtyBit = 1;
                numWrites++;
                cyclesWrite++;
                //numBytesWrite += fInfoVec[i].byteRWSize;
				//numBytesRead += fInfoVec[i].byteRWSize;			
            }
        } 
		/* Case of miss with two types 2a(clean miss), 2b(dirty miss) */
        else 
        {
            status = 0; //status=0 is miss for verbose mode.
            numMiss++;
			
			/* In the case of 2a Clean Miss */
            if (cInfoVec[index].validBit == 0 || cInfoVec[index].dirtyBit == 0) // case 2a cache is clean or empty (CLEAN MISS)
            {
                caseNum = "2a";
                
                if (fInfoVec[i].accessType == 'R') 
                {
                    cInfoVec[index].data = fInfoVec[i].dataRW; // move data, unnecessary but i wanted to.
                    cInfoVec[index].tag = tag; //move new tag to cache.
                    cInfoVec[index].dirtyBit = 0; //set dirtybit to 0
                    numReads++;
					cInfoVec[index].validBit = 1; //set valid bit to 1
					numDataReadMiss++;
					//numBytesWrite += fInfoVec[i].byteRWSize;
					numBytesRead += 16;
					cyclesRead += (missPen + 1); //sum of cycles for read.
					
                } 
				/* In the case of 2b dirty miss */
                else 
                {
                    cInfoVec[index].data = fInfoVec[i].dataRW;
                    cInfoVec[index].tag = tag;
                    cInfoVec[index].dirtyBit = 1;
					cInfoVec[index].validBit = 1;
                    numWrites++; // number of writes
					numDataWriteMiss++; //number of write miss
					//numBytesWrite += 16;
					numBytesRead += 16;
					cyclesWrite += (missPen + 1); //sum of cycles for writes
                }

            } 
            else // case 2b 
            {
                caseNum = "2b";
                if (fInfoVec[i].accessType == 'R') {
                    
                    cInfoVec[index].data = fInfoVec[i].dataRW;
                    cInfoVec[index].tag = tag;
                    cInfoVec[index].dirtyBit = 0;
                    numReads++;
					numDataReadMiss++;
					numDirtyReadMiss++;
					numBytesRead += 16;
					numBytesWrite += 16;
					cyclesRead += (2*missPen + 1); //sum of cycles for reads in case of 2b
                } 
                else 
                {
                    cInfoVec[index].data = fInfoVec[i].dataRW;
                    cInfoVec[index].tag = tag;
                    cInfoVec[index].dirtyBit = 1;
                    numWrites++;
					numDirtyWriteMiss++;
					numDataWriteMiss++;
					numBytesWrite += 16;
					numBytesRead += 16;
					cyclesWrite += (1 + (2*missPen)); //sum of cycles for writes in case of 2b
                }
            }
        }
		/* Start of Verbose mode code */
        if (verboseMode == 1) 
        {
			if(ic1 == i && ic1 <= ic2) // print from given ranges.
			{
			     cout << std::setw(3) << std::dec << i << std::setw(3) << std::hex << index << std::setw(7) << std::hex
                 << tag << std::setw(2) <<  currentValidBit << std::setw(2) 
                 << std::setw(7) << std::hex << prevTag << std::setw(2) << currentDirtyBit
                 << std::setw(2) << status << std::setw(3) << caseNum << endl;
				 ic1++;
			}
        }
    }
	/* Output of simulation information */
	cout << endl << "direct-mapped, writeback, size = " << atoi(argv[2]) << "KB" << endl;
	cout << std::dec << "Loads: " << numReads  << "  Stores: " << numWrites  << "  Total access: " << numReads+numWrites << endl 
		 << "Rmiss: " << numDataReadMiss  << "  Wmiss: " << numDataWriteMiss  << "  Total miss: " << numMiss << endl 
		 << "Dirty rmiss: " << numDirtyReadMiss  << "  Dirty wmiss: " << numDirtyWriteMiss << endl 
		 << "Bytes read: " << numBytesRead <<  "  Bytes written: " << numBytesWrite << endl 
	     << "Read time: " << cyclesRead <<  "  Write time: " << cyclesWrite << endl << "Total time: " << cyclesRead+cyclesWrite << endl << "Miss rate: " << ((double)numMiss/fInfoVec.size());
     
    return 0;
}

