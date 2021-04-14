#ifndef __ERROR_H__
#define __ERROR_H__

//Error Cases
enum ddads_error {
    E_EOK = 0, //EOK
    E_CACHE = 0,	//Not an error [ returning cached value ]
    E_INIT = -1,
    E_INVAL_PARAM = -2,	//Either function parameter causing OOB or Array has no elements etc.
    E_CALLOC = -3,
    E_REALLOC = -4
};

#endif
