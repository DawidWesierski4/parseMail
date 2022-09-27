#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME 2048
#define MAX_DOMAIN 1024

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

struct automaticStatusTestStruct {
   const char* inputName;
   int expectedResult;
};

void
automaticStatusTestParseMail(void) {
   int i;
   struct automaticStatusTestStruct input[] = {
      {"you.froodian@sbcglobal.net",   0},
      {"gemmell@sbcglobal.net",        0},
      {"step.esasaki@att.net",         0},
      {"evilopie@outlook.com",         0},
      {"hans.sthomas@msn.com",         0},
      {"jelmer@optonline.net",         0},
      {"mrobshaw@yahoo.ca",            0},
      {"you.froodiansbcglobal.net",    ERR_INPUT_FORMAT},
      {"gemmellsbcglobal.net",         ERR_INPUT_FORMAT},
      {"step.esasakiatt.net",          ERR_INPUT_FORMAT},
      {"evilopieoutlook.com",          ERR_INPUT_FORMAT},
      {"hans.sthomasmsn.com",          ERR_INPUT_FORMAT},
      {"jelmeroptonline.net",          ERR_INPUT_FORMAT},
      {"mrobshawyahoo.ca",             ERR_INPUT_FORMAT},
      {"y.oufroodian@sbcglobal.net",   ERR_INPUT_FORMAT_SIZE_RULE_VIOLATION},
      {"ge@sbcglobal.net",             ERR_INPUT_FORMAT_SIZE_RULE_VIOLATION},
      {"step.e@att.net",               ERR_INPUT_FORMAT_SIZE_RULE_VIOLATION},
      {"e@outlook.com",                ERR_INPUT_FORMAT_SIZE_RULE_VIOLATION},
      {"hans.sthomas@m",               ERR_INPUT_FORMAT_SIZE_RULE_VIOLATION},
      {"jelmer@o",                     ERR_INPUT_FORMAT_SIZE_RULE_VIOLATION},
      {"m@yahoo.ca",                   ERR_INPUT_FORMAT_SIZE_RULE_VIOLATION},
   };
   int size = sizeof(input) / sizeof(input[0]);
   short succes = 1;
   credentials output;

   for (i = 0; i < size; i++) {
      output.name = malloc(sizeof(input[i].inputName));
      output.surName = malloc(sizeof(input[i].inputName));
      output.domain = malloc(sizeof(input[i].inputName));
      if (ParseMail(input[i].inputName, &output) != input[i].expectedResult) {
         printf("Unexpected result on entry %d\n", i);
         printf("On %s Expected %d got %d\n", input[i].inputName,
                input[i].expectedResult,
                ParseMail(input[i].inputName, &output));
         succes = 0;
      }
      releseCredentialsMemory(&output);
   }

   printf("\nTest of ParseMail result:");
   if (succes) {
      printf("SUCCES\n");
   } else {
      printf("FAIL\n");
   }
}


int main(void)
{
   credentials janK;
   int aux;

   janK.name = (char*)malloc(MAX_NAME);
   janK.surName = (char*)malloc(MAX_NAME);
   janK.domain = (char*)malloc(MAX_DOMAIN);

   automaticStatusTestParseMail();

   if (!janK.name || !janK.domain || !janK.surName) {
      fprintf(stderr, "ERR_ALLOCATING_MEMORY\n");
      return ERR_ALLOCATING_MEMORY;
   }

   aux = ParseMail("jan.kowalski@intel.com", &janK);
   if (!aux) {
      printCredentials(janK);
   } else {
      printf("RESULT: %d\n\n", aux);
   }

   aux = ParseMail("adequatly_longer_test_name.adequatly_longer_test_name"
                   "@adequatly_longer_test_name", &janK);
   if (!aux) {
      printCredentials(janK);
   } else {
      printf("RESULT: %d\n\n", aux);
   }


   aux = ParseMail("jankowalski@intel.com", &janK);
   if (!aux) {
      printCredentials(janK);
   } else {
      printf("RESULT: %d\n\n", aux);
   }

   aux = ParseMail("jan.kowalski@intel.com", &janK);
   if (!aux) {
      printCredentials(janK);
   } else {
      printf("RESULT: %d\n\n", aux);
   }

   aux = ParseMail("jankowalski@@.@.@...com", &janK);
   if (!aux) {
      printCredentials(janK);
   } else {
      printf("RESULT: %d\n\n", aux);
   }

   aux = ParseMail("janko........@@.@.@...com", &janK);
   if (!aux) {
      printCredentials(janK);
   } else {
      printf("RESULT: %d\n\n", aux);
   }

   aux = ParseMail("@", &janK);
   if (!aux) {
      printCredentials(janK);
   } else {
      printf("RESULT: %d\n\n", aux);
   }


   aux = ParseMail(".@", &janK);
   if (!aux) {
      printCredentials(janK);
   } else {
      printf("RESULT: %d\n\n", aux);
   }

   aux = ParseMail("@", &janK);
   if (!aux) {
      printCredentials(janK);
   } else {
      printf("RESULT: %d\n\n", aux);
   }


   aux = ParseMail("adequatly_longer_test_name"
                   ".adequatly_longer__test_surname"
                   "@notadequatly_longer_domain.coma_____________________",
                   &janK);
   if (!aux) {
      printCredentials(janK);
   } else {
      printf("RESULT: %d\n\n", aux);
   }

   aux = ParseMail("notadequatly_longer_alias__________________________________"
                   "__________________________________________________________"
                   "@adequatly_longer_domain.com",
                   &janK);
   if (!aux) {
      printCredentials(janK);
   } else {
      printf("RESULT: %d\n\n", aux);
   }

   aux = ParseMail("adequatly_longer_name."
                   "notadequatly_longer_surname________________________________"
                   "___________________________________________________________"
                   "@adequatly_longer_domain.com",
                   &janK);
   if (!aux) {
      printCredentials(janK);
   } else {
      printf("RESULT: %d\n\n", aux);
   }

   aux = ParseMail("jankowalski@intel.com", &janK);
   if (!aux) {
      printCredentials(janK);
   } else {
      printf("RESULT: %d\n\n", aux);
   }

   aux = ParseMail("jankowa.lski@intel.com", &janK);
   if (!aux) {
      printCredentials(janK);
   } else {
      printf("RESULT: %d\n\n", aux);
   }

   releseCredentialsMemory(&janK);

   aux = ParseMail("jan.kowalski@intel.com", &janK);
   if (!aux) {
      printCredentials(janK);
   } else {
      printf("RESULT: %d\n\n", aux);
   }

   return 0;
}