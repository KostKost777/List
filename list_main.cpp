#include <TXLib.h>
#include <errno.h>

#include "list_functions.h"


int main()
{
    struct StructList list = {};

    list.capacity = 20;

    if (ListCtor(&list))
        goto exit;

    ListDump(&list);

    InsertAfter(&list, 0, 10);

    ListDump(&list);

    InsertAfter(&list, 1, 20);

    ListDump(&list);

    InsertAfter(&list, 2, 30);

    ListDump(&list);

    InsertAfter(&list, 3, 40);

    ListDump(&list);

    InsertAfter(&list, 4, 50);
    InsertAfter(&list, 5, 60);

    ListDump(&list);

    InsertAfter(&list, 0, 9);

    ListDump(&list);

    InsertAfter(&list, 0, 8);

    ListDump(&list);

    InsertAfter(&list, 1, 11);

    ListDump(&list);

    InsertAfter(&list, 0, 7);

    ListDump(&list);

    InsertAfter(&list, 0, 6);

    ListDump(&list);

    DeleteElement(&list, 6);

    ListDump(&list);

    DeleteElement(&list, 11);

    ListDump(&list);

    DeleteElement(&list, 3);

    ListDump(&list);

    for (int i = list.head; i != 0; i = list.next[i]) {
        printf("%d ", list.data[i]);
    }

    exit:
        ListDtor(&list);

        if(!errno)
            return 0;

        return 1;

}
