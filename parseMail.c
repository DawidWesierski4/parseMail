#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parseMail.h"

/*
 * printErrorMsg
 *  Description: prints the ENUM names from ERR_NUMS to stderr and if passed
 *  unknown integer prints ERR_UNKNOWN_ERROR
 *
 * Input
 *  1 int errorEnum -> integer representing enum from ERR_NUMS
 */
void
printErrorMsg(int errorEnum)
{
   switch (errorEnum) {
      case ERR_ALLOCATING_MEMORY:
         fprintf(stderr, "ERR_ALLOCATING_MEMORY\n");
         return;
      case ERR_NULL_POINTER:
         fprintf(stderr, "ERR_NULL_POINTER\n");
         return;
      case ERR_INPUT_FORMAT:
         fprintf(stderr, "ERR_INPUT_FORMAT\n");
         return;
      case ERR_INPUT_FORMAT_SIZE_RULE_VIOLATION:
         fprintf(stderr, "ERR_INPUT_FORMAT_SIZE_RULE_VIOLATION\n");
         return;
      case ERR_INPUT_FORMAT_ILLEGAL_CHAR:
         fprintf(stderr, "ERR_INPUT_FORMAT_ILLEGAL_CHAR\n");
         return;
      default:
         fprintf(stderr, "ERR_UNKNOWN_ERROR\n");
         return;
   }
}

/*
 * checkString
 *
 * Description: checks the string for illegal characters and if the size is
 *  long enough.
 *
 * Input:
 *  1 const char *testString -> string to check for size / characters
 *  2 const char *illegalChars -> illegal characters
 *  3 int minSize -> minimal size that the string should have
 *
 * Output
 *  int
 *  4 (ERR_INPUT_FORMAT_SIZE_RULE_VIOLATION) when string size < minsize
 *  5 (ERR_INPUT_FORMAT_ILLEGAL_CHAR)when string contains characters from
 *    illegal characters string
 */
int checkString(const char *testString, const char *illegalChars, int minSize)
{
   int i,j;

   for (i = 0; testString[i] != '\0'; i++) {
      for (j = 0; illegalChars[j] != '\0'; j++) {
         if (testString[i] == illegalChars[j]) {
            return ERR_INPUT_FORMAT_ILLEGAL_CHAR;
         }
      }
   }

   if (i < minSize) {
      return ERR_INPUT_FORMAT_SIZE_RULE_VIOLATION;
   }

   return 0;
}

/* parses mail surname and domain for the ParseMail function */
int
parseName(const char **mail, const char *charPtr, char **outPtr)
{
   int check, size = charPtr - *mail;

   if (charPtr == NULL) {
      size = strlen(*mail); /* domain */
   } else {
      size = charPtr - *mail; /* name or surname */
   }

   *outPtr = (char*)malloc(size + 1);
   if (*outPtr == NULL) {
      printErrorMsg(ERR_ALLOCATING_MEMORY);
      return ERR_ALLOCATING_MEMORY;
   }

   strncpy(*outPtr, *mail, size);
   (*outPtr)[size] = '\0';

   if (charPtr == NULL) {
      check = checkString(*outPtr+1, "@ ^", 3);
   } else {
      check = checkString(*outPtr, ".@ ^", 3);
   }

   if (check) {
      printErrorMsg(check);
      return check;
   }

   if (!charPtr) {
      return 0;
   } else if (charPtr[0] == '.') {
      *mail = charPtr + 1; /* move pointer past the dot char */
   } else if (charPtr[0] == '@') {
      *mail = charPtr; /* move pointer to the at character */
   }

   return 0;
}

/*
 * ParseMail
 *
 * Description: This function segments string into a credentials struct and
 *  checks if the segments are correct, it allows for 2 formats of data one
 *  where there are specifed name only and one where there are specifed name and
 *  surname data is saved in the memory that out is pointing to
 *
 * Input:
 *  1 const char *mail mail string
 *  2 void *out pointer to the credentials struct where data is specifed
 *
 * Output:
 *  int
 *   0 if the operation was successful
 *   [ERR_NUM] if the operation was unsuccessful
 *
 * Changes:
 *  out->name = memory in heap with string containing name / alias
 *  out->surName = NULL or memory in heap with string containing Surname
 *  out->domain = memory in heap with string containing domain name
 *
 * Disclaimer:
 *  function is made in way that ensures that you can use the releaseCredentials
 *  function afterwards which means that even if error was returned there is no
 *  pointer to unspecified memory its either NULL or allocated memory but the
 *  data pointer is pointing to is undefined if the execution returned an error
 */
int
ParseMail(const char *mail, void *out)
{
   const char *atCharPtr;
   const char *dotCharPtr;
   int atPosition, check;
   credentials *outPtr;

   if (!mail) {
      printErrorMsg(ERR_NULL_POINTER);
      return ERR_NULL_POINTER;
   }

   atCharPtr = strchr(mail, '@');
   if (!out) {
      printErrorMsg(ERR_NULL_POINTER);
      return ERR_NULL_POINTER;
   } else if (!atCharPtr) {
      printErrorMsg(ERR_INPUT_FORMAT);
      return ERR_INPUT_FORMAT;
   }

   outPtr = ((credentials*)out);
   atPosition = atCharPtr - mail;
   dotCharPtr = strchr(mail, '.');
   if (dotCharPtr && atPosition > dotCharPtr - mail) {
      check = parseName(&mail, dotCharPtr, &outPtr->name);
      if (check) {
         return check;
      }

      check = parseName(&mail, atCharPtr, &outPtr->surName);
      if (check) {
         return check;
      }

   } else {
      outPtr->surName = NULL;
      check = parseName(&mail, atCharPtr, &outPtr->name);
      if (check) {
         return check;
      }

   }

   check = parseName(&mail, NULL, &outPtr->domain);
   if (check) {
      return check;
   }

   return 0;
}

/*
 * printCredentials
 *
 * Description: prints Credentials struct in a format:
 *
 * "CREDENTIALS:"
 * "Fist Name: person.name"  | OR | "Alias: person.name"
 * "Surname: person.surName" | OR |
 * "Domain: person.domain"
 */
void
printCredentials(credentials person)
{
   if (person.name == NULL || person.domain == NULL) {
      return;
   }

   printf("\nCREDENTIALS:\n");
   if (person.surName == NULL) {
      printf("Alias: %s\n", person.name);
   } else {
      printf("First Name: %s\n", person.name);
      printf("Surname: %s\n", person.surName);
   }

   printf("Domain: %s\n", person.domain);
   printf("\n");
}

/* releases memory from Credentials struct */
void
releaseCredentialsMemory(credentials *person)
{
   if (!person) {
      return;
   }

   free(person->name);
   person->name = NULL;
   free(person->surName);
   person->surName = NULL;
   free(person->domain);
   person->domain = NULL;
}
