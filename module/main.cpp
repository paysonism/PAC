#include <Windows.h>

#include "module.h"

DWORD WINAPI Init(HINSTANCE hinstDLL) {
  module::run(hinstDLL);
  return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call,
                      LPVOID lpReserved) {
  switch (ul_reason_for_call) {
  case DLL_PROCESS_ATTACH: {
    DisableThreadLibraryCalls(hModule);

    const auto thread =
        CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(Init),
                     hModule, 0, nullptr);

    if (thread)
      CloseHandle(thread);

    break;
  }

  case DLL_THREAD_ATTACH:
  case DLL_THREAD_DETACH:
  case DLL_PROCESS_DETACH:
    
  {
    LOG_INFO("process closing!");
    break;
  }
  }
  return TRUE;
}
