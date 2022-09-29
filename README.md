# Parse Mail
## Description
The parseMail library purpose is to brake down email address into a credentials\
which contains Name, Surname and Domain parts separated into the 3 strings \
stored in the HEAP. Program allows the user to not specify the surname should\
that be the case (alias@domain.com f.e.) the surname pointer will have NULL\
value after successful parseMail function execution. The main purpose is \
to test the parseMail funcion (it contains both automatic and unit tests)

## Input
(const char *mail) (void *out) \
Function requires 2 arguments const pointer to string with the email address \
and void pointer to the credentials struct where the data will be saved called \
out.

## Output
(int) \
Function outputs int value: 0 if the execution was successful otherwise it will\
output error code from ERR_NUMS enumerator.

### Memory leaks risk
Function allocates the memory to the 3 pointers so the address that these\
pointers were pointing to will be lost after successful execution

### Value Checks
Function checks the mail string for illegal characters also it have size \
restrictions

## Building
Use Gcc f.e. on Linux: \
gcc -Wall -Werror -Wextra main.c parseMail.c -o parseMail

## Execution
As this program tests this function the correct way of testing will be: \
./ParseMail 2>/dev/Null
