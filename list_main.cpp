#include <TXLib.h>
#include <errno.h>

#include "list_functions.h"
#include "dump_functions.h"


int main()
{
    atexit(CloseLogFile);
    fprintf(log_file, "<pre>\n");

    struct StructList list = {};

    list.capacity = 13;

    if (ListCtor(&list))
        goto exit;

    INSERT_AFTER(&list, 0, 10);

    INSERT_AFTER(&list, 1, 20);

    INSERT_AFTER(&list, 2, 30);

    INSERT_AFTER(&list, 3, 40);

    INSERT_AFTER(&list, 4, 50);

    INSERT_AFTER(&list, 5, 60);

    list.capacity = 0;

    INSERT_AFTER(&list, 0, 9);

    INSERT_AFTER(&list, 0, 8);

    INSERT_AFTER(&list, 1, 11);

    INSERT_AFTER(&list, 0, 7);

    INSERT_AFTER(&list, 0, 6);

    DELETE_ELEMENT(&list, 6);

    DELETE_ELEMENT(&list, 11);

    DELETE_ELEMENT(&list, 3);

    UserPrintList(&list);

    exit:
        ListDtor(&list);
        printf("END\n");

        if(!errno)
            return 0;

        return 1;

}
