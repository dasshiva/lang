#ifdef _ERROR_H_
#define _ERROR_H_

enum Error {
	EMEMALLOC = -1, // Memory allocation failed
	EARGNULL = -2,  // One or more required arguments were NULL
    EFILE = -3, // A File could not be accessed
};

#endif
