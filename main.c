#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parseMail.h"

void
automaticStatusTestParseMail(void)
{
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
      {"you..roodian@sbcglobal.net",   ERR_INPUT_FORMAT_ILLEGAL_CHAR},
      {"gemmell@sbcglob@l.net",        ERR_INPUT_FORMAT_ILLEGAL_CHAR},
      {"step.esasaki@att net",         ERR_INPUT_FORMAT_ILLEGAL_CHAR},
      {"ev lopie@outlook.com",         ERR_INPUT_FORMAT_ILLEGAL_CHAR},
      {"hans.sth mas@msn.com",         ERR_INPUT_FORMAT_ILLEGAL_CHAR},
      {"jelmer@optonlin .net",         ERR_INPUT_FORMAT_ILLEGAL_CHAR},
      {"mro shaw@yahoo.ca",            ERR_INPUT_FORMAT_ILLEGAL_CHAR},
      {NULL,                           ERR_NULL_POINTER}
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

void
unitTest(const char *one)
{
   credentials janK;
   int aux;
   aux = ParseMail(one, &janK);
   if (!aux) {
      printCredentials(janK);
   } else {
      printf("Test: %s Failed \n",one);
      printf("RESULT: %d\n\n", aux);
   }
   releaseCredentialsMemory(&janK);
}

int main(void)
{
   automaticStatusTestParseMail();

   unitTest("jan.kowalski@intel.com");
   unitTest("adequately_longer_test_name.adequately_longer_test_name"
            "@adequately_longer_test_name");
   unitTest("jankowalski@intel.com");
   unitTest("jan.kowalski@intel.com");
   unitTest("jankowalski@@.@.@...com");
   unitTest("janko........@@.@.@...com");
   unitTest("@");
   unitTest(".@");
   unitTest("@");
   unitTest("adequately_longer_test_name_______________________________________"
             "_________________________________________________________________"
             "_________________________________________________________________"
             "_________________________________________________________________"
             "_________________________________________________________________"
             "_________________________________________________________________"
             "_________________________________________________________________"
             "_________________________________________________________________"
             "_________________________________________________________________"
             "_________________________________________________________________"
             "_________________________________________________________________"
            ".adequately_longer__test_surname"
            "@adequately_longer_domain.coma__________________________________");
   unitTest("adequately_longer_alias___________________________________________"
             "_________________________________________________________________"
             "_________________________________________________________________"
             "_________________________________________________________________"
             "_________________________________________________________________"
             "_________________________________________________________________"
             "_________________________________________________________________"
             "_________________________________________________________________"
             "_________________________________________________________________"
             "_________________________________________________________________"
             "_________________________________________________________________"
             "@adequately_longer_domain.com");
   unitTest("adequately_longer_name."
            "adequately_longer_surname_________________________________________"
            "__________________________________________________________________"
            "__________________________________________________________________"
            "__________________________________________________________________"
            "__________________________________________________________________"
            "__________________________________________________________________"
            "__________________________________________________________________"
            "__________________________________________________________________"
            "__________________________________________________________________"
            "__________________________________________________________________"
            "__________________________________________________________________"
            "@adequately_longer_domain.com");
   unitTest("jankowa.lski@intel.com");
   unitTest("jan.kowalski@intel.com");

   return 0;
}
