#include <TXLib.h>
#include <errno.h>

#include "list_functions.h"


int main()
{
    FILE* log_file = fopen(log_file_name, "w");
    fclose(log_file);

    struct StructList list = {};

    list.capacity = 13;

    if (ListCtor(&list))
        goto exit;

    InsertAfter(&list, 0, 10);

    InsertAfter(&list, 1, 20);

    InsertAfter(&list, 2, 30);

    InsertAfter(&list, 3, 40);

    InsertAfter(&list, 4, 50);

    InsertAfter(&list, 5, 60);

    InsertAfter(&list, 0, 9);

    InsertAfter(&list, 0, 8);

    InsertAfter(&list, 1, 11);

    InsertAfter(&list, 0, 7);

    InsertAfter(&list, 0, 6);

    DeleteElement(&list, 6);

    DeleteElement(&list, 11);

    DeleteElement(&list, 3);

    UserPrintList(list);

    exit:
        ListDtor(&list);

        if(!errno)
            return 0;

        return 1;

}
