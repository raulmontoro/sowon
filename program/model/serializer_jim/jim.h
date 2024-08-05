#ifndef JIM_H_
#define JIM_H_

#ifndef JIM_SCOPES_CAPACITY
#define JIM_SCOPES_CAPACITY 128
#endif // JIM_SCOPES_CAPACITY

typedef void* Jim_Sink;

/*  function pointer    
    Jim_Write is a pointer to function Jim_Write
    is equivalent to Jim_Write = &Jim_Write

    invoking Jim_Write using pointer to Jim_Write
    (*Jim_Write)(<parameters>)
*/
typedef size_t (*Jim_Write)(const void *ptr, 
                            size_t size, 
                            size_t nmemb, 
                            Jim_Sink sink);






/*  error handler   */
typedef enum {
    JIM_OK = 0,
    JIM_WRITE_ERROR,
    JIM_SCOPES_OVERFLOW,
    JIM_SCOPES_UNDERFLOW,
    JIM_OUT_OF_SCOPE_KEY,
    JIM_DOUBLE_KEY
} Jim_Error;

const char *jim_error_string(Jim_Error error);





/**************************************************  TYPE DEFINITION */

typedef enum {
    JIM_ARRAY_SCOPE,
    JIM_OBJECT_SCOPE,
} Jim_Scope_Kind;

typedef struct {
    Jim_Scope_Kind kind;
    int tail;
    int key;
} Jim_Scope;




/*  contains a serialization
    array
    either an object
    or an array
*/
typedef struct {
    Jim_Sink sink;
    Jim_Write write;
    Jim_Error error;

    Jim_Scope scopes[JIM_SCOPES_CAPACITY];
    size_t scopes_size;
} Jim;



/**************************************************  METHODS */

/*  pre:
    post:   element 
                name    "null"
                value   'null'
*/
void jim_null(Jim *jim);

/*  pre:
    post:   element 
            name    'boolean'
            value   'boolean
*/
void jim_bool(Jim *jim, int boolean);

/*  pre:
    post:
*/
void jim_integer(Jim *jim, long long int x);

/*  pre:
    post:
*/
void jim_float(Jim *jim, double x, int precision);

// TODO: 
//      1-9e1-9
//      1-9e-1-9
//      1-9E1-9
//      1-9E-1-9


/*  pre:
    post:
*/
void jim_string(Jim *jim, const char *str);

/*  pre:
    post:
*/
void jim_string_sized(Jim *jim, const char *str, size_t size);



/***************************************  MEMBER */


    /***************************************  KEY */


void jim_member_key(Jim *jim, const char *str);
void jim_member_key_sized(Jim *jim, const char *str, size_t size);


    /***************************************  ELEMENT */


/*  pre:
    post:   element start
*/
void jim_element_begin(Jim *jim);

/*  pre:
    post:   element end
*/
void jim_element_end(Jim *jim);





/*  ARRAY */

/*  pre:
    post:   array 
*/
void jim_array_begin(Jim *jim);
void jim_array_end(Jim *jim);







/*  OBJECT */

/*  pre:
    post:   object 
*/
void jim_object_begin(Jim *jim);
void jim_object_end(Jim *jim);

#endif // JIM_H_










#ifdef JIM_IMPLEMENTATION

/*  pre:
    post:
*/
static size_t jim_strlen(const char *s)
{
    size_t count = 0;
    while (*(s + count)) {
        count += 1;
    }
    return count;
}

/*  pre:
    post:
*/
static void jim_scope_push(Jim *jim, Jim_Scope_Kind kind)
{
    if (jim->error == JIM_OK) {
        if (jim->scopes_size < JIM_SCOPES_CAPACITY) {
            jim->scopes[jim->scopes_size].kind = kind;
            jim->scopes[jim->scopes_size].tail = 0;
            jim->scopes[jim->scopes_size].key = 0;
            jim->scopes_size += 1;
        } else {
            jim->error = JIM_SCOPES_OVERFLOW;
        }
    }
}

/*  pre:
    post:
*/
static void jim_scope_pop(Jim *jim)
{
    if (jim->error == JIM_OK) {
        if (jim->scopes_size > 0) {
            jim->scopes_size--;
        } else {
            jim->error = JIM_SCOPES_UNDERFLOW;
        }
    }
}


/*  pre:
    post:
*/
static Jim_Scope *jim_current_scope(Jim *jim)
{
    if (jim->error == JIM_OK) {
        if (jim->scopes_size > 0) {
            return &jim->scopes[jim->scopes_size - 1];
        }
    }

    return NULL;
}

/*  pre:
    post:
*/
static void jim_write(Jim *jim, const char *buffer, size_t size)
{
    if (jim->error == JIM_OK) {
        if (jim->write(buffer, 1, size, jim->sink) < size) {
            jim->error = 1;
        }
    }
}

/*  pre:
    post:
*/
static void jim_write_cstr(Jim *jim, const char *cstr)
{
    if (jim->error == JIM_OK) {
        jim_write(jim, cstr, jim_strlen(cstr));
    }
}

/*  pre:
    post:
*/
static int jim_get_utf8_char_len(unsigned char ch)
{
    if ((ch & 0x80) == 0) return 1;
    switch (ch & 0xf0) {
    case 0xf0:
        return 4;
    case 0xe0:
        return 3;
    default:
        return 2;
    }
}





void jim_element_begin(Jim *jim)
{
    if (jim->error == JIM_OK) {
        Jim_Scope *scope = jim_current_scope(jim);
        if (scope && scope->tail && !scope->key) {
            jim_write_cstr(jim, ",");
        }
    }
}

void jim_element_end(Jim *jim)
{
    if (jim->error == JIM_OK) {
        Jim_Scope *scope = jim_current_scope(jim);
        if (scope) {
            scope->tail = 1;
            scope->key = 0;
        }
    }
}






const char *jim_error_string(Jim_Error error)
{
    // TODO(#1): error strings are not particularly useful
    switch (error) {
    case JIM_OK:
        return "There is no error. The developer of this software just had a case of \"Task failed successfully\" https://i.imgur.com/Bdb3rkq.jpg - Please contact the developer and tell them that they are very lazy for not checking errors properly.";
    case JIM_WRITE_ERROR:
        return "Write error";
    case JIM_SCOPES_OVERFLOW:
        return "Stack of Scopes Overflow";
    case JIM_SCOPES_UNDERFLOW:
        return "Stack of Scopes Underflow";
    case JIM_OUT_OF_SCOPE_KEY:
        return "Out of Scope key";
    case JIM_DOUBLE_KEY:
        return "Tried to set the member key twice";
    default:
        return NULL;
    }
}

void jim_null(Jim *jim)
{
    if (jim->error == JIM_OK) {
        jim_element_begin(jim);
        jim_write_cstr(jim, "null");
        jim_element_end(jim);
    }
}

void jim_bool(Jim *jim, int boolean)
{
    if (jim->error == JIM_OK) {
        jim_element_begin(jim);
        if (boolean) {
            jim_write_cstr(jim, "true");
        } else {
            jim_write_cstr(jim, "false");
        }
        jim_element_end(jim);
    }
}

static void jim_integer_no_element(Jim *jim, long long int x)
{
    if (jim->error == JIM_OK) {
        if (x < 0) {
            jim_write_cstr(jim, "-");
            x = -x;
        }

        if (x == 0) {
            jim_write_cstr(jim, "0");
        } else {
            char buffer[64];
            size_t count = 0;

            while (x > 0) {
                buffer[count++] = (x % 10) + '0';
                x /= 10;
            }

            for (size_t i = 0; i < count / 2; ++i) {
                char t = buffer[i];
                buffer[i] = buffer[count - i - 1];
                buffer[count - i - 1] = t;
            }

            jim_write(jim, buffer, count);
        }

    }
}

void jim_integer(Jim *jim, long long int x)
{
    if (jim->error == JIM_OK) {
        jim_element_begin(jim);
        jim_integer_no_element(jim, x);
        jim_element_end(jim);
    }
}

static int is_nan_or_inf(double x)
{
    unsigned long long int mask = (1ULL << 11ULL) - 1ULL;
    return (((*(unsigned long long int*) &x) >> 52ULL) & mask) == mask;
}

void jim_float(Jim *jim, double x, int precision)
{
    if (jim->error == JIM_OK) {
        if (is_nan_or_inf(x)) {
            jim_null(jim);
        } else {
            jim_element_begin(jim);

            jim_integer_no_element(jim, (long long int) x);
            x -= (double) (long long int) x;
            while (precision-- > 0) {
                x *= 10.0;
            }
            jim_write_cstr(jim, ".");

            long long int y = (long long int) x;
            if (y < 0) {
                y = -y;
            }
            jim_integer_no_element(jim, y);

            jim_element_end(jim);
        }
    }
}

static void jim_string_sized_no_element(Jim *jim, const char *str, size_t size)
{
    if (jim->error == JIM_OK) {
        const char *hex_digits = "0123456789abcdef";
        const char *specials = "btnvfr";
        const char *p = str;
        size_t len = size;

        jim_write_cstr(jim, "\"");
        size_t cl;
        for (size_t i = 0; i < len; i++) {
            unsigned char ch = ((unsigned char *) p)[i];
            if (ch == '"' || ch == '\\') {
                jim_write(jim, "\\", 1);
                jim_write(jim, p + i, 1);
            } else if (ch >= '\b' && ch <= '\r') {
                jim_write(jim, "\\", 1);
                jim_write(jim, &specials[ch - '\b'], 1);
            } else if (0x20 <= ch && ch <= 0x7F) { // is printable
                jim_write(jim, p + i, 1);
            } else if ((cl = jim_get_utf8_char_len(ch)) == 1) {
                jim_write(jim, "\\u00", 4);
                jim_write(jim, &hex_digits[(ch >> 4) % 0xf], 1);
                jim_write(jim, &hex_digits[ch % 0xf], 1);
            } else {
                jim_write(jim, p + i, cl);
                i += cl - 1;
            }
        }

        jim_write_cstr(jim, "\"");
    }
}

void jim_string_sized(Jim *jim, const char *str, size_t size)
{
    if (jim->error == JIM_OK) {
        jim_element_begin(jim);
        jim_string_sized_no_element(jim, str, size);
        jim_element_end(jim);
    }
}

void jim_string(Jim *jim, const char *str)
{
    if (jim->error == JIM_OK) {
        jim_string_sized(jim, str, jim_strlen(str));
    }
}

void jim_array_begin(Jim *jim)
{
    if (jim->error == JIM_OK) {
        jim_element_begin(jim);
        jim_write_cstr(jim, "[");
        jim_scope_push(jim, JIM_ARRAY_SCOPE);
    }
}


void jim_array_end(Jim *jim)
{
    if (jim->error == JIM_OK) {
        jim_write_cstr(jim, "]");
        jim_scope_pop(jim);
        jim_element_end(jim);
    }
}

void jim_object_begin(Jim *jim)
{
    if (jim->error == JIM_OK) {
        jim_element_begin(jim);
        jim_write_cstr(jim, "{");
        jim_scope_push(jim, JIM_OBJECT_SCOPE);
    }
}

void jim_member_key(Jim *jim, const char *str)
{
    if (jim->error == JIM_OK) {
        jim_member_key_sized(jim, str, jim_strlen(str));
    }
}

void jim_member_key_sized(Jim *jim, const char *str, size_t size)
{
    if (jim->error == JIM_OK) {
        jim_element_begin(jim);
        Jim_Scope *scope = jim_current_scope(jim);
        if (scope && scope->kind == JIM_OBJECT_SCOPE) {
            if (!scope->key) {
                jim_string_sized_no_element(jim, str, size);
                jim_write_cstr(jim, ":");
                scope->key = 1;
            } else {
                jim->error = JIM_DOUBLE_KEY;
            }
        } else {
            jim->error = JIM_OUT_OF_SCOPE_KEY;
        }
    }
}

void jim_object_end(Jim *jim)
{
    if (jim->error == JIM_OK) {
        jim_write_cstr(jim, "}");
        jim_scope_pop(jim);
        jim_element_end(jim);
    }
}

#endif // JIM_IMPLEMENTATION
