/*
 * main.c
 *      Author: drumm
 */


#include <stdio.h>
#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

struct MemoryStruct {
  char *memory;
  size_t size;
};


static size_t
WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
  size_t realsize = size * nmemb;
  struct MemoryStruct *mem = (struct MemoryStruct *)userp;

  mem->memory = realloc(mem->memory, mem->size + realsize + 1);
  if (mem->memory == NULL) {
    /* out of memory! */
    printf("not enough memory (realloc returned NULL)\n");
    exit(EXIT_FAILURE);
  }

  memcpy(&(mem->memory[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = 0;

  return realsize;
}

main(){
	char API[50], GetAddress[115] ; // API Length + 1, 0
	printf("Please enter your API key: ");
	fgets(API, 50, stdin);

	sprintf(GetAddress, "http://eu-cp.creeperhost.net/API/?key=%s&method=Console&command=Get", API);
	//printf("%s\n", GetAddress);

	CURL *curl;
	CURLcode res;

	struct MemoryStruct chunk, prevChunk;




	if ( curl ){
		initscr();
		chunk.memory = malloc(1);  /* will be grown as needed by the realloc above */
		chunk.size = 0;    /* no data at this point */
		curl = curl_easy_init();
		curl_easy_setopt(curl, CURLOPT_URL, GetAddress);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
		curl_easy_setopt(curl, CURLOPT_USERAGENT, "Narna-agent/1.0");
		res = curl_easy_perform(curl);
		printw("%s", chunk.memory);
		refresh();

		prevChunk.memory = chunk.memory;
		prevChunk.size = chunk.size;

		while ( 1 ){
			chunk.memory = malloc(1);  /* will be grown as needed by the realloc above */
			chunk.size = 0;    /* no data at this point */
			curl = curl_easy_init();
			curl_easy_setopt(curl, CURLOPT_URL, GetAddress);
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
			curl_easy_setopt(curl, CURLOPT_USERAGENT, "Narna-agent/1.0");
			res = curl_easy_perform(curl);
			sleep(1);
			printw("%s", chunk.memory);
			refresh();
			//memset(chunk.memory, 0, chunk.size);
			if ( chunk.memory )
				free(chunk.memory);

		}



		endwin();
		curl_easy_cleanup(curl);
	}
}
