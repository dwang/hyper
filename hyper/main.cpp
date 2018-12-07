#include "common_includes.hpp"

int main()
{
	if (!cs_static->initialize()) {
		printf("[!] game is not running!\n");
		return -1;
	}

	while (engine::IsRunning()) {
		if (engine::IsInGame()) {
		}
	}

	return 0;
}