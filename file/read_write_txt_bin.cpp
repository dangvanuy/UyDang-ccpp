/*
 *	Docs:
 *		http://www.cplusplus.com/doc/tutorial/files/
 */


#include <stdio.h>
#include <string.h>
#include <stdlib.h> //system

int num_url = 0;
char listUrl[300][1000];

void read_file_txt(char *filename)
{
	FILE *f;
	num_url = 0;
	f = fopen(filename, "r");
	if (f == NULL){
		return;
	}
	while(!feof(f)){
		memset(listUrl[num_url], 0, sizeof(listUrl[num_url]));
		fgets(listUrl[num_url],1000,f);
		int len = strlen(listUrl[num_url]);
		if (len <= 1 || ((int)listUrl[num_url][0] == 13 && (listUrl[num_url][1] == 10))) {
			continue;
		}
		if (listUrl[num_url][len - 1] == 10)
			listUrl[num_url][len - 1] = '\0';
		if (listUrl[num_url][len - 2] == 13)
			listUrl[num_url][len - 2] = '\0';
		printf("%s\n", listUrl[num_url]);
		num_url += 1;
		if (num_url >= 300)
			break;
	}
	fclose(f);
}

void write_file_txt(char *filename)
{
	FILE * pFile = NULL;
	pFile		= fopen(filename,"w");
	char* stringInfo = "Line 1";
	if (pFile != NULL) {			
		fprintf(pFile, "%s", stringInfo);
		printf("write ok >.............................\n");
		fclose(pFile);
		system("sync");
	}
	else {
		printf("\t*** Can't open file: \n");
	}	
}

#include <iostream>
#include <fstream>
using namespace std;
void WriteFileTxt(char *filename)
{
	ofstream myfile (filename);
	if (myfile.is_open())
	{
		myfile << "This is a line.\n";
		myfile.close();
	}else {
		cout << "Unable to open file";
	}
}

void ReadFileTxt(char *filename)
{
	string line;
	ifstream myfile (filename);
	if (myfile.is_open())
	{
		while ( getline (myfile,line) )
		{
			cout << line << '\n';
		}
		myfile.close();
	} else {
		cout << "Unable to open file"; 
	}
}

struct SData
{
	int a;
	int b;
};

void WriteFileBin(char *filename, struct SData data)
{
	int			uiSize;
	void		*pData;
    ofstream outconf(filename, ios::out | ios::binary);
    if(!outconf)
	{
		printf("Can not open file\n");
		return;
	}
	pData		= (void*)&data;
	uiSize		= sizeof(data);
    outconf.write((char *)pData, uiSize);
    if(!outconf.good())
	{
		printf("Can not write file\n");
		outconf.close();
		return;
	}
	outconf.close();
	printf("Write file DONE\n");
}

void ReadFileBin(char *filename, struct SData *data)
{
	int			uiSize;
	void		*pData;
    ifstream inconf(filename, ios::in | ios::binary);
    if(!inconf)
	{
		printf("Can not open file\n");
		return;
	}
	pData		= (void*)data;
	uiSize		= sizeof(struct SData);
    inconf.read((char *)pData, uiSize);
    if(!inconf.good())
	{
		printf("Can not read file\n");
		inconf.close();
		return;
	}
	inconf.close();
	printf("Read file DONE\n");
}

int main()
{
	struct SData writeData = {1, 2};
	printf("in : data.a = %d\n", writeData.a);
	printf("in : data.b = %d\n", writeData.b);
	WriteFileBin("data.bin", writeData);
	
	struct SData readData;
	ReadFileBin("data.bin", &readData);
	printf("out: data.a = %d\n", readData.a);
	printf("out: data.b = %d\n", readData.b);
	return 0;
}


