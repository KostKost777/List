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

    if (ListCtor(&list, 5))
        goto exit_label;

    INSERT_AFTER(&list, 0, 100, exit_label);

    INSERT_AFTER(&list, 1, 200, exit_label);

    INSERT_AFTER(&list, 2, 300, exit_label);

    INSERT_AFTER(&list, 3, 400, exit_label);

    INSERT_AFTER(&list, 4, 500, exit_label);

    INSERT_AFTER(&list, 5, 600, exit_label);

    INSERT_AFTER(&list, 6, 700, exit_label);

    INSERT_BEFORE(&list, 7, 750, exit_label);

    DELETE_ELEMENT(&list, 3, exit_label);

    INSERT_AFTER(&list, 8, 900, exit_label);

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
