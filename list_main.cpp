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

    if (ListCtor(&list, 11))
        goto exit_label;

    INSERT_AFTER(&list, 0, 100, exit_label);

    INSERT_AFTER(&list, 1, 200, exit_label);

    INSERT_AFTER(&list, 2, 300, exit_label);

    INSERT_AFTER(&list, 2, 250, exit_label);

    INSERT_AFTER(&list, 1, 150, exit_label);

    INSERT_AFTER(&list, 5, 400, exit_label);

    INSERT_AFTER(&list, 2, 160, exit_label);

    INSERT_BEFORE_HEAD(&list, 50, exit_label);

    DELETE_ELEMENT(&list, 4, exit_label);

    DELETE_ELEMENT(&list, 2, exit_label);

    UserPrintList(&list);

    ListDump(&list, __LINE__, __func__, __FILE__);

    DownwardReallocate(&list, false);

    ListDump(&list, __LINE__, __func__, __FILE__);

    UserPrintList(&list);

    SortListByNext(&list);

    UserPrintList(&list);

    Linearization(&list);

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
