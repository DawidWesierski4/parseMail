#ifndef PARSE_MAIL_H
#define PARSE_MAIL_H

#include <stdbool.h>

typedef struct credentialInformation {
   char *name;
   char *surName;
   char *domain;
} credentials;

enum ERR_NUMS {
   ERR_SUCCESSFUL,
   ERR_ALLOCATING_MEMORY,
   ERR_NULL_POINTER,
   ERR_INPUT_FORMAT,
   ERR_INPUT_FORMAT_SIZE_RULE_VIOLATION,
   ERR_INPUT_FORMAT_ILLEGAL_CHAR,
};

extern bool parseMailWarningEnabled;

void
printErrorMsg(int errorEnum);

int
parseName(const char **mail, const char *charPtr, char **outPtr,
          int (*checkFunction)(const char*));

int
ParseMail(const char *mail, void *out);

int
checkString(const char *testString, const char *illegalChars, int minSize);

int
checkName(const char *testString);

int
checkDomain(const char *testString);

void
printCredentials(credentials person);

void
releaseCredentialsMemory(credentials *person);

#endif /* PARSE_MAIL_H */
