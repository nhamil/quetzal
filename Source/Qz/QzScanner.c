#include "Qz/QzScanner.h" 

#include <stdio.h> 

static inline qzuint SimpleTokenLength(QzTokenType type) 
{
    switch (type) 
    {
    case QZ_TOKEN_EEQUAL: 
    case QZ_TOKEN_NEQUAL: 
    case QZ_TOKEN_LEQUAL: 
    case QZ_TOKEN_GEQUAL: 
    case QZ_TOKEN_ADD_EQUAL: 
    case QZ_TOKEN_SUB_EQUAL: 
    case QZ_TOKEN_MUL_EQUAL: 
    case QZ_TOKEN_DIV_EQUAL: 
    case QZ_TOKEN_EXP_EQUAL: 
    case QZ_TOKEN_INCR: 
    case QZ_TOKEN_DECR: 
        return 2; 
    default: 
        return 1; 
    }
}

static inline qzbool IsDigit(char c) 
{
    return c >= '0' && c <= '9'; 
}

static inline qzbool IsAlpha(char c) 
{
    return c >= 'a' && c <= 'z' || c >= 'A' && c < 'Z'; 
}

static inline qzbool IsStartingIdentifier(char c) 
{
    return IsAlpha(c) || c == '_'; 
}

static inline qzbool IsMiddleIdentifider(char c) 
{
    return IsAlpha(c) || IsDigit(c) || c == '_'; 
}

static QzToken ReturnToken(QzScanner *self, QzTokenType type) 
{
    QzToken token = QzMakeToken(type, &self->code[self->curIndex], SimpleTokenLength(type), self->curLine); 
    self->curIndex += SimpleTokenLength(type); 
    return token; 
}

static QzToken NextNumber(QzScanner *self) 
{
    qzuint start = self->curIndex; 

    while (self->curIndex < self->length && IsDigit(self->code[self->curIndex])) self->curIndex++; 

    if (self->curIndex < self->length && self->code[self->curIndex] == '.') 
    {
        self->curIndex++; 
        while (self->curIndex < self->length && IsDigit(self->code[self->curIndex])) self->curIndex++; 
    }

    return QzMakeToken(QZ_TOKEN_NUMBER, &self->code[start], self->curIndex - start, self->curLine); 
}

static QzToken NextIdentifier(QzScanner *self) 
{
    qzulong start = self->curIndex; 

    while (self->curIndex < self->length) 
    {
        if (!IsMiddleIdentifider(self->code[self->curIndex])) 
        {
            break;
        }

        self->curIndex++; 
    }
    return QzMakeToken(QZ_TOKEN_IDENTIFIER, &self->code[start], self->curIndex - start, self->curLine); 
}

static QzToken NextString(QzScanner *self) 
{
    qzulong start = ++self->curIndex; 
    qzbool valid = QZ_FALSE; 
    qzuint line = self->curLine; 

    while (self->curIndex < self->length) 
    {
        if (self->code[self->curIndex] == '"') 
        {
            valid = QZ_TRUE; 
            break; 
        }
        else if (self->code[self->curIndex] == '\n') 
        {
            self->curLine++; 
        } 

        self->curIndex++; 
    }

    if (valid) 
    {
        return QzMakeToken(QZ_TOKEN_STRING, &self->code[start], self->curIndex++ - start, line); 
    }
    else 
    {
        return QzMakeToken(QZ_TOKEN_INVALID, &self->code[start], 1, line); 
    }
}

static QzToken TryKeyword(QzScanner *self, qzulong offset, qzuint len, const char *keyword, QzTokenType type) 
{
    if (self->curIndex + offset < self->length) 
    {
        qzuint i; 
        qzbool equal = QZ_TRUE; 
        for (i = 0; i < len; i++) 
        {
            if (self->code[self->curIndex + i + offset] != keyword[i]) 
            {
                equal = QZ_FALSE; 
                break; 
            }
        }

        if (self->curIndex + offset + len < self->length && IsMiddleIdentifider(self->code[self->curIndex + offset + len])) equal = QZ_FALSE; 

        if (equal) 
        {
            qzuint start = self->curIndex; 
            self->curIndex += len + offset; 
            return QzMakeToken(type, &self->code[start], len + offset, self->curLine); 
        }
    }

    return NextIdentifier(self); 
}

static QzToken NextKeywordOrIdentifier(QzScanner *self) 
{
    qzuint start = self->curIndex; 

    char c = self->code[start]; 
    char next = start + 1 < self->length ? self->code[start + 1] : 0; 

    switch (c) 
    {
        case 'a': 
            if (next == 'n') return TryKeyword(self, 2, 1, "d", QZ_TOKEN_AND); 
            if (next == 's') return TryKeyword(self, 2, 0, "", QZ_TOKEN_AS); 
            break; 
        case 'c': 
            if (next == 'l') return TryKeyword(self, 2, 3, "ass", QZ_TOKEN_CLASS); 
            if (next == 'o') return TryKeyword(self, 2, 3, "nst", QZ_TOKEN_CONST); 
            break; 
        case 'e': 
            if (next == 'l') 
            {
                char next2 = start + 2 < self->length ? self->code[start + 2] : 0; 
                if (next2 == 'i') return TryKeyword(self, 3, 1, "f", QZ_TOKEN_ELIF); 
                if (next2 == 's') return TryKeyword(self, 3, 1, "e", QZ_TOKEN_ELSE); 
            }
            break; 
        case 'f': 
            if (next == 'o') return TryKeyword(self, 2, 1, "r", QZ_TOKEN_FOR); 
            if (next == 'u') return TryKeyword(self, 2, 6, "nction", QZ_TOKEN_FUNCTION); 
            break; 
        case 'i': return TryKeyword(self, 1, 1, "f", QZ_TOKEN_IF); 
        case 'l': return TryKeyword(self, 1, 3, "oad", QZ_TOKEN_LOAD); 
        case 'm': return TryKeyword(self, 1, 5, "odule", QZ_TOKEN_MODULE); 
        case 'n': 
            if (next == 'o') 
            {
                char next2 = start + 2 < self->length ? self->code[start + 2] : 0; 
                if (next2 == 'n') return TryKeyword(self, 3, 1, "e", QZ_TOKEN_NONE); 
                if (next2 == 't') return TryKeyword(self, 3, 0, "", QZ_TOKEN_NOT); 
            }
            break; 
        case 'o': return TryKeyword(self, 1, 1, "r", QZ_TOKEN_OR); 
        case 'p': return TryKeyword(self, 1, 4, "rint", QZ_TOKEN_PRINT); 
        case 'r': return TryKeyword(self, 1, 5, "eturn", QZ_TOKEN_RETURN); 
        case 's': return TryKeyword(self, 1, 5, "tatic", QZ_TOKEN_STATIC); 
        case 'u': return TryKeyword(self, 1, 2, "se", QZ_TOKEN_USE); 
        case 'v': return TryKeyword(self, 1, 2, "ar", QZ_TOKEN_VAR); 
        case 'w': return TryKeyword(self, 1, 4, "hile", QZ_TOKEN_WHILE); 
    }

    return NextIdentifier(self); 
}

void QzCreateScanner(QzScanner *self, qzulong length, const char *code) 
{
    QZ_ASSERT(self && code); 

    self->code = code; 
    self->length = length; 
    self->curIndex = 0; 
    self->curLine = 1; 
}

void QzDestroyScanner(QzScanner *self) 
{
    QZ_ASSERT(self); 
}

QzToken QzScannerNext(QzScanner *self) 
{
    QZ_ASSERT_VAL(self, QzMakeToken(QZ_TOKEN_INVALID, 0, 0, 0)); 

    const char *code = self->code; 
    qzbool inComment = QZ_FALSE; 

    while (self->curIndex < self->length) 
    {
        char c = code[self->curIndex]; 
        char next = self->curIndex + 1 < self->length ? code[self->curIndex + 1] : 0; 

        if (inComment) 
        {
            self->curIndex++; 
            if (c == '\n') 
            {
                self->curLine++; 
                inComment = QZ_FALSE; 
            }
            continue; 
        }

        switch (c) 
        {
            case '#': 
                inComment = QZ_TRUE; 
                continue; 
            case '\n': 
                self->curLine++; 
                self->curIndex++; 
                continue; 
            case ' ': 
                self->curIndex++; 
                continue; 
            case '+': return ReturnToken(self, next == '=' ? QZ_TOKEN_ADD_EQUAL : next == '+' ? QZ_TOKEN_INCR : QZ_TOKEN_ADD); 
            case '-': return ReturnToken(self, next == '=' ? QZ_TOKEN_SUB_EQUAL : next == '-' ? QZ_TOKEN_DECR : QZ_TOKEN_SUB); 
            case '*': return ReturnToken(self, next == '=' ? QZ_TOKEN_MUL_EQUAL : QZ_TOKEN_MUL); 
            case '/': return ReturnToken(self, next == '=' ? QZ_TOKEN_DIV_EQUAL : QZ_TOKEN_DIV); 
            case '^': return ReturnToken(self, next == '=' ? QZ_TOKEN_EXP_EQUAL : QZ_TOKEN_EXP); 
            case ',': return ReturnToken(self, QZ_TOKEN_COMMA); 
            case ';': return ReturnToken(self, QZ_TOKEN_SEMICOLON); 
            case '(': return ReturnToken(self, QZ_TOKEN_PAREN_OPEN);
            case ')': return ReturnToken(self, QZ_TOKEN_PAREN_CLOSE);
            case '[': return ReturnToken(self, QZ_TOKEN_BRACKET_OPEN);
            case ']': return ReturnToken(self, QZ_TOKEN_BRACKET_CLOSE);
            case '{': return ReturnToken(self, QZ_TOKEN_BRACE_OPEN);
            case '}': return ReturnToken(self, QZ_TOKEN_BRACE_CLOSE);
            case '=': return ReturnToken(self, next == '=' ? QZ_TOKEN_EEQUAL : QZ_TOKEN_EQUAL); 
            case '<': return ReturnToken(self, next == '=' ? QZ_TOKEN_LEQUAL : QZ_TOKEN_LESS); 
            case '>': return ReturnToken(self, next == '=' ? QZ_TOKEN_GEQUAL : QZ_TOKEN_GREATER); 
            case '!': return ReturnToken(self, next == '=' ? QZ_TOKEN_NEQUAL : QZ_TOKEN_INVALID); 
            case '"': return NextString(self); 
            case '.': 
                if (IsDigit(next))
                {
                    return NextNumber(self); 
                }
                else 
                {
                    return ReturnToken(self, QZ_TOKEN_PERIOD); 
                }
            default: 
                if (IsDigit(c)) 
                {
                    return NextNumber(self); 
                }
                else if (IsStartingIdentifier(c)) 
                {
                    return NextKeywordOrIdentifier(self); 
                }
                else 
                {
                    return ReturnToken(self, QZ_TOKEN_INVALID); 
                }
        }
    }

    return ReturnToken(self, QZ_TOKEN_EOF); 
}

void QzScannerFillBuffer(QzScanner *self, QzBuffer *buffer) 
{
    QZ_ASSERT(self && buffer && buffer->elemSize == sizeof (QzToken)); 

    QzToken token; 
    while (1) 
    {
        token = QzScannerNext(self); 

        if (token.type == QZ_TOKEN_INVALID || token.type == QZ_TOKEN_EOF) break; 

        QzBufferWrite(buffer, &token); 
    }
}