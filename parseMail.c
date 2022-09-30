#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parseMail.h"

void
printErrorMsg(int errorEnum)
{
   switch (errorEnum) {
      case ERR_ALLOCATING_MEMORY:
         fprintf(stderr, "ERR_ALLOCATING_MEMORY");
         return;
      case ERR_NULL_POINTER:
         fprintf(stderr, "ERR_NULL_POINTER");
         return;
      case ERR_INPUT_FORMAT:
         fprintf(stderr, "ERR_INPUT_FORMAT");
         return;
      case ERR_INPUT_FORMAT_SIZE_RULE_VIOLATION:
         fprintf(stderr, "ERR_INPUT_FORMAT_SIZE_RULE_VIOLATION");
         return;
      case ERR_INPUT_FORMAT_ILLEGAL_CHAR:
         fprintf(stderr, "ERR_INPUT_FORMAT_ILLEGAL_CHAR");
         return;
      default:
         fprintf(stderr, "ERR_UNKNOWN_ERR");
         return;
   }
}

/* rules for names */
int
checkName(const char *name)
{
   int minSize = 3;
   int maxSize = 32;
   int i = 0;

   while (name[i] != '\0') {
      /* We don't need to put @ in the illegal character list here
       * as everything past the at is considered domain */
      if (name[i] == '.' || name[i] == ' ') {
         return ERR_INPUT_FORMAT_ILLEGAL_CHAR;
      } else if (i > maxSize) {
         return ERR_INPUT_FORMAT_SIZE_RULE_VIOLATION;
      }

      i++;
   }

   if (i < minSize) {
      return ERR_INPUT_FORMAT_SIZE_RULE_VIOLATION;
   }

   return 0;
}

/* rules for domain */
int
checkDomain(const char *domain)
{
   int minSize = 3;
   int maxSize = 32;
   int i = 1;

   if (domain[0] != '@') {
      return ERR_INPUT_FORMAT_ILLEGAL_CHAR;
   }

   while (domain[i] != '\0') {
      if (domain[i] == '@' || domain[i] == ' ') {
         return ERR_INPUT_FORMAT_ILLEGAL_CHAR;
      } else if (i > maxSize) {
         return ERR_INPUT_FORMAT_SIZE_RULE_VIOLATION;
      }

      i++;
   }

   if (i < minSize) {
      return ERR_INPUT_FORMAT_SIZE_RULE_VIOLATION;
   }

   return 0;
}

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

   if(charPtr == NULL) {
      check = checkDomain(*outPtr);
   } else {
      check = checkName(*outPtr);
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

int
parseDomain(const char* mail, credentials *outPtr)
{
   int check, size = strlen(mail);
   outPtr->domain = (char*)malloc(size);
   if (outPtr->domain == NULL) {
      printErrorMsg(ERR_ALLOCATING_MEMORY);
      return ERR_ALLOCATING_MEMORY;
   }

   strcpy(outPtr->domain, mail);
   check = checkDomain(outPtr->domain);
   if (check) {
      printErrorMsg(check);
      return check;
   }

   return 0;
}

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

void
printCredentials(credentials person)
{
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

void
releaseCredentialsMemory(credentials *person)
{
   if(!person) {
      return;
   }

   free(person->name);
   person->name = NULL;
   free(person->surName);
   person->surName = NULL;
   free(person->domain);
   person->domain = NULL;
}
