#ifndef _ERROR_H_
#define _ERROR_H_

#define OK (1)

enum Error {
	EMEMALLOC = -1, // Memory allocation failed
	EARGNULL = -2,  // One or more required arguments were NULL
    EFILE = -3, // A file could not be accessed
};

#endif
