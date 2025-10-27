#include <TXLib.h>
#include <errno.h>

#include "list_functions.h"

ReturnStatus ListCtor(struct StructList* list)
{
    assert(list != NULL);

    list->data = (int*)calloc(list->capacity, sizeof(int));

    list->next = (int*)calloc(list->capacity, sizeof(int));

    list->prev = (int*)calloc(list->capacity, sizeof(int));

    if (list->data == NULL || list->next == NULL || list->prev == NULL) {
        printf("Memory was not allocated");
        errno = error;
        return error;
    }

    SetDefaultNext(list);
    SetDefaultPrev(list);

    list->head = 1;
    list->tail = 1;
    list->free = 1;

    return success;
}

void ListDtor(struct StructList* list)
{
    assert(list != NULL);

    free(list->data);
    free(list->next);
    free(list->prev);

    list->data = NULL;
    list->next = NULL;
    list->prev = NULL;
}

void SetDefaultNext(struct StructList* list)
{
    assert(list != NULL);

    list->next[0] = 0;

    for (size_t i = 1; i < list->capacity; ++i)
        list->next[i] = i + 1;

    list->next[list->capacity - 1] = 0;

}

void SetDefaultPrev(struct StructList* list)
{
    assert(list != NULL);

    list->prev[0] = 0;

    for (size_t i = 1; i < list->capacity; ++i)
        list->prev[i] = -1;

}

enum ReturnStatus InsertAfter(struct StructList* list,
                              size_t prev_index, int value)
{
    assert(list != NULL);

    if (list->tail == list->capacity - 1) {
        printf("List is full");
        return error;
    }

    printf("\nпїЅпїЅпїЅпїЅпїЅпїЅпїЅ пїЅпїЅпїЅпїЅпїЅ %u\n", prev_index);

    // вставка первого элемента
    if (prev_index == 0 && list->tail == 1) {

        list->data[list->free] = value;
        list->prev[list->free] = 0;

        list->free++;
    }

    // вставка нового первого элемента

    else if (prev_index == 0) {

        list->data[list->free] = value;

        list->prev[list->free] = list->prev[list->head];
        list->prev[list->head] = list->free;

        int twin_head = list->head;
        int twin_free = list->free;

        list->head = list->free;
        list->free = list->next[list->free];
        list->next[twin_free] = twin_head;

    }

    //вставка в конец
    else if (list->tail == prev_index) {

        list->tail++;

        list->data[list->free] = value;

        list->next[prev_index] = list->free;

        list->prev[list->free] = prev_index;

        list->free = list->next[list->free];

        list->next[list->tail] = 0;

    }

    //вставка между элементами

    else {

        list->data[list->free] = value;

        int twin_free = list->free;

        list->free = list->next[list->free];

        list->next[twin_free] = list->next[prev_index];

        list->prev[list->next[prev_index]] = twin_free;

        list->next[prev_index] = twin_free;

        list->prev[twin_free] = prev_index;

    }

    return success;
}

enum ReturnStatus DeleteElement(struct StructList* list, size_t del_index)
{
    assert(list != NULL);

    //список пуст
    if (list->tail == 1) {
        printf("List is empty");
        return error;
    }

    //удаление с конца
    if (del_index == list->tail) {

        list->data[del_index] = 0;

        list->tail = list->prev[del_index];

        list->next[list->prev[del_index]] = 0;
        list->next[del_index] = list->free;
        list->free = del_index;

        list->prev[del_index] = -1;

        return success;
    }

    //удаление первого элемента
    if (del_index == list->head) {
       list->data[del_index] = 0;

       list->head = list->next[del_index];

       list->next[del_index] = list->free;
       list->free = del_index;

       list->prev[del_index] = -1;

       return success;
    }

    list->data[del_index] = 0;
    list->next[list->prev[del_index]] = list->next[del_index];

    list->prev[list->next[del_index]] = list->prev[del_index];

    list->prev[del_index] = -1;

    list->next[del_index] = list->free;
    list->free = del_index;

    return success;
}

void ListDump(struct StructList list)
{

    static int file_counter = 0;

    char* image_file_name = NULL;

    SetNewImageFileName(file_counter, &image_file_name);

    printf("|||FILE NAME: %s|||\n", image_file_name);

    FILE* graphiz_file = fopen(image_file_name, "w");

    fprintf(graphiz_file, "digraph {\n"
                          "rankdir = HR;\n");

    const char* color = NULL;

    fprintf(graphiz_file, "node%u "
                              "[shape = rectangle; "
                              "style = filled; "
                              "fillcolor = \"#ECF96A\"; "
                              "color = \"#80000\"; "
                              "label = \"HEAD\"; ]\n",
                                         list.capacity);

    fprintf(graphiz_file, "node%u "
                              "[shape = rectangle; "
                              "style = filled; "
                              "fillcolor = \"#ECF96A\"; "
                              "color = \"#80000\"; "
                              "label = \"TAIL\"; ]\n",
                                         list.capacity + 1);

    fprintf(graphiz_file, "node%u "
                              "[shape = rectangle; "
                              "style = filled; "
                              "fillcolor = \"#ECF96A\"; "
                              "color = \"#80000\"; "
                              "label = \"FREE\"; ]\n",
                                         list.capacity + 2);

    const int ORIGINAL_FREE = list.free;

    for (size_t i = 0; i < list.capacity; i++) {

        if (i == list.free){
            color = "#F14065";
            list.free = list.next[list.free];
        }
        else
            color = "#77DD77";

        fprintf(graphiz_file, "node%u "
                              "[shape = rectangle; "
                              "style = filled; "
                              "fillcolor = \"%s\"; "
                              "color = \"#80000\"; "
                              "label = \"INDEX = %u \\n"
                                         "DATA = %d  \\n"
                                         "NEXT = %d "
                                         "PREV = %d"
                                         "\"; ]\n",
                                         i, color, i,
                                         list.data[i],
                                         list.next[i],
                                         list.prev[i]);
    }

    fprintf(graphiz_file, "{ rank = same; ");

    for (size_t i = 1; i < list.capacity; ++i) {
        fprintf(graphiz_file, "node%u; ", i);
    }

    fprintf(graphiz_file, "}\n");

    fprintf(graphiz_file, "{ rank = same; node%u; node%u; node%u}\n", list.capacity,
                                                                    list.capacity + 1,
                                                                    list.capacity + 2);
    fprintf(graphiz_file, "node0 -> node1 "
                          "[color = \"transparent\"]\n");

    for (size_t i = list.head; i != list.tail; i = list.next[i]) {
        fprintf(graphiz_file, "node%u -> node%d "
                              "[color = \"red\"]\n", i, list.next[i]);
    }

    fprintf(graphiz_file, "node%u -> node0 "
                          "[color = \"blue\"]\n", list.tail);

    for (size_t i = list.tail; i != list.head; i = list.prev[i]) {
        fprintf(graphiz_file, "node%u -> node%d "
                              "[color = \"blue\"]\n", i, list.prev[i]);
    }

    fprintf(graphiz_file, "node%u -> node%u "
                          "[color = \"yellow\"]\n", list.capacity, list.head);

    fprintf(graphiz_file, "node%u -> node%u "
                          "[color = \"yellow\"]\n", list.capacity + 1, list.tail);

    fprintf(graphiz_file, "node%u -> node%u "
                          "[color = \"yellow\"]\n", list.capacity + 2, ORIGINAL_FREE);

    fprintf(graphiz_file, "}");


    fclose(graphiz_file);
    free(image_file_name);
    file_counter++;
}

void SetNewImageFileName(int file_counter, char** new_file_name)
{

    char graphiz_file_name[100] = "image";
    const char* file_extension = ".txt";

    char str_file_counter[10] = "";

    snprintf(str_file_counter, sizeof(str_file_counter),
             "%d", file_counter);

    strncat(graphiz_file_name, str_file_counter,  sizeof(str_file_counter));
    strncat(graphiz_file_name, file_extension,  10);

    *new_file_name = strdup(graphiz_file_name);
}






