#ifndef SET_GET_FUNC
#define SET_GET_FUNC

//  GET
int GetHead(struct StructList* list);

int GetTail(struct StructList* list);

int GetFree(struct StructList* list);

int GetNumOfEl(struct StructList* list);

int GetCapacity(struct StructList* list);

int GetDataEl(struct StructList* list, int index);

int GetPrevEl(struct StructList* list, int index);

int GetNextEl(struct StructList* list, int index);

int GetErrCode(struct StructList* list);

//SET

void SetHead(struct StructList* list, int value);

void SetTail(struct StructList* list, int value);

void SetFree(struct StructList* list, int value);

void SetNumOfEl(struct StructList* list, int value);

void SetCapacity(struct StructList* list, int value);

void SetDataEl(struct StructList* list, int index, int value);

void SetPrevEl(struct StructList* list, int index, int value);

void SetNextEl(struct StructList* list, int index, int value);

void SetErrCode(struct StructList* list, int value);

#endif
