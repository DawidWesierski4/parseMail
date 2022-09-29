#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parseMail.h"

void
automaticStatusTestParseMail(void) {
   struct automaticStatusTestStruct {
   const char* inputName;
   int expectedResult;
   };
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
   short success = 1;
   credentials output;

   for (i = 0; i < size; i++) {
      if (ParseMail(input[i].inputName, &output) != input[i].expectedResult) {
         printf("Unexpected result on entry %d\n", i);
         printf("On %s Expected %d got %d\n", input[i].inputName,
                input[i].expectedResult,
                ParseMail(input[i].inputName, &output));
         success = 0;
      }
      releaseCredentialsMemory(&output);
   }

   printf("\nTest of ParseMail result:");
   if (success) {
      printf("SUCCESS\n");
   } else {
      printf("FAIL\n");
   }
}

int main(void)
{
   credentials janK;
   int aux;

   automaticStatusTestParseMail();

   aux = ParseMail("jan.kowalski@intel.com", &janK);
   if (!aux) {
      printCredentials(janK);
   } else {
      printf("RESULT: %d\n\n", aux);
   }
   releaseCredentialsMemory(&janK);

   aux = ParseMail("adequately_longer_test_name.adequately_longer_test_name"
                   "@adequately_longer_test_name", &janK);
   if (!aux) {
      printCredentials(janK);
   } else {
      printf("RESULT: %d\n\n", aux);
   }
   releaseCredentialsMemory(&janK);

   aux = ParseMail("jankowalski@intel.com", &janK);
   if (!aux) {
      printCredentials(janK);
   } else {
      printf("RESULT: %d\n\n", aux);
   }
   releaseCredentialsMemory(&janK);

   aux = ParseMail("jan.kowalski@intel.com", &janK);
   if (!aux) {
      printCredentials(janK);
   } else {
      printf("RESULT: %d\n\n", aux);
   }
   releaseCredentialsMemory(&janK);

   aux = ParseMail("jankowalski@@.@.@...com", &janK);
   if (!aux) {
      printCredentials(janK);
   } else {
      printf("RESULT: %d\n\n", aux);
   }
   releaseCredentialsMemory(&janK);

   aux = ParseMail("janko........@@.@.@...com", &janK);
   if (!aux) {
      printCredentials(janK);
   } else {
      printf("RESULT: %d\n\n", aux);
   }
   releaseCredentialsMemory(&janK);

   aux = ParseMail("@", &janK);
   if (!aux) {
      printCredentials(janK);
   } else {
      printf("RESULT: %d\n\n", aux);
   }
   releaseCredentialsMemory(&janK);


   aux = ParseMail(".@", &janK);
   if (!aux) {
      printCredentials(janK);
   } else {
      printf("RESULT: %d\n\n", aux);
   }
   releaseCredentialsMemory(&janK);

   aux = ParseMail("@", &janK);
   if (!aux) {
      printCredentials(janK);
   } else {
      printf("RESULT: %d\n\n", aux);
   }
   releaseCredentialsMemory(&janK);


   aux = ParseMail("adequately_longer_test_name"
                   ".adequately_longer__test_surname"
                   "@notadequately_longer_domain.coma_____________________",
                   &janK);
   if (!aux) {
      printCredentials(janK);
   } else {
      printf("RESULT: %d\n\n", aux);
   }
   releaseCredentialsMemory(&janK);

   aux = ParseMail("notadequately_longer_alias__________________________________"
                   "__________________________________________________________"
                   "@adequately_longer_domain.com",
                   &janK);
   if (!aux) {
      printCredentials(janK);
   } else {
      printf("RESULT: %d\n\n", aux);
   }
   releaseCredentialsMemory(&janK);

   aux = ParseMail("adequately_longer_name."
                   "notadequately_longer_surname________________________________"
                   "___________________________________________________________"
                   "@adequately_longer_domain.com",
                   &janK);
   if (!aux) {
      printCredentials(janK);
   } else {
      printf("RESULT: %d\n\n", aux);
   }
   releaseCredentialsMemory(&janK);

   aux = ParseMail("jankowalski@intel.com", &janK);
   if (!aux) {
      printCredentials(janK);
   } else {
      printf("RESULT: %d\n\n", aux);
   }
   releaseCredentialsMemory(&janK);

   aux = ParseMail("jankowa.lski@intel.com", &janK);
   if (!aux) {
      printCredentials(janK);
   } else {
      printf("RESULT: %d\n\n", aux);
   }
   releaseCredentialsMemory(&janK);

   aux = ParseMail("jan.kowalski@intel.com", &janK);
   if (!aux) {
      printCredentials(janK);
   } else {
      printf("RESULT: %d\n\n", aux);
   }
   releaseCredentialsMemory(&janK);

   return 0;
}
