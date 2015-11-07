#include "jni.h"
#include <stdio.h>
#include <vector>
#include <stdlib.h>
#include <string.h>
#include <cstring>
#include <iterator>
#include <vector>
#include <iostream>
#include <omp.h>
#include <time.h>
#include <chrono>
#define NUM_THREADS 2
#define total 13
#define NUM_SECTOR 16  
#define NUM_SPEED 6
using namespace std;


int capacity[total];
vector<float> windSpeeds;
vector<float> windDirs;
vector<string> stationIds;
int result[NUM_SECTOR][NUM_SPEED]={0};
char dataDir[100]="/home/viraj/Downloads/mesonet";
class WeatherData{

	public:
		static void getFloatData(JavaVM *vm,JNIEnv *env,char attr[100],string &fileN,char dir[100],int size)
		 { 

			int getEnvStat = vm->GetEnv((void **)&env, JNI_VERSION_1_6);
			 if (vm->AttachCurrentThread((void **) &env, NULL) != 0) {
            		std::cout << "Failed to attach" << std::endl;
        	}	

			//Input A String    
			jstring directory = env->NewStringUTF(dir);
			char *fileNam = new char[fileN.length() + 1];
			strcpy(fileNam,fileN.c_str());
			jstring fileName = env->NewStringUTF(fileNam);	
			jstring attribute = env->NewStringUTF(attr);

			//Search For Class
			jclass parseData = env->FindClass("ParseData");  
			if(parseData == nullptr) 
			{
				printf("\nERROR: class not found !");
				vm->DetachCurrentThread();
				return;
			}

			// Find Method	Get Float Attribute 
			jmethodID getFloatAttribute = env->GetStaticMethodID(parseData, "getFloatAttribute", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)[F");  // find method
			if(getFloatAttribute == nullptr)
			{  
			 	printf("\nERROR: Method getStringAttribute() not found!");
				vm->DetachCurrentThread();
				return;   	   
			}				

			jfloatArray floatArray=(jfloatArray)env->CallStaticObjectMethod(parseData, getFloatAttribute,fileName,directory,attribute);                      // call Method
			if(floatArray==nullptr)	     
			{	
				printf("\nMethod GetAttribute Failed!\n");
				vm->DetachCurrentThread();
				return;		
			}	

			jfloat *narr = env->GetFloatArrayElements(floatArray, NULL);
			char* ws = "windSpeed";
			char* wd = "windDir";

			if(strcmp(attr, ws) == 0)
				windSpeeds.insert(windSpeeds.begin() , narr,  narr+size);
			else if(strcmp(attr, wd)==0)
				windDirs.insert(windDirs.begin() , narr,  narr+size);
			vm->DetachCurrentThread();
		
 		}

 		
 		static void deleteFile(JavaVM *vm,JNIEnv *env,string &fileN,char dir[100])
 		{

 			int getEnvStat = vm->GetEnv((void **)&env, JNI_VERSION_1_6);
			if (vm->AttachCurrentThread((void **) &env, NULL) != 0)
			{
            		std::cout << "Failed to attach" << std::endl;
        			return;
        	}	
 			jstring directory = env->NewStringUTF(dir);
			char *fileNam = new char[fileN.length() + 1];
			strcpy(fileNam,fileN.c_str());
			jstring fileName = env->NewStringUTF(fileNam);

			//Search For Class
			jclass parseData = env->FindClass("ParseData");  
			if(parseData == nullptr) 
			{
				printf("\nERROR: class not found !");
				vm->DetachCurrentThread();
				return;
			}
									  
			// Find Method	Get Float Attribute 
			jmethodID deleteFile = env->GetStaticMethodID(parseData, "deleteFile", "(Ljava/lang/String;Ljava/lang/String;)V");  // find method
			if(deleteFile == nullptr)
			{  
			 	printf("\nERROR: Method getStringAttribute() not found!");
				vm->DetachCurrentThread();
				return;   	   
			}			

			env->CallStaticVoidMethod(parseData, deleteFile,fileName,directory);                      // call Method

			vm->DetachCurrentThread();

 		}


 		static void getStringData(JavaVM *vm,JNIEnv *env,char attr[100],string &fileN,char dir[100],int size)
		 { 

			
			int getEnvStat = vm->GetEnv((void **)&env, JNI_VERSION_1_6);
			if (vm->AttachCurrentThread((void **) &env, NULL) != 0) 
			{
            		std::cout << "Failed to attach" << std::endl;
        	}	
			//Input A String    
			jstring directory = env->NewStringUTF(dir);
			char *fileNam = new char[fileN.length() + 1];
			strcpy(fileNam,fileN.c_str());
			jstring fileName = env->NewStringUTF(fileNam);	
			jstring attribute = env->NewStringUTF(attr);

			//Search For Class
			jclass parseData = env->FindClass("ParseData");  
			if(parseData == nullptr) 
			{
				printf("\nERROR: class not found !");
				vm->DetachCurrentThread();
				return;
			}
						  
			// Find Method	Get Float Attribute 
			jmethodID getStringAttribute = env->GetStaticMethodID(parseData, "getStringAttribute", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)[Ljava/lang/String;");  // find method
			if(getStringAttribute == nullptr)
			{  
			 	printf("\nERROR: Method getFloatAttribute() not found!");
				vm->DetachCurrentThread();
				return;   	   
			}				

			jobjectArray objectArray=(jobjectArray)env->CallStaticObjectMethod(parseData, getStringAttribute,fileName,directory,attribute);                      // call Method


			if(objectArray==nullptr)	     
			{	
				printf("\nMethod GetAttribute Failed!\n");
				vm->DetachCurrentThread();
				return;		
			}	
			int stringCount = env->GetArrayLength(objectArray);
    		for (int i=0; i<stringCount; i++) 
    		{
        		jstring string = (jstring) env->GetObjectArrayElement(objectArray, i);
        		const char *rawString = env->GetStringUTFChars(string, 0);
        		stationIds.push_back(rawString);	        		
        		env->ReleaseStringUTFChars(string,rawString);
    		}
       		env->DeleteLocalRef(objectArray);
			vm->DetachCurrentThread();		
 		}

		static void getSize(JavaVM *vm,JNIEnv *env,char attr[100],string &fileN,char dir[100],int offset)
		{
			int getEnvStat = vm->GetEnv((void **)&env, JNI_VERSION_1_6);
			if(vm->AttachCurrentThread((void **) &env, NULL) != 0) 
			{
            		cout << "Failed to attach" << std::endl;
            		return;
            }
			//Input A String    
			jstring directory = env->NewStringUTF(dir);
			char *fileNam = new char[fileN.length() + 1];
			strcpy(fileNam,fileN.c_str());
			jstring fileName = env->NewStringUTF(fileNam);	
			jstring attribute = env->NewStringUTF(attr);

			//Search For Class
			jclass parseData = env->FindClass("ParseData");  
			if(parseData == nullptr) 
			{
				printf("\nERROR: class not found !");
				vm->DetachCurrentThread();
				return;   	   
			}

			//Search For Method
			jmethodID getInt = env->GetStaticMethodID(parseData, "getSize", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)I");
			if(getInt == nullptr)
			{
				printf("\nERROR: Method getSize() not found!");
				vm->DetachCurrentThread();
				return;   	   
			}

			jint size=env->CallStaticIntMethod(parseData, getInt,fileName,directory,attribute);
			capacity[offset]=size;
			vm->DetachCurrentThread();
		}
		


		
		static int calcSpeedBin(float windSpd) {
		    if(windSpd == 0) return 0;
		    else if(windSpd > 0 && windSpd <=5) return 1;
		    else if(windSpd > 5 && windSpd <=10) return 2;
		    else if(windSpd > 10 && windSpd <=15) return 3;
		    else if(windSpd > 15 && windSpd <=25) return 4;
		    else return 5;
		}

		static int calcDirectBin(float windDir, int numSectors) {
		    return int(windDir)%numSectors;
		}


		
		static int** calcBins(vector<float> windSpeeds, vector<float> windDirs)
		{
		    int numSector = 16;
		    int numSpds = 6;
		    int** wr = new int*[16];
		    // #pragma omp parallel for
		    for (int i = 0; i < 16; ++i)
		    {
				wr[i] = new int[6];
		    }

		    for(int i = 0; i < numSector; i++)
		     {
				for(int j = 0; j < numSpds; j++) 
				{
			    	wr[i][j] = 0;
				}
		    }
		    int s,d;
		    for(int i = 0; i < windSpeeds.size(); i++)
		    {
				s = calcSpeedBin(windSpeeds[i]);
				d = calcDirectBin(windDirs[i], numSector);
				if(s < 0 || s > 5) 
				    continue;
				if(d < 0 || d > 15)
				    continue;
				wr[d][s]++;
		    }
		    int count = 0;
		    for(int i = 0; i < numSector; i++) 
		    {
				for(int j = 0; j < numSpds; j++) 
				{
			    	count += wr[i][j];
				}
		    }
			return wr;	
		}
		
		

		





		static void calcBins1(vector<float> allWindSpeeds, vector<float> allWindDirs,vector<string> allStationIds,string stationId) 
		{
			int n_rec = allWindSpeeds.size();

			int nDiv8 = n_rec / 8 * 8;

			string* stationIds = &allStationIds[0];

				float windSpeed, windDir;
		        int s, d;
				int arr[NUM_SECTOR][NUM_SPEED] = {0};

				for(int i = 0; i < nDiv8; i+=8) {
					if(stationIds[i] == stationId) {
						windSpeed = windSpeeds[i];
						windDir = windDirs[i];
						if(windSpeed > 0 && windSpeed < 300) {
							if(windDir > 0 && windDir <= 360) { 
								if(windSpeed == 0) s = 0;
								else if(windSpeed > 0 && windSpeed <=5) s = 1;
								else if(windSpeed > 5 && windSpeed <=10) s = 2;
								else if(windSpeed > 10 && windSpeed <=15) s = 3;
								else if(windSpeed > 15 && windSpeed <=25) s = 4;
								else s = 5;
								d = windDir/22.5;
								arr[d][s]++;
							}
						} 
					}

					if(stationIds[i+1] == stationId) {
						windSpeed = windSpeeds[i+1];
						windDir = windDirs[i+1];
						if(windSpeed > 0 && windSpeed < 300) {
							if(windDir > 0 && windDir <= 360) { 
								if(windSpeed == 0) s = 0;
								else if(windSpeed > 0 && windSpeed <=5) s = 1;
								else if(windSpeed > 5 && windSpeed <=10) s = 2;
								else if(windSpeed > 10 && windSpeed <=15) s = 3;
								else if(windSpeed > 15 && windSpeed <=25) s = 4;
								else s = 5;
								d = windDir/22.5;
								arr[d][s]++;
							}
						}
					}
					if(stationIds[i+2] == stationId) {
						windSpeed = windSpeeds[i+2];
						windDir = windDirs[i+2];
						if(windSpeed > 0 && windSpeed < 300) {
							if(windDir > 0 && windDir <= 360) { 
								if(windSpeed == 0) s = 0;
								else if(windSpeed > 0 && windSpeed <=5) s = 1;
								else if(windSpeed > 5 && windSpeed <=10) s = 2;
								else if(windSpeed > 10 && windSpeed <=15) s = 3;
								else if(windSpeed > 15 && windSpeed <=25) s = 4;
								else s = 5;
								d = windDir/22.5;
								arr[d][s]++;
							}
						}
					} 
					if(stationIds[i+3] == stationId) {
						windSpeed = windSpeeds[i+3];
						windDir = windDirs[i+3];
						if(windSpeed > 0 && windSpeed < 300) {
							if(windDir > 0 && windDir <= 360) { 
								if(windSpeed == 0) s = 0;
								else if(windSpeed > 0 && windSpeed <=5) s = 1;
								else if(windSpeed > 5 && windSpeed <=10) s = 2;
								else if(windSpeed > 10 && windSpeed <=15) s = 3;
								else if(windSpeed > 15 && windSpeed <=25) s = 4;
								else s = 5;
								d = windDir/22.5;
								arr[d][s]++;
							}
						}
					}
					if(stationIds[i+4] == stationId) {
						windSpeed = windSpeeds[i+4];
						windDir = windDirs[i+4];
						if(windSpeed > 0 && windSpeed < 300) {
							if(windDir > 0 && windDir <= 360) { 
								if(windSpeed == 0) s = 0;
								else if(windSpeed > 0 && windSpeed <=5) s = 1;
								else if(windSpeed > 5 && windSpeed <=10) s = 2;
								else if(windSpeed > 10 && windSpeed <=15) s = 3;
								else if(windSpeed > 15 && windSpeed <=25) s = 4;
								else s = 5;
								d = windDir/22.5;
								arr[d][s]++;
							}
						}
					} 
					if(stationIds[i+5] == stationId) {
						windSpeed = windSpeeds[i+5];
						windDir = windDirs[i+5];
						if(windSpeed > 0 && windSpeed < 300) {
							if(windDir > 0 && windDir <= 360) { 
								if(windSpeed == 0) s = 0;
								else if(windSpeed > 0 && windSpeed <=5) s = 1;
								else if(windSpeed > 5 && windSpeed <=10) s = 2;
								else if(windSpeed > 10 && windSpeed <=15) s = 3;
								else if(windSpeed > 15 && windSpeed <=25) s = 4;
								else s = 5;
								d = windDir/22.5;
								arr[d][s]++;
							}
						}
					}
					if(stationIds[i+6] == stationId) {
						windSpeed = windSpeeds[i+6];
						windDir = windDirs[i+6];
						if(windSpeed > 0 && windSpeed < 300) {
							if(windDir > 0 && windDir <= 360) { 
								if(windSpeed == 0) s = 0;
								else if(windSpeed > 0 && windSpeed <=5) s = 1;
								else if(windSpeed > 5 && windSpeed <=10) s = 2;
								else if(windSpeed > 10 && windSpeed <=15) s = 3;
								else if(windSpeed > 15 && windSpeed <=25) s = 4;
								else s = 5;
								d = windDir/22.5;
								arr[d][s]++;
							}
						}
					}
					if(stationIds[i+7] == stationId) {
						windSpeed = windSpeeds[i+7];
						windDir = windDirs[i+7];
						if(windSpeed > 0 && windSpeed < 300) {
							if(windDir > 0 && windDir <= 360) { 
								if(windSpeed == 0) s = 0;
								else if(windSpeed > 0 && windSpeed <=5) s = 1;
								else if(windSpeed > 5 && windSpeed <=10) s = 2;
								else if(windSpeed > 10 && windSpeed <=15) s = 3;
								else if(windSpeed > 15 && windSpeed <=25) s = 4;
								else s = 5;
								d = windDir/22.5;
								arr[d][s]++;
							}
						}
					}
				}
				for(int i = 0; i < NUM_SECTOR; i++) {
					for(int j = 0; j < NUM_SPEED; j++) {
						#pragma omp atomic update
						result[i][j] += arr[i][j];
					}
				}
		    	
			for(int i = nDiv8; i < n_rec; i++) {
				if(stationIds[i] == stationId) {
					windSpeed = windSpeeds[i];
					windDir = windDirs[i]/22.5;
					if(windSpeed > 0 && windSpeed < 300) {
						if(windDir > 0 && windDir <= 360) { 
							if(windSpeed == 0) s = 0;
							else if(windSpeed > 0 && windSpeed <=5) s = 1;
							else if(windSpeed > 5 && windSpeed <=10) s = 2;
							else if(windSpeed > 10 && windSpeed <=15) s = 3;
							else if(windSpeed > 15 && windSpeed <=25) s = 4;
							else s = 5;
							d = windDir/22.5;
							result[d][s]++;
						}
					}
				}
			}
		}


};




int main(int argc, char** argv) {

	
	//Creating VM And ENV Variables
	JavaVM *vm;
	JNIEnv *env;
	JavaVMInitArgs vm_args;
	JavaVMOption* options = new JavaVMOption[1];


	//VM Arguments
	options[0].optionString = (char *)"-Djava.class.path=.:jars/netcdfAll-4.5.jar";
	vm_args.version = JNI_VERSION_1_6;
	vm_args.nOptions = 1;
	vm_args.ignoreUnrecognized = 1;
	vm_args.options=options;

	// Construct a VM
	jint res = JNI_CreateJavaVM(&vm, (void **)&env, &vm_args);

	//Delete Options
	delete options; 
        //////////////////////////////////////////////////////////////////////////////////////////////////

	
	//array of file names
	string fileName[total];


	fileName[0]="20130101_0300.gz"; 
	fileName[1]="20130101_0700.gz"; 
	fileName[2]="20130101_1100.gz";
	fileName[3]="20130101_0000.gz"; 
	fileName[4]="20130101_0400.gz";
	fileName[5]="20130101_0800.gz"; 
	fileName[6]="20130101_1200.gz";
	fileName[7]="20130101_0100.gz";
	fileName[8]="20130101_0500.gz";
	fileName[9]="20130101_0900.gz";
	fileName[10]="20130101_0200.gz";  
	fileName[11]="20130101_0600.gz";
	fileName[12]="20130101_1000.gz";
	//for(int i=0;i<total;i++)
	//	fileName[i]="20130101_0000.gz";
	
	clock_t tStart = clock();

	//Parallel Execution of for loop to get the size for each file
	for ( int i = 0; i < total; i++)
		WeatherData::getSize(vm,env,"latitude",fileName[i],dataDir,i);
	
	//Parallel Execution of for loop to get the attributes
	for ( int i = 0; i < total; i++)
	{
		WeatherData::getFloatData(vm,env,"windSpeed",fileName[i],dataDir,capacity[i]);
		WeatherData::getFloatData(vm,env,"windDir",fileName[i],dataDir,capacity[i]);
	}

	//Get Station Ids
	for ( int i = 0; i < total; i++)
	{
		WeatherData::getStringData(vm,env,"stationId",fileName[i],dataDir,capacity[i]);
		WeatherData::deleteFile(vm,env,fileName[i],dataDir);
	}
	
	WeatherData::calcBins1(windSpeeds,windDirs,stationIds,stationIds[7000]);	

	//print result in matrix	
	for(int i = 0; i < 16; i++) 
	{
		for(int j = 0; j < 6; j++) 
		{
			printf("%d\t",result[i][j]);
		}
		printf("\n");
	}	 

	//Print Time Taken For Execution
	printf("\n\nTime taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);

	// Shutdown the VM.
	vm->DestroyJavaVM();
}
