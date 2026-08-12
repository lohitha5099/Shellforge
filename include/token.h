#ifndef TOKEN_H
#define TOKEN_H

typedef enum {
    TOKEN_WORD,
    TOKEN_END
} TokenType;

typedef struct {
    TokenType type;
    char *value;
} Token;

/* Token functions */
Token create_token(TokenType type, const char *value);
void print_token(Token token, int index);
void free_token(Token *token);

#endif
