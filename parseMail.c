#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parseMail.h"

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
parseName(const char **mail, const char *dotCharPtr, credentials *outPtr)
{
   int check, size = dotCharPtr - *mail;
   outPtr->name = (char*)malloc(size+1);
   if (outPtr->name == NULL) {
      fprintf(stderr, "ERR_ALLOCATING_MEMORY");
      return ERR_ALLOCATING_MEMORY;
   }

   strcpy(outPtr->name, "\0");
   strncat(outPtr->name, *mail, size);
   check = checkName(outPtr->name);
   if (check) {
      fprintf(stderr, "ERR_INPUT_FORMAT\n");
      return check;
   }

   *mail = dotCharPtr + 1; /* move pointer past the dot char */
   return 0;
}

int
parseSurname(const char **mail, const char *atCharPtr, credentials *outPtr)
{
   int check, size = atCharPtr - *mail;
   outPtr->surName = (char*)malloc(size+1);
   if (outPtr->surName == NULL) {
      fprintf(stderr, "ERR_ALLOCATING_MEMORY");
      return ERR_ALLOCATING_MEMORY;
   }

   strcpy(outPtr->surName, "\0");
   strncat(outPtr->surName, *mail, atCharPtr - *mail);
   check = checkName(outPtr->surName);
   if (outPtr->name == outPtr->surName) {
      outPtr->surName = NULL;
   }

   if (check) {
      fprintf(stderr, "ERR_INPUT_FORMAT\n");
      return check;
   }
   *mail = atCharPtr; /* move pointer to the at character */
   return 0;
}

int
parseDomain(const char* mail, credentials *outPtr)
{
   int check, size = strlen(mail);
   outPtr->domain = (char*)malloc(size);
   if (outPtr->domain == NULL) {
      fprintf(stderr, "ERR_ALLOCATING_MEMORY");
      return ERR_ALLOCATING_MEMORY;
   }

   strcpy(outPtr->domain, mail);
   check = checkDomain(outPtr->domain);
   if (check) {
      fprintf(stderr, "ERR_INPUT_FORMAT\n");
      return check;
   }

   return 0;
}

int
ParseMail(const char *mail, void *out)
{
   if (!mail) {
      fprintf(stderr, "ERR_NULL_POINTER\n");
      return ERR_NULL_POINTER;
   }

   const char *atCharPtr = strchr(mail, '@');
   const char *dotCharPtr;
   int atPosition, check;
   credentials *outPtr;

   if (!out) {
      fprintf(stderr, "ERR_NULL_POINTER\n");
      return ERR_NULL_POINTER;
   } else if (!atCharPtr) {
      fprintf(stderr, "ERR_INPUT_FORMAT\n");
      return ERR_INPUT_FORMAT;
   }

   outPtr = ((credentials*)out);
   atPosition = atCharPtr - mail;
   dotCharPtr = strchr(mail, '.');
   if (dotCharPtr && atPosition > dotCharPtr - mail) {
      check = parseName(&mail, dotCharPtr, outPtr);
      if (check) {
         return check;
      }

   } else {
      outPtr->surName = outPtr->name;
   }
   /*
    * if there was an '.' character before '@':
    * out content
    *  ["name"] <- name
    *  [MAX_NAME bytes of memory ] <- surName
    *  [MAX_DOMAIN bytes of memory] <- domain
    * mail points to
    *      \/
    *  name.surname@domain.com
    * otherwise:
    * out content
    *  [MAX_NAME bytes of memory ] <- name, surName
    *  [MAX_DOMAIN bytes of memory] <- domain
    * mail points to
    * \/
    *  alias@domain.com
    */
   check = parseSurname(&mail, atCharPtr, outPtr);
   if (check) {
      return check;
   }
   check = parseDomain(mail, outPtr);
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
