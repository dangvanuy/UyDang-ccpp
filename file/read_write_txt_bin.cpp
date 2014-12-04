#include <stdio.h>
#include <string.h>

int num_url = 0;
char listUrl[300][1000];

void ReadFile_txt(char filename)
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


int main()
{
	ReadFile_txt("mcast.txt");
	return 0;
}



