#include "stdafx.h"

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved )
{
	if (reason == DLL_PROCESS_ATTACH) {
		//Memory::Init();
		//// Disable intro
		//Memory *m = Memory::Find_t("platform:/movies/rockstar_logos");
		//if (m != nullptr)
		//	m->put("./nonexistingfilenonexistingfil");
		//delete m;

		// Disable license term
		/*m = Memory::Find("72 1F E8 ? ? ? 00 8B 0D ? ? ? 01 FF C1 48");
		if (m != nullptr)
			m->nop(2);
		delete m;
*/
		//m = Memory::Find("48 85 C9 0F 84 ? 00 00 00 48 8D 55 A7 E8");
		//m += 13;
		//m->call(Memory::Return<int, 0>);
		//delete m;

		//m = Memory::Find("8D 4A 03 E8 ? ? ? ? E8 ? ? ? ? 84 C0 75 1E");
		//m += 2;
		//m->put<uint8_t>(8);
		//delete m;

		scriptRegister(hModule, InitGameScript);
		scriptRegisterAdditionalThread(hModule, Rendering);
		keyboardHandlerRegister(CChat::Get()->ScriptKeyboardMessage);
		keyboardHandlerRegister(OnKeyboardMessage);
	}
	if (reason == DLL_PROCESS_DETACH) {
		scriptUnregister(hModule);
	}
	return TRUE;
}

