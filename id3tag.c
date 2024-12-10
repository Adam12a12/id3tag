#include <stdio.h>
#include <id3v2lib.h>
#include <dirent.h>
#include <string.h> 
#include <stdlib.h>
#include <unistd.h>
#include "id3tag.h"

static int one(){
	return 1;
}

int checkSuffix(const char *str, const char *suffix){
    if (!str || !suffix){
      return -1;
    }
    size_t lenstr = strlen(str);
    size_t lensuffix = strlen(suffix);
    if (lensuffix > lenstr){
      return -1;
    }
    return strcmp(str + (lenstr - lensuffix), suffix);
}

char* returnExt(const char *str){
	char* ext = strrchr(str,'.');
	return ext;
}

int prepareName(ID3v2_Tag *tag, char* newName){
	ID3v2_TextFrame* track_frame = ID3v2_Tag_get_track_frame(tag);
	ID3v2_TextFrame* title_frame = ID3v2_Tag_get_title_frame(tag);
	for(int i = 0; i < strlen(track_frame->data->text); i++){
		if(track_frame->data->text[i] == '/'){
			newName[i] = '\0';
			break;
		}
		newName[i] = track_frame->data->text[i];
	}
	strcat(newName," - ");
	strcat(newName,title_frame->data->text);
	return 0;
}

int readDir(char* dir){
	const char* ext = ".mp3";
	struct dirent **eps;
	int n = scandir(dir, &eps, one, alphasort);
	if(n >= 0){
		for(int i = 0; i < n; i++){
			if(checkSuffix(eps[i]->d_name,ext) == 0){
				renameFile(eps[i]->d_name);
			}
		}
	}
}

int renameFile(char *fPath){
	ID3v2_Tag* tag = ID3v2_read_tag(fPath);
	if(tag == NULL){
		printf("File does not have a tag\n");
		return -1;
	}
	char newName[PATH_MAX];
	if(prepareName(tag, newName) != 0){
		printf("Error preparing name\n");
	}
	strcat(newName,returnExt(fPath));
	if(rename(fPath, newName) != 0){
		perror("Error");
	}
}

int printTag(char *fPath){
	ID3v2_Tag* tag = ID3v2_read_tag(fPath);
	if(tag == NULL){
		printf("File does not have a tag\n");
		return -1;
	}
	ID3v2_TextFrame* track_frame = ID3v2_Tag_get_track_frame(tag);
	ID3v2_TextFrame* title_frame = ID3v2_Tag_get_title_frame(tag);
	ID3v2_TextFrame* artist_frame = ID3v2_Tag_get_artist_frame(tag);
	ID3v2_TextFrame* album_frame = ID3v2_Tag_get_album_frame(tag);
	printf("Title: %s\n",title_frame->data->text);
	printf("Track: %s\n",track_frame->data->text);
	printf("Artist: %s\n",artist_frame->data->text);
	printf("Album: %s\n",album_frame->data->text);
}


int main(int argc, char *argv[]){

	if(argc<2){
		printf("Options:\n");
		printf("p: Prints file tag\n");
		printf("r: Adds track number to the file name\n");
		return -1;
	}
	int opt;
	while((opt = getopt(argc, argv, "p:r:d:")) != -1)
	{
		switch(opt)
		{

			case 'r':
				renameFile(optarg);
				break;
			case 'p':
				printTag(optarg);
				break;
			case 'd':
				readDir(optarg);
				break;
			case ':':
				printf("option needs a value\n");
				break;
			case '?':
				printf("unknown option: %c\n", optopt);
				break;
		}
	}
	for(; optind < argc; optind++){
		printf("extra arguments: %s\n", argv[optind]);
	}
	return 0;
}


