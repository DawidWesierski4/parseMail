#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdatomic.h>
#include "parseMail.h"

/* strcmp wrapper that returns 0 when both string are NULL */
bool
strcmpNullSafe(const char *stringOne, const char *stringTwo) {
   if (stringOne == NULL || stringTwo == NULL) {
      return stringOne != stringTwo;
   } else {
      return strcmp(stringOne, stringTwo);
   }
}

bool
unitTest(const char *inputMail, int expectedResult, const char *expectedName,
         const char *expectedSurName, const char *expectedDomain)
{
   credentials testedInOut;
   int aux;
   bool results[3];

   setParseMailWarnings(false); /* ignore warnings */
   aux = ParseMail(inputMail, &testedInOut);
   results[0] = !strcmpNullSafe(testedInOut.name, expectedName);
   results[1] = !strcmpNullSafe(testedInOut.surName, expectedSurName);
   results[2] = !strcmpNullSafe(testedInOut.domain, expectedDomain);

   if (aux == expectedResult && results[0] && results[1] && results[2]) {
      return 0;
   } else {
      printf("\nTest: %s Failed \n", inputMail);
      if (!results[0]) {
         printf("\nReturned wrong name\nExpected:\"%s\"\nActual\"%s\"\n",
                expectedName, testedInOut.name);
      }

      if (!results[1]) {
         printf("\nReturned wrong surname\nExpected:\"%s\"\nActual\"%s\"\n",
                expectedSurName, testedInOut.surName);
      }

      if (!results[2]) {
         printf("\nReturned wrong domain\nExpected:\"%s\"\nActual\"%s\"\n",
                expectedDomain, testedInOut.domain);
      }

      if (expectedResult != aux) {
         printf("\nReturned ErrCode is wrong \nExpected:\"%d\"\nActual\"%d\"\n",
                expectedResult, aux);
         printf("\nReturned error message:\n");
         setParseMailWarnings(true); /* turning on warnings */
         printErrorMsg(aux);
      }

      releaseCredentialsMemory(&testedInOut);
      return true;
   }
}

void
automaticTestsParseMail(void)
{
   int counter = 0;
   const char *input;

   input = "jan.kowalski@intel.com";
   counter += unitTest(input, SUCCESSFUL,
                       "jan", "kowalski", "@intel.com");

   input = "janusz.kow@intel.com";
   counter += unitTest(input, SUCCESSFUL,
                       "janusz", "kow", "@intel.com");

   input = "januszkow@intel.com";
   counter += unitTest(input, SUCCESSFUL,
                       "januszkow", NULL, "@intel.com");

   input = "kow@ina";
   counter += unitTest(input, SUCCESSFUL,
                       "kow", NULL, "@ina");

   input = "j.kowalski@intel.com";
   counter += unitTest(input, ERR_INPUT_FORMAT_SIZE_RULE_VIOLATION,
                       NULL, NULL, NULL);

   input = "ka@intel______________________________________________________com";
   counter += unitTest(input, ERR_INPUT_FORMAT_SIZE_RULE_VIOLATION, 
                       NULL, NULL, NULL);

   input = "janusz.kaskjdf;lasjkfdjalskdfjlaksjdflkajsfdlkajsfllaskdfjalsdk@in";
   counter += unitTest(input, ERR_INPUT_FORMAT_SIZE_RULE_VIOLATION,
                       NULL, NULL, NULL);

   input = "jasfasdfasdffadsdfa.@intel.com";
   counter += unitTest(input, ERR_INPUT_FORMAT_SIZE_RULE_VIOLATION,
                       NULL, NULL, NULL);

   input = ".kaasdfadfasdfsa@intel.com";
   counter += unitTest(input, ERR_INPUT_FORMAT_SIZE_RULE_VIOLATION, 
                       NULL, NULL, NULL);

   input = "janusz.kasdfasdfasfasdfsafasdfasdfasdfasdfasdfkjsahdfkjashfjkhkaj@";
   counter += unitTest(input, ERR_INPUT_FORMAT_SIZE_RULE_VIOLATION,
                       NULL, NULL, NULL);

   input = "janusz.kasdf.sdfasfasdfsafasdfasdfasdfasdfasdfkjsahdfkjashfjk@hkaj";
   counter += unitTest(input, ERR_INPUT_FORMAT_ILLEGAL_CHAR,
                       NULL, NULL, NULL);

   input = "janusz.kasdfasdfasfasdfsafasdfasdfasdfasdfasdfkjsahdfkjashfjk@@kaj";
   counter += unitTest(input, ERR_INPUT_FORMAT_ILLEGAL_CHAR,
                       NULL, NULL, NULL);

   input = "janusz_kasd___dfasfasdfsafasdfasdfasdfasdfasdfkjsahdfkjashfjk@hka ";
   counter += unitTest(input, ERR_INPUT_FORMAT_ILLEGAL_CHAR,
                       NULL, NULL, NULL);

   if (counter) {
      printf("\nNUMBER OF FAILED TESTS = %d \n\n",counter);
   } else {
      printf("\nAll tests ended with SUCCESS\n");
   }
}

int
main(void)
{
   automaticTestsParseMail();
   return 0;
}
