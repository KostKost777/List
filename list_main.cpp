#include <TXLib.h>
#include <errno.h>

#include "list_functions.h"
#include "dump_functions.h"
#include "set_get_functions.h"

int main()
{
    int out_index = 0;

    struct StructList list = {};

    atexit(CloseLogFile);

    if (OpenLogFile())
        goto exit_label;

    if (ListCtor(&list, 10))
        goto exit_label;

    INSERT_AFTER(&list, 0, 10, exit_label);

    INSERT_AFTER(&list, 1, 20, exit_label);

    INSERT_AFTER(&list, 2, 30, exit_label);

    INSERT_AFTER(&list, 3, 40, exit_label);

    INSERT_AFTER(&list, 4, 55, exit_label);

    INSERT_AFTER(&list, 5, 59, exit_label);

    INSERT_BEFORE(&list, 2, 15, exit_label);

    INSERT_BEFORE(&list, 2, 60, exit_label);

    INSERT_BEFORE(&list, 1, 5, exit_label);

    INSERT_BEFORE(&list, 1, 5, exit_label);

    INSERT_BEFORE(&list, 1, 5, exit_label);

    INSERT_BEFORE(&list, 1, 5, exit_label);

    UserPrintList(&list);

    exit_label:

        ListDtor(&list);

        if (out_index != -1) {
            printf("END SUCCESS\n");
            return 0;
        }

        printf("END WITH ERROR\n");
        return 1;
}
