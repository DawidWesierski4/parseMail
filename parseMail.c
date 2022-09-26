#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_NAME 64
#define MAX_DOMAIN 32

enum ERR_NUMS {
   ERR_ALLOCATING_MEMORY = 1,
   ERR_NULL_POINTER,
   ERR_STR_BUFF_OVERFLOW,
   ERR_INPUT_FORMAT,
   ERR_INPUT_FORMAT_SIZE_RULE_VIOLATION,
   ERR_INPUT_FORMAT_ILLEGAL_CHAR
};

typedef struct credentialInformation {

   char *name;
   char *surName;
   char *domain;
} credentials;

/* rules for names */
int
checkName(const char *name)
{
   int minSize = 3;
   int i = 0;

   while (name[i] != '\0') {
      /* We don't need to put @ in the illegal character list here
       * as everything past the at is considered domain */
      if (name[i] == '.' || name[i] == ' ') {
         return ERR_INPUT_FORMAT_ILLEGAL_CHAR;
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
   int i = 1;

   if (domain[0] != '@') {
      return ERR_INPUT_FORMAT_ILLEGAL_CHAR;
   }

   while (domain[i] != '\0') {
      if (domain[i] == '@' || domain[i] == ' ') {
         return ERR_INPUT_FORMAT_ILLEGAL_CHAR;
      }
      i++;
   }

   if (i < minSize) {
      return ERR_INPUT_FORMAT_SIZE_RULE_VIOLATION;
   }

   return 0;
}

int
ParseMail(const char *mail, void *out)
{
   const char *atCharPtr = strchr(mail, '@');
   const char *dotCharPtr;
   int atPosition, size, check;
   credentials *outPtr = ((credentials*)out);

   if (!out || outPtr->name == NULL || outPtr->domain == NULL) {
      fprintf(stderr, "ERR_ALLOCATING_MEMORY\n");
      return ERR_NULL_POINTER;
   } else if (!atCharPtr) {
      fprintf(stderr, "ERR_INPUT_FORMAT\n");
      return ERR_INPUT_FORMAT;
   }

   atPosition = atCharPtr - mail;
   dotCharPtr = strchr(mail, '.');

   if (dotCharPtr && atPosition > dotCharPtr - mail) {
      if (dotCharPtr - mail > MAX_NAME - 1) {
         fprintf(stderr, "ERR_STR_BUFF_OVERFLOW\n");
         return ERR_STR_BUFF_OVERFLOW;
      }

      if (outPtr->surName == NULL) {
          outPtr->surName = (char*)malloc(MAX_NAME);
      }

      strcpy(outPtr->name, "\0");
      strncat(outPtr->name, mail, dotCharPtr - mail);
      check = checkName(outPtr->name);

      if (check) {
         fprintf(stderr, "ERR_INPUT_FORMAT\n");
         return check;
      }

      mail = dotCharPtr + 1; /* move pointer past the dot char */
   } else {
      free(outPtr->surName);
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
   if (atCharPtr - mail > MAX_NAME - 1) {
      if (outPtr->name ==
          outPtr->surName) {
         /* to make sure that we do not free the same pointer 2 times */
         outPtr->surName = NULL;
      }
      fprintf(stderr, "ERR_STR_BUFF_OVERFLOW\n");
      return ERR_STR_BUFF_OVERFLOW;
   }

   strcpy(outPtr->surName, "\0");
   strncat(outPtr->surName, mail, atCharPtr - mail);

   check = checkName(outPtr->surName);

   if (outPtr->name == outPtr->surName) {
      /* to make sure that we do not free the same pointer 2 times */
      outPtr->surName = NULL;
   }

   if (check) {
      fprintf(stderr, "ERR_INPUT_FORMAT\n");
      return check;
   }

   mail = atCharPtr;
   size = strlen(mail);
   if (size > MAX_DOMAIN) {
      fprintf(stderr, "ERR_STR_BUFF_OVERFLOW\n");
      return ERR_STR_BUFF_OVERFLOW;
   }

   strcpy(outPtr->domain, mail);

   check = checkDomain(outPtr->domain);
   if (check) {
      fprintf(stderr, "ERR_INPUT_FORMAT\n");
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
releseCredentialsMemory(credentials *person) {
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


int main(void)
{
   credentials janK;
   int aux;

   janK.name = (char*)malloc(MAX_NAME);
   janK.surName = (char*)malloc(MAX_NAME);
   janK.domain = (char*)malloc(MAX_DOMAIN);

   if (!janK.name || !janK.domain || !janK.surName) {
      fprintf(stderr, "ERR_ALLOCATING_MEMORY\n");
      return ERR_ALLOCATING_MEMORY;
   }

   aux = ParseMail("jan.kowalski@intel.com", (void*)&janK);
   if (!aux) {
      printCredentials(janK);
   } else {
      printf("RESULT: %d\n\n", aux);
   }

   aux = ParseMail("adequatly_longer_test_name.adequatly_longer_test_name"
                   "@adequatly_longer_test_name", (void*)&janK);
   if (!aux) {
      printCredentials(janK);
   } else {
      printf("RESULT: %d\n\n", aux);
   }


   aux = ParseMail("jankowalski@intel.com", (void*)&janK);
   if (!aux) {
      printCredentials(janK);
   } else {
      printf("RESULT: %d\n\n", aux);
   }

   aux = ParseMail("jan.kowalski@intel.com", (void*)&janK);
   if (!aux) {
      printCredentials(janK);
   } else {
      printf("RESULT: %d\n\n", aux);
   }

   aux = ParseMail("jankowalski@@.@.@...com", (void*)&janK);
   if (!aux) {
      printCredentials(janK);
   } else {
      printf("RESULT: %d\n\n", aux);
   }

   aux = ParseMail("janko........@@.@.@...com", (void*)&janK);
   if (!aux) {
      printCredentials(janK);
   } else {
      printf("RESULT: %d\n\n", aux);
   }

   aux = ParseMail("@", (void*)&janK);
   if (!aux) {
      printCredentials(janK);
   } else {
      printf("RESULT: %d\n\n", aux);
   }


   aux = ParseMail(".@", (void*)&janK);
   if (!aux) {
      printCredentials(janK);
   } else {
      printf("RESULT: %d\n\n", aux);
   }

   aux = ParseMail("@", (void*)&janK);
   if (!aux) {
      printCredentials(janK);
   } else {
      printf("RESULT: %d\n\n", aux);
   }


   aux = ParseMail("adequatly_longer_test_name"
                   ".adequatly_longer__test_surname"
                   "@notadequatly_longer_domain.coma_____________________",
                   (void*)&janK);
   if (!aux) {
      printCredentials(janK);
   } else {
      printf("RESULT: %d\n\n", aux);
   }

   aux = ParseMail("notadequatly_longer_alias__________________________________"
                   "__________________________________________________________"
                   "@adequatly_longer_domain.com",
                   (void*)&janK);
   if (!aux) {
      printCredentials(janK);
   } else {
      printf("RESULT: %d\n\n", aux);
   }

   aux = ParseMail("adequatly_longer_name."
                   "notadequatly_longer_surname________________________________"
                   "___________________________________________________________"
                   "@adequatly_longer_domain.com",
                   (void*)&janK);
   if (!aux) {
      printCredentials(janK);
   } else {
      printf("RESULT: %d\n\n", aux);
   }

   aux = ParseMail("jankowalski@intel.com", (void*)&janK);
   if (!aux) {
      printCredentials(janK);
   } else {
      printf("RESULT: %d\n\n", aux);
   }

   aux = ParseMail("jankowa.lski@intel.com", (void*)&janK);
   if (!aux) {
      printCredentials(janK);
   } else {
      printf("RESULT: %d\n\n", aux);
   }

   releseCredentialsMemory(&janK);

   aux = ParseMail("jan.kowalski@intel.com", (void*)&janK);
   if (!aux) {
      printCredentials(janK);
   } else {
      printf("RESULT: %d\n\n", aux);
   }

   return 0;
}