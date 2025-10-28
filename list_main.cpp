#include <TXLib.h>
#include <errno.h>

#include "list_functions.h"


int main()
{
    FILE* log_file = fopen(log_file_name, "w");
    fclose(log_file);

    struct StructList list = {};

    list.capacity = 10;

    if (ListCtor(&list))
        goto exit;

    ListDump(list);

    InsertAfter(&list, 0, 10);

    InsertAfter(&list, 1, 20);

    InsertAfter(&list, 2, 30);

    InsertAfter(&list, 3, 40);

    InsertAfter(&list, 4, 50);

    InsertAfter(&list, 5, 60);

    InsertAfter(&list, 3, 35);

    ListDump(list);



    for (int i = list.head; i != 0; i = list.next[i]) {
        printf("%d ", list.data[i]);
    }

    exit:
        ListDtor(&list);

        if(!errno)
            return 0;

        return 1;

}
