#ifndef CCOMPIELR_H
#define CCOMPIELR_H

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

struct pos
{
    int line;
    int col;
    const char *filename;
};

#define S_EQ(str, str2) \
    (str && str2 && (strcmp(str, str2) == 0))

#define NUMERIC_CASE \
    case '0':        \
    case '1':        \
    case '2':        \
    case '3':        \
    case '4':        \
    case '5':        \
    case '6':        \
    case '7':        \
    case '8':        \
    case '9'

#define OPERATOR_CASE_EXCLUDING_DIVISION \
    case '+':                            \
    case '-':                            \
    case '*':                            \
    case '>':                            \
    case '<':                            \
    case '^':                            \
    case '%':                            \
    case '!':                            \
    case '=':                            \
    case '~':                            \
    case '|':                            \
    case '&':                            \
    case '(':                            \
    case '[':                            \
    case ',':                            \
    case '.':                            \
    case '?'

#define SYMBOL_CASE \
    case '{':       \
    case '}':       \
    case ':':       \
    case ';':       \
    case '#':       \
    case '\\':      \
    case ')':       \
    case ']'
enum
{
    LEXICAL_ANALYSIS_ALL_OK,
    LEXICAL_ANALYSIS_INPUT_ERROR
};

enum
{
    TOKEN_TYPE_IDENTIFIER,
    TOKEN_TYPE_KEYWORD,
    TOKEN_TYPE_OPERATOR,
    TOKEN_TYPE_SYMBOL,
    TOKEN_TYPE_NUMBER,
    TOKEN_TYPE_STRING,
    TOKEN_TYPE_COMMENT,
    TOKEN_TYPE_NEWLINE
};

enum
{
    NUMBER_TYPE_NORMAL,
    NUMBER_TYPE_LONG,
    NUMBER_TYPE_FLOAT,
    NUMBER_TYPE_DOUBLE
};

struct token
{
    int type;
    int flags;
    struct pos pos;

    union
    {
        char cval;
        char *sval;
        unsigned int inum;
        unsigned long lnum;
        unsigned long long llnum;
        void *any;
    };

    struct token_number
    {
        int type;
    } num;

    // True if their is whitespace betweeen the token and the next token
    bool whitespace;

    //(5+10) all tokens "5", "+", and "10" points at the firts bracket "8"
    const char *between_brackets;
    struct bufer *parantheses_buffer;
    struct lex_process_functions *function;

    void *private;
};

struct lex_process;
typedef char (*LEX_PROCESS_NEXT_CHAR)(struct lex_process *process);
typedef char (*LEX_PROCESS_PEAK_CHAR)(struct lex_process *process);
typedef void (*LEX_PROCESS_PUSH_CHAR)(struct lex_process *process, char c);

struct lex_process_functions
{
    LEX_PROCESS_NEXT_CHAR next_char;
    LEX_PROCESS_PEAK_CHAR peak_char;
    LEX_PROCESS_PUSH_CHAR push_char;
};

struct lex_process
{
    struct pos pos;
    struct vector *token_vec;
    struct compile_process *compiler;

    // how many brackets at the moment
    int current_expression_count;
    struct buffer *paranthese_buffer;
    struct lex_process_functions *function;

    void *private;
};

enum
{
    COMILER_FILE_COMPILED_OK = 0,
    COMPILER_FAILED_WITH_ERRORS = 1
};

//tree structure of scopes that holds scope entiries, and contains the scope offset and alignment data for everything that is allocated on stack
struct scope
{
    int flags;

    //void*
    struct vector* entities;

    // the total number of bytes this scope uses. Aligned to 16 bytes.
    size_t size;

    //NULL if no parent.
    struct scope* parent;

};

enum
{
    SYMBOL_TYPE_NODE,
    SYMBOL_TYPE_NATIVE_FUNCTION,
    SYMBOL_TYPE_UNKNOWN
};

struct symbol
{
    const char* name;
    int type;
    void* data;
};

struct codegen_entry_point
{
    //id of the entry point
    int id;
};

struct codegen_exit_point
{
    //id of the exit point
    int id;
};

struct string_table_element
{
    //the string that the lemenet is related to. "hello world"
    const char* str;
    //assembly label that points to the memory where the strinc can be found.
    const char label[50];
};

struct code_generator
{   
    //vector of struct string_table_element;
    struct vector* string_table;

    struct vector* codegen_entry_points;
    struct vector* codegen_exit_points;
};


struct compile_process
{
    // how to compile
    int flags;

    struct pos pos;
    struct compile_process_input_file
    {
        FILE *fp;
        const char *abs_path;
    } cfile;

    // vector of tokens after lexical analysis
    struct vector *token_vec;

    struct vector *node_vec;
    struct vector *node_tree_vec;

    FILE *ofile;

    struct
    {
        struct scope* root;
        struct scope* current;
    } scope;

    struct 
    {
        //current active symbol table.
        struct vector* table;
        
        // struct vctor* multiple symbol tables
        struct vector* tables;
    } symbols;

    struct code_generator* generator;
    
};
enum
{
    NODE_TYPE_EXPRESSION,
    NODE_TYPE_EXPRESSION_PARANTHESES,
    NODE_TYPE_NUMBER,
    NODE_TYPE_IDENTIFIER,
    NODE_TYPE_STRING,
    NODE_TYPE_VARIABLE,
    NODE_TYPE_VARIABLE_LIST,
    NODE_TYPE_LIST,
    NODE_TYPE_FUNCTION,
    NODE_TYPE_BODY,
    NODE_TYPE_STATEMENT_RETURN,
    NODE_TYPE_STATEMENT_IF,
    NODE_TYPE_STATEMENT_ELSE,
    NODE_TYPE_STATEMENT_WHILE,
    NODE_TYPE_STATEMENT_DO_WHILE,
    NODE_TYPE_STATEMENT_FOR,
    NODE_TYPE_STATEMENT_BREAK,
    NODE_TYPE_STATEMENT_CONTINUE,
    NODE_TYPE_STATEMENT_SWITCH,
    NODE_TYPE_STATEMENT_CASE,
    NODE_TYPE_STATEMENT_DEFAULT,
    NODE_TYPE_STATEMENT_GOTO,
    NODE_TYPE_UNARY,
    NODE_TYPE_TENARY,
    NODE_TYPE_LABEL,
    NODE_TYPE_STRUCT,
    NODE_TYPE_UNION,
    NODE_TYPE_BRACKET,
    NODE_TYPE_CAST,
    NODE_TYPE_BLANK
};

enum
{
    NODE_FLAG_INSIDE_EXPRESSION = 0x00000001,
    NODE_FLAG_IS_FORWARD_DECLARATION =  0x00000010,
    NODE_FLAG_HAS_VARIABLE_COMBINED = 0x00000100
};

enum
{
    PARSE_ALL_OK,
    PARSE_GENERAL_ERROR
};

enum
{
    CODEGEN_ALL_OK,
    CODEGEN_GENERAL_ERROR
};


struct array_brackets
{
    //vector of struct node*
    struct vector* n_brackets;
};

struct node;
struct datatype
{
    int flags;
    // type of long, int, float
    int type;
    // i.e. long int
    struct datatype *secondary;
    const char *type_str;
    // size of the type
    size_t size;
    //***p pointer deppth of 3
    int pointer_depth;
    union
    {
        struct node *struct_node;
        struct node *union_node;
    };

    struct array
    {
        struct array_brackets* brackets;
        // total array size= datatype size * each index
        size_t size;
    } array;
};

struct parsed_switch_case
{
    int index;
};



struct stack_frame_data
{
    /*
    * the datatype that was pushed to the stack
    */
   struct datatype dtype;

};

struct stack_frame_element
{
    //stack frame element flags
    int flags;

    //type of the frame leement
    int type;

    /*
    * the name of the frame lement, not the variable name. i.e result_value
    */
    const char * name;

    //offset to the base pointer
    int offset_from_bp;

    struct stack_frame_data data;
};

#define STACK_PUSH_SIZE 4

enum
{
    STACK_FRAME_ELEMENT_TYPE_LOCAL_VARIABLE,
    STACK_FRAME_ELEMENT_TYPE_SAVED_REGISTER,
    STACK_FRAME_ELEMENT_TYPE_SAVED_BT,
    STACK_FRAME_ELEMENT_TYPE_PUSHED_VALUE,
    STACK_FRAME_ELEMENT_TYPE_UNKNOWN
};

enum
{
    STACK_FRAME_ELEMENT_FLAG_IS_ADDRESS = 0b00000001,
    STACK_FRAME_ELEMENT_FLAG_ELEMENT_NOT_FOUND = 0b00000010,
    STACK_FRAME_ELEMENT_FLAG_IS_NUMERICAL = 0b00000100, 
    STACK_FRAME_ELEMENT_FLAG_HAS_DATATYPE = 0b00001000
};


struct node
{
    int type;
    int flags;

    struct pos pos;

    struct node_binded
    {
        // pointer to pbody node
        struct node *owner;

        // pointer to the function this node is in
        struct node *function;
    } binded;

    union
    {
        struct exp
        {
            struct node *left;
            struct node *right;
            const char *op;
        } exp;

        struct parenthesis
        {
            // expression node inside the parentheses
            struct node* exp;
        } parenthesis;

        struct var
        {
            struct datatype type;
            int padding;
            const char* name;
            //alligned offset
            int aoffset;
            struct node* val;
        } var;

        struct node_tenary
        {
            struct node* true_node;
            struct node* false_node;
        } tenary;

        struct varlist
        {
            //list of struct node variables
            struct vector* list;
        } var_list;

        struct bracket
        {
        //int x[50]. [50] is the bracket node. Inner is NODE_TYPE_NUMBER with value of 50
        struct node* inner;
        } bracket;

        struct _struct
        {
            const char* name;
            struct node* body_n; //the body {}
            struct node* var;   // single instantiation, null ptr if {body};
        } _struct;

        struct _union
        {
            const char* name;
            struct node* body_n; //the body {}
            struct node* var;   // single instantiation, null ptr if {body};
        } _union;

        struct body
        {
            struct vector* statements;

             // size of all statements
            size_t size;
        
            //true if the variable size has been increased because of padding
            bool padded;

            //ptr to the larges variable node in the statements vector
            struct node* larges_var_node;

        } body;

        struct function
        {
            int flags;
            //return type
            struct datatype rtype;

            //function name
            const char* name;

            struct function_arguments
            {
                //vector of functions parameters, must be type NODE_TYPE_VARIABLE
                struct vector* vector;

                //how much to add to EBP to find the first arguemt
                size_t stack_addition;
            } args;
            //prointer to the function body node, NULL if this is a prototype
            struct node* body_n;

            //stack size for all variables inside the function
            size_t stack_size;

            struct stack_frame
            {
                // a vector of stack frame elements
                struct vector* elements;
            } frame;

        } func;

        struct statement
        {
            struct return_stmt
            {   //the expression of the return
                struct node* exp;
            } return_stmt;

            struct if_stmt
            {
                //if(cond) body
                struct node* cond_node;
                struct node* body_node;

                // if().. else.. points to the else
                struct node* next;

            } if_stmt;

            struct else_stmt
            {
                struct node* body_node;
            } else_stmt;

            struct for_stmt
            {
                //for( int i = 0; i < 30; i++){}
                struct node* init_node;
                struct node* cond_node;
                struct node* loop_node;
                struct node* body_node;
            } for_stmt;

            struct while_stmt
            {
                struct node* exp_node;
                struct node* body_node;
            } while_stmt;

            struct do_while_stmt
            {
                struct node* exp_node;
                struct node* body_node;                
            } do_while_stmt;

            struct switch_stmt
            {   
                struct node* exp;
                struct node* body;
                struct vector* cases;
                bool has_default_case;

            }switch_stmt;

            struct case_stmt
            {
                struct node* exp;
            } _case;

            struct goto_stm
            {
                struct node* label;
            } _goto;

        }stmt;

        struct node_label
        {    
            struct node* name;
        } label;

        struct cast 
        {
            struct datatype dtype;
            struct node* operand;
        }cast;
    };

    union
    {
        char cval;
        const char *sval;
        unsigned int inum;
        unsigned long lnum;
        unsigned long long llnum;
    };

};

enum
{
    RESOLVER_ENTITY_FLAG_IS_STACK                   = 0b00000001,
    RESOLVER_ENTITY_FLAG_NO_MERGE_WITH_NEXT_ENTTY   = 0b00000010,
    RESOLVER_ENTITY_FLAG_NO_MERGE_WITH_LEFT_ENTITY  = 0b00000100,
    RESOLVER_ENTITY_FLAG_DO_INDIRECTION             = 0b00001000,
    RESOLVER_ENTITY_FLAG_JUST_USE_OFFSET            = 0b00010000,
    RESOLVER_ENTITY_FLAG_IS_POINTER_ARRAY_ENTIRY    = 0b00100000,
    RESOLVER_ENTITY_FLAG_WAS_CASTED                 = 0b01000000,
    RESOLVER_ENTITY_FLAG_USES_ARRAY_BRACKETS        = 0b10000000
};

enum
{
    RESOLVER_ENTITY_TYPE_VARIABLE,
    RESOLVER_ENTITY_TYPE_FUNCTION,
    RESOLVER_ENTITY_TYPE_STRUCTURE,
    RESOLVER_ENTITY_TYPE_FUNCTION_CALL,
    RESOLVER_ENTITY_TYPE_ARRAY_BRACKET,
    RESOLVER_ENTITY_TYPE_RULE,
    RESOLVER_ENTITY_TYPE_GENERAL,
    RESOLVER_ENTITY_TYPE_UNARY_GET_ADDRESS,
    RESOLVER_ENTITY_TYPE_UNARY_INDIRECTION,
    RESOLVER_ENTITY_TYPE_UNSUPPORTED,
    RESOLVER_ENTITY_TYPE_CAST
};

enum
{
    RESOLVER_SCOPE_FLAG_IS_STACK = 0b00000001
};


enum
{
    RESOLVER_RESULT_FLAG_FAILED                                 = 0b00000001,
    RESOLVER_RESULT_FLAG_RUNTIME_NEEDED_TO_FINISH_PATH          = 0b00000010,
    RESOLVER_RESULT_FLAG_PROCESSING_ARRAY_ENTITIES              = 0b00000100,
    RESOLVER_RESULT_FLAG_HAS_POINTER_ARRAY_ACCESS               = 0b00001000,
    RESOLVER_RESULT_FLAG_FIST_ENTITY_LOAD_TO_EBX                = 0b00010000,
    RESOLVER_RESULT_FLAG_FIST_ENITITY_PUSH_VALUE                = 0b00100000,
    RESOLVER_RESULT_FLAG_FINAL_INDIRECTION_REQUIRED_FOR_VALUE   = 0b01000000,
    RESOLVER_RESULT_FLAG_DOES_GET_ADDRESS                       = 0b10000000
};


struct resolver_array_data
{
    //holds nodes of type resolver_entity
    struct vector* array_entities;
};

struct resolver_result
{
    //This is the fist entity in our resolver result
    struct resolver_entity* first_entity_const;

    //the entity represent the variable at the start of this expression
    struct resolver_entity * identifier;

    //equal to the last structure or union entity discovered
    struct resolver_entity* last_struct_union_entity;

    struct resolver_array_data array_data;

    //the root entity of our result
    struct resolver_entity* entity;
    //the last entity of our result
    struct resolver_entity* last_entity;
    int flags;
    //the total numver of entities
    size_t count;

    struct resolver_result_base
    {
        //[ebp -4], [name -4]
        char address[60];
        //ebp, global, variable_name
        char base_address[60];
        //offset from the base -4
        int offset;

    }base;
};

struct resolver_scope
{  
    //resolver scope flags
    int flags;
    //vector of resolver_entity*
    struct vector* entities;
    struct resolver_scope* next;
    struct resolver_scope* prev;

    //private data fo the resolver scope
    void* private;
};

struct resolver_entity
{
    int type;
    int flag;
    //name of the resolved entiry, function/variable name
    const char* name;

    //offset from the stack EBP + offset
    int offset;

    //node that the entity is related to
    struct node* node;

    union 
    {
        struct resolver_entity_var_data
        {
            struct datatype dtype;
            struct resolver_array_runtime_
            {
                struct datatype dtype;
                struct node* index_node;
                int multiplier;
            } array_runtime;
        } var_data;

        struct resolver_array
        {
            struct datatype dtype;
            struct node* array_index_node;
            int index;
        } array;

        struct resolver_entity_function_call_data
        {
            // struct node* vector
            struct vector* arguments;
            //total bytes used by teh function call
            size_t stack_size;

        }func_call_data;


        struct resolver_entity_rule
        {
            struct reslover_entity_rule_left
            {
                int flags;
            } left;
            struct reslover_entity_rule_right
            {
                int flags;
            } right;
        } rule;

        struct resolver_indirection
        {
            //pointer depth ***int is pointer depth 3
            int depth;
        } indirection;
    };

    struct entity_last_resolve
    {
        struct node* referencing_node;
    } last_resolve;
    
    //the dataype of the resolver entity
    struct datatype dtype;

    //the scopoe this eniity belongs to
    struct resolver_scope* scope;

    //the result of the resolution
    struct resolver_result* result;

    //the resolver process
    struct resolver_process* process;

    //private data that only the resolver enitity created knows about;
    void* private;    

    //the next enitity;
    struct resolver_entity* next;

    //the previous entity
    struct resolver_entity* prev;

};

typedef void*(*RESOLVER_NEW_ARRAY_BRACKET_ENTITY)(struct resolver_result* result, struct node* array_entity_node);
typedef void(*RESOLVER_DELETE_SCOPE)(struct resolver_scope* scope);
typedef void(*RESOLVER_DELETE_ENTITY)(struct resolver_entity* entity);
typedef struct resolver_entity*(*RESOLVER_MERGE_ENTITIES)(struct resolver_process* process, struct resolver_result* result, struct resolver_entity* left_entity, struct resolver_entity* right_entity);
typedef void*(*RESOLVER_MAKE_PRIVATE)(struct resolver_entity* entity, struct node* node, int offset, struct resolver_scope* scope);
typedef void(*RESOLVER_SET_RESULT_BASE)(struct resolver_result* result, struct resolver_entity* base_entity);


struct resolver_callbacks
{
    RESOLVER_NEW_ARRAY_BRACKET_ENTITY  new_array_entity;
    RESOLVER_DELETE_SCOPE delete_scope;
    RESOLVER_DELETE_ENTITY delete_entity;
    RESOLVER_MERGE_ENTITIES merge_entities;
    RESOLVER_MAKE_PRIVATE  make_private;
    RESOLVER_SET_RESULT_BASE  set_result_base;
};

struct resolver_process
{
    struct resolver_scopes
    {
        struct resolver_scope* root;
        struct resolver_scope* curret;
    } scope;

    struct compile_process* compiler;
    struct resolver_callbacks callbacks;
};


enum
{
    DATATYPE_FLAG_IS_SIGNED         = 0b00000000001,
    DATATYPE_FLAG_IS_STATIC         = 0b00000000010,
    DATATYPE_FLAG_IS_CONST          = 0b00000000100,
    DATATYPE_FLAG_IS_POINTER        = 0b00000001000,
    DATATYPE_FLAG_IS_ARRAY          = 0b00000010000,
    DATATYPE_FLAG_IS_EXTERN         = 0b00000100000,
    DATATYPE_FLAG_IS_RESTRICT       = 0b00001000000,
    DATATYPE_FLAG_IGNORE_TYPE_CHECKING  = 0b00010000000,
    DATATYPE_FLAG_IS_SECONDARY      = 0b00100000000,
    DATATYPE_FLAG_STRUCT_UNION_NO_NAME  = 0b01000000000,
    DATATYPE_FLAG_IS_LITERAL        = 0b10000000000
};

enum
{
    DATA_TYPE_VOID,
    DATA_TYPE_CHAR,
    DATA_TYPE_SHORT,
    DATA_TYPE_INTEGER,
    DATA_TYPE_LONG,
    DATA_TYPE_FLOAT,
    DATA_TYPE_DOUBLE,
    DATA_TYPE_STRUCT,
    DATA_TYPE_UNION,
    DATA_TYPE_UNKNOWN
};

enum
{
    DATA_TYPE_EXPECT_PRIMITIVE,
    DATA_TYPE_EXPECT_UNION,
    DATA_TYPE_EXPECT_STRUCT
};
enum
{
    DATA_SIZE_ZERO = 0,
    DATA_SIZE_BYTE = 1,
    DATA_SIZE_WORD = 2,
    DATA_SIZE_DWORD = 4,
    DATA_SIZE_DDWORD = 8
};

enum
{
    STRUCT_ACCESS_BACKWARDS = 0b00000001,
    STRUCT_STOP_AT_POINTER_ACCESS = 0b00000010
};

enum
{
    FUNCTION_NODE_FLAG_IS_NATIVE = 0b00000001
};

int compile_file(const char *filename, const char *out_file, int flags);
struct compile_process *compile_process_create(const char *filename, const char *filename_out, int flags);

char compile_process_next_char(struct lex_process *lex_process);
char compile_process_peak_char(struct lex_process *lex_process);
void compile_process_push_char(struct lex_process *lex_process, char c);

void compiler_error(struct compile_process *compiler, const char *msg, ...);
void compiler_warning(struct compile_process *compiler, const char *msg, ...);

struct lex_process *lex_process_create(struct compile_process *compiler, struct lex_process_functions *functions, void *private);
void lex_process_free(struct lex_process *process);
void *lex_process_private(struct lex_process *process);
void *lex_process_tokens(struct lex_process *process);
int lex(struct lex_process *process);
int parse(struct compile_process *process);
int codegen(struct compile_process* process);
struct code_generator* codegenerator_new(struct compile_process* process);

bool token_is_keyword(struct token *token, const char *value);
bool token_is_identifier(struct token* token);
/*
    builds tokens for the input string
*/
struct lex_process *tokens_build_for_string(struct compile_process *compiler, const char *str);
bool token_is_symbol(struct token *token, char c);
bool token_is_nl_or_commet_or_newline_seperator(struct token *token);

void node_set_vector(struct vector *vec, struct vector *root_vec);
void node_push(struct node *node);
bool is_access_operator(const char* op);
bool is_access_node(struct node* node);
bool is_array_operator(const char* op);
bool is_array_node(struct node* node);
bool is_parentheses_operator(const char* op);
bool is_parentheses_node(struct node* node);
bool is_access_node_with_op(struct node* node, const char* op);

struct node *node_peek_or_null();
struct node *node_peek();
struct node *node_pop();
struct node *node_create(struct node *_node);
void make_exp_node(struct node *left_node, struct node *right_node, const char *op);
void make_bracket_node(struct node* node);
void make_body_node(struct vector* body_vec, size_t size, bool padded, struct node* largest_var_node);
void make_struct_node(const char * name, struct node* body_node);
void make_union_node(const char * name, struct node* body_node);
void make_function_node(struct datatype* ret_type, const char* name, struct vector* arguemnts, struct node* body_node);
void make_exp_parentheses_node(struct node* exp_node);
void make_if_node(struct node* cond_node, struct node* body_node, struct node* next_node);
void make_else_node(struct node* body_node);
void make_return_node(struct node* exp_node);
void make_for_node(struct node* init_node, struct node* cond_node, struct node* loop_node, struct node* body_node);
void make_while_node(struct node* exp_node, struct node* body_node);
void make_do_while_node(struct node* exp_node, struct node* body_node);
void make_switch_node(struct node* exp_node, struct node* body_node, struct vector* cases, bool has_default_case);
void make_continue_node();
void make_break_node();
void make_label_node(struct node* name_node);
void make_goto_node(struct node* label_node);
void make_case_node(struct node* exp_node);
void make_tenary_node(struct node* true_node, struct node* false_node);
void make_cast_node(struct datatype* dtype, struct node* operand_node);

bool node_is_expressioable(struct node *node);
struct node *node_peek_expressionable_or_null();
bool node_is_struct_or_union_variable(struct node* node);
bool node_is_struct_or_union(struct node* node);
bool variable_node_is_primitive(struct node* node);

struct node* node_from_sym(struct symbol* sym);
struct node* node_from_symbol(struct compile_process* current_process, const char* name);
struct node* struct_node_for_name(struct compile_process* current_process, const char* name);
struct node* union_node_for_name(struct compile_process* current_process, const char* name);
bool node_is_expression_or_parantheses(struct node* node);
bool node_is_value_type(struct node* node);
bool node_is_expression(struct node* node, const char* op);
bool is_array_node(struct node* node);
bool is_node_assignment(struct node* node);
bool keyword_is_datatype(const char *str);
bool token_is_primitive_keyword(struct token* token);
bool token_is_operator(struct token* token, const char* val);
bool datatype_is_struct_or_union(struct datatype* dtype);
bool datatype_is_struct_or_union_for_name(const char* name);
size_t datatype_size_for_array_access(struct datatype* dtype);
size_t datatype_element_size(struct datatype* dtype);
size_t datatype_size_no_ptr(struct datatype* dtype);
size_t datatype_size(struct datatype* dtype);
bool datatype_is_primitive(struct datatype* dtype);
struct array_brackets* array_brackets_new();

void array_brackets_free(struct array_brackets* brackets);
void array_brackets_add(struct array_brackets* bracekts, struct node* bracket_node);
struct vector* array_brackets_node_vector(struct  array_brackets* brackets);
size_t array_brackets_calculat_size_from_index(struct datatype* dtype, struct array_brackets* brackets, int index);
size_t array_brackets_calculate_size(struct datatype* dtype, struct array_brackets* brackets);
int array_total_indexes(struct datatype* dtype);

//get the variable size for the given node
size_t variable_size(struct node* var_node);
//sums the variable size from the given variable list node
size_t variable_size_for_list(struct node* var_list_node);
struct node* variable_struct_or_union_body_node(struct node *node);
struct node* variable_node(struct node*node);
struct node* variable_node_or_list(struct node* node);


int array_multiplier(struct datatype * dtype, int index, int index_value);
int array_offset(struct datatype* dtype, int index, int index_value);
struct node* variable_struct_or_union_largest_variable_node(struct node* var_node);
struct node* body_largest_variable_node(struct node* body_node);
int struct_offset(struct compile_process* compile_process, const char* struct_name, const char* var_name, struct node** var_node_out, int last_pos, int flags);

int padding(int val ,int to);
int align_value(int val, int to);
int align_value_treat_positive(int val, int to);
int compute_sum_padding(struct vector* vec);

struct scope* scope_alloc();
void scope_dealloc(struct scope* scope);
struct scope* scope_create_root(struct compile_process* process);
void scope_free_root(struct compile_process* process);
struct scope* scope_new(struct compile_process* process, int flags);
void scope_iteration_start(struct scope* scope);
void* scope_iteration_back(struct scope* scope);
void* scope_last_entity_at_scope(struct scope* scope);
void* scope_last_entity_from_scope_stop_at(struct scope* scope, struct scope* stop_scope);
void* scope_last_entity_stop_at(struct compile_process* processs, struct scope* stop_scope);
void* scope_last_entity(struct compile_process* process);
void scope_push(struct compile_process* process, void*ptr, size_t elem_size);
void scope_finish(struct compile_process* process);
struct scope* scope_current(struct compile_process* process);

void symresolver_build_for_node(struct compile_process* process, struct node* node);
struct symbol* symresolver_get_symbol(struct compile_process* process, const char* name);
void symresolver_initialize(struct compile_process* process);
void symresolver_new_table(struct compile_process* process);
void symresolver_end_table(struct compile_process* process);
struct symbol* symresolver_get_symbol_for_native_function(struct compile_process* process, const char* name);
size_t function_node_argument_stack_addition(struct node* node);

void stackframe_pop(struct node* func_node);
struct stack_frame_element* stackframe_back(struct node* funct_node);
struct stack_frame_element* stackframe_back_expect(struct node* func_node, int expecting_type, const char* expecting_name);
void stackframe_pop_expecting(struct node* func_node, int expecting_type, const char* expecting_name);
void stackframe_peek_start(struct node* func_node);
struct stack_frame_elements* stackframe_peek(struct node* func_node);
void stackframe_push( struct node* func_node, struct stack_frame_element* element);
void stackframe_sub(struct node* func_node, int type, const char* name, size_t amount);
void stackframe_add(struct node* func_node, int type, const char* name, size_t amount);
void stackframe_assert_empty(struct node* func_node);


#define TOTAL_OPERATOR_GROUPS 14
#define MAX_PERATORS_IN_GROUP 12

enum
{
    ASSOSCIATIVITY_LEFT_TO_RIGHT,
    ASSOSCIATIVITY_RIGHT_TO_LEFT
};

struct expresssionable_op_precedence_group
{
    char *operators[MAX_PERATORS_IN_GROUP];
    int associtivity;
};



struct fixup;
/*
* Fixes the fixup
* Returns true if the fixup was successful.
*/
typedef bool(*FIXUP_FIX)(struct fixup* fixup);

/*
* Signifies the fizup has been removed from memory.
* the implementor of this function pointer should free any memory related to the fixup.
*/
typedef void(*FIXUP_END)(struct fixup* fixup);

struct fixup_config
{
    FIXUP_FIX fix;
    FIXUP_END end;
    void *private;
};

struct fixup_system
{
    //vector of the fixups
    struct vector* fixups;
};

enum
{
    FIXUP_FLAG_RESOLVED = 0b00000001
};

struct fixup
{
    int flags;
    struct fixup_system* system;
    struct fixup_config config;
};

struct fixup_system* fix_sys_new();
struct fixup_config* fixup_config(struct fixup * fixup);
void fixup_free(struct fixup* fixup);
void fixup_start_iteration(struct fixup_system* system);
struct fixup* fixup_next(struct fixup_system* system);
void fixup_sys_fixups_free(struct fixup_system* system);
void fixup_sys_free(struct fixup_system* system);
int fixup_sys_unresolved_fixups_count(struct fixup_system* system);
struct fixup* fixup_register(struct fixup_system* system, struct fixup_config* config);
bool fixup_resolve(struct fixup* fixup);
void* fixup_private(struct fixup* fixup);
bool fixups_resolve(struct fixup_system* system);
#endif /* CCOMPIELR_H*/