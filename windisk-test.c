/*
 *  -----------------------------------------
 *  WinDisk Test 1.0-alpha
 *  -----------------------------------------
 *  Author: Adam Blaszczyk
 *          http://wyciekpamieci.blogspot.com
 *  Date:   2016-03-16
 *  -----------------------------------------
 *  
 *  Compilation (Windows):
 *          gcc windisk-test.c -o windisk-test
 *
 *  Usage:
 *          windisk-test.exe
 *
 */

#include <stdio.h>
#include <windows.h>
#include <time.h>

int main(void) {

  BYTE data[512];
  DWORD bytesRead;
  HANDLE device = NULL;
  int sector;
  
  ULARGE_INTEGER avail, total, free;
  unsigned long long total_bytes, total_sectors;
  BOOL success;
  
  char drv1[16];
  char str1[16] = ":\\";
  char drv2[16];
  char str2[16] = "\\\\.\\";
  char str3[16] = ":";
  
  long t0, t1, dt;
  
  printf("\nWINDISK TEST\n");
  printf("============\n\n");
  printf("Enter drive letter: ");
  scanf("%s", &drv1);
  strcpy(drv2, drv1);
  
  strcat(drv1, str1);    // str1 = "X:\\"
  
  strcat(str2, drv2);
  strcat(str2, str3);    // str2 = "\\\\.\\X:"
    
  success = GetDiskFreeSpaceEx(drv1, &avail, &total, &free);
  total_bytes = (unsigned long long)total.QuadPart;
  printf("\n%lld total bytes.\n", total_bytes);
  total_sectors = total_bytes / 512;
  printf("%lld total sectors.\n\n", total_sectors);
  
  printf("%c <  15 ms\n", 176);  // if CLOCKS_PER_SEC = 1000
  printf("%c < 200 ms\n", 177);
  printf("%c > 200 ms\n", 178);
  printf("X   bad sector (or other error)\n\n");
  
  printf("Press [ENTER] to continue...\n");
  fflush(stdin);
  getchar();

  device = CreateFile(str2,           // Drive to open
      GENERIC_READ,                   // Access mode
      FILE_SHARE_READ,                // Share Mode
      NULL,                           // Security Descriptor
      OPEN_EXISTING,                  // How to create
      0,                              // File attributes
      NULL);                          // Handle to template

  if(device != NULL) {
  
    for(sector = 0; sector < total_sectors; sector++) {
      SetFilePointer (device, sector * 512, NULL, FILE_BEGIN);

      t0 = clock();
      success = ReadFile(device, data, 512, &bytesRead, NULL);
      t1 = clock();
      dt = t1 - t0;

      if(!success) {
        printf("X");
      }
      else {
        if(dt <= 15)
          printf("%c", 176);
        if((dt > 15) && (dt <= 200))
          printf("%c", 177);
        if(dt > 200)
          printf("%c", 178);
      }
    }

    CloseHandle(device);
  }

  printf("\n\nPress [ENTER] to exit...\n");
  fflush(stdin);
  getchar();

  return 0;
}
