
#include <string.h>
#include "search-engine.h"
#include "webcrawl.h"
#include "stdio.h"
#include "stdlib.h"
#include <time.h>


#define BILLION 1E9

struct timespec start;
struct timespec end;
int num_searches = 0;
double diff = 0;


SearchEngine::SearchEngine( int port, DictionaryType dictionaryType):
  MiniHTTPD(port)
{
  // Create dictionary of the indicated type

  // Populate dictionary and sort it if necessary

  if (dictionaryType == ArrayDictionaryType)
  {
		_wordToURLList = new ArrayDictionary();
  }
	else if (dictionaryType == HashDictionaryType)
  {
		_wordToURLList = new HashDictionary();
  }
	else if (dictionaryType == AVLDictionaryType)
  {
		_wordToURLList = new AVLDictionary();
  }
	else if (dictionaryType == BinarySearchDictionaryType)
  {
		_wordToURLList = new BinarySearchDictionary();
  }
	else
		_wordToURLList = NULL;

    FILE *urlFile;
    urlFile = fopen("url.txt", "r");
    char *line = new char[3000];
    int i = 0;

    URLRecord **urlArray = new URLRecord*[10000];
    for(i = 0; i < 10000; i++)
    {
      urlArray[i] = new URLRecord();
    }

    i = 0;
    while(fgets(line, 3000, urlFile))
    {
      if(i % 3 == 0)    // This is the line with the url. fix this if it doesnt work
      {
        char *line1 = new char[3000];
        int j = 0;
        int index = 0;
        while(line[j] != ' ') // skip to first space
        {
          j++;
        }
        j++;      // skip the space
        for(index = j; index < 3000; index++)
        {
          line1[index - j] = line[index];
        }
        urlArray[i/3] -> _url = strdup(line1);
      }
      else if(i % 3 == 1)     // Line with description
      {
        char *line2 = new char[3000];
        int j = 0;
        int index = 0;
        while(line[j] != ' ') // skip to first space
        {
          j++;
        }
        j++;      // skip the space
        for(index = j; index < 3000; index++)
        {
          line2[index - j] = line[index];
        }
        urlArray[i/3] -> _description = strdup(line2);
      }
  //    else if(i % 3 == 2)     Blank line. Skip
      i++;
    } // end while

    fclose(urlFile);
    // Right now we have an array of URLRecords with urls and descriptions

    FILE *wordFile;
    wordFile = fopen("word.txt", "r");
    int urlList[3000];

    while(fgets(line, 3000, wordFile))
    {
      int m;
      for(m = 0; m < 3000; m++)
      {
        urlList[m] = -1;
      }
      int urlListIndex = 0;
      i = 0;
      char *wordEntry = new char[5000];
      int wordIndex = 0;
      while(line[i] != ' ' && line[i] != '\0' && line[i] != '\n')
      {
        wordEntry[wordIndex++] = line[i];
        i++;
      }
      i++;
      wordEntry[wordIndex] = '\0'; // null character at end of word
      // Right now we have the word (first word of the line before the space)
      // Next go through the line and get the indexes then switch them to integers and save them in an array
      int k = 0;
      char *urlIndex = new char[5000];
      while(line[i - 1] != '\n')
      {
        if(line[i] != ' ' && line[i] != '\n' && line[i] != '\0')
        {
          urlIndex[k] = line[i];
          i++;
          k++;
        }
        else
        {
          i++;
          urlIndex[k] = '\0';
          if(k != 0)
          {
            urlList[urlListIndex++] = atoi(urlIndex);
          }
          k = 0;
        }
      }
      // addRecord to _wordToURLList with word and indexes
      URLRecordList *head = new URLRecordList();
      head -> _urlRecord = urlArray[urlList[0]];
      head -> _next = NULL;
      URLRecordList *prev = head;
      int h = 1;
      while(urlList[h] >= 0)
      {
        URLRecordList *n = new URLRecordList();
        n -> _urlRecord = urlArray[urlList[h]];
        n -> _next = NULL;
        prev -> _next = n;
        prev = n;
        h++;
      }
      _wordToURLList -> addRecord(wordEntry, (URLRecordList*)head);
    } // end while
}

void
SearchEngine::dispatch( FILE * fout, const char * documentRequested)
{
  if (strcmp(documentRequested, "/")==0) {
    // Send initial form
    fprintf(fout, "<TITLE>CS251 Search</TITLE>\r\n");
    fprintf(fout, "<CENTER><H1><em>Amjad's Search</em></H1>\n");
    fprintf(fout, "<H2>\n");
    fprintf(fout, "<FORM ACTION=\"search\">\n");
    fprintf(fout, "Search:\n");
    fprintf(fout, "<INPUT TYPE=\"text\" NAME=\"word\" MAXLENGTH=\"80\"><P>\n");
    fprintf(fout, "</H2>\n");
    fprintf(fout, "</FORM></CENTER>\n");
    return;
  }

  // TODO: The words to search in "documentRequested" are in the form
  // /search?word=a+b+c
  //
  // You need to separate the words before search
  // Search the words in the dictionary and find the URLs that
  // are common for al the words. Then print the URLs and descriptions
  // in HTML. Make the output look nicer.

  // Here the URLs printed are hardwired

char tempWord[300];
char words_str[strlen(documentRequested) + 1];
char* words[300];
int i = 0;
int j = 0;
int k = 0;
for(i = 13; i < strlen(documentRequested); i++)    // fills up a string with the words from documentRequested
{
  if(documentRequested[i] != '+')
  {
    tempWord[j++] = documentRequested[i];
    words_str[i-13] = documentRequested[i];
  }
  else
  {
    words_str[i-13] = ' ';
    tempWord[j] = '\0';
    words[k++] = strdup(tempWord);
    for(int l = 0; l < strlen(tempWord); l++)
    {
      tempWord[l] = 0;
    }
    j = 0;
  }
}
tempWord[j] = '\0';
words[k++] = strdup(tempWord);
int numWords = k;
while(k < 300)
{
  words[k++] = 0;
}
words_str[i-13] = '\0';

  if(documentRequested[3] != 'v')
  {
    fprintf( stderr, "Search for words: \"%s\"\n", words_str);

    fprintf( fout, "<TITLE>Search Results</TITLE>\r\n");
    fprintf( fout, "<H1> <Center><em>Amjad's Search</em></H1>\n");
    fprintf( fout, "<H2> Search Results for \"%s\"</center></H2>\n", words_str );
  }


  clock_gettime(CLOCK_REALTIME, &start);


  URLRecordList **rl = new URLRecordList*[300];
  URLRecord **finalList = new URLRecord*[10000];
  int size = 0;
  URLRecordList *temp = NULL;
  URLRecordList *temp2 = NULL;
  for(int b = 0; b < numWords; b++)
  {
    rl[b] = (URLRecordList*)_wordToURLList -> findRecord(words[b]);
  }


  // for(int s = 0; s < numWords; s++)
  // {
  //   temp = rl[s];
  //   while(temp != NULL)
  //   {
  //     fprintf( fout, "<h3>%d. <a href=\"%s\">%s</a><h3>\n", s+1, temp -> _urlRecord -> _url, temp -> _urlRecord -> _url );
  //     fprintf( fout, "<blockquote>%s<p></blockquote>\n", temp -> _urlRecord -> _description );
  //     temp = temp -> _next;
  //   }
  // }

  bool found = false;
  bool found2 = true;

  for(int d = 0; d < numWords; d++)
  {
    temp = rl[d];
    while(temp != NULL)
    {
      for(int f = 0; f < numWords; f++)
      {
        if(f != d)
        {
          temp2 = rl[f];
          while(temp2 != NULL)
          {
            if(strcmp(temp -> _urlRecord -> _url, temp2 -> _urlRecord -> _url) == 0)
            {
              found = true;
            }
            temp2 = temp2 -> _next;
          }
          if(!found)
          {
            found2 = false;
          }
        }
        found = false;
      }
      if(found2)
      {
        bool found3 = false;
        for(int g = 0; g < size; g++)
        {
          if(strcmp(finalList[g] -> _url, temp -> _urlRecord -> _url) == 0)
          {
            found3 = true;
          }
        }
        if(!found3)
        {
          finalList[size++] = temp -> _urlRecord;
        }
      }
      found = false;
      found2 = true;
      temp = temp -> _next;
    }
  }

  clock_gettime(CLOCK_REALTIME, &end);
  diff += (end.tv_sec - start.tv_sec ) + (end.tv_nsec - start.tv_nsec ) / BILLION;
  num_searches++;

  for(int s = 0; s < size; s++)
  {
      fprintf( fout, "<h3>%d. <a href=\"%s\">%s</a><h3>\n", s+1, finalList[s] -> _url, finalList[s] -> _url );
      fprintf( fout, "<blockquote>%s<p></blockquote>\n", finalList[s] -> _description );
  }

  fprintf(fout, "<BR>\n");
	fprintf(fout, "<h3>Search Time: %lf ns</h3>\n", diff);
	fprintf(fout, "<h3>Average Search Time: %lf ns</h3>\n", diff/num_searches);

  // Add search form at the end
  fprintf(fout, "<HR><H2>\n");
  fprintf(fout, "<FORM ACTION=\"search\">\n");
  fprintf(fout, "Search:\n");
  fprintf(fout, "<INPUT TYPE=\"text\" NAME=\"word\" MAXLENGTH=\"80\"><P>\n");
  fprintf(fout, "</H2>\n");
  fprintf(fout, "</FORM>\n");
}

void
printUsage()
{
  const char * usage =
    "Usage: search-engine port (array | hash | avl | bsearch)\n"
    "  It starts a search engine at this port using the\n"
    "  data structure indicated. Port has to be larger than 1024.\n";

  fprintf(stderr, "%s", usage);
}

int main(int argc, char ** argv)
{
  if (argc < 3) {
    printUsage();
    return 1;
  }

  // Get port
  int port;
  sscanf( argv[1], "%d", &port);

  // Get DictionaryType
  const char * dictType = argv[2];
  DictionaryType dictionaryType;
  if (!strcmp(dictType, "array")) {
    dictionaryType = ArrayDictionaryType;
  }
  else if (!strcmp(dictType, "hash")) {
    dictionaryType = HashDictionaryType;
  }
  else if (!strcmp(dictType, "avl")) {
    dictionaryType = AVLDictionaryType;
  }
  else if (!strcmp(dictType, "bsearch")) {
    dictionaryType = BinarySearchDictionaryType;
  }
  else {
    printUsage();
    return 0;
  }

  SearchEngine httpd(port, dictionaryType);

  httpd.run();

  return 0;
}
