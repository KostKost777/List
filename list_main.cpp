#include <TXLib.h>
#include <errno.h>

#include "list_functions.h"
#include "dump_functions.h"

int main()
{
    struct StructList list = {};

    atexit(CloseLogFile);

    if (OpenLogFile())
        goto exit;

    list.capacity = 5;

    if (ListCtor(&list))
        goto exit;

    INSERT_AFTER(&list, 0, 10);

    INSERT_AFTER(&list, 1, 20);

    INSERT_AFTER(&list, 2, 30);

    INSERT_AFTER(&list, 3, 40);

    UserPrintList(&list);

    exit:

        ListDtor(&list);

        if(!errno) {
            printf("END SUCCESS\n");
            return 0;
        }

        printf("END WITH ERROR\n");
        return 1;

}
