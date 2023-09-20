#include "tamtypes.h"
#include "sifrpc.h"
#include "loadfile.h"
#include "iopcontrol.h"
#include "sbv_patches.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <libmc.h>
#include <iopheap.h>
#include <stdio.h>

extern u8 usbd_irx[];
extern int size_usbd_irx;

extern u8 usbhdfsd_irx[];
extern int size_usbhdfsd_irx;

void loadModule(char *name, u8 *irx, int size) {
    if (size == 0) {
        printf("[ERROR] module is empty\n");

        return;
    }

    int id, res = 0;

    id = SifExecModuleBuffer(irx, size, 0, NULL, &res);
    if (id < 1) {
        printf("[ERROR] module %s exec error:%d\n", name, id);

        return;
    }
}

int PS2_Init() {
    SifInitRpc(0);
	while(!SifIopReset("", 0)){};
	while(!SifIopSync()){};
	SifInitRpc(0);

	SifLoadFileInit();
	SifInitIopHeap();

	sbv_patch_enable_lmb();
	sbv_patch_disable_prefix_check();

	// USB mass support
    loadModule("usbd", usbd_irx, size_usbd_irx);
    loadModule("usbhdfsd", usbhdfsd_irx, size_usbhdfsd_irx);

	int ret = 0;

	SifLoadModule("rom0:XSIO2MAN", 0, NULL);
  	if (ret < 0) {
	  return 0;
  	}

  	ret = SifLoadModule("rom0:XPADMAN", 0, NULL);
  	if (ret < 0) {
	  return 0;
  	}

  	ret = SifLoadModule("rom0:XMCMAN", 0, NULL);
  	if (ret < 0) {
	  return 0;
  	}
  	
	ret = SifLoadModule("rom0:XMCSERV", 0, NULL);
  	if (ret < 0) {
      printf("Failed to load module: XMCSERV");
	  return 0;
  	}

	if(mcInit(MC_TYPE_XMC) < 0) {
		printf("Failed to initialise memcard server!\n");
		return 0;
	}

	sleep(1);

    return 1;
}