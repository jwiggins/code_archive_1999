#include <OS.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
  thread_info t_info_start, t_info_finish;
  thread_id thid = atoi(argv[1]);

  printf(
  "name                            user     kernel   %%\n"
  "---------------------------------------------------\n");
  while(1)
  {
    get_thread_info(thid, &t_info_start);
    snooze(1000 * 1000 * 5);
    get_thread_info(thid, &t_info_finish);

    printf("\r\r%31s %7Ld %7Ld %5.2f\n", t_info_finish.name,
    (t_info_finish.user_time - t_info_start.user_time),
    (t_info_finish.kernel_time - t_info_start.kernel_time),
    (((t_info_finish.user_time - t_info_start.user_time) +
    (t_info_finish.kernel_time - t_info_start.kernel_time))*100.
    /(float)(1000 * 1000 * 5)));
  }

  return 0;
}
