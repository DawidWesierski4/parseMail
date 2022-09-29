#ifndef PARSE_MAIL_H
#define PARSE_MAIL_H

typedef struct credentialInformation {
   char *name;
   char *surName;
   char *domain;
} credentials;

enum ERR_NUMS {
   ERR_ALLOCATING_MEMORY = 1,
   ERR_NULL_POINTER,
   ERR_INPUT_FORMAT,
   ERR_INPUT_FORMAT_SIZE_RULE_VIOLATION,
   ERR_INPUT_FORMAT_ILLEGAL_CHAR
};

int
checkName(const char *name);

int
checkDomain(const char *domain);

int
ParseMail(const char *mail, void *out);

void
printCredentials(credentials person);

void
releaseCredentialsMemory(credentials *person);

void
automaticStatusTestParseMail(void);

#endif /* PARSE_MAIL_H */
