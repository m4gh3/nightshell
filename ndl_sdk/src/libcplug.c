#define ARM9
#include <nds.h>
#include <regex.h>

char __cpsr_mask;
VoidFn	__irq_vector[1];
char __itcm_start;
int getentropy (void *, size_t)
{ return 0; }
char __dtcm_start;
int     regcomp(regex_t *__restrict, const char *__restrict, int)
{ return 0; }
int regexec(const regex_t *__restrict, const char *__restrict, size_t, regmatch_t [__restrict], int)
{ return 0; }
vuint32 __irq_flags[1];
void	regfree(regex_t *)
{}
int sigprocmask (int, const sigset_t *, sigset_t *)
{ return 0; }
