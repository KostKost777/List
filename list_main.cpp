#include <TXLib.h>
#include <errno.h>

#include "list_functions.h"
#include "dump_functions.h"
#include "set_get_functions.h"

int main()
{
    int ret_value = 0;

    struct StructList list = {};

    atexit(CloseLogFile);

    if (OpenLogFile())
        goto exit_label;

    if (ListCtor(&list, 5))
        goto exit_label;

    INSERT_AFTER(&list, 0, 200, exit_label);

    INSERT_AFTER(&list, 1, 100, exit_label);

    INSERT_AFTER(&list, 2, 500, exit_label);

    INSERT_AFTER(&list, 3, 400, exit_label);

    INSERT_AFTER(&list, 2, 399, exit_label);

    INSERT_AFTER(&list, 0, 12, exit_label);

    INSERT_AFTER(&list, 5, 1, exit_label);

    INSERT_AFTER(&list, 4, 230, exit_label);

    INSERT_AFTER(&list, 6, 888, exit_label);

    INSERT_AFTER(&list, 7, 991, exit_label);

    UserPrintList(&list);

    DELETE_ELEMENT(&list, 3, exit_label);
    DELETE_ELEMENT(&list, 4, exit_label);
    DELETE_ELEMENT(&list, 1, exit_label);

    DownwardReallocate(&list);

    //SortList(&list);

    UserPrintList(&list);

    ListDump(&list, __LINE__, __func__, __FILE__);

    exit_label:

        ListDtor(&list);

        if (ret_value != -1) {
            printf("END SUCCESS\n");
            return 0;
        }

        printf("END WITH ERROR\n");
        return 1;
}
