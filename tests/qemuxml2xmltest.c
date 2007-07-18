#include <stdio.h>
#include <string.h>

#include <sys/types.h>
#include <fcntl.h>

#include "testutils.h"
#include "qemu_conf.h"
#include "internal.h"

static char *progname;
struct qemud_driver driver;

#define MAX_FILE 4096


static int testCompareXMLToXMLFiles(const char *xml) {
    char xmlData[MAX_FILE];
    char *xmlPtr = &(xmlData[0]);
    char *actual = NULL;
    int ret = -1;
    struct qemud_vm_def *vmdef;
    struct qemud_vm vm;

    if (virtTestLoadFile(xml, &xmlPtr, MAX_FILE) < 0)
        goto fail;

    if (!(vmdef = qemudParseVMDef(NULL, &driver, xmlData, "test")))
        goto fail;

    vm.def = vmdef;
    vm.pid = -1;
    vm.id = -1;
    vmdef->vncActivePort = vmdef->vncPort;

    if (!(actual = qemudGenerateXML(NULL, &driver, &vm, vmdef, 0)))
        goto fail;

    if (strcmp(xmlData, actual)) {
        if (getenv("DEBUG_TESTS")) {
            printf("Expect %4d '%s'\n", (int)strlen(xmlData), xmlData);
            printf("Actual %4d '%s'\n", (int)strlen(actual), actual);
        }
        goto fail;
    }

    ret = 0;

 fail:
    free(actual);

    return ret;
}

static int testCompareXMLToXMLHelper(const void *data) {
    char xml[PATH_MAX];
    snprintf(xml, PATH_MAX, "qemuxml2argvdata/qemuxml2argv-%s.xml", (const char*)data);
    return testCompareXMLToXMLFiles(xml);
}


int
main(int argc, char **argv)
{
    int ret = 0;

    progname = argv[0];

    if (argc > 1) {
        fprintf(stderr, "Usage: %s\n", progname);
        exit(EXIT_FAILURE);
    }

    driver.qemuVersion = 0 * 1000 * 100 + (8 * 1000) + 1;
    driver.qemuCmdFlags = QEMUD_CMD_FLAG_VNC_COLON | 
        QEMUD_CMD_FLAG_NO_REBOOT;

    if (virtTestRun("QEMU XML-2-ARGV minimal",
                    1, testCompareXMLToXMLHelper, "minimal") < 0)
        ret = -1;

    if (virtTestRun("QEMU XML-2-ARGV Boot CDROM",
                    1, testCompareXMLToXMLHelper, "boot-cdrom") < 0)
        ret = -1;

    if (virtTestRun("QEMU XML-2-ARGV Boot Network",
                    1, testCompareXMLToXMLHelper, "boot-network") < 0)
        ret = -1;

    if (virtTestRun("QEMU XML-2-ARGV Boot Floppy",
                    1, testCompareXMLToXMLHelper, "boot-floppy") < 0)
        ret = -1;

    if (virtTestRun("QEMU XML-2-ARGV Clock UTC",
                    1, testCompareXMLToXMLHelper, "clock-utc") < 0)
        ret = -1;

    if (virtTestRun("QEMU XML-2-ARGV Clock Localtime",
                    1, testCompareXMLToXMLHelper, "clock-localtime") < 0)
        ret = -1;

    if (virtTestRun("QEMU XML-2-ARGV Disk CDROM",
                    1, testCompareXMLToXMLHelper, "disk-cdrom") < 0)
        ret = -1;

    if (virtTestRun("QEMU XML-2-ARGV Disk Floppy",
                    1, testCompareXMLToXMLHelper, "disk-floppy") < 0)
        ret = -1;

    if (virtTestRun("QEMU XML-2-ARGV Disk Many",
                    1, testCompareXMLToXMLHelper, "disk-many") < 0)
        ret = -1;

    if (0 && virtTestRun("QEMU XML-2-ARGV Graphics VNC",
                    1, testCompareXMLToXMLHelper, "graphics-vnc") < 0)
        ret = -1;

    if (virtTestRun("QEMU XML-2-ARGV Graphics SDL",
                    1, testCompareXMLToXMLHelper, "graphics-sdl") < 0)
        ret = -1;

    if (virtTestRun("QEMU XML-2-ARGV Input USB Mouse",
                    1, testCompareXMLToXMLHelper, "input-usbmouse") < 0)
        ret = -1;

    if (virtTestRun("QEMU XML-2-ARGV Input USB Tablet",
                    1, testCompareXMLToXMLHelper, "input-usbtablet") < 0)
        ret = -1;

    if (virtTestRun("QEMU XML-2-ARGV Misc ACPI",
                    1, testCompareXMLToXMLHelper, "misc-acpi") < 0)
        ret = -1;

    if (virtTestRun("QEMU XML-2-ARGV Misc No Reboot",
                    1, testCompareXMLToXMLHelper, "misc-no-reboot") < 0)
        ret = -1;

    if (virtTestRun("QEMU XML-2-ARGV Net User",
                    1, testCompareXMLToXMLHelper, "net-user") < 0)
        ret = -1;


    exit(ret==0 ? EXIT_SUCCESS : EXIT_FAILURE);
}

/*
 * Local variables:
 *  indent-tabs-mode: nil
 *  c-indent-level: 4
 *  c-basic-offset: 4
 *  tab-width: 4
 * End:
 */
