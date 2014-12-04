/*
* http://man7.org/linux/man-pages/man3/pthread_setschedparam.3.html
*/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#if 0
#define handle_error_en(en, msg) \
	   do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)
#else
#define handle_error_en(en, msg) \
	do { printf("[err=%d][line = %d] %s\n", en, __LINE__, msg); exit(EXIT_FAILURE); } while (0)
#endif
static void
usage(char *prog_name, char *msg)
{
   if (msg != NULL)
	   fputs(msg, stderr);

   printf("Usage: %s [options]\n", prog_name);
   printf("Options are:\n");
#define fpe(msg) printf("\t%s", msg);          /* Shorter */
   fpe("-a<policy><prio> Set scheduling policy and priority in\n");
   fpe("                 thread attributes object\n");
   fpe("                 <policy> can be\n");
   fpe("                     f  SCHED_FIFO\n");
   fpe("                     r  SCHED_RR\n");
   fpe("                     o  SCHED_OTHER\n");
   fpe("-A               Use default thread attributes object\n");
   fpe("-i {e|s}         Set inherit scheduler attribute to\n");
   fpe("                 'explicit' or 'inherit'\n");
   fpe("-m<policy><prio> Set scheduling policy and priority on\n");
   fpe("                 main thread before pthread_create() call\n");
   exit(EXIT_FAILURE);
}

static int
get_policy(char p, int *policy)
{
   switch (p) {
   case 'f': *policy = SCHED_FIFO;     return 1;
   case 'r': *policy = SCHED_RR;       return 1;
   case 'o': *policy = SCHED_OTHER;    return 1;
   default:  return 0;
   }
}

static void
display_sched_attr(int policy, struct sched_param *param)
{
   printf("    policy=%s, priority=%d\n",
		   (policy == SCHED_FIFO)  ? "SCHED_FIFO" :
		   (policy == SCHED_RR)    ? "SCHED_RR" :
		   (policy == SCHED_OTHER) ? "SCHED_OTHER" :
		   "???",
		   param->sched_priority);
}

static void
display_thread_sched_attr(char *msg)
{
   int policy, s;
   struct sched_param param;

   s = pthread_getschedparam(pthread_self(), &policy, &param);
   if (s != 0)
	   handle_error_en(s, "pthread_getschedparam");

   printf("%s\n", msg);
   display_sched_attr(policy, &param);
}

static void *
thread_start(void *arg)
{
   printf("[thread_start] is Running\n");
   display_thread_sched_attr("Scheduler attributes of new thread");

   return NULL;
}

/////////////////////// thread init start
static void
display_pthread_attr(pthread_attr_t *attr, char *prefix)
{
   int s, i;
   size_t v;
   void *stkaddr;
   struct sched_param sp;

   s = pthread_attr_getdetachstate(attr, &i);
   if (s != 0)
	   handle_error_en(s, "pthread_attr_getdetachstate");
   printf("%sDetach state        = %s\n", prefix,
		   (i == PTHREAD_CREATE_DETACHED) ? "PTHREAD_CREATE_DETACHED" :
		   (i == PTHREAD_CREATE_JOINABLE) ? "PTHREAD_CREATE_JOINABLE" :
		   "???");

   s = pthread_attr_getscope(attr, &i);
   if (s != 0)
	   handle_error_en(s, "pthread_attr_getscope");
   printf("%sScope               = %s\n", prefix,
		   (i == PTHREAD_SCOPE_SYSTEM)  ? "PTHREAD_SCOPE_SYSTEM" :
		   (i == PTHREAD_SCOPE_PROCESS) ? "PTHREAD_SCOPE_PROCESS" :
		   "???");

   s = pthread_attr_getinheritsched(attr, &i);
   if (s != 0)
	   handle_error_en(s, "pthread_attr_getinheritsched");
   printf("%sInherit scheduler   = %s\n", prefix,
		   (i == PTHREAD_INHERIT_SCHED)  ? "PTHREAD_INHERIT_SCHED" :
		   (i == PTHREAD_EXPLICIT_SCHED) ? "PTHREAD_EXPLICIT_SCHED" :
		   "???");

   s = pthread_attr_getschedpolicy(attr, &i);
   if (s != 0)
	   handle_error_en(s, "pthread_attr_getschedpolicy");
   printf("%sScheduling policy   = %s\n", prefix,
		   (i == SCHED_OTHER) ? "SCHED_OTHER" :
		   (i == SCHED_FIFO)  ? "SCHED_FIFO" :
		   (i == SCHED_RR)    ? "SCHED_RR" :
		   "???");

   s = pthread_attr_getschedparam(attr, &sp);
   if (s != 0)
	   handle_error_en(s, "pthread_attr_getschedparam");
   printf("%sScheduling priority = %d\n", prefix, sp.sched_priority);

   s = pthread_attr_getguardsize(attr, &v);
   if (s != 0)
	   handle_error_en(s, "pthread_attr_getguardsize");
   printf("%sGuard size          = %d bytes\n", prefix, v);

   s = pthread_attr_getstack(attr, &stkaddr, &v);
   if (s != 0)
	   handle_error_en(s, "pthread_attr_getstack");
   printf("%sStack address       = %p\n", prefix, stkaddr);
   printf("%sStack size          = 0x%x bytes\n", prefix, v);
}

static void *
thread_start_init(void *arg)
{
   int s;
   pthread_attr_t gattr;

   /* pthread_getattr_np() is a non-standard GNU extension that
	  retrieves the attributes of the thread specified in its
	  first argument */

   s = pthread_getattr_np(pthread_self(), &gattr);
   if (s != 0)
	   handle_error_en(s, "pthread_getattr_np");

   printf("Thread attributes:\n");
   display_pthread_attr(&gattr, "\t");

   exit(EXIT_SUCCESS);         /* Terminate all threads */
}

int test_thread_init(int argc, char *argv[])
{
   pthread_t thr;
   pthread_attr_t attr;
   pthread_attr_t *attrp;      /* NULL or &attr */
   int s;

   attrp = NULL;

   /* If a command-line argument was supplied, use it to set the
	  stack-size attribute and set a few other thread attributes,
	  and set attrp pointing to thread attributes object */

   if (argc > 1) {
	   int stack_size;
	   void *sp = NULL;

	   attrp = &attr;

	   s = pthread_attr_init(&attr);
	   if (s != 0)
		   handle_error_en(s, "pthread_attr_init");

	   s = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	   if (s != 0)
		   handle_error_en(s, "pthread_attr_setdetachstate");

	   s = pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
	   if (s != 0)
		   handle_error_en(s, "pthread_attr_setinheritsched");

	   stack_size = strtoul(argv[1], NULL, 0);

	   s = posix_memalign(&sp, sysconf(_SC_PAGESIZE), stack_size);
	   if (s != 0)
		   handle_error_en(s, "posix_memalign");

	   printf("posix_memalign() allocated at %p\n", sp);

	   printf("Set stack_size: %d\n", stack_size);
	   if (sp == NULL){
			printf("stack_address : no set\n");
	   }else {
			printf("stack_address : %p\n", sp);
	   }
	   
	   s = pthread_attr_setstack(&attr, sp, stack_size);		   
	   if (s != 0)
		   handle_error_en(s, "pthread_attr_setstack");
   }

   s = pthread_create(&thr, attrp, &thread_start_init, NULL);
   if (s != 0)
	   handle_error_en(s, "pthread_create");

   if (attrp != NULL) {
	   s = pthread_attr_destroy(attrp);
	   if (s != 0)
		   handle_error_en(s, "pthread_attr_destroy");
   }

   pause();    /* Terminates when other thread calls exit() */
}
/////////////////////// thread init end

int test_thread_policy(int argc, char *argv[])
{
   int s, opt, inheritsched, use_null_attrib, policy;
   pthread_t thread;
   pthread_attr_t attr;
   pthread_attr_t *attrp;
   char *attr_sched_str, *main_sched_str, *inheritsched_str;
   struct sched_param param;

   /* Process command-line options */

   use_null_attrib = 0;
   attr_sched_str = NULL;	
   main_sched_str = NULL;
   inheritsched_str = NULL;

   while ((opt = getopt(argc, argv, "a:Ai:m:")) != -1) {
	   switch (opt) {
	   case 'a': attr_sched_str = optarg;      break;
	   case 'A': use_null_attrib = 1;          break;
	   case 'i': inheritsched_str = optarg;    break;
	   case 'm': main_sched_str = optarg;      break;
	   default:  usage(argv[0], "Unrecognized option\n");
	   }
   }
   if (use_null_attrib &&
		   (inheritsched_str != NULL || attr_sched_str != NULL))
	   usage(argv[0], "Can't specify -A with -i or -a\n");

   /* Optionally set scheduling attributes of main thread,
	  and display the attributes */

   if (main_sched_str != NULL) {
	   if (!get_policy(main_sched_str[0], &policy))
		   usage(argv[0], "Bad policy for main thread (-s)\n");
	   param.sched_priority = strtol(&main_sched_str[1], NULL, 0);

	   s = pthread_setschedparam(pthread_self(), policy, &param);
	   if (s != 0)
		   handle_error_en(s, "pthread_setschedparam");
   }

   display_thread_sched_attr("Scheduler settings of main thread");
   printf("\n");

   /* Initialize thread attributes object according to options */

   attrp = NULL;

   if (!use_null_attrib) {
	   s = pthread_attr_init(&attr);
	   if (s != 0)
		   handle_error_en(s, "pthread_attr_init");
	   attrp = &attr;
   }

   if (inheritsched_str != NULL) {
	   if (inheritsched_str[0] == 'e')
		   inheritsched = PTHREAD_EXPLICIT_SCHED;
	   else if (inheritsched_str[0] == 'i')
		   inheritsched = PTHREAD_INHERIT_SCHED;
	   else
		   usage(argv[0], "Value for -i must be 'e' or 'i'\n");

	   s = pthread_attr_setinheritsched(&attr, inheritsched);
	   if (s != 0)
		   handle_error_en(s, "pthread_attr_setinheritsched");
   }

   if (attr_sched_str != NULL) {
	   if (!get_policy(attr_sched_str[0], &policy))
		   usage(argv[0],
				   "Bad policy for 'attr' (-a)\n");
	   param.sched_priority = strtol(&attr_sched_str[1], NULL, 0);

	   s = pthread_attr_setschedpolicy(&attr, policy);
	   if (s != 0)
		   handle_error_en(s, "pthread_attr_setschedpolicy");
	   s = pthread_attr_setschedparam(&attr, &param);
	   if (s != 0)
		   handle_error_en(s, "pthread_attr_setschedparam");
   }

   /* If we initialized a thread attributes object, display
	  the scheduling attributes that were set in the object */

   if (attrp != NULL) {
	   s = pthread_attr_getschedparam(&attr, &param);
	   if (s != 0)
		   handle_error_en(s, "pthread_attr_getschedparam");
	   s = pthread_attr_getschedpolicy(&attr, &policy);
	   if (s != 0)
		   handle_error_en(s, "pthread_attr_getschedpolicy");

	   printf("Scheduler settings in 'attr'\n");
	   display_sched_attr(policy, &param);

	   s = pthread_attr_getinheritsched(&attr, &inheritsched);
	   printf("    inheritsched is %s\n",
			   (inheritsched == PTHREAD_INHERIT_SCHED)  ? "INHERIT" :
			   (inheritsched == PTHREAD_EXPLICIT_SCHED) ? "EXPLICIT" :
			   "???");
	   printf("\n");
   }

   /* Create a thread that will display its scheduling attributes */

   s = pthread_create(&thread, attrp, &thread_start, NULL);
   if (s != 0)
	   handle_error_en(s, "pthread_create");

   /* Destroy unneeded thread attributes object */

   s = pthread_attr_destroy(&attr);
   if (s != 0)
	   handle_error_en(s, "pthread_attr_destroy");

   s = pthread_join(thread, NULL);
   if (s != 0)
	   handle_error_en(s, "pthread_join");

    pause();//exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[])
{
	test_thread_init(argc, argv);
	test_thread_policy(argc, argv);
	 pause();
	return 0;
}
