// ------------------------------------------------------------------------------------------------------------------------
// KeepAwake! An executable to keep your PC awake by simulating a key-press
// by: Jerome Vonk - October 2017
// Inspired by https://batchloaf.wordpress.com/2012/04/17/simulating-a-keystroke-in-win32-c-or-c-using-sendinput/
// ------------------------------------------------------------------------------------------------------------------------


#include <windows.h>
#include <stdio.h>
#include <chrono>

using namespace std;

// ---------------------------------------------------------------
// Main entry point for the executable
// ---------------------------------------------------------------
void hideConsole(void)
{
   // Hide console
   ShowWindow(GetConsoleWindow(), SW_HIDE);
   FreeConsole();
}

// ---------------------------------------------------------------
// Simulate that a key was pressed
// ---------------------------------------------------------------
void simulateKey(WORD wKeyCode)
{
   // Virtual key-codes can be found here: https://msdn.microsoft.com/en-us/library/windows/desktop/dd375731(v=vs.85).aspx

   // This structure is used to create the keyboard input event.
   INPUT ip;

   // Set up a generic keyboard event.
   ip.type           = INPUT_KEYBOARD;
   ip.ki.wScan       = 0; // hardware scan code for key
   ip.ki.time        = 0;
   ip.ki.dwExtraInfo = 0;

   // Press the key
   ip.ki.wVk = wKeyCode;
   ip.ki.dwFlags = 0; // 0 for key press
   SendInput(1, &ip, sizeof(INPUT));

   // Release the key
   ip.ki.dwFlags = KEYEVENTF_KEYUP; // use this event for key release
   SendInput(1, &ip, sizeof(INPUT));
}

// ---------------------------------------------------------------
// Keep awake for the amount of time desired
// ---------------------------------------------------------------
void keepAwakeFor(DWORD dwTime)
{
   auto start = chrono::system_clock::now();

   DWORD dwCounter = 1;

   std::chrono::duration<double> elapsed_seconds = chrono::system_clock::now() - start;

   while ( elapsed_seconds.count() <= dwTime*60 )
   {
      // Simulate a key every 59 seconds
      if ( 0 == dwCounter % 59 )
      {
         // Simulate a key-press
         simulateKey(VK_F13); // since F13 is not present on keyboards, this should not interfere

         // Reset the counter
         dwCounter = 0;
      }

      Sleep(1000);
      dwCounter++;

      elapsed_seconds = chrono::system_clock::now() - start;
   }

   return;
}


// ---------------------------------------------------------------
// Main entry point for the executable
// ---------------------------------------------------------------
int main(int argc, char* argv[])
{
   DWORD dwRunningTime = 60; // 1 hour, expressed in minutes

   if (argc > 1)
   {
      // If the user enters arguments, assume he knows what he's doing, just double-check and hide the console
      dwRunningTime = atol(argv[1]);

      if (dwRunningTime < 1)
      {
         dwRunningTime = 1 * 60;
      }
      else if (dwRunningTime > 24 * 60)
      {
         dwRunningTime = 1 * 60;
      }

      hideConsole();
   }
   else
   {
      printf("How long should we keep the PC awake? (in minutes): \n");
      scanf_s("%ld", &dwRunningTime);

      if (dwRunningTime < 1)
      {
         printf("Should not be less than a minute. Defaulting to 1h\n");
         dwRunningTime = 1 * 60;
      }
      else if (dwRunningTime > 24 * 60)
      {
         printf("Should not be more than 24h. Defaulting to 1h\n");
         dwRunningTime = 1 * 60;
      }
   }

   printf("KeepAwake! Will be active for %ld minute(s)\n", dwRunningTime );

   for (int i = 5; i > 0; i--)
   {
      printf("%d..", i);
      Sleep(1000);
   }

   printf("\nStarting now!\n");

   keepAwakeFor(dwRunningTime);
   
   return 0;
}

 
