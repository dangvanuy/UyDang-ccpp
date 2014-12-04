#include <stdio.h>
#include <string.h>


#include <fstream>
using namespace std;
//
int exists_test0 (char* name) {
    ifstream f(name); //ifstream: **0.485s**
    if (f.good()) {
        f.close();
        return true;
    } else {
        f.close();
        return false;
    }   
}

int exists_test1 (char* name) {
    if (FILE *file = fopen(name, "r")) { //fopen: **0.302s**
        fclose(file);
        return 1;
    } else {
        return 0;
    }   
}

#include <unistd.h>
int exists_test2 (char* name) {
	 //int access(const char *pathname, int mode);
    return ( access( name, F_OK ) != -1 ); //access(): **0.202s**
}

#include <sys/stat.h>
int IsExistFile(const char *filePath)
{
	if (filePath == NULL || strcmp(filePath, "") == 0)
	{
		printf("File not aviable\n");
		return 0;
	}
	struct stat buffer;
	int err = stat(filePath, &buffer); //stat(): **0.134s**
	if (err == 0){		
		return 1;
	}
	return 0;
}

#if 0
#include <boost/filesystem>
using boost::filesystem;
int find_file( const char* dir_path,         	// in this directory,
                const char* file_name, 			// search for this name,
                path & path_found )           	// placing path here if found
{
  if ( !IsExistFile( dir_path ) ) return 0;
  directory_iterator end_itr; 					// default construction yields past-the-end
  for ( directory_iterator itr( dir_path );
        itr != end_itr;
        ++itr )
  {
    if ( is_directory(itr->status()) )
    {
      if ( find_file( itr->path(), file_name, path_found ) ) return 1;
    }
    else if ( itr->leaf() == file_name ) 		// see below
    {
      path_found = itr->path();
      return 1;
    }
  }
  return 0;
}


#endif

#include <dirent.h> //for alphasort
#define PATH_LENGTH 1024
int find_file_all_subdirectory( const char* dir_path,   // in this directory,
                const char* file_name, // search for this name,
                char** path_found ) 
{
	struct dirent **namelist;
	char fullpath[PATH_LENGTH] = {0};
    int n = scandir(dir_path, &namelist, 0, alphasort);
	if (n < 0)
		return 0;
	for (unsigned int i = 0;i < n ;i++) {		
		if ((strcmp(namelist[i]->d_name, ".") == 0) || (strcmp(namelist[i]->d_name, "..") == 0)){
			continue;
		} 
		if (namelist[i]->d_type == DT_REG){ //is file
			//printf("Checking File: %s\n", namelist[i]->d_name);
			if (strcmp(file_name, namelist[i]->d_name) == 0){
				strncpy(fullpath, dir_path, PATH_LENGTH);
				strcat(fullpath, "/");
				strncat(fullpath, namelist[i]->d_name, PATH_LENGTH);
				(*path_found) = (char*)malloc(strlen(fullpath));
				strcpy(*path_found, fullpath);
				printf("path_found = %s\n", *path_found);
				return 1;
			}
		}
    }
	for (unsigned int i = 0;i < n ;i++) {		
		if ((strcmp(namelist[i]->d_name, ".") == 0) || (strcmp(namelist[i]->d_name, "..") == 0)){
			continue;
		}
		if (namelist[i]->d_type == DT_DIR) {// is directory
			//printf("Recursively in folder: %s\n", namelist[i]->d_name);
			strncpy(fullpath, dir_path, PATH_LENGTH);
			strcat(fullpath, "/");			
			strncat(fullpath, namelist[i]->d_name, PATH_LENGTH);
			if ( find_file_all_subdirectory(fullpath, file_name, path_found ) ) 
				return 1;
		}
	}
	return 0;
}

int main(int arr, char *agrs[])
{
	if (arr < 2){
		printf("Format: ./exits filepath\n");
		return 0;
	}
	//if (IsExistFile(agrs[1]) == 1){
	//if (exists_test2(agrs[1]) == 1){
	//if (exists_test1(agrs[1]) == 1){
	if (exists_test0(agrs[1]) == 1){
		printf("File is Exist\n");
	} else {
		printf("Not such as file or folder\n");
	}
	return 1;
}

//g++ -o exits Check_file_exist_or_not.cpp
//mips-linux-gnu-g++ -o exits Check_file_exist_or_not.cpp


