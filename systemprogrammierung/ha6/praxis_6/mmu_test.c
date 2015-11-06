#include <stdint.h>
#include <stdio.h>
#include "mmu.h"

//here you can insert your own tests
int main(){
	init_page_table();

	protect_system();

	switch_mode(USER);

  // Access every address range as User
  printf("Mode: User; Trying to access Kernel RAM\n");
	printf("%d\n" ,(int)memory_access((int16_t)(4*16), WRITE));

  printf("Mode: User; Trying to access shared RAM\n");
	printf("%d\n" ,(int)memory_access((int16_t)(16000), WRITE));

  printf("Mode: User; Trying to access free address\n");
	printf("%d\n" ,(int)memory_access((int16_t)(17000), WRITE));

  printf("Mode: User; Trying to access Flash Storage\n");
	printf("%d\n" ,(int)memory_access((int16_t)(18000), WRITE));

  printf("Mode: User; Trying to access IO adress\n");
	printf("%d\n" ,(int)memory_access((int16_t)(30722), READ));

  printf("Mode: User; Trying to access adress on IO page after IO adress\n");
	printf("%d\n" ,(int)memory_access((int16_t)(30725), READ));

  // access every address range as Kernel
  switch_mode(KERNEL);
  printf("Mode: Kernel; Trying to access Kernel RAM\n");
	printf("%d\n" ,(int)memory_access((int16_t)(4*16), WRITE));

  printf("Mode: Kernel; Trying to access shared RAM\n");
	printf("%d\n" ,(int)memory_access((int16_t)(16000), WRITE));

  printf("Mode: Kernel; Trying to access free address\n");
	printf("%d\n" ,(int)memory_access((int16_t)(17000), WRITE));

  printf("Mode: Kernel; Trying to access Flash Storage\n");
	printf("%d\n" ,(int)memory_access((int16_t)(18000), WRITE));

  printf("Mode: Kernel; Trying to access IO adress\n");
	printf("%d\n" ,(int)memory_access((int16_t)(30722), READ));

  printf("Mode: Kernel; Trying to access adress on IO page after IO adress\n");
	printf("%d\n" ,(int)memory_access((int16_t)(30725), READ));

  // try all types of access on one (existing) address
  switch_mode(KERNEL);
  printf("Mode: Kernel; Trying to access Flash Storage (write)\n");
	printf("%d\n" ,(int)memory_access((int16_t)(18400), WRITE));
  printf("Mode: Kernel; Trying to access Flash Storage (read)\n");
	printf("%d\n" ,(int)memory_access((int16_t)(18400), READ));

  switch_mode(USER);
  printf("Mode: User; Trying to access Flash Storage (write)\n");
	printf("%d\n" ,(int)memory_access((int16_t)(18400), WRITE));
  printf("Mode: User; Trying to access Flash Storage (read)\n");
	printf("%d\n" ,(int)memory_access((int16_t)(18400), READ));

  // AGE_KERNEL_ONLY IM USER MODUS: SOLLTE KEINEN EFFEKT HABEN
  // change rights: Kernel only
  printf("Set rights to kernel only\n");
  page_kernel_only(18400 / 16);

  // try all types of access on one (existing) address
  switch_mode(KERNEL);

  // WIEDER PAGE_KERNEL_ONLY AUFRUFEN: JETZT IM KERNEL MODUS
  // change rights: Kernel only
  printf("Set rights to kernel only\n");
  page_kernel_only(18400 / 16);

  printf("Mode: Kernel; Trying to access Flash Storage (write)\n");
	printf("%d\n" ,(int)memory_access((int16_t)(18400), WRITE));
  printf("Mode: Kernel; Trying to access Flash Storage (read)\n");
	printf("%d\n" ,(int)memory_access((int16_t)(18400), READ));

  switch_mode(USER);
  printf("Mode: User; Trying to access Flash Storage (write)\n");
	printf("%d\n" ,(int)memory_access((int16_t)(18400), WRITE));
  printf("Mode: User; Trying to access Flash Storage (read)\n");
	printf("%d\n" ,(int)memory_access((int16_t)(18400), READ));

  // HAT KEINEN EFFEKT
  // change rights: User access allowed
  printf("Set rights to user access allowed\n");
  open_page_for_user(18400 / 16);

  // try all types of access on one (existing) address
  switch_mode(KERNEL);

  // CHANGE RIGHTS IM KERNEL MODUS
  // change rights: User access allowed
  printf("Set rights to user access allowed\n");
  open_page_for_user(18400 / 16);

  printf("Mode: Kernel; Trying to access Flash Storage (write)\n");
	printf("%d\n" ,(int)memory_access((int16_t)(18400), WRITE));
  printf("Mode: Kernel; Trying to access Flash Storage (read)\n");
	printf("%d\n" ,(int)memory_access((int16_t)(18400), READ));

  switch_mode(USER);
  printf("Mode: User; Trying to access Flash Storage (write)\n");
	printf("%d\n" ,(int)memory_access((int16_t)(18400), WRITE));
  printf("Mode: User; Trying to access Flash Storage (read)\n");
	printf("%d\n" ,(int)memory_access((int16_t)(18400), READ));

  // HAT KEINEN EFFEKT
  // change rights: read only
  printf("Set rights to read only\n");
  page_read_only(18400 / 16);

  // try all types of access on one (existing) address
  switch_mode(KERNEL);

   // CHANGE RIGHTS IM KERNEL MODUS
  // change rights: read only
  printf("Set rights to read only\n");
  page_read_only(18400 / 16);

  printf("Mode: Kernel; Trying to access Flash Storage (write)\n");
  printf("%d\n" ,(int)memory_access((int16_t)(18400), WRITE));
  printf("Mode: Kernel; Trying to access Flash Storage (read)\n");
	printf("%d\n" ,(int)memory_access((int16_t)(18400), READ));

  switch_mode(USER);
  printf("Mode: User; Trying to access Flash Storage (write)\n");
	printf("%d\n" ,(int)memory_access((int16_t)(18400), WRITE));
  printf("Mode: User; Trying to access Flash Storage (read)\n");
	printf("%d\n" ,(int)memory_access((int16_t)(18400), READ));

  // HAT KEINEN EFFEKT
  // change rights: read write
  printf("Set rights to read write\n");
  page_read_write(18400 / 16);

  // try all types of access on one (existing) address
  switch_mode(KERNEL);

   // CHANGE RIGHTS IM KERNEL MODUS
  // change rights: read write
  printf("Set rights to read write\n");
  page_read_write(18400 / 16);

  printf("Mode: Kernel; Trying to access Flash Storage (write)\n");
	printf("%d\n" ,(int)memory_access((int16_t)(18400), WRITE));
  printf("Mode: Kernel; Trying to access Flash Storage (read)\n");
	printf("%d\n" ,(int)memory_access((int16_t)(18400), READ));

  switch_mode(USER);
  printf("Mode: User; Trying to access Flash Storage (write)\n");
	printf("%d\n" ,(int)memory_access((int16_t)(18400), WRITE));
  printf("Mode: User; Trying to access Flash Storage (read)\n");
	printf("%d\n" ,(int)memory_access((int16_t)(18400), READ));

  // change page: no longer available
  printf("Set page to unavailable\n");
  close_page(18400 / 16);

  // try all types of access on one (existing) address
  switch_mode(KERNEL);

  // change page: no longer available
  printf("Set page to unavailable\n");
  close_page(18400 / 16);

  printf("Mode: Kernel; Trying to access Flash Storage (write)\n");
	printf("%d\n" ,(int)memory_access((int16_t)(18400), WRITE));
  printf("Mode: Kernel; Trying to access Flash Storage (read)\n");
	printf("%d\n" ,(int)memory_access((int16_t)(18400), READ));

  switch_mode(USER);
  printf("Mode: User; Trying to access Flash Storage (write)\n");
	printf("%d\n" ,(int)memory_access((int16_t)(18400), WRITE));
  printf("Mode: User; Trying to access Flash Storage (read)\n");
	printf("%d\n" ,(int)memory_access((int16_t)(18400), READ));

  // change page: available again
  printf("Set page to available\n");
  open_page(18400 / 16);

  // try all types of access on one (existing) address
  switch_mode(KERNEL);

  // change page: available again
  printf("Set page to available\n");
  open_page(18400 / 16);

  printf("Mode: Kernel; Trying to access Flash Storage (write)\n");
	printf("%d\n" ,(int)memory_access((int16_t)(18400), WRITE));
  printf("Mode: Kernel; Trying to access Flash Storage (read)\n");
	printf("%d\n" ,(int)memory_access((int16_t)(18400), READ));

  switch_mode(USER);
  printf("Mode: User; Trying to access Flash Storage (write)\n");
	printf("%d\n" ,(int)memory_access((int16_t)(18400), WRITE));
  printf("Mode: User; Trying to access Flash Storage (read)\n");
	printf("%d\n" ,(int)memory_access((int16_t)(18400), READ));

  // change page: remap physical address
  printf("Remap physical address\n");
  remap(18400 / 16, 16100 / 16);

  // try all types of access on one (existing) address
  switch_mode(KERNEL);

  // change page: remap physical address
  printf("Remap physical address\n");
  remap(18400 / 16, 16100 / 16);

  printf("Mode: Kernel; Trying to access Flash Storage (write)\n");
	printf("%d\n" ,(int)memory_access((int16_t)(18400), WRITE));
  printf("Mode: Kernel; Trying to access Flash Storage (read)\n");
	printf("%d\n" ,(int)memory_access((int16_t)(18400), READ));

  switch_mode(USER);
  printf("Mode: User; Trying to access Flash Storage (write)\n");
	printf("%d\n" ,(int)memory_access((int16_t)(18400), WRITE));
  printf("Mode: User; Trying to access Flash Storage (read)\n");
	printf("%d\n" ,(int)memory_access((int16_t)(18400), READ));



	return 0;

}
