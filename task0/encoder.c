#include "util.h"

#define SYS_EXIT  1
#define SYS_READ  3
#define SYS_WRITE 4
#define SYS_OPEN  5
#define SYS_CLOSE 6

#define STDIN     0
#define STDOUT    1
#define STDERR    2

unsigned char password[] = "123";
int debug = 1;
int infile;
int outfile;
int sign = 1;
int key_index = 0;
char* key = "A";

int my_strcmp(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(unsigned char *)s1 - *(unsigned char *)s2;
}

int encode(int c){
    if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')){
       int shift = (key[key_index] - 'A') * sign;

       if (c >= 'A' && c <= 'Z'){
         c += shift;
         if (c > 'Z') c -= 26;
         else if (c < 'A') c += 26;
       }
       else{
         c += shift;
         if (c > 'z') c -= 26;
         else if (c < 'a') c += 26;
       }

      key_index++;
      if (key[key_index] == '\0')
         key_index = 0;
   }
   return c;
}

extern int system_call();

int main(int argc, char **argv){
  int i;
  char c_buf;
  infile = STDIN;
  outfile = STDOUT;

  for (i = 0; i < argc; i++){
     if (debug){
        system_call(SYS_WRITE, STDERR, argv[i], strlen(argv[i]));
        system_call(SYS_WRITE, STDERR, "\n", 1);
     }

     if (argv[i][0] == '-' && argv[i][1] == 'D'){
        debug = 0;
     }
     else if (argv[i][0] == '+' && argv[i][1] == 'D'){
         if (my_strcmp(argv[i] + 2, (char*)password) == 0){
            debug = 1;
         }
     }
     else if (argv[i][0] == '+' && argv[i][1] == 'V'){
          sign = 1;
          key = argv[i] + 2;
     }
     else if (argv[i][0] == '-' && argv[i][1] == 'V'){
          sign = -1;
          key = argv[i] + 2;
     }
     else if (argv[i][0] == '-' && argv[i][1] == 'i'){
          infile = system_call(SYS_OPEN, argv[i] + 2, 0, 0);
          if (infile < 0){
             system_call(SYS_WRITE, STDERR, "cannot open file\n", 17);
             system_call(SYS_EXIT, 0x55, 0, 0);
          }
     }
     else if (argv[i][0] == '-' && argv[i][1] == 'o'){
          outfile = system_call(SYS_OPEN, argv[i] + 2, 577, 420);
          if (outfile < 0){
             system_call(SYS_WRITE, STDERR, "cannot open file\n", 17);
             system_call(SYS_EXIT, 0x55, 0, 0);
          }
     }
  }

  while (system_call(SYS_READ, infile, &c_buf, 1) > 0) {
      char encoded_c = encode((int)c_buf);
      system_call(SYS_WRITE, outfile, &encoded_c, 1);
  }

  if (infile != STDIN)
     system_call(SYS_CLOSE, infile);
  if (outfile != STDOUT)
     system_call(SYS_CLOSE, outfile);

  return 0;
}