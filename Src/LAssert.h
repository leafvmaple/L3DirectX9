#ifndef _LM_ASSERT_H_
#define _LM_ASSERT_H_

#define BOOL_ERROR_RETURN(Condition) \
	if (!(Condition)) {              \
		return 0;                    \
	}

#define BOOL_ERROR_LOG_RETURN(Condition, LOG, ...)   \
	if (!(Condition)) {                              \
		printf(LOG, __VA_ARGS__);                    \
		return 0;                                    \
	}

#define BOOL_ERROR_CONTINUE(Condition)   \
	if (!Condition) {                    \
		continue;                        \
	}

#define BOOL_ERROR_BREAK(Condition)   \
	if (!Condition) {                 \
		break;                        \
	}

#define HRESULT_ERROR_RETURN(hr) \
	if (!SUCCEEDED(hr)) {        \
		return 0;                \
	}

#define HRESULT_ERROR_LOG_RETURN(hr, LOG, ...)   \
	if (!SUCCEEDED(hr)) {                        \
		printf(LOG, __VA_ARGS__);                \
		return 0;                                \
	}

#define HRESULT_ERROR_CONTINUE(hr)   \
	if (!SUCCEEDED(hr)) {            \
		continue;                    \
	}

#define HRESULT_ERROR_BREAK(hr)   \
	if (!SUCCEEDED(hr)) {         \
		break;                    \
	}

#define SAFE_FREE(p)    \
	if ((p)) {          \
		free((p));      \
		(p) = NULL;     \
	}

#define SAFE_CLOSE_HANDLE(h)    \
	if ((h)) {                  \
		CloseHandle((h));       \
		(h) = NULL;             \
	}

#endif // !_LM_ASSERT_H_